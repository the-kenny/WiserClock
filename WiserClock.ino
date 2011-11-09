

#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "DS3231.h"
#include "HT1632.h"
#include "SDuFAT.h"
#include "Sound.h"
#include "Buttons.h"


void setup() {
	setupSpeaker();
	setupButtons();
	beep();
	ht1632_setup();
}



void loop() {
  for(int i = 0; i <= 9; ++i) {
    ht1632_putBigDigit(1,1, i, 0, GREEN, 14);
    delay(200);
  }
}
