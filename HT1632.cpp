#include "HT1632.h"
#include <avr/pgmspace.h>	// fonts are now loaded in program space;
#include "font3.h"
#include "fontBig.h"
#include "fontSmall.h"
#include "fontTiny.h"

#define CHIP_MAX 4 //Four HT1632Cs on one board
#define CLK_DELAY



#define HT1632_DATA	14	// Data pin (pin 7 of display connector)
#define HT1632_CS		13	//  Chip Select (pin 1 of display connnector)
#define HT1632_WRCLK	12	// Write clock pin (pin 5 of display connector)
#define HT1632_CLK	11	// clock pin (pin 2 of display connector)



//Atmega644 Version of fastWrite - for pins 0-15
#define fWriteA(_pin_, _state_) ( _pin_ < 8 ? (_state_ ? PORTB |= 1 << _pin_ : \
PORTB &= ~(1 << _pin_ )) : (_state_ ? PORTD |= 1 << (_pin_ -8) : PORTD &= ~(1 << (_pin_ -8) )))

//Atmega644 Version of fastWrite - for pins 16-31 (Note: PORTA mapping reversed from others)
#define fWriteB(_pin_, _state_) ( _pin_ < 24 ? (_state_ ? PORTC |= 1 << (_pin_ -16) : \
PORTC &= ~(1 << (_pin_ -16))) : (_state_ ? PORTA |= 1 << (31- _pin_) : PORTA &= ~(1 << (31- _pin_) )))



//*********************************************************************************************************
// our own copy of the "video" memory; 64 bytes for each of the 4 screen quarters;
// each 64-element array maps 2 planes:
// indexes from 0 to 31 are allocated for green plane;
// indexes from 32 to 63 are allocated for red plane;
// when a bit is 1 in both planes, it is displayed as orange (green + red);
byte ht1632_shadowram[64][4] = {0};




//**************************************************************************************************
//Function Name: OutputCLK_Pulse
//Function Feature: enable CLK_74164 pin to output a clock pulse
//Input Argument: void
//Output Argument: void
//**************************************************************************************************
void OutputCLK_Pulse(void) //Output a clock pulse
{
	fWriteA(HT1632_CLK, HIGH);
	//  digitalWrite(HT1632_CLK, HIGH);
	fWriteA(HT1632_CLK, LOW);
	//  digitalWrite(HT1632_CLK, LOW);
}


//**************************************************************************************************
//Function Name: OutputA_74164
//Function Feature: enable pin A of 74164 to output 0 or 1
//Input Argument: x: if x=1, 74164 outputs high. If x?1, 74164 outputs low.
//Output Argument: void
//**************************************************************************************************
void OutputA_74164(unsigned char x) //Input a digital level to 74164
{
	fWriteA(HT1632_CS, (x==1 ? HIGH : LOW));
//    digitalWrite(HT1632_CS, (x==1 ? HIGH : LOW));
}


//**************************************************************************************************
//Function Name: ChipSelect
//Function Feature: enable HT1632C
//Input Argument: select: HT1632C to be selected
// If select=0, select none.
// If s<0, select all.
//Output Argument: void
//**************************************************************************************************
void ChipSelect(int select)
{
  unsigned char tmp = 0;
  if(select<0) //Enable all HT1632Cs
  {
    OutputA_74164(0);
    CLK_DELAY;
    for(tmp=0; tmp<CHIP_MAX; tmp++)
    {
      OutputCLK_Pulse();
    }
  }
  else if(select==0) //Disable all HT1632Cs
  {
    OutputA_74164(1);
    CLK_DELAY;
    for(tmp=0; tmp<CHIP_MAX; tmp++)
    {
      OutputCLK_Pulse();
    }
  }
  else
  {
    OutputA_74164(1);
    CLK_DELAY;
    for(tmp=0; tmp<CHIP_MAX; tmp++)
    {
      OutputCLK_Pulse();
    }
    OutputA_74164(0);
    CLK_DELAY;
    OutputCLK_Pulse();
    CLK_DELAY;
    OutputA_74164(1);
    CLK_DELAY;
    tmp = 1;
    for( ; tmp<select; tmp++)
    {
      OutputCLK_Pulse();
    }
  }
}


//*********************************************************************************************************

