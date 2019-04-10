#include "McrDebug.h"
#include <Arduino.h>

#ifdef DEBUG_FEATURE

// set this value to false to 'software' disable DEBUG feature
bool McrDebug::m_activated = true;

void McrDebug::MsgInit(const int Speed)
{
	_init(Speed);
}

void McrDebug::MsgDebug(const __FlashStringHelper * Format, ...)
{
	va_list va;
	va_start(va, Format);
	_print(F("DEBUG"), Format, va);
	va_end(va);
}

void McrDebug::MsgInfo(const __FlashStringHelper * Format, ...)
{
	va_list va;
	va_start(va, Format);
	_print(F("INFO"), Format, va);
	va_end(va);
}

void McrDebug::MsgUser(const __FlashStringHelper * Format, ...)
{
	va_list va;
	va_start(va, Format);
	_print(F("USER"), Format, va);
	va_end(va);
}

void McrDebug::MsgWarning(const __FlashStringHelper * Format, ...)
{
	va_list va;
	va_start(va, Format);
	_print(F("WARNING"), Format, va);
	va_end(va);
}

void McrDebug::MsgError(const __FlashStringHelper * Format, ...)
{
	va_list va;
	va_start(va, Format);
	_print(F("ERROR"), Format, va);
	va_end(va);
}

void McrDebug::MsgFatal(const __FlashStringHelper * Format, ...)
{
	va_list va;
	va_start(va, Format);
	_print(F("FATAL"), Format, va);
	va_end(va);
}

void McrDebug::_init(const int Speed /*= 9600*/)
{
	if(m_activated == false)
		return;
static bool m_init = false;
	if(m_init == true)
		return;
	Serial.begin(Speed);
	m_init = true;
}

void McrDebug::_print(const __FlashStringHelper * Header, const __FlashStringHelper * Format, va_list va)
{
	if(m_activated == false)
		return;
	_init();

	// prepare the string buffer
	char buffer[128];
	vsnprintf_P(buffer, sizeof(buffer), (const char *)Format, va);

	// print the string and the header
	Serial.print(Header);
	Serial.print(F("==>"));
	Serial.println(buffer);
}

#endif //#ifdef DEBUG_FEATURE