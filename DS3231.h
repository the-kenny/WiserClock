#ifndef _DS3231_H_
#define _DS3231_H_


// include types & constants of Wiring core API
#include <Arduino.h>

#define DS3231_SEC	0
#define DS3231_MIN	1
#define DS3231_HR		2
#define DS3231_DOW	3
#define DS3231_DATE	4
#define DS3231_MTH	5
#define DS3231_YR		6
#define DS3231_AL1_SEC	7
#define DS3231_AL1_MIN	8
#define DS3231_AL1_HR	9
#define DS3231_AL1_DAY	0x0A
#define DS3231_AL2_MIN	0x0B
#define DS3231_AL2_HR	0x0C
#define DS3231_AL2_DAY	0x0D
#define DS3231_CTRL	0x0E
#define DS3231_STATUS	0x0F
#define DS3231_AGING	0x10
#define DS3231_TEMP_MSB	0x11
#define DS3231_TEMP_LSB	0x12


#define DS3231_BASE_YR 2000

#define DS3231_CTRL_ID B1101000


// Define register bit masks
#define DS3231_CLOCKHALT B10000000

#define DS3231_LO_BCD  B00001111
#define DS3231_HI_BCD  B11110000

#define DS3231_HI_SEC  B01110000
#define DS3231_HI_MIN  B01110000
#define DS3231_HI_HR   B00110000
#define DS3231_LO_DOW  B00000111
#define DS3231_HI_DATE B00110000
#define DS3231_HI_MTH  B00110000
#define DS3231_HI_YR   B11110000

#define DS3231_DATASTART 0x08


class DS3231 {
public:
  DS3231();
  void get(int *, boolean);
  int get(int, boolean);
  int min_of_day(boolean);
  void set(int, int);
  void start(void);
  void stop(void);
  float getTemperature();
  void saveDateOnly(void);
  void saveTimeOnly(void);
  byte enableSQW();

private:
  byte rtc_bcd[0x13]; // used prior to read/set ds3231 registers;
  void read_rtc(void);
  void save_rtc(void);
};


extern DS3231 RTC;

#endif // _DS3231_H_

