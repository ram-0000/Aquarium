#include "McrDS1302.h"
#include "McrDebug.h"

McrDS1302::McrDS1302(const uint8_t RST_pin,
							const uint8_t DAT_pin,
							const uint8_t CLK_pin)
{
	// save pin number
	m_RST_pin = RST_pin;
	if(m_RST_pin ==  0)
		FATAL("Pin RST is not defined for DS1302");

	m_DAT_pin = DAT_pin;
	if(m_DAT_pin ==  0)
		FATAL("Pin DAT is not defined for DS1302");

	m_CLK_pin = CLK_pin;
	if(m_CLK_pin ==  0)
		FATAL("Pin CLK is not defined for DS1302");

	// check the size of the structure, if mismatch, there is probably an alignment problem
	if(sizeof(ds1302_struct) != 9)
		FATAL("Size of ds1302_struct is not correct. It is %d and should be 9", sizeof(ds1302_struct));

	// set pin mode
	_init_pin_mode();

	// check if RTC write is enabled
	if(_IsWriteProtect() == false)
	{
		// clock is halted, starting it
		DEBUG("RTC write is enabled, disabling it...");
		_WriteProtect(true);
	}

	// check if RTC is started
	if(_IsClockHalt() == true)
	{
		// clock is halted, starting it
		DEBUG("RTC is stopped, starting it...");
		_ClockHalt(false);
	}

	// read all clock registers for the first time
	Get();

	// register 8 is not read by burst read, explicit read is needed
	_readRTC(8);
}

McrDS1302::~McrDS1302(void)
{
}

void McrDS1302::Dump(void) const
{
	DEBUG("");
	DEBUG("DS1302 Dump");
	const ds1302_struct * p = &m_bufferClock;
	/*
	DEBUG("Clock register 0 = %d", p->register0);
	DEBUG("Clock register 1 = %d", p->register1);
	DEBUG("Clock register 2 = %d", p->register2);
	DEBUG("Clock register 3 = %d", p->register3);
	DEBUG("Clock register 4 = %d", p->register4);
	DEBUG("Clock register 5 = %d", p->register5);
	DEBUG("Clock register 6 = %d", p->register6);
	DEBUG("Clock register 7 = %d", p->register7);
	DEBUG("Clock register 8 = %d", p->register8);
	*/

	DEBUG("CH=%d, Seconds10=%d, Seconds=%d", p->CH, p->Second10, p->Second);
	DEBUG("reserved1=%d, Minutes10=%d, Minutes=%d", p->reserved1, p->Minute10, p->Minute);
	DEBUG("hour_12_24=%d, reserved2=%d, Hours10=%d, Hours=%d", p->h24.hour_12_24, p->h24.reserved2, p->h24.Hour10, p->h24.Hour);
	DEBUG("reserved3=%d, DayMonth10=%d, DayMonth=%d", p->reserved3, p->DayMonth10, p->DayMonth);
	DEBUG("reserved4=%d, Month10=%d, Month=%d", p->reserved4, p->Month10, p->Month);
	DEBUG("reserved5=%d, DayWeek=%d", p->reserved5, p->DayWeek);
	DEBUG("Year10=%d, Year=%d", p->Year10, p->Year);
	DEBUG("reserved6=%d, WP=%d", p->reserved6, p->WP);
	DEBUG("TCS=%d, DS=%d, RS=%d", p->TCS, p->DS, p->RS);
}

void McrDS1302::_init_pin_mode(void)
{
	// Reset pin
	digitalWrite(m_RST_pin, LOW); // default, not enabled
	pinMode(m_RST_pin, OUTPUT);  

	// Clock pin
	digitalWrite(m_CLK_pin, LOW); // default, clock low
	pinMode(m_CLK_pin, OUTPUT);

	// Data pin
	digitalWrite(m_DAT_pin, LOW); // default, chipset not enables low
	pinMode(m_DAT_pin, OUTPUT);
}

bool McrDS1302::Get(void)
{
	// read all clock registers
	_read_burst_clock();
	//Dump();
	return true;
}

bool McrDS1302::Get(	uint8_t & hour, uint8_t & minute, uint8_t & second,
							uint16_t & year, uint8_t & month, uint8_t & dayMonth,
							uint8_t & dayWeek)
{
	// read all clock registers
	Get();

	// update values
	hour = Hour();
	minute = Minute();
	second = Second();
	year = Year();
	month = Month();
	dayMonth = DayMonth();
	dayWeek = DayWeek();
	return true;
}

bool McrDS1302::Set(	const uint8_t hour, const uint8_t minute, const uint8_t second,
							const uint16_t year, const uint8_t month, const uint8_t dayMonth,
							const uint8_t dayWeek)
{
	// update all values
	Hour(hour);
	Minute(minute);
	Second(second);
	Year(year);
	Month(month);
	DayMonth(dayMonth);
	DayWeek(dayWeek);

	// and write them
	_write_burst_clock();
	return true;
}

