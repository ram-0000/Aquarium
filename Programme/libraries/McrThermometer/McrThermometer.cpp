#include "McrThermometer.h"
#include <stdlib.h>
#include <string.h>
#include "McrMath.h"

// code to display ° carac on arduino LCD
char McrThermometer::m_carac_degree = (char)223;
char McrThermometer::m_unit_degree[] = { m_carac_degree, 'C', 0 };

McrThermometer::McrThermometer(int identifier /*= 0*/)
{
	Identifier(identifier);
}

McrThermometer::~McrThermometer(void)
{
}

float McrThermometer::Fahrenheit(void) const
{
	float ret = Celsius();
	ret *= 1.8;
	ret += 32.0;
	return ret;
}

const char * McrThermometer::CelsiusStr(void) const
{
static char buffer[16];
	return McrMath::FloatToString(buffer, Celsius(), UnitDegree(), 2);
}

const char * McrThermometer::FahrenheitStr(void) const
{
static char buffer[16];
	return McrMath::FloatToString(buffer, Fahrenheit(), UnitDegree(), 2);
}
