#ifndef CLOCK_FACE_H
#define CLOCK_FACE_H

#include "SerialHandler.h"

struct ClockFace: public SerialHandler {
  int hours, minutes, seconds;
  int day, month, year;
  float temperature;

  long lastUpdate;

  virtual void setTime(int h, int m, int s) {
    hours = h; minutes = m; seconds = s;
  }

  virtual void setDate(int d, int m, int y) {
    day = d; month = m; year = y;
  }

  virtual void setTemperature(float t) {
    temperature = t;
  }

  //Button handling
  virtual void menuPressed() {}
  virtual void setPressed() {}
  virtual void plusPressed() {}

  virtual void init() {}
  virtual void updateDisplay() = 0;

  //Serial Handling
  virtual bool handleSerialEvent(const String& event) { return false; }
};

#endif
