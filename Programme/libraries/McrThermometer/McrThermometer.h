#ifndef __INCLUDE_MCRTHERMOMETER__
#define __INCLUDE_MCRTHERMOMETER__

class McrThermometer
{
public:
	McrThermometer(int identifier = 0);
	virtual ~McrThermometer(void);

	// function that must be implemented to read temperature value
	virtual bool Read(void) = 0;

	virtual float Celsius(void) const = 0;
	float Fahrenheit(void) const;

	const char * CelsiusStr(void) const;
	const char * FahrenheitStr(void) const;

	inline int Identifier(void) const { return m_identifier; }
	inline void  Identifier(int val) { m_identifier = val; }

	inline static const char * UnitDegree(void) { return m_unit_degree; }
	inline static char CaracDegree(void) { return m_carac_degree; }

protected:

private:

	int m_identifier;
	static char m_carac_degree;
	static char m_unit_degree[];

	// non copyable class, no need for copy constructor
	McrThermometer(const McrThermometer & Other);
	McrThermometer(McrThermometer & Other);

	// non copyable class, no need for assignment operator
	McrThermometer & operator=(const McrThermometer & Other);
	McrThermometer & operator=(McrThermometer & Other);
};

#endif //#ifndef __INCLUDE_MCRTHERMOMETER__