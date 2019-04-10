#ifndef __INCLUDE_MCRFEATURECHAUFFAGE__
#define __INCLUDE_MCRFEATURECHAUFFAGE__

#include "McrFeature.h"
#include "McrThermometer.h"
#include "McrEeprom.h"

class McrFeatureChauffage : public McrFeature
{
public:
	McrFeatureChauffage(McrThermometer * thermometer,
							  McrEeprom * eeprom,
							  const __FlashStringHelper * purpose = NULL,
							  McrRelay * relay1 = NULL,
							  McrRelay * relay2 = NULL,
							  McrRelay * relay3 = NULL,
							  McrRelay * relay4 = NULL);

	virtual void Run(const int hour = -1, const int minute = -1);

	virtual void Reload(void);

protected:

private:

	McrThermometer * m_thermometer;
	float m_min_temperature;
	float m_max_temperature;
	McrEeprom * m_eeprom;

	// non copyable class, no need for copy constructor
	McrFeatureChauffage(const McrFeatureChauffage & Other);
	McrFeatureChauffage(McrFeatureChauffage & Other);

	// non copyable class, no need for assignment operator
	McrFeatureChauffage & operator=(const McrFeatureChauffage & Other);
	McrFeatureChauffage & operator=(McrFeatureChauffage & Other);
};

#endif //#ifndef __INCLUDE_MCRFEATURECHAUFFAGE__