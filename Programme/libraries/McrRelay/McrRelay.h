#ifndef __INCLUDE_MCRRELAY__
#define __INCLUDE_MCRRELAY__

#include <Arduino.h>

class McrRelay
{
public:
	McrRelay(const uint8_t id,
				const uint8_t pin,
				const bool positive_logic,
				const __FlashStringHelper * purpose = NULL);
	~McrRelay(void);

	inline void On(void) { Set(true); }
	inline void Off(void) { Set(false); }
	inline void Switch(void) { if(IsOn() == true) Off(); else On(); }
	void Set(bool state);

	inline bool IsOn(void) const { return m_state == true; }
	bool IsOff(void) const { return m_state == false; }

protected:

private:
	uint8_t m_id;
	uint8_t m_pin;
	bool m_positivelogic;
	bool m_state;
	const __FlashStringHelper * m_purpose;

	inline uint8_t Pin(void) const { return m_pin; }
	inline uint8_t Id(void) const { return m_id; }
	inline const __FlashStringHelper * Purpose(void) const { return m_purpose; }

	// non copyable class, no need for copy constructor
	McrRelay(const McrRelay & Other);
	McrRelay(McrRelay & Other);

	// non copyable class, no need for assignment operator
	McrRelay & operator=(const McrRelay & Other);
	McrRelay & operator=(McrRelay & Other);
};

#endif //#ifndef __INCLUDE_MCRRELAY__
