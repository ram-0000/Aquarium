#include "McrEeprom.h"
#include "McrDebug.h"
#include <EEPROM.h>

#define START_ADDR_EEPROM				0

#define TEMPERATURE_CONSIGNE_MIN		20.0
#define TEMPERATURE_CONSIGNE_DEFAULT	27.0
#define TEMPERATURE_CONSIGNE_MAX		35.0

#define TEMPERATURE_DELTA_MIN			0.0
#define TEMPERATURE_DELTA_DEFAULT		0.2
#define TEMPERATURE_DELTA_MAX			5.0

McrEeprom::McrEeprom(void)
{
	_read();
	_dump();
}

McrEeprom::~McrEeprom(void)
{
}

void McrEeprom::_read(void)
{
	EEPROM.get(START_ADDR_EEPROM, m_var);
}

void McrEeprom::_write(void)
{
	EEPROM.put(START_ADDR_EEPROM, m_var);
}

void McrEeprom::Default(void)
{
	SetTemperatureConsigne(TEMPERATURE_CONSIGNE_DEFAULT);
	SetTemperatureDelta(TEMPERATURE_DELTA_DEFAULT);
}

void McrEeprom::_dump(void) const
{
	DEBUG("Temperature Consigne=%s", McrMath::FloatToString(GetTemperatureConsigne()));
	DEBUG("Temperature Delta=%s",  McrMath::FloatToString(GetTemperatureDelta()));
}

void McrEeprom::SetTemperatureConsigne(float val)
{
	// record
	m_var.temperature_consigne = val;

	// adjust
	if(m_var.temperature_consigne > TEMPERATURE_CONSIGNE_MAX)
		m_var.temperature_consigne = TEMPERATURE_CONSIGNE_MAX;
	if(m_var.temperature_consigne < TEMPERATURE_CONSIGNE_MIN)
		m_var.temperature_consigne = TEMPERATURE_CONSIGNE_MIN;

	// write
	_write();
}

void McrEeprom::SetTemperatureDelta(float val)
{
	// record
	m_var.temperature_delta = val;

	// adjust
	if(m_var.temperature_delta > TEMPERATURE_DELTA_MAX)
		m_var.temperature_delta = TEMPERATURE_DELTA_MAX;
	if(m_var.temperature_delta < TEMPERATURE_DELTA_MIN)
		m_var.temperature_delta = TEMPERATURE_DELTA_MIN;

	// write
	_write();
}

const char * McrEeprom::GetTemperatureConsigneStr(void) const
{
static char buffer[16];
	strcpy(buffer, McrMath::FloatToString(GetTemperatureConsigne(), NULL, 2));
	return buffer;
}

const char * McrEeprom::GetTemperatureDeltaStr(void) const
{
static char buffer[16];
	strcpy(buffer, McrMath::FloatToString(GetTemperatureDelta(), NULL, 2));
	return buffer;
}
