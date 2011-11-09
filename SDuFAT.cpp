/*
 * SDuFAT.cpp - Use of SD Cards
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
 
//*********************************************************************************************************
//*	Edit History, started May, 2011
//*	please put your initials and comments here anytime you make changes
//*********************************************************************************************************
//* May 29/11 (rp) changed: "Serial.println();" to "//  Serial.println();" at line 163;
//*
//*********************************************************************************************************

#include "SDuFAT.h"


// buffers and other needed variables
byte buffer[513]; // the buffer cannot be any smaller, SD cards are read/written in blocks of 512 bytes

int inByte = -1;
static unsigned long sector, sectors, length;


/// CONSTRUCTORS ////////////////////////////////////////////////////////////////////////

SDuFAT::SDuFAT()
{
    // nothing to do
  buffer[512] = 0;
}


// (fc, Jul16/09)
// getSectors - returns number of sectors (512 bytes long) in the file;
long SDuFAT::getSectors(const char* filename)
{
  mmc::initialize();

  if (microfat::initialize(buffer))
  {

    if (microfat::locateFileStart(filename, sector, length))
    {
      if (length > BYTESPERSECTOR) {
        sectors = length / BYTESPERSECTOR; 
        length = BYTESPERSECTOR;
      }

      else return 0;
    } 
    else return 0;
  } 
  else return 0;

  return sectors;
}


// usedBytes - returns how many bytes are used in the file
long SDuFAT::usedBytes(const char* filename)
{
  long bytesUsed = 0;
  mmc::initialize();

  if (microfat::initialize(buffer))
  {

    if (microfat::locateFileStart(filename, sector, length))
    {
      if (length > BYTESPERSECTOR) {
        sectors = length / BYTESPERSECTOR; 
        length = BYTESPERSECTOR;
      }

      for( long j = 0; j <= sectors; j++) 
        if (RES_OK == mmc::readSectors(buffer, sector + j, 1))
        {
          if (length > BYTESPERSECTOR)
            length = BYTESPERSECTOR;

          for(int i = 0; i < length; ++i)
          {
            if (buffer[i] == EOF) {
              return bytesUsed;
            }
            bytesUsed++;
          }

        } 
        else return 0;
    } 
    else return 0;
  } 
  else return 0;
  return bytesUsed;
}


// (fc)
// returns the buffer;
char* SDuFAT::readBytes(const char* filename, long sectorNum)
{
  mmc::initialize();

  if (microfat::initialize(buffer))
  {

    if (microfat::locateFileStart(filename, sector, length))
    {
      if (length > BYTESPERSECTOR) {
        sectors = length / BYTESPERSECTOR; 
        length = BYTESPERSECTOR;
      }
/*
      Serial.print("First sector of the file: ");
      Serial.print(sector);

      Serial.print("Total sectors: ");
      Serial.println(sectors);

      Serial.print("Reading sector: ");
      Serial.println(sector + sectorNum);
*/
      // read 1 sector to the buffer;
      if (RES_OK == mmc::readSectors(buffer, sector+sectorNum, 1))
      {
         // do nothing;
      } 
      else return 0;
    } 
    else return 0;
  } 
  else return 0;

//  Serial.println();

  return (char*) buffer;
}


// del - erases the file given as parameter for real it just writes the first character of
//       the file's blocks with EOF, the rest will be kept in the card. It could be used
//       to recover data the old-school way
int SDuFAT::del(const char* filename)
{
  mmc::initialize();

  if (microfat::initialize(buffer))
  {

    if (microfat::locateFileStart(filename, sector, length))
    {
      sectors = length / BYTESPERSECTOR;

      for(long m = 0; m <= sectors; m++) {
        if (RES_OK == mmc::readSectors(buffer, sector + m, 1))
        {
          buffer[0] = EOF;

          for (int i = 0; i < 10; ++i)
          {
            if (RES_OK == mmc::writeSectors(buffer, sector + m, 1))
            {
            } 
            else return 1;
          }
        } 
        else return 1;
      }
    } 
    else return 1;
  } 
  else return 1;
  return 0;
}



// ls - prints info about the file to the screen
int SDuFAT::ls(const char* filename)
{
  mmc::initialize();

  if (microfat::initialize(buffer))
  {

    if (microfat::locateFileStart(filename, sector, length))
    {
      Serial.println("length\tsectors\tused");
      Serial.print(length);
      Serial.print("B\t");

      if (length > BYTESPERSECTOR) {
        length = BYTESPERSECTOR;
      }

      sectors = length / BYTESPERSECTOR; 
      Serial.print(sectors);
      Serial.print("\t");

      long bytesUsed = 0;
      for( long j = 0; j <= sectors; j++) 
        if (RES_OK == mmc::readSectors(buffer, sector + j, 1))
        {
          if (length > BYTESPERSECTOR)
            length = BYTESPERSECTOR;

          for(int i = 0; i < length; ++i)
          {
            if (buffer[i] == EOF) {
              Serial.print(bytesUsed);
              Serial.println("B");
              return 0;
            }
            bytesUsed++;
          }

        } 
        else return 1;
    } 
    else return 2;
  } 
  else return 3;
  return 0;
}


SDuFAT SD = SDuFAT();

