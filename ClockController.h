#include "DS3231.h"
#include "ClockFace.h"

struct ClockController {
  DS3231 rtc;
  int hours,minutes,seconds,day,month,year;
  float temperature;

  ClockFace* currentFace;

  void setup();
  void tick();

  bool beepEnabled;

private:
  void checkForBeep();
  int lastBeepMinutes; //If 30, next beep happens at min=0, if 0, next beep is at min=30
};
