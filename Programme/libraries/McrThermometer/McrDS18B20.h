#ifndef __INCLUDE_MCRDS18B20__
#define __INCLUDE_MCRDS18B20__

#include "McrThermometer.h"
#include "OneWire.h"
#include <Arduino.h>

// number of bytes used by an DS18B20 address
#define NB_BYTE_ADDRESS 8

// number of bytes used when reading scratchpad DS18B20 data
#define NB_BYTE_READ 9

class McrDS18B20 : public McrThermometer
{
public:
	McrDS18B20(const int identifier, const uint8_t Address[NB_BYTE_ADDRESS], OneWire & Bus);
	virtual ~McrDS18B20(void);

	// function to read value
	virtual bool Read(void);
	inline virtual float Celsius(void) const { return m_celsius; }

protected:

private:
	OneWire & m_Bus;
	uint8_t m_Address[NB_BYTE_ADDRESS];
	int m_resolution;
	float m_celsius;
	
	void _SetResolution(const int NbBit);
	inline int _GetResolution(void) const { return m_resolution; }
	int _GetDelayMs(void) const;

	void _selectChip(void) const;
	void _startConversion(void) const;
	uint16_t _readScratchpad(void) const;

};

#endif //#ifndef __INCLUDE_MCRDS18B20__