uint8_t McrDS1302::Second(void) const
{
	return (m_bufferClock.Second10 * 10) + m_bufferClock.Second;
}

uint8_t McrDS1302::Minute(void) const
{
	return (m_bufferClock.Minute10 * 10) + m_bufferClock.Minute;
}

uint8_t McrDS1302::Hour(void) const
{
	return (m_bufferClock.h24.Hour10 * 10) + m_bufferClock.h24.Hour;
}

uint8_t McrDS1302::DayMonth(void) const
{
	return (m_bufferClock.DayMonth10 * 10) + m_bufferClock.DayMonth;
}

uint8_t McrDS1302::DayWeek(void) const
{
	return m_bufferClock.DayWeek;
}

uint8_t McrDS1302::Month(void) const
{
	return (m_bufferClock.Month10 * 10) + m_bufferClock.Month;
}

uint16_t McrDS1302::Year(void) const
{
	return 2000 + (m_bufferClock.Year10 * 10) + m_bufferClock.Year;
}

void McrDS1302::Second(const uint8_t val)
{
	// split value
	uint8_t unit = 0;
	uint8_t decade = 0;
	_split(val, decade, unit);

	// update value
	m_bufferClock.Second = unit;
	m_bufferClock.Second10 = decade;
}
void McrDS1302::Minute(const uint8_t val)
{
	// split value
	uint8_t unit = 0;
	uint8_t decade = 0;
	_split(val, decade, unit);

	// update value
	m_bufferClock.Minute = unit;
	m_bufferClock.Minute10 = decade;
}

void McrDS1302::Hour(const uint8_t val)
{
	// split value
	uint8_t unit = 0;
	uint8_t decade = 0;
	_split(val, decade, unit);
	
	// update value
	m_bufferClock.h24.Hour = unit;
	m_bufferClock.h24.Hour10 = decade;
	m_bufferClock.h24.hour_12_24 = false;		// 24 hours format
}

void McrDS1302::DayMonth(const uint8_t val)
{
	// split value
	uint8_t unit = 0;
	uint8_t decade = 0;
	_split(val, decade, unit);

	// update value
	m_bufferClock.DayMonth = unit;
	m_bufferClock.DayMonth10 = decade;
}

void McrDS1302::DayWeek(const uint8_t val)
{
	// update value
	m_bufferClock.DayWeek = val;
}

void McrDS1302::Month(const uint8_t val)
{
	// split value
	uint8_t unit = 0;
	uint8_t decade = 0;
	_split(val, decade, unit);

	// update value
	m_bufferClock.Month = unit;
	m_bufferClock.Month10 = decade;
}

void McrDS1302::Year(const uint16_t val)
{
	// split value
	uint8_t unit = 0;
	uint8_t decade = 0;
	_split(val - 2000, decade, unit);

	// update value
	m_bufferClock.Year = unit;
	m_bufferClock.Year10 = decade;
}

// --------------------------------------------------------
// readRTC burst mode
//
// This function reads 8 bytes clock data in burst mode
// from the DS1302.
//
void McrDS1302::_read_burst_clock(void)
{
	_enable_chipset();

	_send_command_clk_burst_read();

	// only first 8 bytes can be read
	for(uint8_t i = 0; i != 8; i++)
		m_bufferClock.buf_register[i] = _read_serial_io();

	_disable_chipset();
}

// --------------------------------------------------------
// writeRTC burst mode
//
// This function writes 8 bytes clock data in burst mode
// to the DS1302.
//
void McrDS1302::_write_burst_clock(void)
{
	_WriteProtect(false);
	_enable_chipset();

	_send_command_clk_burst_write();

	// only first 8 bytes can be written
	for(uint8_t i = 0; i != 8; i++)
		_write_serial_io(m_bufferClock.buf_register[i]);  

	_disable_chipset();
	_WriteProtect(true);
}

// --------------------------------------------------------
// Set or clear Clock halt flag bit
// When this bit is set to logic 1, the clock oscillator
// is stopped and the DS1302 is placed into a low-power 
// standby mode with a current drain of less than 100nA. When
// this bit is written to logic 0, the clock will start.
void McrDS1302::_ClockHalt(const bool value)
{
	// check if already done
	if(_IsClockHalt() == value)
		return;

	// CH flag is in register 0
	int address = 0;

	// read register 0
	_readRTC(address);

	// set CH value
	m_bufferClock.CH = value;

	// update register 0
	_writeRTC(address, true);
}

// --------------------------------------------------------
// Set or clear Write-protect bit
// Before any write operation to the clock or RAM, must be 0.
// When 1, the write-protect bit prevents a write operation
// to any other register.
void McrDS1302::_WriteProtect(const bool value)
{
	// check if already done
	if(_IsWriteProtect() == value)
		return;

	// WP flag is in register 7
	int address = 7;

	// read register 7
	_readRTC(address);

	// set WP value
	m_bufferClock.WP = value;

	// update register 7
	_writeRTC(address, false);
}

