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
#include "PongFace.h"

ClockController controller;
SimpleFace simple;
PongFace pong;

void buttonClicked(ButtonType button) {
  controller.buttonClicked(button);
}

void setup() {
	setupSpeaker();
	setupButtons();
	beep();
	ht1632_setup();

    randomSeed(analogRead(A0));

    controller.setup();

    //Connect button callbacks
    buttonCallback = &buttonClicked;

    controller.currentFace = &pong;
    controller.beepEnabled = true;
//  controller.currentFace = &simple;
}

//Font 0 is awesome
void loop() {
  controller.tick();
}
