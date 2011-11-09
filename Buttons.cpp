// Buttons.cpp


#include "Buttons.h"

// button debouncing adapted from http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210559123/7;
#define BOUNCE_TIME_BUTTON		600	// bounce time in ms for the menu button;
#define BOUNCE_TIME_IR			500	// bounce time in ms for the IR receiver;


// last time the respective button was pressed; used for debouncing;
volatile unsigned long timeBtnMenu	=	0;
volatile unsigned long timeBtnSet 	=	0;
volatile unsigned long timeBtnPlus	=	0;



//*********************************************************************************************************
void setupButtons()
{
	pinMode(BUTTON_MENU_PIN,	INPUT);
	pinMode(BUTTON_SET_PIN,		INPUT);
	pinMode(BUTTON_PLUS_PIN,	INPUT);
}



//*********************************************************************************************************
void checkButtons()
{
	// check buttons;
	if (digitalRead(BUTTON_MENU_PIN) == LOW)
	{
	        // debouncing;
	        if (abs(millis() - timeBtnMenu) < BOUNCE_TIME_BUTTON)  return;

		//wiseClock.processButtonMenu();

	        timeBtnMenu	=	millis();
	}

	if (digitalRead(BUTTON_SET_PIN) == LOW)
	{
	        // debouncing;
	        if (abs(millis() - timeBtnSet) < BOUNCE_TIME_BUTTON)  return;

		//wiseClock.processButtonSet();

	        timeBtnSet	=	millis();
	}

	if (digitalRead(BUTTON_PLUS_PIN) == LOW)
	{
	        // debouncing;
	        if (abs(millis() - timeBtnPlus) < BOUNCE_TIME_BUTTON)  return;

		//wiseClock.processButtonPlus();

	        timeBtnPlus	=	millis();
	}
}
//*********************************************************************************************************
boolean readButtons()
{
	if ((digitalRead(BUTTON_MENU_PIN) == LOW) || (digitalRead(BUTTON_SET_PIN) == LOW) || (digitalRead(BUTTON_PLUS_PIN) == LOW))
	{
		return true;
	}
	else
	{
		return false;
	}
}







