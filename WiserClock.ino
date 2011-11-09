

#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "DS3231.h"
#include "HT1632.h"
#include "SDuFAT.h"
#include "Sound.h"
#include "Buttons.h"
#include "ClockController.h"
#include "SimpleFace.h"

ClockController controller;
SimpleFace simple;

void setup() {
	setupSpeaker();
	setupButtons();
	beep();
	ht1632_setup();

    controller.setup();
    controller.currentFace = &simple;
}

//Font 0 is awesome
void loop() {
  controller.tick();
}
