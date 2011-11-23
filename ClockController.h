#include "DS3231.h"
#include "ClockFace.h"
#include "Buttons.h"

struct ClockController {
  DS3231 rtc;
  int hours,minutes,seconds,day,month,year;
  float temperature;

  ClockFace* currentFace;

  bool beepEnabled;

  void setup();
  void tick();

  void buttonClicked(ButtonType button);

private:
  void checkForBeep();
  bool beepTriggerEnabled;

  String buffer;
  void checkSerial();
  void dispatchSerial(const String& line);
};
