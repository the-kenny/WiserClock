#include "ClockFace.h"
#include "HT1632.h"

struct SimpleFace: public ClockFace {
  long dotMillis;
  bool dotsVisible;

  virtual void updateDisplay() {
    byte bigColor = GREEN;

    ht1632_putBigDigit(2, 2, hours/10, 5, bigColor, 6);
    ht1632_putBigDigit(8, 2, hours % 10, 5, bigColor, 6);
    ht1632_putBigDigit(16, 2, minutes/10, 5, bigColor, 6);
    ht1632_putBigDigit(22, 2, minutes%10, 5, bigColor, 6);

    long now = millis();
    if(now-dotMillis > 2000) {
      dotMillis = now;

      dotsVisible = !dotsVisible;
      byte color = dotsVisible ? BLACK : GREEN;

      ht1632_plot(15, 6, color);
      ht1632_plot(15, 9, color);
    }
  }
};
