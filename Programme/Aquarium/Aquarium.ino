#include <McrMath.h>
#include <McrEeprom.h>
#include <LiquidCrystal.h>
#include <McrDebug.h>
#include <McrVccGnd.h>
#include <McrMenu.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <McrFeature.h>
#include <McrFeatureBrassage.h>
#include <McrContact.h>
#include <OneWire.h>
#include <McrDS18B20.h>
#include <McrThermometer.h>
#include <McrRelay.h>
#include <McrClock.h>
#include <McrDS1302.h>

// create the menu
McrMenu menu;

void setup()
{
	INIT(9600);
	DEBUG("setup(): Aquarium");
	menu.Configure();

	// reset time
/*	int hour = 16;
	int minute = 22;
	int second = 0;
	int year =  2017;
	int month = 3;
	int day = 12;
	int week_day = 7;  // monday is 1, sunday is 7
	menu.m_clk->Set(hour, minute, second, year, month, day, week_day);*/
}

void loop()
{
	//DEBUG("loop(): Aquarium");
	// read IR code (if any)
	menu.IR();

	// play the menu
	menu.Timer();

	// wait and restart
	delay(50);
}
