#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define BUTTON_MENU_PIN	0	// "Menu" button on D0 (PB0);
#define BUTTON_SET_PIN	1	// "Set"  button on D1 (PB1);
//------------------------------------------------------------------
// revision 1.0 requires h/w intervention;
// button Plus was on D2 and SQW/INT from DS3231 was on D3;
// they need to be swapped;
//------------------------------------------------------------------
#define BUTTON_PLUS_PIN	2	// "Plus" button on D3 (PB3);

enum ButtonType {
  BUTTON_MENU, BUTTON_SET, BUTTON_PLUS
};

// last time the respective button was pressed; used for debouncing;
extern volatile unsigned long timeBtnMenu;
extern volatile unsigned long timeBtnSet;
extern volatile unsigned long timeBtnPlus;

extern void (*buttonCallback)(ButtonType);

void setupButtons();
void checkButtons();
boolean readButtons();



#endif
