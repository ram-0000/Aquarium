#include "McrMath.h"
#include <Arduino.h>

char * McrMath::FloatToString(char * buffer,
										float value,
										const char * Unit /*= NULL*/,
										const int NbDec /*= 3*/)
{
	// perform float to string
	dtostrf(value, 0, NbDec, buffer);

	// replace '.' by ',' which is more frenchy !!!
	char * p = strchr(buffer, '.');
	if(p != NULL) *p = ',';


	// add unit
	if(Unit != NULL)
		strcat(buffer, Unit);
	return buffer;
}

char * McrMath::FloatToString(float value,
								const char * Unit /*= NULL*/,
								const int NbDec /*= 3*/)
{
static char buffer[16];
	return FloatToString(buffer, value, Unit, NbDec);
}