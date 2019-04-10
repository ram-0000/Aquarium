#include "McrFeatureOsmolateur.h"
#include "McrDebug.h"

McrFeatureOsmolateur::McrFeatureOsmolateur(McrContact * contact_low,
														 McrContact * contact_high,
														 const __FlashStringHelper * purpose /*= NULL*/,
														 McrRelay * relay1 /*= NULL*/,
														 McrRelay * relay2 /*= NULL*/,
														 McrRelay * relay3 /*= NULL*/,
														 McrRelay * relay4 /*= NULL*/)
: McrFeature(purpose, relay1, relay2, relay3, relay4)
{
	m_contact_low = contact_low;
	m_contact_high = contact_high;
}

void McrFeatureOsmolateur::Run(const int /*hour = -1*/, const int /*minute = -1*/)
{
	// if manual mode, nothing to do
	if(IsAutomatic() == false)
		return;

	if( (m_contact_high->IsOn() == true) && (IsStarted() == true) )
	{
		DEBUG("Contact High is On, Stopping osmolateur");
		Stop(true);
	}
	else if( (m_contact_low->IsOff() == true) && (IsStarted() == false) )
	{
		DEBUG("Contact Low is Off, Starting osmolateur");
		Start(true);
	}
}
