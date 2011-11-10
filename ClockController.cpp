#include "ClockController.h"
#include "Sound.h"

void ClockController::setup() {
  //rtc.start();
  if(currentFace != NULL)
    currentFace->init();
  tick();
}

void ClockController::tick() {
  int	time[7];

  rtc.get(time, true);

  seconds  = time[0];
  minutes  = time[1];
  hours    = time[2];
  //int dow     = time[3];
  day     = time[4];
  month   = time[5];
  year    = time[6];

  temperature = rtc.getTemperature();

  if(currentFace != NULL) {
    if(seconds != currentFace->seconds ||
       minutes != currentFace->minutes ||
       hours   != currentFace->hours)
      currentFace->setTime(hours, minutes, seconds);

    if(day   != currentFace->day ||
       month != currentFace->month ||
       year  != currentFace->year)
      currentFace->setDate(day, month, year);

    if(temperature != currentFace->temperature)
      currentFace->setTemperature(temperature);

    currentFace->lastUpdate = millis();
    currentFace->updateDisplay();
  }

  checkForBeep();
}

void ClockController::buttonClicked(ButtonType button) {

}

void ClockController::checkForBeep() {
  if(!beepEnabled) return;

  //If min > 30, the last lastBeepMinutes should be = 30
  if(minutes > 30 && lastBeepMinutes == 0)
    lastBeepMinutes = 30;

  if((lastBeepMinutes == 30 && minutes == 0) ||
     (lastBeepMinutes == 0  && minutes == 30)) {
    beep();
    lastBeepMinutes = minutes;
  }
}
