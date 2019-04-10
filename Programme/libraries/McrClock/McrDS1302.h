#ifndef __INCLUDE_MCRDS1302__
#define __INCLUDE_MCRDS1302__

#include "McrClock.h"

class McrDS1302 : public McrClock
{
public:
	McrDS1302(const uint8_t RST_pin,
				 const uint8_t DAT_pin,
				 const uint8_t CLK_pin);
	virtual ~McrDS1302(void);
	
	void Dump(void) const;

	// get function that must be implemented
	virtual bool Get(void);
	virtual bool Get(	uint8_t & hour, uint8_t & minute, uint8_t & second,
							uint16_t & year, uint8_t & month, uint8_t & dayMonth,
							uint8_t & dayWeek);

	virtual uint8_t Second(void) const;
	virtual uint8_t Minute(void) const;
	virtual uint8_t Hour(void) const;

	virtual uint8_t DayMonth(void) const;
	virtual uint8_t DayWeek(void) const;
	virtual uint8_t Month(void) const;
	virtual uint16_t Year(void) const;

	// set functions that must be implemented
	virtual bool Set(	const uint8_t hour, const uint8_t minute, const uint8_t second,
							const uint16_t year, const uint8_t month, const uint8_t dayMonth,
							const uint8_t dayWeek);

	virtual void Second(const uint8_t val);
	virtual void Minute(const uint8_t val);
	virtual void Hour(const uint8_t val);

	virtual void DayMonth(const uint8_t val);
	virtual void DayWeek(const uint8_t val);
	virtual void Month(const uint8_t val);
	virtual void Year(const uint16_t val);

protected:

private:
// Structure for the first 8 registers.
// These 8 bytes can be read at once with
// the 'clock burst' command.
// Note that this structure contains an anonymous union.
// It might cause a problem on other compilers.
typedef union
{
	uint8_t buf_register[9];

	struct
	{
		// register 0
		union
		{
			struct
			{
				uint8_t Second:4;			// low decimal digit 0-9
				uint8_t Second10:3;		// high decimal digit 0-5
				uint8_t CH:1;				// CH = Clock Halt
			};
			uint8_t register0;
		};

		// register 1
		union
		{
			struct
			{
				uint8_t Minute:4;
				uint8_t Minute10:3;
				uint8_t reserved1:1;
			};
			uint8_t register1;
		};

		// register 2
		union
		{
			struct
			{
				uint8_t Hour:4;
				uint8_t Hour10:2;
				uint8_t reserved2:1;
				uint8_t hour_12_24:1;	// 0 for 24 hour format
			} h24;
			struct
			{
				uint8_t Hour:4;
				uint8_t Hour10:1;
				uint8_t AM_PM:1;			// 0 for AM, 1 for PM
				uint8_t reserved2:1;
				uint8_t hour_12_24:1;	// 1 for 12 hour format
			} h12;
			uint8_t register2;
		};

		// register 3
		union
		{
			struct
			{
				uint8_t DayMonth:4;		// Day of month, 1 = first day
				uint8_t DayMonth10:2;
				uint8_t reserved3:2;
			};
			uint8_t register3;
		};
	  
		// register 4
		union
		{
			struct
			{
				uint8_t Month:4;			// Month, 1 = January
				uint8_t Month10:1;
				uint8_t reserved4:3;
			};
			uint8_t register4;
		};

		// register 5
		union
		{
			struct
			{
				uint8_t DayWeek:3;		// Day of week, 1 = first day (any day)
				uint8_t reserved5:5;
			};
			uint8_t register5;
		};

		// register 6
		union
		{
			struct
			{
				uint8_t Year:4;			// Year, 0 = year 2000
				uint8_t Year10:4;
			};
			uint8_t register6;
		};

		// register 7
		union
		{
			struct
			{
				uint8_t reserved6:7;
				uint8_t WP:1;				// WP = Write Protect
			};
			uint8_t register7;
		};

			// register 8
		union
		{
			struct
			{
				uint8_t TCS:4;
				uint8_t DS:2;
				uint8_t RS:2;
			};
			uint8_t register8;
		};
	};
}ds1302_struct;

	// pin used by RTC
	uint8_t m_RST_pin;
	uint8_t m_DAT_pin;
	uint8_t m_CLK_pin;

	// the structure mapping internal registers of the RTC
	ds1302_struct m_bufferClock;

	void _init_pin_mode(void);

	inline void _send_command_clk_burst_read(void)
		{ _send_command_clk_read(0xff); }
	inline void _send_command_clk_burst_write(void)
		{ _send_command_clk_write(0xff); }
	inline void _send_command_clk_read(const uint8_t Address)
		{ _send_command_clk(Address, true); }
	inline void _send_command_clk_write(const uint8_t Address)
		{ _send_command_clk(Address, false); }
	inline void _send_command_clk(const uint8_t Address, const bool RdWr)
		{ _send_command(false, Address, RdWr); }
	void _send_command(const bool RamClk, const uint8_t Address, const bool RdWr);
	uint8_t _build_command(const bool RamClk, const uint8_t Address, const bool RdWr) const;

	void _read_burst_clock(void);
	void _write_burst_clock(void);

	void _readRTC(uint8_t address);
	void _writeRTC(const uint8_t address, const bool unlock);

	uint8_t _read_serial_io(void) const;
	void _write_serial_io(const uint8_t value) const;

	inline void _enable_chipset(void) const { _set_rst_chipset(true); }
	inline void _disable_chipset(void) const { _set_rst_chipset(false); }
	void _set_rst_chipset(const bool val) const;

	void _WriteProtect(const bool value);
	inline bool _IsWriteProtect(void) const { return m_bufferClock.WP; }

	void _ClockHalt(const bool value);
	inline bool _IsClockHalt(void) const { return m_bufferClock.CH; }

	void _split(const uint8_t value, uint8_t & decade, uint8_t & unit);
};

#endif //#ifndef __INCLUDE_MCRDS1302__