void ht1632_setup()
{
  pinMode(HT1632_CS, OUTPUT);
  digitalWrite(HT1632_CS, HIGH); 	/* unselect (active low) */
  pinMode(HT1632_WRCLK, OUTPUT);
  pinMode(HT1632_DATA, OUTPUT);
  pinMode(HT1632_CLK, OUTPUT);

  for (int j=1; j<5; j++)
  {
    ht1632_sendcmd(j, HT1632_CMD_SYSDIS);  // Disable system
    ht1632_sendcmd(j, HT1632_CMD_COMS00);
    ht1632_sendcmd(j, HT1632_CMD_MSTMD); 	/* Master Mode */
    ht1632_sendcmd(j, HT1632_CMD_RCCLK);  // HT1632C
    ht1632_sendcmd(j, HT1632_CMD_SYSON); 	/* System on */
    ht1632_sendcmd(j, HT1632_CMD_LEDON); 	/* LEDs on */
  }

  ht1632_clear();
  delay(100);
}


//*********************************************************************************************************
/*
 * plot a point on the display, with the upper left hand corner
 * being (0,0), and the lower right hand corner being (31, 15);
 * parameter "color" could have one of the 4 values:
 * black (off), red, green or yellow;
 */
void ht1632_plot (int x, int y, byte color)
{
  if (x<0 || x>=X_MAX || y<0 || y>=Y_MAX)
    return;

  if (color != BLACK && color != GREEN && color != RED && color != ORANGE)
    return;

  byte existingPixel = get_shadowram(x, y);
  if (existingPixel == color)
    return;		// nothing to change;

  byte nChip = 1 + x/16 + (y>7?2:0) ;
  x = x % 16;
  y = y % 8;
  byte addr = (x<<1) + (y>>2);
  byte bitval = 8>>(y&3);  // compute which bit will need set
  switch (color)
  {
    case BLACK:
      // clear the bit in both planes;
      ht1632_shadowram[addr][nChip-1] &= ~bitval;
      ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      addr = addr + 32;
      ht1632_shadowram[addr][nChip-1] &= ~bitval;
      ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      break;
    case GREEN:
      // set the bit in the green plane and clear the bit in the red plane;
      ht1632_shadowram[addr][nChip-1] |= bitval;
      ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      addr = addr + 32;
      ht1632_shadowram[addr][nChip-1] &= ~bitval;
      ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      break;
    case RED:
      // clear the bit in green plane and set the bit in the red plane;
      ht1632_shadowram[addr][nChip-1] &= ~bitval;
      ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      addr = addr + 32;
      ht1632_shadowram[addr][nChip-1] |= bitval;
      ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      break;
    case ORANGE:
      // set the bit in both the green and red planes;
      ht1632_shadowram[addr][nChip-1] |= bitval;
      ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      addr = addr + 32;
      ht1632_shadowram[addr][nChip-1] |= bitval;
      ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      break;
  }
}


//*********************************************************************************************************
/*
 * ht1632_clear
 * clear the display, and the shadow memory, and the snapshot
 * memory.  This uses the "write multiple words" capability of
 * the chipset by writing all 96 words of memory without raising
 * the chipselect signal.
 */
void ht1632_clear()
{
  // clear our own shadow memory;
  for (int i=1; i< 5; i++)
  {
    for (int j=0; j < 64; j++)
      ht1632_shadowram[j][i-1] = 0;
  }

  // clear the display memory;
  for (byte i=0; i<96; i++)
  {
    ht1632_senddata(1, i, 0);  // clear the display!
    ht1632_senddata(2, i, 0);  // clear the display!
    ht1632_senddata(3, i, 0);  // clear the display!
    ht1632_senddata(4, i, 0);  // clear the display!
  }
}


//*********************************************************************************************************
/*
 * snapshot_shadowram
 * Copy the shadow ram into the snapshot ram (the upper bits)
 * This gives us a separate copy so we can plot new data while
 * still having a copy of the old data.  snapshotram is NOT
 * updated by the plot functions (except "clear").
 */
void snapshot_shadowram()
{
  for (int nQuarter=0; nQuarter<4; nQuarter++)
  {
    for (byte i=0; i<64; i++)
    {
      // copy the video bits (lower 4) in the upper 4;
      byte val = ht1632_shadowram[i][nQuarter];
      val = (val & 0x0F) + (val << 4);
      ht1632_shadowram[i][nQuarter] = val;
    }
  }
}



