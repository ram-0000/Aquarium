#ifndef __INCLUDE_MCRFEATUREBRASSAGE__
#define __INCLUDE_MCRFEATUREBRASSAGE__

#include "McrFeature.h"

class McrFeatureBrassage : public McrFeature
{
public:
	McrFeatureBrassage(	const __FlashStringHelper * purpose = NULL,
								McrRelay * relay1 = NULL,
								McrRelay * relay2 = NULL,
								McrRelay * relay3 = NULL,
								McrRelay * relay4 = NULL);

	virtual void Run(const int hour = -1, const int minute = -1);

protected:

private:

	typedef struct
	{ 	int start_hour;
		int start_minute;
		int stop_hour;
		int stop_minute;
		bool state_relay_1;
		bool state_relay_2;
		bool state_relay_3;
		bool state_relay_4;
	} Sequence_t;

	static Sequence_t m_sequence[];

	// non copyable class, no need for copy constructor
	McrFeatureBrassage(const McrFeatureBrassage & Other);
	McrFeatureBrassage(McrFeatureBrassage & Other);

	// non copyable class, no need for assignment operator
	McrFeatureBrassage & operator=(const McrFeatureBrassage & Other);
	McrFeatureBrassage & operator=(McrFeatureBrassage & Other);
};

#endif //#ifndef __INCLUDE_MCRFEATUREBRASSAGE____INCLUDE_MCRFEATUREBRASSAGE__