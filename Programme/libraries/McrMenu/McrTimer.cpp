#include "McrTimer.h"
#include "McrMenu.h"

McrTimer::McrTimer(McrTimerCallback callback, McrMenu * context)
{
	m_callback = callback;
	m_context = context;
	Stop();
}

McrTimer::~McrTimer(void)
{
}

void McrTimer::Start(McrTimerDelay delay, bool repeat /*= true*/)
{
	m_delay = delay;
	m_repeat = repeat;
	m_start_millis = millis();
}

void McrTimer::Run(McrTimerDelay Now)
{
	// check if started
	if(IsStarted() == true)
		return;

	// compute delta time between now and start time
	McrTimerDelay delta = Now - m_start_millis;

	// check if not overflow (occurs every 50 days) and not elapsed
	if( (delta >= 0) && (delta < m_delay) )
		return;

	// time is overflow or elapsed
	if(m_repeat == false)
		Stop();
	else
		Start(m_delay, true);

	// call the callback function
	// Yes, I know, syntax is not very simple but that's life, it works like this !!!
	if( (m_context != NULL) && (m_callback != NULL) )
		(m_context->*m_callback)();
}

