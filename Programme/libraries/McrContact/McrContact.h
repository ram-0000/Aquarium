#ifndef __INCLUDE_MCRCONTACT__
#define __INCLUDE_MCRCONTACT__

#include <Arduino.h>

class McrContact
{
public:
	McrContact(	const uint8_t id,
					const uint8_t pin,
					const bool positive_logic,
					const __FlashStringHelper * purpose = NULL);
	~McrContact(void);

	bool IsOn(void) const;
	inline bool IsOff(void) const { return ! IsOn(); }

protected:

private:
	uint8_t m_id;
	uint8_t m_pin;
	bool m_positive_logic;
	const __FlashStringHelper * m_purpose;

	inline uint8_t Pin(void) const { return m_pin; }
	inline uint8_t Id(void) const { return m_id; }
	inline const __FlashStringHelper * Purpose(void) const { return m_purpose; }

	// non copyable class, no need for copy constructor
	McrContact(const McrContact & Other);
	McrContact(McrContact & Other);

	// non copyable class, no need for assignment operator
	McrContact & operator=(const McrContact & Other);
	McrContact & operator=(McrContact & Other);
};

#endif //#ifndef __INCLUDE_MCRCONTACT__