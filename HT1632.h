#ifndef _HT1632_
#define _HT1632_


#include <Arduino.h>


#define X_MAX 32
#define Y_MAX 16

// possible values for a pixel;
#define BLACK  0
#define GREEN  1
#define RED    2
#define ORANGE 3



#define plot(x,y,v)  ht1632_plot(x,y,v)
#define cls          ht1632_clear



/*
 * commands written to the chip consist of a 3 bit "ID", followed by
 * either 9 bits of "Command code" or 7 bits of address + 4 bits of data.
 */
#define HT1632_ID_CMD 4		/* ID = 100 - Commands */
#define HT1632_ID_RD  6		/* ID = 110 - Read RAM */
#define HT1632_ID_WR  5		/* ID = 101 - Write RAM */
#define HT1632_ID_BITS (1<<2)   /* IDs are 3 bits */

#define HT1632_CMD_SYSDIS 0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYSON  0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LEDOFF 0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LEDON  0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLOFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLVMD  0x10	/* CMD= 0001-00xx-x Slave Mode */
#define HT1632_CMD_MSTMD  0x14	/* CMD= 0001-01xx-x Master Mode */
#define HT1632_CMD_RCCLK  0x18	/* CMD= 0001-10xx-x Use on-chip clock */
#define HT1632_CMD_EXTCLK 0x1C	/* CMD= 0001-11xx-x Use external clock */
#define HT1632_CMD_COMS00 0x20	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS01 0x24	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS10 0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS11 0x2C	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM    0xA0	/* CMD= 101x-PPPP-x PWM duty cycle */
#define HT1632_CMD_BITS (1<<7)



extern byte ht1632_shadowram[64][4];		// our copy of the display's RAM


void		snapshot_shadowram();
byte		get_snapshotram(byte x, byte y);
byte		get_shadowram(byte x, byte y);

void		ht1632_setup();
void		ht1632_clear();

void		ht1632_writebits(byte bits, byte firstbit);
void		ht1632_sendcmd(byte chipNo, byte command);
void		ht1632_senddata(byte chipNo, byte address, byte data);
void		ht1632_plot	       (int x, int y, byte color);
void		ht1632_putchar     (int x, int y, char c, byte color);
void		ht1632_putSmallChar(int x, int y, char c, byte color);
void		ht1632_putTinyChar (int x, int y, char c, byte color);
void		ht1632_putBigDigit (int x, int y, int digit, int fontNbr, byte color, int columns);

void		displayStaticLine(char* text, byte color);
void		setBrightness(byte level);


#endif  // _HT1632_

