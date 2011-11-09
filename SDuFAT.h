/*
 * SDuFAT.h - Use of SD Cards
 *
 * Copyright (C) 2008 Libelium Comunicaciones Distribuidas S.L.
 * http://www.libelium.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * However, D.J. Cuartielles has to credit the following people, since
 * this library is just a wrapper on code written by others, who deserve
 * all the credit for their effort making this possible:
 *
 *  ** sd2iec - SD/MMC to Commodore serial bus interface/controller
 *     Copyright (C) 2007,2008  Ingo Korb <ingo@akana.de>
 *
 *  ** Inspiration and low-level SD/MMC access based on code from MMC2IEC
 *     by Lars Pontoppidan, Aske Olsson, Pascal Dufour, DTU, Denmark
 *
 * Created February 14th, 2009 by David Cuartielles -aka BlushingBoy-
 */

#include "mmc.h"
#include "microfat.h"
#include <Arduino.h>

#ifndef SDuFAT_h
#define SDuFAT_h

/// DEFINES /////////////////////////////////////////////////////////////////////////////

// define command codes
#define LS 0
#define DEL 1
#define CAT 2
#define PRINT 3
#define WRITE 4
#define APPEND 5
#define SUCCESS 6
#define ERROR 7
#define WARNING 8

// define command help
#define LSm "file info"
#define DELm "deting file"
#define CATm "file content"
#define PRINTm "add string"
#define WRITEm "init file, type text"
#define APPENDm "add text"
#define SUCCESSm "..done\n"
#define ERRORm "..error!\n"
#define WARNINGm "..warning!\n"


// define verbose modes
#define ON 1
#define OFF 0

// define the end of file and end of line characters
#define EOL '.'  // used to end the package to write to the board
#define EOF 0x03  // used to mark end of file, 0x03 is end of text, for many editors 0x0A marks end of file, but can be confused with EOL

class SDuFAT
{
  private:
  public:

/// CONSTRUCTORS ////////////////////////////////////////////////////////////////////////

SDuFAT(void);

/// FUNCTIONS ///////////////////////////////////////////////////////////////////////////

// del - erases the file given as parameter for real it just writes the first character of
//       the file's blocks with EOF, the rest will be kept in the card. It could be used
//       to recover data the old-school way
int del(const char* filename);

// ls - prints info about the file to the screen
int ls(const char* filename);


// printEvent - displays what is happening
void printEvent(int event, const char* filename);

// usedBytes - returns how many bytes are used in the file
long usedBytes(const char* filename);

// (fc, Jul16/09)
long getSectors(const char* filename);

// startSector - returns the sector wher the file starts
long startSector(const char* filename);


// (fc)
// returns the buffer;
char* readBytes(const char* filename, long sectorNum);
};

extern SDuFAT SD;

#endif

