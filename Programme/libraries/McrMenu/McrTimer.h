#ifndef __INCLUDE_MCRTIMER__
#define __INCLUDE_MCRTIMER__

#include <Arduino.h>

class McrMenu;

class McrTimer
{
public:
	typedef signed long McrTimerDelay;
	typedef void (McrMenu::*McrTimerCallback)(void);

	McrTimer(McrTimerCallback callback, McrMenu * context);
	virtual ~McrTimer(void);

	void Start(McrTimerDelay delay, bool repeat = true);

	void Run(McrTimerDelay Now);

	inline bool IsStarted(void) const { return m_delay == -1; }
	inline void Stop(void) { m_delay = -1; }

protected:

private:
	McrTimerCallback m_callback;
	McrMenu * m_context;
	McrTimerDelay m_delay;
	bool m_repeat;
	McrTimerDelay m_start_millis;
};

#endif //#ifndef __INCLUDE_MCRTIMER__