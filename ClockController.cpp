#include "ClockController.h"

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

    int second  = time[0];
    int minute  = time[1];
    int hour    = time[2];
    int dow     = time[3];
    int day     = time[4];
    int month   = time[5];
    int year    = time[6];

    if(second != currentFace->seconds ||
       minute != currentFace->minutes ||
       hour != currentFace->hours)
      currentFace->setTime(hour, minute, second);

    if(day != currentFace->day ||
       month != currentFace->month ||
       year != currentFace->year)
      currentFace->setDate(day, month, year);

    currentFace->lastUpdate = millis();
    currentFace->updateDisplay();
  }
}
