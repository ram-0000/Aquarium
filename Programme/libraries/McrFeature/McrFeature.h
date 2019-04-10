#ifndef __INCLUDE_MCRFEATURE__
#define __INCLUDE_MCRFEATURE__

#include "McrRelay.h"

class McrFeature
{
public:
	McrFeature(	const __FlashStringHelper * purpose = NULL,
					McrRelay * relay1 = NULL,
					McrRelay * relay2 = NULL,
					McrRelay * relay3 = NULL,
					McrRelay * relay4 = NULL);
	virtual ~McrFeature(void);

	void Stop(bool Automatic);
	void Start(bool Automatic);
	virtual void Run(const int hour = -1, const int minute = -1);
	inline void Automatic(void) { m_automatic = true; }

	virtual inline void Reload(void) { }

protected:
	inline bool IsAutomatic(void) const { return m_automatic; }
	inline bool IsStarted(void) const { return m_started; }

	inline McrRelay * Relay1(void) { return m_relay_1; }
	inline McrRelay * Relay2(void) { return m_relay_2; }
	inline McrRelay * Relay3(void) { return m_relay_3; }
	inline McrRelay * Relay4(void) { return m_relay_4; }


private:
	const __FlashStringHelper * m_purpose;
	inline const __FlashStringHelper * Purpose(void) const { return m_purpose; }

	McrRelay * m_relay_1;
	McrRelay * m_relay_2;
	McrRelay * m_relay_3;
	McrRelay * m_relay_4;

	bool m_automatic;
	bool m_started;

	// non copyable class, no need for copy constructor
	McrFeature(const McrFeature & Other);
	McrFeature(McrFeature & Other);

	// non copyable class, no need for assignment operator
	McrFeature & operator=(const McrFeature & Other);
	McrFeature & operator=(McrFeature & Other);
};

#endif //#ifndef __INCLUDE_MCRFEATURE__