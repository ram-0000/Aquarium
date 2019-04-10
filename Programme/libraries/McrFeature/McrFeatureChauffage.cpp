#include "McrFeatureChauffage.h"
#include "McrDebug.h"

McrFeatureChauffage::McrFeatureChauffage(McrThermometer * thermometer,
													  McrEeprom * eeprom,
													  const __FlashStringHelper * purpose /*= NULL*/,
													  McrRelay * relay1 /*= NULL*/,
													  McrRelay * relay2 /*= NULL*/,
													  McrRelay * relay3 /*= NULL*/,
													  McrRelay * relay4 /*= NULL*/)
: McrFeature(purpose, relay1, relay2, relay3, relay4)
{
	m_thermometer = thermometer;
	m_eeprom = eeprom;
	Reload();
}

void McrFeatureChauffage::Reload(void)
{
	DEBUG("Reloading EEPROM temperature values...");
	if(m_eeprom == NULL)
	{
		// set default value
		m_min_temperature = 20.0;
		m_max_temperature = 30.0;
		return;
	}

	// update temperature from eeprom
	m_min_temperature = m_eeprom->GetTemperatureConsigne() - m_eeprom->GetTemperatureDelta();
	m_max_temperature = m_eeprom->GetTemperatureConsigne() + m_eeprom->GetTemperatureDelta();
}

void McrFeatureChauffage::Run(const int /*hour = -1*/, const int /*minute = -1*/)
{
	// if manual mode, nothing to do
	if(IsAutomatic() == false)
		return;

	// get current temperature
	float current = m_thermometer->Celsius();

	if( (current < m_min_temperature) && (IsStarted() == false) )
	{
		DEBUG("Low temperature  %s, Starting water heating", m_thermometer->CelsiusStr());
		Start(true);
	}
	else if( (current > m_max_temperature) && (IsStarted() == true) )
	{
		DEBUG("High temperature %s, Stopping warter heat", m_thermometer->CelsiusStr());
		Stop(true);
	}
}
