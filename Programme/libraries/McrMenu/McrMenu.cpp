#include "McrMenu.h"
#include "McrDebug.h"


byte plus_minus_array[] = {	B00100,
							B00100,
							B11111,
							B00100,
							B00100,
							B00000,
							B11111,
							B00000 };
int plus_minus_code = 1;	// do not use 0

McrMenu::McrMenu(void)
{
}

McrMenu::~McrMenu(void)
{
}

void McrMenu::Configure(void)
{
	// eeprom part
	m_eeprom = new McrEeprom();

	// clock part
	m_vccgnd_clk = new McrVccGnd(12, 11);
	m_clk = new McrDS1302(8, 9, 10);

	// temperature part
#define ADDR_SONDE_EXTERIEUR	(const byte *)"\x28\xff\x7c\xa2\x63\x15\x01\xa6"
#define ADDR_SONDE_AQUARIUM		(const byte *)"\x28\xff\xab\xda\x04\x15\x03\x7e"
#define ADDR_SONDE_DECANTE		(const byte *)"\x28\xff\x29\xdd\x61\x15\x01\x9c"
#define ADDR_SONDE_TEST			(const byte *)"\x28\x94\x7c\x03\x00\x00\x80\x3b"

	m_onewire   = new OneWire(52);
	m_exterieur = new McrDS18B20(1, ADDR_SONDE_EXTERIEUR, *m_onewire);
	m_aquarium  = new McrDS18B20(2, ADDR_SONDE_AQUARIUM , *m_onewire);
	m_decante   = new McrDS18B20(3, ADDR_SONDE_DECANTE  , *m_onewire);

	// relays part
	m_relay[ 0] = new McrRelay( 0, 22, false, F("Brassage 1 (avant gauche)"));
	m_relay[ 1] = new McrRelay( 1, 23, false, F("Brassage 2 (arrière gauche)"));
	m_relay[ 2] = new McrRelay( 2, 24, false, F("Lumière 1"));
	m_relay[ 3] = new McrRelay( 3, 25, false, F("Chauffage 1"));
	m_relay[ 4] = new McrRelay( 4, 26, false, F("Pompe de remontée"));
	m_relay[ 5] = new McrRelay( 5, 27, false, F("Écumeur"));
	m_relay[ 6] = new McrRelay( 6, 28, true , F("Disponible 1"));
	m_relay[ 7] = new McrRelay( 7, 29, true , F("Disponible 2"));
	m_relay[ 8] = new McrRelay( 8, 30, false, F("Brassage 3 (avant droit)"));
	m_relay[ 9] = new McrRelay( 9, 31, false, F("Brassage 4 (arrière droit)"));
	m_relay[10] = new McrRelay(10, 32, false, F("Lumière 2"));
	m_relay[11] = new McrRelay(11, 33, false, F("Chauffage 2"));
	m_relay[12] = new McrRelay(12, 34, false, F("Réacteur à calcaire"));
	m_relay[13] = new McrRelay(13, 35, true , F("Osmolateur"));
	m_relay[14] = new McrRelay(14, 36, true , F("Disponible 3"));
	m_relay[15] = new McrRelay(15, 37, true , F("Disponible 4"));

	// contact part
	m_contact_low  = new McrContact(0, 19, true, F("Water Low level"));
	m_contact_high = new McrContact(1, 20, true, F("Water High level"));

	// features part
	m_feature_pompe_remontee	= new McrFeature(F("Pompe de remontée"), m_relay[4]);
	m_feature_ecumeur 			= new McrFeature(F("Ecumeur"), m_relay[5]);
	m_feature_rac 				= new McrFeature(F("Réacteur à calcaire"), m_relay[12]);
	m_feature_lumiere			= new McrFeature(F("Lumière"), m_relay[2], m_relay[10]);
	m_feature_chauffage			= new McrFeatureChauffage(m_decante, m_eeprom, F("Chauffage"), m_relay[3], m_relay[11]);
	m_feature_available			= new McrFeature(F("Available"), m_relay[6], m_relay[7], m_relay[14], m_relay[15]);
	m_feature_brassage			= new McrFeatureBrassage(F("Brassage"), m_relay[0], m_relay[1], m_relay[8], m_relay[9]);
	m_feature_osmolateur 		= new McrFeatureOsmolateur(m_contact_low, m_contact_high, F("Osmolateur"), m_relay[13]);

	// available relays are set in manual mode and stopped
	m_feature_available->Stop(false);

	// IR device part
	m_ir = new IRrecv(53);
	m_ir->enableIRIn();
	m_ir->blink13(true);

	// LCD part
	m_lcd = new LiquidCrystal(38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48);
	m_lcd->begin(16, 2); // 16 characters, 2 lines
	m_display_state = 0;

	// create +/- code in LCD
	m_lcd->createChar(plus_minus_code, plus_minus_array);

	// create and start timers
	m_timer_temperature = new McrTimer(&McrMenu::_callback_temperature, this);
	m_timer_temperature->Start(TEMPERATURE_TIMER_DELAY, true);

	m_timer_feature = new McrTimer(&McrMenu::_callback_feature, this);
	m_timer_feature->Start(FEATURE_TIMER_DELAY, true);

	m_ir_code = 0;
	m_timer_ir = new McrTimer(&McrMenu::_callback_ir_timeout, this);

	m_timer_display = new McrTimer(&McrMenu::_callback_display, this);
	m_timer_display->Start(DISPLAY_TIMER_DELAY, true);

	// start everything for the first time
	_callback_temperature();
	_callback_feature();
	_callback_ir_timeout();
	_callback_display();
}

