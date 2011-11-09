#include "DS3231.h"
#include "ClockFace.h"

struct ClockController {
  DS3231 rtc;
  ClockFace* currentFace;

  void setup();
  void tick();
};
