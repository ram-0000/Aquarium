#include "McrDS18B20.h"
#include "McrDebug.h"

McrDS18B20::McrDS18B20(const int identifier, const uint8_t Address[NB_BYTE_ADDRESS], OneWire & Bus)
: m_Bus(Bus)
{
	Identifier(identifier);

	// clean the address
	memset(m_Address, 0, sizeof(m_Address));
	
	// check that we have a DS18B20
	if(Address[0] != 0x28)
	{
		FATAL("Thermometer %d is not a DS18B20!", Identifier());
		return;
	}
	
	// check the CRC
	if(OneWire::crc8(Address, 7) != Address[7])
	{
		FATAL("CRC address of DS18B20 %d is not valid!", Identifier());
		return;
	}

	// copy the address
	memcpy(m_Address, Address, NB_BYTE_ADDRESS);

	// set resolution to 12 bits
	_SetResolution(12);

	// read temperature for the first time
	Read();
}

McrDS18B20::~McrDS18B20(void)
{
}

void McrDS18B20::_selectChip(void) const
{
	// select chip by its address
	m_Bus.reset();
	m_Bus.select(m_Address);
}

void McrDS18B20::_startConversion(void) const
{
	// send command with no parasite power at the end
	m_Bus.write(0x44);
}

uint16_t McrDS18B20::_readScratchpad(void) const
{
	// read Scratchpad
	m_Bus.write(0xBE);

	// read all bytes
	uint8_t data[NB_BYTE_READ];
	for(int i = 0; i != NB_BYTE_READ; i++)
	{
		data[i] = m_Bus.read();
	}

	// check CRC
	if(OneWire::crc8(data, 8) != data[8])
	{
		FATAL("CRC data read from DS18B20 %d are not valid!", Identifier());
		return 0;
	}

	// get raw data
	uint16_t raw = (data[1] << 8) | data[0];
	return raw;
}

bool McrDS18B20::Read(void)
{
	// select the chip
	_selectChip();

	// start conversion
	_startConversion();

	// wait sufficient time
	delay(_GetDelayMs());

	// select the chip
	_selectChip();

	// read Scratchpad
	uint16_t raw = _readScratchpad();

	// get value
	bool negatif = false;
	if( (raw & 0x8000) == 0x8000)
	{
		// it is a negative number, perform a 2 complement
		raw = ~raw + 1;
		negatif = true;
	}
	m_celsius = (float)raw / 16.0;
	if(negatif == true)
		m_celsius *= -1;

	return true;
}

void McrDS18B20::_SetResolution(const int NbBit)
{
	// resolution can be only 9, 10, 11 or 12 bits
	m_resolution = NbBit;
uint8_t val = 0;
	switch(NbBit)
	{
		default : val = 0x1f;		// unknown resolution, set 9 bits
		case 9 : val = 0x1f;
		case 10 : val = 0x3f;
		case 11 : val = 0x5f;
		case 12 : val = 0x7f;
	}

	// select chip by its address
	m_Bus.reset();
	m_Bus.select(m_Address);    

	// write Scratchpad memory
	m_Bus.write(0x4E);
	m_Bus.write(0);
	m_Bus.write(0);
	m_Bus.write(val);
}

int McrDS18B20::_GetDelayMs(void) const
{
	switch(_GetResolution())
	{
		default : return 1000; 	// unknown resolution, return arbitrary delay
		case 9 : return 94; 		// in fact 93.75
		case 10 : return 188; 	// in fact 187.5
		case 11 : return 375;
		case 12 : return 750;
	}
}