void McrMenu::IR(void)
{
int ir_code = _get_ir_code();
	if(ir_code == -1)
		return;

	int val = -1;
	switch(ir_code)
	{
		case 0x6897 : val = 0; break;
		case 0x30cf : val = 1; break;
		case 0x18e7 : val = 2; break;
		case 0x7a85 : val = 3; break;
		case 0x10ef : val = 4; break;
		case 0x38c7 : val = 5; break;
		case 0x5aa5 : val = 6; break;
		case 0x42bd : val = 7; break;
		case 0x4ab5 : val = 8; break;
		case 0x52ad : val = 9; break;
	}

	// check for unknown code
	if(val == -1) return;

	// update IR code
	m_ir_code *= 10;
	m_ir_code += val;
	//INFO("IR=0x%.4x, code=%d", ir_code, m_ir_code);
	_display_ir();

	// valid IR code are greater than 99 (at least 3 digits)
	if(m_ir_code < 100)
	{
		// start timer, only few seconds without human action to reset IR code
		m_timer_ir->Start(IR_TIMER_DELAY, false);
	}
	else
	{
		// stop timer
		m_timer_ir->Stop();

		// execute action
		_ir_action();

		// reset IR code
		m_ir_code = 0;
	}
}

int McrMenu::_get_ir_code(void)
{
	// read IR code (if any)
	decode_results results;
	if(m_ir->decode(&results) == false)
		return -1;

	// restart receive
	m_ir->resume(); 
	if(results.bits != 32)
		return -1;

	// IR received
	return results.value & 0xffff;
}

