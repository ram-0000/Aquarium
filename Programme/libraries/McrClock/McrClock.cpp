#include "McrClock.h"
#include "McrDebug.h"

McrClock::McrClock(void)
{
}

McrClock::~McrClock(void)
{
}

const char * McrClock::DayWeekStr(	const uint8_t dayWeek,
												const bool Short /*= true*/,
												const bool English /*= true*/)
{
	switch(dayWeek)
	{
		default:
		{
			ERROR("DayWeek value (%d) is not valid", dayWeek);
			return "???";
		}

		// Monday
		case 1: 
			if(Short == true)
			{
				if(English == true) return "Mon";
				return "Lun";
			}
			if(English == true) return "Monday";
			return "Lundi";

		// Tuesday
		case 2: 
			if(Short == true)
			{
				if(English == true) return "Tue";
				return "Mar";
			}
			if(English == true) return "Tuesday";
			return "Mardi";

		// Wednesday
		case 3: 
			if(Short == true)
			{
				if(English == true) return "Wed";
				return "Mer";
			}
			if(English == true) return "Wednesday";
			return "Mercredi";

		// Thursday
		case 4: 
			if(Short == true)
			{
				if(English == true) return "Thu";
				return "Jeu";
			}
			if(English == true) return "Thursday";
			return "Jeudi";

		// Friday
		case 5: 
			if(Short == true)
			{
				if(English == true) return "Fri";
				return "Ven";
			}
			if(English == true) return "Friday";
			return "Vendredi";

		// Saturday
		case 6: 
			if(Short == true)
			{
				if(English == true) return "Sat";
				return "Sam";
			}
			if(English == true) return "Saturday";
			return "Samedi";

		// Sunday
		case 7:
			if(Short == true)
			{
				if(English == true) return "Sun";
				return "Dim";
			}
			if(English == true) return "Sunday";
			return "Dimanche";
	}
}

const char * McrClock::MonthStr(	const uint8_t month,
											const bool Short /*= true*/,
											const bool English /*= true*/)
{
	switch(month)
	{
		default:
		{
			ERROR("Month value (%d) is not valid", month);
			return "???";
		}

		// January
		case 1:
			if(Short == true)
			{
				if(English == true) return "Jan";
				return "Jan";
			}
			if(English == true) return "January";
			return "Janvier";

		// February
		case 2:
			if(Short == true)
			{
				if(English == true) return "Feb";
				return "Fév";
			}
			if(English == true) return "February";
			return "Février";

		// March
		case 3:
			if(Short == true)
			{
				if(English == true) return "Mar";
				return "Mar";
			}
			if(English == true) return "March";
			return "Mars";

		// April
		case 4:
			if(Short == true)
			{
				if(English == true) return "Apr";
				return "Avr";
			}
			if(English == true) return "April";
			return "Avril";

		// May
		case 5:
			if(Short == true)
			{
				if(English == true) return "May";
				return "Mai";
			}
			if(English == true) return "May";
			return "Mai";

		// June
		case 6:
			if(Short == true)
			{
				if(English == true) return "Jun";
				return "Jui";
			}
			if(English == true) return "June";
			return "Juin";

		// July
		case 7:
			if(Short == true)
			{
				if(English == true) return "Jul";
				return "Jui";
			}
			if(English == true) return "July";
			return "Juillet";

		// August
		case 8:
			if(Short == true)
			{
				if(English == true) return "Aug";
				return "Aou";
			}
			if(English == true) return "August";
			return "Août";

		// September
		case 9:
			if(Short == true)
			{
				if(English == true) return "Sep";
				return "Sep";
			}
			if(English == true) return "September";
			return "Septembre";

		// October
		case 10:
			if(Short == true)
			{
				if(English == true) return "Oct";
				return "Oct";
			}
			if(English == true) return "October";
			return "Octobre";

		// November
		case 11:
			if(Short == true)
			{
				if(English == true) return "Nov";
				return "Nov";
			}
			if(English == true) return "November";
			return "Novembre";

		// December
		case 12:
			if(Short == true)
			{
				if(English == true) return "Dec";
				return "Déc";
			}
			if(English == true) return "December";
			return "Décembre";
	}
}

