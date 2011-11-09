#ifndef CLOCK_FACE_H
#define CLOCK_FACE_H

struct ClockFace {
  int hours, minutes, seconds;
  int day, month, year;
  int temperature;

  long lastUpdate;

  virtual void setTime(int h, int m, int s) {
    hours = h; minutes = m; seconds = s;
  }

  virtual void setDate(int d, int m, int y) {
    day = d; month = m; year = y;
  }

  virtual void updateDisplay() = 0;
};

#endif
