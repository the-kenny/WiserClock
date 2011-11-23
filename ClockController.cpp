#include "ClockController.h"
#include "Sound.h"
#include "Settings.h"

void ClockController::setup() {
  //rtc.start();
  if(currentFace != NULL)
    currentFace->init();

  beepEnabled = EEPROM.read(SETTINGS_BEEP_ENABLED);
  beepTriggerEnabled = false;

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

  if(seconds > 30) {
    beepTriggerEnabled = true;
  } else if(beepTriggerEnabled) {
    if(minutes == 0)
      soundChimeLong();
    else if(minutes == 30)
      soundChimeShort();

    if(minutes == 0 || minutes == 30)
      beepTriggerEnabled = false;
  }
}

void ClockController::checkSerial() {
  byte incoming;
  while(Serial.available()) {
    incoming = Serial.read();
    if(incoming == '\n') {
      //Serial.println("Got line: " + buffer);
      dispatchSerial(buffer);
      buffer = "";
    } else
      buffer += (char)incoming;
  }
}

void ClockController::dispatchSerial(const String& line) {
  if(line == "beep")
    beep();
  else if(line.startsWith("hourBeep ")) {
    bool enabled = (line.substring(10,11).toInt()==1);
    EEPROM.write(SETTINGS_BEEP_ENABLED, enabled);
    beepEnabled = enabled;
    Serial.print(beepEnabled ? "Enabling" : "Disabling");
    Serial.println(" beep");
  } else if(line.startsWith("time ")) {
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
    } else {
      Serial.println("Couldn't parse time: " + timeString);
    }
  } else if(line.startsWith("date ")) {
    String dateString = line.substring(5);
    int idx = dateString.indexOf('/');
    if(idx != -1) {
      int day = dateString.substring(0,2).toInt();
      int month  = dateString.substring(3,5).toInt();
      int year  = dateString.substring(6,10).toInt();
      Serial.print("Setting date to: ");
      Serial.print(day);
      Serial.print('/');
      Serial.print(month);
      Serial.print('/');
      Serial.println(year);

      rtc.stop();
      rtc.set(DS3231_DATE, day);
      rtc.set(DS3231_MTH, month);
      rtc.set(DS3231_YR, year);
      rtc.start();
    } else {
      Serial.println("Couldn't parse date: " + dateString);
    }
  } else if(line.equalsIgnoreCase("Open the pod bay doors, HAL")){
    Serial.println("Sorry. I can't let you do that, Dave.");
  } else {
    Serial.println("I'm not wise enough to understand: " + line);
  }
}
