#include "McrFeatureBrassage.h"
#include "McrDebug.h"

McrFeatureBrassage::Sequence_t McrFeatureBrassage::m_sequence[] =
{ {  0, 0, 24, 0, true, true, true, true   },
/*  {  0, 0,  4, 0, true, true, true, true   },
  {  4, 0,  5, 0, true, true, false, false },
  {  5, 0,  6, 0, false, false, true, true },
  {  6, 0,  7, 0, true, true, false, false },
  {  7, 0,  8, 0, false, false, true, true },

  {  8, 0, 12, 0, true, true, true, true   },
  { 12, 0, 13, 0, true, true, false, false },
  { 13, 0, 14, 0, false, false, true, true },
  { 14, 0, 15, 0, true, true, false, false },
  { 15, 0, 16, 0, false, false, true, true },

  { 16, 0, 20, 0, true, true, true, true   },
  { 20, 0, 21, 0, true, true, false, false },
  { 21, 0, 22, 0, false, false, true, true },
  { 22, 0, 23, 0, true, true, false, false },
  { 23, 0, 24, 0, false, false, true, true },*/
};

McrFeatureBrassage::McrFeatureBrassage(const __FlashStringHelper * purpose /*= NULL*/,
													McrRelay * relay1 /*= NULL*/,
													McrRelay * relay2 /*= NULL*/,
													McrRelay * relay3 /*= NULL*/,
													McrRelay * relay4 /*= NULL*/)
: McrFeature(purpose, relay1, relay2, relay3, relay4)
{
}

void McrFeatureBrassage::Run(const int hour /*= -1*/, const int minute /*= -1*/)
{
	if( (hour == -1)  || (minute == -1) )
	{
		ERROR("hour and minute must be set");
		return;
	}
	
	// if manual mode, nothing to do
	if(IsAutomatic() == false)
		return;

	// run over all sequences
	for(int boucle = 0; boucle != sizeof(m_sequence) / sizeof(m_sequence[0]); boucle++)
	{
		const Sequence_t & seq = m_sequence[boucle];

		// check if current hour is inside sequence time
		if(hour < seq.start_hour)
			continue;
		if( (hour == seq.start_hour) && (minute < seq.start_minute) )
			continue;

		if(hour >= seq.stop_hour)
			continue;
		if( (hour == seq.stop_hour) && (minute >= seq.stop_minute) )
			continue;

		// this sequence is valid, set relays
		if(Relay1() != NULL)
			Relay1()->Set(seq.state_relay_1);
		if(Relay2() != NULL)
			Relay2()->Set(seq.state_relay_2);
		if(Relay3() != NULL)
			Relay3()->Set(seq.state_relay_3);
		if(Relay4() != NULL)
			Relay4()->Set(seq.state_relay_4);
		break;
	}
}
