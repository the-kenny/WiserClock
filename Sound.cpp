// Sound.cpp

//*********************************************************************************************************
//*	Edit History, started April, 2011
//*	please put your initials and comments here anytime you make changes
//*********************************************************************************************************
//* Apr  12/11 (rp) Added "Are You Sleeping" song
//* May  29/11 (rp) Added chime();
//*********************************************************************************************************


#include <Arduino.h>
#include "Sound.h"
#include "Buttons.h"

#define _USE_ARDUINO_TONE
#define _USE_ARE_YOU_SLEEPING  // Change this line into comment if you want the "old' 5 tone sound


#define SPEAKER_PIN1	22	// PC6;
#define SPEAKER_PIN2	23	// PC7;


#ifndef _USE_ARDUINO_TONE
	// instance of the tone library (by B.Hagman);
	Tone noiseMaker;	// used for beeps and alarm;
#endif




//*********************************************************************************************************
void setupSpeaker()
{
	// REM: should exploit the 2 sound levels, by driving both speaker pins;
	pinMode(SPEAKER_PIN1, OUTPUT);
	digitalWrite(SPEAKER_PIN1, LOW);

	pinMode(SPEAKER_PIN2, OUTPUT);

#ifndef _USE_ARDUINO_TONE
	noiseMaker.begin(SPEAKER_PIN2);
#endif
}


//*********************************************************************************************************
void soundChimeShort()
{
	tone(SPEAKER_PIN2, NOTE_FS7, 200);
	delay(200);
}

//*********************************************************************************************************
void soundChimeLong()
{
int	notes[]		=	{NOTE_F7, NOTE_FS7};
int durations[]	=	{150, 200};

	for (int thisNote = 0; thisNote < 2; thisNote ++)
	{
		tone(SPEAKER_PIN2, notes[thisNote], durations[thisNote]);
		delay(durations[thisNote]);
	}
}

//*********************************************************************************************************
void beep()
{
#ifdef _USE_ARDUINO_TONE
	tone(SPEAKER_PIN2, NOTE_C8, 200);
#else
	// beep after reset;
	noiseMaker.play(NOTE_C8, 200);
#endif
}