void McrMenu::_ir_action(void)
{
	DEBUG("Executing action code %d", m_ir_code);
	if(m_ir_code == 0)
		return;

	if(m_ir_code == 100) m_feature_brassage->Stop(false);
	else if(m_ir_code == 101) m_feature_brassage->Automatic();
	else if(m_ir_code == 102) m_feature_brassage->Start(false);

	else if(m_ir_code == 110) m_feature_lumiere->Stop(false);
	else if(m_ir_code == 111) m_feature_lumiere->Automatic();
	else if(m_ir_code == 112) m_feature_lumiere->Start(false);

	else if(m_ir_code == 120) m_feature_chauffage->Stop(false);
	else if(m_ir_code == 121) m_feature_chauffage->Automatic();
	else if(m_ir_code == 122) m_feature_chauffage->Start(false);
	else if(m_ir_code == 123) m_eeprom->Default();
	else if(m_ir_code == 124) m_eeprom->SetTemperatureConsigne(m_eeprom->GetTemperatureConsigne() + 0.1);
	else if(m_ir_code == 125) m_eeprom->SetTemperatureConsigne(m_eeprom->GetTemperatureConsigne() - 0.1);
	else if(m_ir_code == 126) m_eeprom->SetTemperatureDelta(m_eeprom->GetTemperatureDelta() + 0.1);
	else if(m_ir_code == 127) m_eeprom->SetTemperatureDelta(m_eeprom->GetTemperatureDelta() - 0.1);

	else if(m_ir_code == 130) m_feature_pompe_remontee->Stop(false);
	else if(m_ir_code == 131) m_feature_pompe_remontee->Automatic();
	else if(m_ir_code == 132) m_feature_pompe_remontee->Start(false);

	else if(m_ir_code == 140) m_feature_ecumeur->Stop(false);
	else if(m_ir_code == 141) m_feature_ecumeur->Automatic();
	else if(m_ir_code == 142) m_feature_ecumeur->Start(false);

	else if(m_ir_code == 150) m_feature_rac->Stop(false);
	else if(m_ir_code == 151) m_feature_rac->Automatic();
	else if(m_ir_code == 152) m_feature_rac->Start(false);

	else if(m_ir_code == 160) m_feature_osmolateur->Stop(false);
	else if(m_ir_code == 161) m_feature_osmolateur->Automatic();
	else if(m_ir_code == 162) m_feature_osmolateur->Start(false);

	else if(m_ir_code == 170) m_feature_available->Stop(false);
	else if(m_ir_code == 171) m_feature_available->Automatic();
	else if(m_ir_code == 172) m_feature_available->Start(false);

	else if(m_ir_code == 980)
	{
		// add 1 seconde
		uint16_t year;
		uint8_t month, dayMonth, dayWeek, hour, minute, second;

		// get hour
		m_clk->Get(hour, minute, second, year, month, dayMonth, dayWeek);

		// adjust hour
		second++;
		if(second == 60)
		{
			second = 0;
			minute++;
		}
		if(minute == 60)
		{
			minute = 0;
			hour++;
		}

		// set hour
		m_clk->Set(hour, minute, second, year, month, dayMonth, dayWeek);
	}
	else if(m_ir_code == 981)
	{
		// substract 1 seconde
		uint16_t year;
		uint8_t month, dayMonth, dayWeek, hour, minute, second;

		// get hour
		m_clk->Get(hour, minute, second, year, month, dayMonth, dayWeek);

		// adjust hour
		second--;
		if(second == (uint8_t)-1)
		{
			second = 59;
			minute--;
		}
		if(minute == (uint8_t)-1)
		{
			minute = 59;
			hour--;
		}

		// set hour
		m_clk->Set(hour, minute, second, year, month, dayMonth, dayWeek);
	}
	else if(m_ir_code == 982)
	{
		// add 1 minute
		uint16_t year;
		uint8_t month, dayMonth, dayWeek, hour, minute, second;

		// get hour
		m_clk->Get(hour, minute, second, year, month, dayMonth, dayWeek);

		// adjust hour
		minute++;
		if(minute == 60)
		{
			minute = 0;
			hour++;
		}

		// set hour
		m_clk->Set(hour, minute, second, year, month, dayMonth, dayWeek);
	}
	else if(m_ir_code == 983)
	{
		// substract 1 minute
		uint16_t year;
		uint8_t month, dayMonth, dayWeek, hour, minute, second;

		// get hour
		m_clk->Get(hour, minute, second, year, month, dayMonth, dayWeek);

		// adjust hour
		minute--;
		if(minute == (uint8_t)-1)
		{
			minute = 59;
			hour--;
		}

		// set hour
		m_clk->Set(hour, minute, second, year, month, dayMonth, dayWeek);
	}
	else if(m_ir_code == 984)
	{
		// add 1 hour
		uint16_t year;
		uint8_t month, dayMonth, dayWeek, hour, minute, second;

		// get hour
		m_clk->Get(hour, minute, second, year, month, dayMonth, dayWeek);

		// adjust hour
		hour++;

		// set hour
		m_clk->Set(hour, minute, second, year, month, dayMonth, dayWeek);
	}
	else if(m_ir_code == 985)
	{
		// substract 1 hour
		uint16_t year;
		uint8_t month, dayMonth, dayWeek, hour, minute, second;

		// get hour
		m_clk->Get(hour, minute, second, year, month, dayMonth, dayWeek);

		// adjust hour
		hour--;

		// set hour
		m_clk->Set(hour, minute, second, year, month, dayMonth, dayWeek);
	}
	else if(m_ir_code == 990)
	{
		m_feature_brassage->Stop(false);
		m_feature_lumiere->Stop(false);
		m_feature_chauffage->Stop(false);
		m_feature_pompe_remontee->Stop(false);
		m_feature_ecumeur->Stop(false);
		m_feature_rac->Stop(false);
		m_feature_osmolateur->Stop(false);
		m_feature_available->Stop(false);
	}
	else if(m_ir_code == 991)
	{
		m_feature_brassage->Automatic();
		m_feature_lumiere->Automatic();
		m_feature_chauffage->Automatic();
		m_feature_pompe_remontee->Automatic();
		m_feature_ecumeur->Automatic();
		m_feature_rac->Automatic();
		m_feature_osmolateur->Automatic();
		//m_feature_available->Automatic();		// no automatic feature for available relays
	}
	else if(m_ir_code == 992)
	{
		m_feature_brassage->Start(false);
		m_feature_lumiere->Start(false);
		m_feature_chauffage->Start(false);
		m_feature_pompe_remontee->Start(false);
		m_feature_ecumeur->Start(false);
		m_feature_rac->Start(false);
		m_feature_osmolateur->Start(false);
		m_feature_available->Start(false);
	}
	else if(m_ir_code == 993)
	{
		// reboot hardware, see http://playground.arduino.cc/Main/ArduinoReset
		asm volatile ("jmp 0");  
	}
}

