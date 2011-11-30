#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include <Arduino.h>

class SerialHandler {
public:
  virtual bool handleSerialEvent(const String& event) = 0;
};

#endif
