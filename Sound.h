// Sound.h

#ifndef _SOUND_H_
#define _SOUND_H_


#include <Arduino.h>


//*	Apr  1,	2010	<MLS> modified to use the non-C++ version of tone
//* May 29, 2011    <rp>  added void soundChimeShort(); void soundChimeLong();

#define	_USE_ARDUINO_TONE
#ifdef _USE_ARDUINO_TONE
	//*	tone is included in WProgram.h
	#include "ToneNotes.h"

#else
	#include "WiseTone.h"
#endif



void setupSpeaker();
void soundAlarm();
void soundChimeShort();
void soundChimeLong();
void beep();


#endif  // _SOUND_H_