void McrDS1302::_set_rst_chipset(const bool val) const
{
	// Set CE state
	// LOW to start a session
	// HIGH to stop the session
	int state = HIGH;
	if(val == false)
		state = LOW;
	digitalWrite(m_RST_pin, state);

	// tCC = 4us
	delayMicroseconds(4);
}

// --------------------------------------------------------
// _read_serial_io
//
// A helper function for reading a byte with bit toggle
//
uint8_t McrDS1302::_read_serial_io(void) const
{
	// Set IO line for input
	pinMode(m_DAT_pin, INPUT);

	uint8_t value = 0;
	for(uint8_t bit = 0; bit != 8; bit++)
	{
		// While insert function SCLK low and delay executed
		// data ready for reading

		// read bit, and set it in place in 'value' variable
		int val = digitalRead(m_DAT_pin);
		if(val == HIGH) val = 1;
		else val = 0;
		bitWrite(value, bit, val);

		// This routine is called immediately after a togglewrite.
		// The first bit is present on IO_PIN at call, so only 7
		// additional clock pulses are required.
		// But in burst read all pules needed, and only last
		// byte should not have tail pulse.
		// So, while DS1302 ignore additional pulse,
		// we do nothing...

		// Clock up, prepare for next
		digitalWrite(m_CLK_pin, HIGH);
		delayMicroseconds(1);

		// Clock down, value is ready after some time.
		digitalWrite(m_CLK_pin, LOW);
		delayMicroseconds(1);			// tCL=1000ns, tCDD=800ns
	}
	return value;
}

// --------------------------------------------------------
// _write_serial_io
//
// A helper function for writing a byte with bit toggle
//
void McrDS1302::_write_serial_io(const uint8_t value) const
{
	// Set IO line for input
	pinMode(m_DAT_pin, OUTPUT);

	for(uint8_t bit = 0; bit != 8; bit++)
	{
		// set a bit of the data on the I/O-line
		int val = bitRead(value, bit);
		if(val == 1) val = HIGH;
		else val = LOW;
		digitalWrite(m_DAT_pin, val);  
		delayMicroseconds(1);			// tDC = 200ns

		// clock up, data is read by DS1302
		digitalWrite(m_CLK_pin, HIGH);
		delayMicroseconds(1);			// tCH = 1000ns, tCDH = 800ns

		digitalWrite(m_CLK_pin, LOW);
		delayMicroseconds(1);			// tCL=1000ns, tCDD=800ns
	}
}

// --------------------------------------------------------
// readRTC
//
// This function reads a byte from the DS1302
// (clock or ram).
//
// The address could be like "0x80" or "0x81",
// the lowest bit is set anyway.
//
void McrDS1302::_readRTC(uint8_t address)
{
	_enable_chipset();

	_send_command_clk_read(address);

	uint8_t value = _read_serial_io();

	_disable_chipset();

	m_bufferClock.buf_register[address] = value;
}

// --------------------------------------------------------
// writeRTC
//
// This function writes a byte to the DS1302 (clock or ram).
//
// The address could be like "0x80" or "0x81",
// the lowest bit is cleared anyway.
//
void McrDS1302::_writeRTC(const uint8_t address, const bool unlock)
{
	if(unlock == true)
		_WriteProtect(false);
	_enable_chipset();

	_send_command_clk_write(address);

	uint8_t value = m_bufferClock.buf_register[address];

	_write_serial_io(value);

	_disable_chipset();
	if(unlock == true)
		_WriteProtect(true);
}

uint8_t McrDS1302::_build_command(const bool RamClk, const uint8_t Address, const bool RdWr) const
{
	// RAM part is not implemented in this class
	if(RamClk == true)
		FATAL("RAM access of DS1302 is not implemented in this class");

	uint8_t cmd = 0;
	// bit 7, 1 bit ==> always 1
	cmd |= 1 & 0x01;

	// bit 6, 1 bit ==> Ram/Clk selector (1 = RAM, 0 = CLOCK)
	cmd <<= 1;
	cmd |= RamClk & 0x01;

	// bit 5, 4, 3, 2, 1, 5 bits ==> address on 5 bits
	cmd <<= 5;
	cmd |= Address & 0x1f;

	// bit 0, 1 bit ==> Rd/Wr selector (1 = READ, 0 = WRITE)
	cmd <<= 1;
	cmd |= RdWr & 0x01;

	return cmd;
}

void McrDS1302::_send_command(const bool RamClk, const uint8_t Address, const bool RdWr)
{
	uint8_t cmd = _build_command(RamClk, Address, RdWr);
	_write_serial_io(cmd);
}

void McrDS1302::_split(const uint8_t value, uint8_t & decade, uint8_t & unit)
{
	decade = value / 10;
	unit = value - (10 * decade);
}
