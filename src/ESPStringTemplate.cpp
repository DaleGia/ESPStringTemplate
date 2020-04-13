/*
ESPStringTemplate.cpp

Copyright (c) 2020 Dale Giancono. All rights reserved..
This file defines the ESPStringTemplate class. It can be used
to build the content of strings and have "tokens" 
within those strings replaced with substrings. The content is stored using 
SPIFFS. It is particularly useful as a static web page generator that 
uses low amounts of RAM and FLASH due to it's SPIFFs ussage.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "ESPStringTemplate.h"
#include <FS.h>

TokenStringPair::TokenStringPair()
{
  return;
};

TokenStringPair::TokenStringPair(const char* token, const char* string)
{
  this->token = token;
  this->string = string;
  return;
}

void TokenStringPair::setPair(const char* token, const char* string)
{
  this->token = token;
  this->string = string;     
  return;
}
    
void TokenStringPair::setToken(char* token)
{
  this->token = token;
  return;
}

void TokenStringPair::setString(char* string)
{
  this->string = string;
  return;
}

const char* TokenStringPair::getToken(void)
{
  return this->token;
}

const char* TokenStringPair::getString(void)
{
  return this->string;
}

ESPStringTemplate::ESPStringTemplate(const char* filename)
{
  File file;
  
  this->filename = filename;
  file = SPIFFS.open(this->filename, "w+");
  file.close();
  return;
}

bool ESPStringTemplate::add_P(PGM_P stringToAdd)
{
  File file;
  size_t bytesToWrite;
  size_t bytesWritten;
  bool success;
  
  success = false;
  bytesWritten = 0;
  bytesToWrite = strlen_P(stringToAdd);
  file = SPIFFS.open(this->filename, "a");
  
  if(file)
  {  
    for(int ii = 0; ii < bytesToWrite; ii++)
    {
      bytesWritten += file.write((char)pgm_read_byte(stringToAdd+ii));
    }
    file.close();

    if(bytesWritten == bytesToWrite)
    {
      success = true;
    }
  }

  return success;
}


bool ESPStringTemplate::add(const char* stringToAdd)
{
  File file;
  size_t bytesToWrite;
  size_t bytesWritten;
  bool success;
  
  success = false;
  bytesWritten = 0;
  bytesToWrite = strlen(stringToAdd);
  file = SPIFFS.open(this->filename, "a");

  if(file)
  {  
    for(int ii = 0; ii < bytesToWrite; ii++)
    {
      bytesWritten += file.write((char)*(stringToAdd+ii));
    }
    file.close();
    
    if(bytesWritten == bytesToWrite)
    {
      success = true;
    }
  }
  
  return success;
}

bool ESPStringTemplate::add_P(PGM_P stringToAdd, const char* token, const char* string)
{
  File file;
  size_t bytesToWrite;
  size_t bytesWritten;
  bool success;
  String editString;

  success = false;
  bytesWritten = 0;
  bytesToWrite = strlen_P(stringToAdd);
  
  for(int ii = 0; ii < bytesToWrite; ii++)
  {
    editString += (char)pgm_read_byte(stringToAdd+ii);
  }

  editString.replace(token, string);
  bytesToWrite = editString.length();
  file = SPIFFS.open(this->filename, "a");
  if(file)
  {  
    const char* stringPointer = editString.c_str();
    for(int ii = 0; ii < editString.length(); ii++)
    {
      bytesWritten += file.write((char)*(stringPointer+ii));
    }
    file.close();

    if(bytesWritten == bytesToWrite)
    {
      success = true;
    }
  }

  return success;
}


bool ESPStringTemplate::add(const char* stringToAdd, const char* token, const char* string)
{
  File file;
  String editString;
  size_t bytesToWrite;
  size_t bytesWritten;
  bool success;
  
  success = false;
  bytesWritten = 0;
  bytesToWrite = strlen(stringToAdd);
  editString = stringToAdd;
  
  editString.replace(token, string);
  const char* stringPointer = editString.c_str();
  file = SPIFFS.open(this->filename, "a");
    
  if(file)
  {
    for(int ii = 0; ii < editString.length(); ii++)
    {
      bytesWritten += file.write((char)*(stringPointer+ii));
    }
    file.close();

    if(bytesWritten == bytesToWrite)
    {
      success = true;
    }
  }
}

bool ESPStringTemplate::add_P(PGM_P stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  File file;
  String editString;
  size_t bytesToWrite;
  size_t bytesWritten;
  bool success;
  
  success = false;
  bytesWritten = 0;
  bytesToWrite = strlen_P(stringToAdd);
  editString = stringToAdd;
  
  for(int ii = 0; ii < bytesToWrite; ii++)
  {
    editString += (char)pgm_read_byte(stringToAdd+ii);
  }

  for(int ii = 0; ii < numberOfPairs; ii++)
  {
    editString.replace(pairList[ii].getToken(), pairList[ii].getString());
  }
  
  file = SPIFFS.open(this->filename, "a");
  const char* stringPointer = editString.c_str();
  for(int ii = 0; ii < editString.length(); ii++)
  {
    bytesWritten += file.write((char)*(stringPointer+ii));
  }
  file.close();
  if(file)
  {
    if(bytesWritten == bytesToWrite)
    {
      success = true;
    }
  }
  
  return success;
}

bool ESPStringTemplate::add(const char* stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  File file;
  String editString = stringToAdd;
  for(int ii = 0; ii < numberOfPairs; ii++)
  {
    editString.replace(pairList[ii].getToken(), pairList[ii].getString());
  }

  file = SPIFFS.open(this->filename, "a");
  const char* stringPointer = editString.c_str();
  for(int ii = 0; ii < editString.length(); ii++)
  {
    file.write((char)*(stringPointer+ii));
  }
  file.close();
}

void ESPStringTemplate::clear(void)
{
  File file;
  String filename;
  filename = file.name();
  SPIFFS.remove(filename.c_str());
  file = SPIFFS.open(this->filename, "w");
  file.close();
  return;
}

const char* ESPStringTemplate::getFilename(void)
{
  return this->filename;
}
  
