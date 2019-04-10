#include "McrMenu.h"
#include "McrDebug.h"

McrVccGnd::McrVccGnd(const uint8_t VCC_pin,
							const uint8_t GND_pin)
{
	// set Ground pin
	m_GND_pin = GND_pin;
	digitalWrite(m_GND_pin, LOW);
	pinMode(m_GND_pin, OUTPUT);

	// set VCC pin
	m_VCC_pin = VCC_pin;
	digitalWrite(m_VCC_pin, HIGH);
	pinMode(m_VCC_pin, OUTPUT);
}

McrVccGnd::~McrVccGnd(void)
{
	// reset VCC pin
	digitalWrite(m_VCC_pin, LOW);
	pinMode(m_VCC_pin, INPUT);

	// reset Ground pin
	digitalWrite(m_GND_pin, LOW);
	pinMode(m_GND_pin, INPUT);
}