//*********************************************************************************************************
/*
 * get_snapshotram
 * get a pixel value from the snapshot ram instead of the actual video memory;
 * return BLACK, GREEN, RED or ORANGE;
 */
byte get_snapshotram(byte x, byte y)
{
   byte nQuarter = x/16 + (y>7?2:0);  // 0..3;
   x = x % 16;
   y = y % 8;
   byte addr = (x<<1) + (y>>2);
   byte bitval = 8>>(y&3);  // compute the required bit;

   byte greenByte = ht1632_shadowram[addr]   [nQuarter] >> 4;
   byte redByte   = ht1632_shadowram[addr+32][nQuarter] >> 4;

   byte retVal = 0;
   byte val = (greenByte & bitval) ? 1 : 0;
   val += (redByte & bitval) ? 2 : 0;
   return val;
}


//*********************************************************************************************************
/*
 * ht1632_writebits
 * Write bits (up to 8) to h1632 on pins ht1632_data, ht1632_wrclk
 * Chip is assumed to already be chip-selected
 * Bits are shifted out from MSB to LSB, with the first bit sent
 * being (bits & firstbit), shifted till firsbit is zero.
 */
void ht1632_writebits (byte bits, byte firstbit)
{
	while (firstbit)
	{
		fWriteA(HT1632_WRCLK, LOW);	// digitalWrite(HT1632_WRCLK, LOW);

		if (bits & firstbit)
		{
			fWriteA(HT1632_DATA, HIGH);	// digitalWrite(HT1632_DATA, HIGH);
		}
		else
		{
			fWriteA(HT1632_DATA, LOW);	// digitalWrite(HT1632_DATA, LOW);
		}

		fWriteA(HT1632_WRCLK, HIGH);	// digitalWrite(HT1632_WRCLK, HIGH);
		firstbit >>= 1;
	}
}


//*********************************************************************************************************
/*
 * ht1632_sendcmd
 * Send a command to the ht1632 chip.
 */
void ht1632_sendcmd (byte chipNo, byte command)
{
  ChipSelect(chipNo);
  ht1632_writebits(HT1632_ID_CMD, 1<<2);  // send 3 bits of id: COMMMAND
  ht1632_writebits(command, 1<<7);  // send the actual command
  ht1632_writebits(0, 1); 	/* one extra dont-care bit in commands. */
  ChipSelect(0);
}



//*********************************************************************************************************
/*
 * ht1632_senddata
 * send a nibble (4 bits) of data to a particular memory location of the
 * ht1632.  The command has 3 bit ID, 7 bits of address, and 4 bits of data.
 *    Select 1 0 1 A6 A5 A4 A3 A2 A1 A0 D0 D1 D2 D3 Free
 * Note that the address is sent MSB first, while the data is sent LSB first!
 * This means that somewhere a bit reversal will have to be done to get
 * zero-based addressing of words and dots within words.
 */
void ht1632_senddata (byte chipNo, byte address, byte data)
{
  ChipSelect(chipNo);
  ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
  ht1632_writebits(address, 1<<6); // Send address
  ht1632_writebits(data, 1<<3); // send 4 bits of data
  ChipSelect(0);
}




//*********************************************************************************************************
/*
 * Copy a character glyph from the myfont data structure to
 * display memory, with its upper left at the given coordinate
 * This is unoptimized and simply uses plot() to draw each dot.
 * (fc, Jan 30/2011) display character using the specified color;
 */
void ht1632_putchar(int x, int y, char c, byte color)
{
	// fonts defined for ascii 32 and beyond (index 0 in font array is ascii 32);
	byte charIndex;

	// replace undisplayable characters with blank;
	if (c < 32 || c > 127)
	{
		charIndex	=	0;
	}
	else
	{
		charIndex	=	c - 32;
	}

	// move character definition, pixel by pixel, onto the display;
	// fonts are defined as one byte per row;
	for (byte row=0; row<8; row++)
	{
		byte rowDots	=	pgm_read_byte_near(&myfont[charIndex][row]);
		for (byte col=0; col<6; col++)
		{
			if (rowDots & (1<<(5-col)))
				ht1632_plot(x+col, y+row, color);
			else
				ht1632_plot(x+col, y+row, BLACK);
		}
	}
}