void McrMenu::Timer(void)
{
	McrTimer::McrTimerDelay now = millis();
	m_timer_temperature->Run(now);
	m_timer_feature->Run(now);
	m_timer_ir->Run(now);
	m_timer_display->Run(now);
}

void McrMenu::_callback_temperature(void)
{
	// read temperature sensor
	McrThermometer * t = m_exterieur;
	if(t != NULL)
	{
		t->Read();
		DEBUG("Thermometer %d is %s C", t->Identifier(), t->CelsiusStr());
	}

	t = m_aquarium;
	if(t != NULL)
	{
		t->Read();
		DEBUG("Thermometer %d is %s C", t->Identifier(), t->CelsiusStr());
	}

	t = m_decante;
	if(t != NULL)
	{
		t->Read();
		DEBUG("Thermometer %d is %s C", t->Identifier(), t->CelsiusStr());
	}
	
	// reload temperature from eeprom
	if(m_feature_chauffage != NULL)
		m_feature_chauffage->Reload();
}

void McrMenu::_callback_feature(void)
{
	// update clock
	m_clk->Get();
	int hour = m_clk->Hour();
	int minute = m_clk->Minute();

	//DEBUG("Executing features");
	m_feature_pompe_remontee->Run();
	m_feature_ecumeur->Run();
	m_feature_rac->Run();
	m_feature_lumiere->Run();
	m_feature_chauffage->Run();
	m_feature_available->Run();
	m_feature_brassage->Run(hour, minute);
	m_feature_osmolateur->Run();
}

void McrMenu::_callback_ir_timeout(void)
{
	//DEBUG("Infra Red timeout");
	m_ir_code = 0;
}

void McrMenu::_callback_display(void)
{
	//DEBUG("Display timeout");
	if(m_ir_code != 0)
	{
		_display_ir();
		return;
	}

	if(m_display_state == 0)		_display_date();
	else if(m_display_state == 1)	_display_temperature_exterieur();
	else if(m_display_state == 2)	_display_temperature_aquarium();
	else if(m_display_state == 3)	_display_temperature_decante();
	else if(m_display_state == 4)	_display_relay_state();
	else if(m_display_state == 5) 	_display_contact_state();
	else if(m_display_state == 6)	_display_temperature_consigne();

	m_display_state++;
	if(m_display_state == 7)
		m_display_state = 0;
}

void McrMenu::_display_ir(void)
{
	_lcd_print(0, F("IR Code : %3d"), m_ir_code);
}

void McrMenu::_display_date(void)
{
	m_clk->Get();
	_lcd_print(0, F("%s %.2d/%.2d/%.4d"), m_clk->DayWeekStr(), m_clk->DayMonth(), m_clk->Month(), m_clk->Year());
	_lcd_print(1, F("%.2d:%.2d:%.2d"), m_clk->Hour(), m_clk->Minute(), m_clk->Second());
}

void McrMenu::_display_temperature_exterieur(void)
{
	_lcd_print(0, F("External T%c"), McrThermometer::CaracDegree());
	_lcd_print(1, F("%s"), m_exterieur == NULL ? "Not present" : m_exterieur->CelsiusStr());
}

