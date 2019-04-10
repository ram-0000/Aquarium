#ifndef __INCLUDE_MCRMENU__
#define __INCLUDE_MCRMENU__

#include <McrTimer.h>
#include <McrVccGnd.h>
#include <McrRelay.h>
#include <McrDS1302.h>
#include <OneWire.h>
#include <McrDS18B20.h>
#include <McrContact.h>
#include <McrFeature.h>
#include <McrFeatureBrassage.h>
#include <McrFeatureOsmolateur.h>
#include <McrFeatureChauffage.h>
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <McrEeprom.h>

#define TEST_MOTHERBOARD

class McrMenu
{
public:
	McrMenu(void);
	~McrMenu(void);

	void Configure(void);

	void IR(void);
	void Timer(void);
	
	// EEprom value
	McrEeprom 		* m_eeprom;

	// clock sensor
	McrVccGnd		* m_vccgnd_clk;
	McrClock		* m_clk;

	// IR sensor
	IRrecv			* m_ir;

	// temperature sensors
	OneWire			* m_onewire;
	McrThermometer	* m_exterieur;
	McrThermometer	* m_aquarium;
	McrThermometer	* m_decante;

	// relays
	McrRelay		* m_relay[16];

	// contacts
	McrContact		* m_contact_low;
	McrContact		* m_contact_high;

	// features
	McrFeature		* m_feature_ecumeur;
	McrFeature		* m_feature_lumiere;
	McrFeature		* m_feature_chauffage;
	McrFeature		* m_feature_pompe_remontee;
	McrFeature		* m_feature_rac;
	McrFeature		* m_feature_available;
	McrFeature		* m_feature_brassage;
	McrFeature		* m_feature_osmolateur;

	// LCD display
	LiquidCrystal 	* m_lcd;

protected:

private:
	// temperature part
	#define TEMPERATURE_TIMER_DELAY 60000	// temperature is read only every 60 seconds
	McrTimer * m_timer_temperature;
	void _callback_temperature(void);

	// feature part
	#define FEATURE_TIMER_DELAY 1000			// feature is executed every 1 second
	McrTimer * m_timer_feature;
	void _callback_feature(void);

	// infra red part
	int m_ir_code;
	int _get_ir_code(void);
	void _ir_action(void);
	#define IR_TIMER_DELAY 10000				// feature is executed every 10 seconds
	McrTimer * m_timer_ir;
	void _callback_ir_timeout(void);

	// display part
	#define DISPLAY_TIMER_DELAY 3000			// display is executed every 3 second
	McrTimer * m_timer_display;
	int m_display_state;
	void _callback_display(void);
	void _display_ir(void);
	void _display_date(void);
	void _display_temperature_exterieur(void);
	void _display_temperature_aquarium(void);
	void _display_temperature_decante(void);
	void _display_relay_state(void);
	void _display_contact_state(void);
	void _display_temperature_consigne(void);

	void _lcd_print(int line, const __FlashStringHelper * Format, ...);

};

#endif //#ifndef __INCLUDE_MCRMENU__