//*********************************************************************************************************
/*
 * Copy a character glyph from the smallFont data structure to
 * display memory, with its upper left at the given coordinate
 * This is unoptimized and simply uses plot() to draw each dot.
 */
void ht1632_putSmallChar(int x, int y, char c, byte color)
{
	// fonts defined for ascii 32 and beyond (index 0 in font array is ascii 32);
	byte charIndex;

	// replace undisplayable characters with blank;
	if (c < 32 || c > 127)
	{
		charIndex	=	0;
	}
	else
	{
		charIndex	=	c - 32;
	}

	// move character definition, pixel by pixel, onto the display;
	// fonts are defined as one byte per row;
	for (byte row=0; row<8; row++)
	{
		byte rowDots	=	pgm_read_byte_near(&smallFont[charIndex][row]);
		for (byte col=0; col<6; col++)
		{
			if (rowDots & (1<<(5-col)))
				ht1632_plot(x+col, y+row, color);
			else
				ht1632_plot(x+col, y+row, BLACK);
		}
	}
}


//*********************************************************************************************************
/*
 * return the value of a pixel from the video memory (either BLACK, RED, GREEN, ORANGE);
 */
byte get_shadowram(byte x, byte y)
{
   byte nQuarter = x/16 + (y>7?2:0) ;
   x = x % 16;
   y = y % 8;
   byte addr = (x<<1) + (y>>2);
   byte bitval = 8>>(y&3);
   byte retVal = 0;
   byte val = (ht1632_shadowram[addr][nQuarter] & bitval) ? 1 : 0;
   val += (ht1632_shadowram[addr+32][nQuarter] & bitval) ? 2 : 0;
   return val;
}


//*********************************************************************************************************
void displayStaticLine(char* text, byte color)
{
	// try to center the text;
	int len	=	strlen(text);
	int nx	=	(X_MAX+1 - 6*len) / 2;
	if (nx < 0)
	{
		nx = 0;		// text too long to fit on the screen;
	}
	for (int i=0; i<len; i++)
	{
		ht1632_putchar(nx + i*6, 0, text[i], color);
	}
}


//*********************************************************************************************************
/* (fc, Aug 1/10)
 * display a big digit, defined on a 6x12 grid;
 * the purpose is to display the time H12:MM on the 24x16 matrix;
 */
// (rp, Oct 2010) modified to use multiple big fonts;
void ht1632_putBigDigit(int x, int y, int digit, int fontNbr, byte color, int columns)
{
	// move character definition, pixel by pixel, onto the display;
	// a big digit is defined as 12 rows, one byte per row;

    int fontOffset = fontNbr * CHARS_IN_FONT;

	for (byte row=0; row < BYTES_PER_CHARS; row++)
	{
		byte rowDots = pgm_read_byte_near(&bigFont[digit + fontOffset][row]);
		for (byte col=0; col<columns; col++)
		{
			if (rowDots & (1<<((columns - 1) - col)))
				ht1632_plot(x+col, y+row, color);
			else
				ht1632_plot(x+col, y+row, BLACK);
		}
	}
}


//*********************************************************************************************************
/*
 * Copy a character glyph from the tinyFont data structure to
 * display memory, with its upper left at the given coordinate
 * This is unoptimized and simply uses plot() to draw each dot.
 */
void ht1632_putTinyChar(int x, int y, char c, byte color)
{
	// fonts defined for ascii 32 and beyond (index 0 in font array is ascii 32);
	byte charIndex;

	// replace undisplayable characters with blank;
	if (c < 32 || c > 126)
	{
		charIndex	=	0;
	}
	else
	{
		charIndex	=	c - 32;
	}

	// move character definition, pixel by pixel, onto the display;
	// fonts are defined as one byte per row;
	for (byte row=0; row<6; row++)
	{
		byte rowDots	=	pgm_read_byte_near(&tinyFont[charIndex][row]);
		for (byte col=0; col<4; col++)
		{
			if (rowDots & (1<<(3-col)))
				ht1632_plot(x+col, y+row, color);
			else
				ht1632_plot(x+col, y+row, BLACK);
		}
	}
}


void setBrightness(byte nLevel)
{
  if (nLevel > 5)
      nLevel = 5;

  for (int i=1; i<=4; i++)
    ht1632_sendcmd(i, HT1632_CMD_PWM + nLevel*3);

}

