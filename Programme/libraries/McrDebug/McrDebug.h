#ifndef __INCLUDE_MCRDEBUG__
#define __INCLUDE_MCRDEBUG__

// comment the following line to completely remove debug feature
#define DEBUG_FEATURE

#include <stdarg.h>
#include <WString.h>

#ifdef DEBUG_FEATURE

#define INIT(Speed) McrDebug::MsgInit(Speed)

#define DEBUG(p1, ...) McrDebug::MsgDebug(F(p1), ##__VA_ARGS__)
#define INFO(p1, ...) McrDebug::MsgInfo(F(p1), ##__VA_ARGS__)
#define USER(p1, ...) McrDebug::MsgUser(F(p1), ##__VA_ARGS__)
#define WARNING(p1, ...) McrDebug::MsgWarning(F(p1), ##__VA_ARGS__)
#define ERROR(p1, ...) McrDebug::MsgError(F(p1), ##__VA_ARGS__)
#define FATAL(p1, ...) McrDebug::MsgFatal(F(p1), ##__VA_ARGS__)

class McrDebug
{
public:
	static void MsgInit(const int Speed);
	static void MsgDebug(const __FlashStringHelper * Format, ...);
	static void MsgInfo(const __FlashStringHelper * Format, ...);
	static void MsgUser(const __FlashStringHelper * Format, ...);
	static void MsgWarning(const __FlashStringHelper * Format, ...);
	static void MsgError(const __FlashStringHelper * Format, ...);
	static void MsgFatal(const __FlashStringHelper * Format, ...);

protected:

private:
	McrDebug(void);		// private constructor for static only class
	 ~McrDebug(void);		// private destructor for static only class

	static void _init(const int Speed = 9600);
	static void _print(const __FlashStringHelper * Header, const __FlashStringHelper * Format, va_list va);
	static bool m_activated;
};

#else //#ifdef DEBUG_FEATURE

#define INIT(Speed)
#define DEBUG0(p1)
#define DEBUG(...)
#define INFO0(p1)
#define INFO(...)
#define USER0(p1)
#define USER(...)
#define WARNING0(p1)
#define WARNING(...)
#define ERROR0(p1)
#define ERROR(...)
#define FATAL0(p1)
#define FATAL(...)

#endif //#ifdef DEBUG_FEATURE

#endif //#ifndef __INCLUDE_MCRDEBUG__