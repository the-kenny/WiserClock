#include "ClockController.h"
#include "Sound.h"

void ClockController::setup() {
  //rtc.start();
  if(currentFace != NULL)
    currentFace->init();
  tick();
}

void ClockController::tick() {
  if(currentFace != NULL) {
    int	time[7];

	rtc.get(time, true);

    seconds  = time[0];
    minutes  = time[1];
    hours    = time[2];
    //int dow     = time[3];
    day     = time[4];
    month   = time[5];
    year    = time[6];

    if(seconds != currentFace->seconds ||
       minutes != currentFace->minutes ||
       hours   != currentFace->hours)
      currentFace->setTime(hours, minutes, seconds);

    if(day   != currentFace->day ||
       month != currentFace->month ||
       year  != currentFace->year)
      currentFace->setDate(day, month, year);

    currentFace->lastUpdate = millis();
    currentFace->updateDisplay();
  }

  checkForBeep();
}

void ClockController::checkForBeep() {
  if(!beepEnabled) return;

  if((lastBeepMinutes == 30 && minutes == 0) ||
     (lastBeepMinutes == 0  && minutes == 30)) {
    beep();
    lastBeepMinutes = minutes;
  }
}
