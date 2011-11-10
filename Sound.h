#ifndef _SOUND_H_
#define _SOUND_H_

#include <Arduino.h>

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


