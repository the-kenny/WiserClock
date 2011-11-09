/*
 DS3231.cpp - library for DS3231 rtc
 Code copied and adapted from matt.joyce@gmail.com, December, 2007.
 Released into the public domain.
 */

//*********************************************************************************************************
//*	Edit History, started June, 2011
//*	please put your initials and comments here anytime you make changes
//*********************************************************************************************************
//*	Jun  20,	2011	?? start() now preserves the seconds;
//*	Jun  20,	2011	?? saveDateOnly() and SaveTimeOnly() added;
//*
//*********************************************************************************************************


#include "DS3231.h"

//Hack: See main sketch file
#include <../Wire/Wire.h>



DS3231::DS3231()
{
  Wire.begin();
}


// PRIVATE FUNCTIONS

// Aquire data from the RTC chip in BCD format
// refresh the buffer
void DS3231::read_rtc(void)
{
  // use the Wire lib to connect to tho rtc
  // reset the register pointer to zero
  Wire.beginTransmission(DS3231_CTRL_ID);
  Wire.write((uint8_t)0x00);
  Wire.endTransmission();

  // request the 7 bytes of data    (secs, min, hr, dow, date. mth, yr)
  Wire.requestFrom(DS3231_CTRL_ID, 7);
  for(int i=0; i<7; i++)
  {
    // store data in raw bcd format
    rtc_bcd[i]=Wire.read();
  }
}

// update the data on the IC from the bcd formatted data in the buffer
void DS3231::save_rtc(void)
{
  Wire.beginTransmission(DS3231_CTRL_ID);
  Wire.write((uint8_t)0x00); // reset register pointer
  for(int i=0; i<7; i++)
  {
    Wire.write(rtc_bcd[i]);
  }
  Wire.endTransmission();
}


// PUBLIC FUNCTIONS
void DS3231::get(int *rtc, boolean refresh)   // Aquire data from buffer and convert to int, refresh buffer if required
{
  if(refresh) read_rtc();
  for(int i=0;i<7;i++)  // cycle through each component, create array of data
  {
    rtc[i]=get(i, 0);
  }
}

int DS3231::get(int c, boolean refresh)  // aquire individual RTC item from buffer, return as int, refresh buffer if required
{
  if(refresh) read_rtc();
  int v=-1;
  switch(c)
  {
  case DS3231_SEC:
    v=(10*((rtc_bcd[DS3231_SEC] & DS3231_HI_SEC)>>4))+(rtc_bcd[DS3231_SEC] & DS3231_LO_BCD);
    break;
  case DS3231_MIN:
    v=(10*((rtc_bcd[DS3231_MIN] & DS3231_HI_MIN)>>4))+(rtc_bcd[DS3231_MIN] & DS3231_LO_BCD);
    break;
  case DS3231_HR:
    v=(10*((rtc_bcd[DS3231_HR] & DS3231_HI_HR)>>4))+(rtc_bcd[DS3231_HR] & DS3231_LO_BCD);
    break;
  case DS3231_DOW:
    v=rtc_bcd[DS3231_DOW] & DS3231_LO_DOW;
    break;
  case DS3231_DATE:
    v=(10*((rtc_bcd[DS3231_DATE] & DS3231_HI_DATE)>>4))+(rtc_bcd[DS3231_DATE] & DS3231_LO_BCD);
    break;
  case DS3231_MTH:
    v=(10*((rtc_bcd[DS3231_MTH] & DS3231_HI_MTH)>>4))+(rtc_bcd[DS3231_MTH] & DS3231_LO_BCD);
    break;
  case DS3231_YR:
    v=(10*((rtc_bcd[DS3231_YR] & DS3231_HI_YR)>>4))+(rtc_bcd[DS3231_YR] & DS3231_LO_BCD)+DS3231_BASE_YR;
    break;
  } // end switch
  return v;
}