void McrMenu::_display_temperature_aquarium(void)
{
	_lcd_print(0, F("Aquarium T%c"), McrThermometer::CaracDegree());
	_lcd_print(1, F("%s"), m_aquarium == NULL ? "Not present" : m_aquarium->CelsiusStr());
}

void McrMenu::_display_temperature_decante(void)
{
	_lcd_print(0, F("Decante T%c"), McrThermometer::CaracDegree());
	_lcd_print(1, F("%s"), m_decante == NULL ? "Not present" : m_decante->CelsiusStr());
}

void McrMenu::_display_temperature_consigne(void)
{
	_lcd_print(0, F("Consigne T%c"), McrThermometer::CaracDegree());
	_lcd_print(1, F("%s %c %s"),
				m_eeprom->GetTemperatureConsigneStr(),
				plus_minus_code,
				m_eeprom->GetTemperatureDeltaStr());
}

void McrMenu::_display_relay_state(void)
{
	// b for brassage
	// l for lumiere
	// c for chauffage
	// e for ecumeur
	// r for reacteur à calcaire
	// p for pompe de remontée
	// o for osmolateur
	// a for available
//	char str[17] = { 'b', 'b', 'l', 'c', 'p', 'e', 'a', 'a', 'b', 'b', 'l', 'c', 'r', 'o', 'a', 'a', '\0' };
	char str[17] = { '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '\0' };

	int idx = 0;
	/*  0 */if(m_relay[ 0]->IsOn() == true) str[idx] = 'B'; idx++;
	/*  1 */if(m_relay[ 1]->IsOn() == true) str[idx] = 'B'; idx++;
	/*  2 */if(m_relay[ 8]->IsOn() == true) str[idx] = 'B'; idx++;
	/*  3 */if(m_relay[ 9]->IsOn() == true) str[idx] = 'B'; idx++;
	
	/*  4 */if(m_relay[ 2]->IsOn() == true) str[idx] = 'L'; idx++;
	/*  5 */if(m_relay[10]->IsOn() == true) str[idx] = 'L'; idx++;
	
	/*  6 */if(m_relay[ 3]->IsOn() == true) str[idx] = 'C'; idx++;
	/*  7 */if(m_relay[11]->IsOn() == true) str[idx] = 'C'; idx++;
	
	/*  8 */if(m_relay[ 4]->IsOn() == true) str[idx] = 'P'; idx++;
	/*  9 */if(m_relay[ 5]->IsOn() == true) str[idx] = 'E'; idx++;
	/* 10 */if(m_relay[12]->IsOn() == true) str[idx] = 'R'; idx++;
	/* 11 */if(m_relay[13]->IsOn() == true) str[idx] = 'O'; idx++;

	/* 12 */if(m_relay[ 6]->IsOn() == true) str[idx] = 'A'; idx++;
	/* 13 */if(m_relay[ 7]->IsOn() == true) str[idx] = 'A'; idx++;
	/* 14 */if(m_relay[14]->IsOn() == true) str[idx] = 'A'; idx++;
	/* 15 */if(m_relay[15]->IsOn() == true) str[idx] = 'A'; idx++;

	_lcd_print(0, F("Relays state"));
	_lcd_print(1, F("%s"), str);
}

void McrMenu::_display_contact_state(void)
{
	char str[4] = { 'h', ' ', 'l', '\0' };

	if(m_contact_high->IsOn() == true)
		str[0] = 'H';

	if(m_contact_low->IsOn() == true)
		str[2] = 'L';

	_lcd_print(0, F("Contacts state"));
	_lcd_print(1, F("%s"), str);
}

void McrMenu::_lcd_print(int line, const __FlashStringHelper * Format, ...)
{
	// clear display if writing line 0
	// create the text to display
	va_list va;
	va_start(va, Format);

	// prepare the string buffer
	char buffer[32];
	vsnprintf_P(buffer, sizeof(buffer), (const char *)Format, va);

	if(m_lcd != NULL)
	{
		if(line == 0)
			m_lcd->clear();

		// compute column to center the text
		int col = (16 - strlen(buffer)) / 2;

		// set the cursor position
		m_lcd->setCursor(col, line);

		// display text
		m_lcd->print(buffer);
	}
	else
	{
		DEBUG("%s", buffer);
	}
	va_end(va);
}

