#include "McrFeature.h"

McrFeature::McrFeature(	const __FlashStringHelper * purpose /*= NULL*/,
								McrRelay * relay1 /*= NULL*/,
								McrRelay * relay2 /*= NULL*/,
								McrRelay * relay3 /*= NULL*/,
								McrRelay * relay4 /*= NULL*/)
{
	m_purpose = purpose;
	m_relay_1 = relay1;
	m_relay_2 = relay2;
	m_relay_3 = relay3;
	m_relay_4 = relay4;

	// reset all relays and set automatic mode
	Stop(true);
}

McrFeature::~McrFeature(void)
{
	Stop(false);
}

void McrFeature::Stop(bool Automatic)
{
	// set manual mode
	m_automatic = Automatic;
	m_started = false;

	// reset all relays
	if(Relay1() != NULL)
		Relay1()->Off();
	if(Relay2() != NULL)
		Relay2()->Off();
	if(Relay3() != NULL)
		Relay3()->Off();
	if(Relay4() != NULL)
		Relay4()->Off();
}

void McrFeature::Start(bool Automatic)
{
	// set manual mode
	m_automatic = Automatic;
	m_started = true;

	// set all relays
	if(Relay1() != NULL)
		Relay1()->On();
	if(Relay2() != NULL)
		Relay2()->On();
	if(Relay3() != NULL)
		Relay3()->On();
	if(Relay4() != NULL)
		Relay4()->On();
}

void McrFeature::Run(const int /*hour = -1*/, const int /*minute = -1*/)
{
	// if manual mode, nothing to do
	if(IsAutomatic() == false)
		return;

	// set all relays
	if(Relay1() != NULL)
		Relay1()->On();
	if(Relay2() != NULL)
		Relay2()->On();
	if(Relay3() != NULL)
		Relay3()->On();
	if(Relay4() != NULL)
		Relay4()->On();
}
