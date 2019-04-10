#include "McrContact.h"

McrContact::McrContact(	const uint8_t id,
								const uint8_t pin,
								const bool positive_logic,
								const __FlashStringHelper * purpose /*= NULL*/)
{
	m_id = id;
	m_pin = pin;
	pinMode(Pin(), INPUT_PULLUP);
	m_positive_logic = positive_logic;
	m_purpose = purpose;
}

McrContact::~McrContact(void)
{
}

bool McrContact::IsOn(void) const
{
	// read the value
	int val = digitalRead(Pin());
	
	// compute result accortding to internal logic
	if(m_positive_logic == true)
	{
		if(val == LOW)
			return true;
		return false;
	}

	if(val == HIGH)
		return true;
	return false;
}
