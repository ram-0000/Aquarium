#ifndef __INCLUDE_MCRMATH__
#define __INCLUDE_MCRMATH__

#include <Arduino.h>

class McrMath
{
public:
	static char * FloatToString(char * buffer,
										 float value,
										 const char * Unit = NULL,
										 const int NbDec = 3);

	// warning, non reentrant function
	static char * FloatToString(float value,
										 const char * Unit = NULL,
										 const int NbDec = 3);

protected:

private:
	McrMath(void);			// private constructor for static only class
	~McrMath(void);		// private destructor for static only class

};

#endif //#ifndef __INCLUDE_MCRMATH__