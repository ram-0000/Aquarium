#ifndef __INCLUDE_MCRCLOCK__
#define __INCLUDE_MCRCLOCK__

#include <Arduino.h>

class McrClock
{
public:
	McrClock(void);
	virtual ~McrClock(void);

	// get functions that must be implemented
	virtual bool Get(void) = 0;
	virtual bool Get(	uint8_t & hour, uint8_t & minute, uint8_t & second,
							uint16_t & year, uint8_t & month, uint8_t & dayMonth,
							uint8_t & dayWeek) = 0;
	virtual uint8_t Second(void) const = 0;
	virtual uint8_t Minute(void) const = 0;
	virtual uint8_t Hour(void) const = 0;

	virtual uint8_t DayMonth(void) const = 0;
	virtual uint8_t DayWeek(void) const = 0;
	virtual uint8_t Month(void) const = 0;
	virtual uint16_t Year(void) const = 0;

	// set functions that must be implemented
	virtual bool Set(	const uint8_t hour, const uint8_t minute, const uint8_t second,
							const uint16_t year, const uint8_t month, const uint8_t dayMonth,
							const uint8_t dayWeek) = 0;
	virtual void Second(const uint8_t val) = 0;
	virtual void Minute(const uint8_t val) = 0;
	virtual void Hour(const uint8_t val) = 0;

	virtual void DayMonth(const uint8_t val) = 0;
	virtual void DayWeek(const uint8_t val) = 0;
	virtual void Month(const uint8_t val) = 0;
	virtual void Year(const uint16_t val) = 0;

	inline const char * DayWeekStr(const bool Short = true, const bool English = true)
		{ return DayWeekStr(DayWeek(), Short, English); }
	static const char * DayWeekStr(const uint8_t dayWeek, const bool Short = true, const bool English = true);

	inline const char * MonthStr(const bool Short = true, const bool English = true)
		{ return MonthStr(Month(), Short, English); }
	static const char * MonthStr(const uint8_t month, const bool Short = true, const bool English = true);

protected:

private:

	// non copyable class, no need for copy constructor
	McrClock(const McrClock & Other);
	McrClock(McrClock & Other);

	// non copyable class, no need for assignment operator
	McrClock & operator=(const McrClock & Other);
	McrClock & operator=(McrClock & Other);
};

#endif //#ifndef __INCLUDE_MCRCLOCK__