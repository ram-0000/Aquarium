#include "McrRelay.h"
#include "McrDebug.h"

McrRelay::McrRelay(const uint8_t id,
						 const uint8_t pin,
						 const bool positive_logic,
						 const __FlashStringHelper * purpose /*= NULL*/)
{
	m_id = id;
	m_pin = pin;
	pinMode(Pin(), OUTPUT);
	m_positivelogic = positive_logic;
	m_purpose = purpose;

	// default state = Off
	m_state = m_positivelogic;
	Off();
}

McrRelay::~McrRelay(void)
{
	Off();
}

void McrRelay::Set(bool state)
{
	if(state == true)
	{
		// set relay on
		if(IsOn() == true)
			return;
		
		// set new state
		if(m_positivelogic == true)
			digitalWrite(Pin(), LOW);
		else
			digitalWrite(Pin(), HIGH);
		m_state = true;
		DEBUG("Relay %d is On", Id());
	}
	else
	{
		// set relay off
		if(IsOff() == true)
			return;

		// set new state
		if(m_positivelogic == true)
			digitalWrite(Pin(), HIGH);
		else
			digitalWrite(Pin(), LOW);
		m_state = false;
		DEBUG("Relay %d is Off", Id());
	}
}

