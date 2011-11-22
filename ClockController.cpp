#include "ClockController.h"
#include "Sound.h"
#include "Settings.h"

void ClockController::setup() {
  //rtc.start();
  if(currentFace != NULL)
    currentFace->init();

  beepEnabled = EEPROM.read(SETTINGS_BEEP_ENABLED);

  tick();

  Serial.begin(9600);
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
  checkSerial();
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
  } else if(lastBeepMinutes == 30 && minutes == 0 && hours == 0) {
    beep(); delay(50); beep();
    lastBeepMinutes = minutes;
  }
}

void ClockController::checkSerial() {
  byte incoming;
  while(Serial.available()) {
    incoming = Serial.read();
    if(incoming == '\n') {
      Serial.println("Got line: " + buffer);
      dispatchSerial(buffer);
      buffer = "";
    } else
      buffer += (char)incoming;
    }
}

void ClockController::dispatchSerial(const String& line) {
  if(line == "beep")
    beep();
  else if(line.startsWith("time ")) {
    String timeString = line.substring(5);
    int idx = timeString.indexOf(':');
    if(idx != -1) {
      int hours = timeString.substring(0,2).toInt();
      int mins  = timeString.substring(3,5).toInt();
      Serial.print("Setting time to: ");
      Serial.print(hours);
      Serial.print(':');
      Serial.println(mins);

      rtc.stop();
      rtc.set(DS3231_SEC, 0);
      rtc.set(DS3231_MIN, mins);
      rtc.set(DS3231_HR, hours);
      rtc.start();
    }
  }
}
