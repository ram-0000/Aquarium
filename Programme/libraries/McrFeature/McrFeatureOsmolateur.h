#ifndef __INCLUDE_MCRFEATUREOSMOLATEUR__
#define __INCLUDE_MCRFEATUREOSMOLATEUR__

#include "McrFeature.h"
#include "McrContact.h"

class McrFeatureOsmolateur : public McrFeature
{
public:
	McrFeatureOsmolateur(McrContact * contact_low,
								McrContact * contact_high,
								const __FlashStringHelper * purpose = NULL,
								McrRelay * relay1 = NULL,
								McrRelay * relay2 = NULL,
								McrRelay * relay3 = NULL,
								McrRelay * relay4 = NULL);

	virtual void Run(const int hour = -1, const int minute = -1);

protected:

private:

	McrContact * m_contact_low;
	McrContact * m_contact_high;

	// non copyable class, no need for copy constructor
	McrFeatureOsmolateur(const McrFeatureOsmolateur & Other);
	McrFeatureOsmolateur(McrFeatureOsmolateur & Other);

	// non copyable class, no need for assignment operator
	McrFeatureOsmolateur & operator=(const McrFeatureOsmolateur & Other);
	McrFeatureOsmolateur & operator=(McrFeatureOsmolateur & Other);
};

#endif //#ifndef __INCLUDE_MCRFEATUREOSMOLATEUR__