void DS3231::set(int c, int v)  // Update buffer, then update the chip
{
  switch(c)
  {
  case DS3231_SEC:
    if(v<60 && v>-1)
    {
      //preserve existing clock state (running/stopped)
      int state=rtc_bcd[DS3231_SEC] & DS3231_CLOCKHALT;
      rtc_bcd[DS3231_SEC]=state | (((v / 10)<<4) + (v % 10));
    }
    break;
  case DS3231_MIN:
    if(v<60 && v>-1)
    {
      rtc_bcd[DS3231_MIN]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS3231_HR:
    // TODO : AM/PM  12HR/24HR
    if(v<24 && v>-1)
    {
      rtc_bcd[DS3231_HR]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS3231_DOW:
    if(v<8 && v>-1)
    {
      rtc_bcd[DS3231_DOW]=v;
    }
    break;
  case DS3231_DATE:
    if(v<32 && v>-1)
    {
      rtc_bcd[DS3231_DATE]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS3231_MTH:
    if(v<13 && v>-1)
    {
      rtc_bcd[DS3231_MTH]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS3231_YR:
    // (fc, Feb 5/2011) also process years > 2000 (DS3231_BASE_YR);
    if (v >= DS3231_BASE_YR)
      v = v - DS3231_BASE_YR;
    if(v<99 && v>-1)
    {
      rtc_bcd[DS3231_YR]=((v / 10)<<4) + (v % 10);
    }
    break;
  } // end switch
  save_rtc();
}

int DS3231::min_of_day(boolean refresh)
{
  // return minutes of day  (0-1440)
  if(refresh) read_rtc();
  int MoD=(get(DS3231_HR,false)*60)+get(DS3231_MIN,false);
  return MoD;
}

void DS3231::stop(void)
{
  // set the ClockHalt bit high to stop the rtc
  // this bit is part of the seconds byte
  rtc_bcd[DS3231_SEC]=rtc_bcd[DS3231_SEC] | DS3231_CLOCKHALT;
  save_rtc();
}

void DS3231::start(void)
{
  // unset the ClockHalt bit to start the rtc but keep the current seconds count
  rtc_bcd[DS3231_SEC]=rtc_bcd[DS3231_SEC] & ~DS3231_CLOCKHALT;
  save_rtc();
}


float DS3231::getTemperature()
{
  byte tMSB, tLSB;
  float temp3231 = 0;

  //temp registers (11h-12h) get updated automatically every 64s
  Wire.beginTransmission(DS3231_CTRL_ID);
  Wire.write((uint8_t)0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_CTRL_ID, 2);

  if(Wire.available()) {
    tMSB = Wire.read(); //2's complement int portion
    tLSB = Wire.read(); //fraction portion

    temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
    temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
  }

  return temp3231;
}

// update the data on the IC from the bcd formatted data in the buffer
void DS3231::saveDateOnly(void)
{
  Wire.beginTransmission(DS3231_CTRL_ID);
  Wire.write((uint8_t)0x03); // start with RTC_DOW;
  for(int i=3; i<7; i++)
  {
    Wire.write(rtc_bcd[i]);
  }
  Wire.endTransmission();
}


// update the data on the IC from the bcd formatted data in the buffer
void DS3231::saveTimeOnly(void)
{
  Wire.beginTransmission(DS3231_CTRL_ID);
  Wire.write((uint8_t)0x00); // start with RTC_SEC;
  for(int i=0; i<3; i++)
  {
    Wire.write(rtc_bcd[i]);
  }
  Wire.endTransmission();
}


byte DS3231::enableSQW(void)
{
  Wire.beginTransmission(DS3231_CTRL_ID);
  Wire.write((uint8_t)0x0E);	// control register
  Wire.endTransmission();
  Wire.requestFrom(DS3231_CTRL_ID, 1);

  byte ctrl;
  if(Wire.available()) {
    ctrl = Wire.read();
  }

  Wire.beginTransmission(DS3231_CTRL_ID);
  Wire.write((uint8_t)0x0E);	// control register
  Wire.write(ctrl & B11100011);		// all bits 0;
  Wire.endTransmission();

  return ctrl;
}


DS3231 RTC;


