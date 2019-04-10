#ifndef __INCLUDE_MCREEPROM__
#define __INCLUDE_MCREEPROM__

#include <Arduino.h>
#include "McrMath.h"

class McrEeprom
{
public:
	McrEeprom(void);
	~McrEeprom(void);

	void Default(void);

	void SetTemperatureConsigne(float val);
	inline float GetTemperatureConsigne(void) const { return m_var.temperature_consigne; }
	const char * GetTemperatureConsigneStr(void) const;

	void SetTemperatureDelta(float val);
	inline float GetTemperatureDelta(void) const { return m_var.temperature_delta; }
	const char * GetTemperatureDeltaStr(void) const;

protected:

private:
	typedef struct
	{	float temperature_consigne;
		float temperature_delta;
	} Eeprom_t;
	Eeprom_t m_var;

	void _read(void);
	void _write(void);
	void _dump(void) const;

	// non copyable class, no need for copy constructor
	McrEeprom(const McrEeprom & Other);
	McrEeprom(McrEeprom & Other);

	// non copyable class, no need for assignment operator
	McrEeprom & operator=(const McrEeprom & Other);
	McrEeprom & operator=(McrEeprom & Other);
};

#endif //#ifndef __INCLUDE_MCREEPROM__
