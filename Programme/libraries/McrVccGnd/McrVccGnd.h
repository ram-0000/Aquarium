#ifndef __INCLUDE_MCRVCCGND__
#define __INCLUDE_MCRVCCGND__

class McrVccGnd
{
public:
	McrVccGnd(const uint8_t VCC_pin,
				 const uint8_t GND_pin);
	~McrVccGnd(void);
	
private:
	uint8_t m_GND_pin;
	uint8_t m_VCC_pin;

};

#endif //#ifndef __INCLUDE_MCRVCCGND__