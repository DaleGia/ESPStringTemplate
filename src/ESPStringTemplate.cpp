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
  this->filename = filename;
  this->content = SPIFFS.open(this->filename, "w+");
  this->content.close();
  return;
}

void ESPStringTemplate::add_P(PGM_P stringToAdd)
{
  this->content = SPIFFS.open(this->filename, "a");
  for(int ii = 0; ii < strlen_P(stringToAdd); ii++)
  {
    this->content.write((char)pgm_read_byte(stringToAdd+ii));
  }
  this->content.close();
}


void ESPStringTemplate::add(const char* stringToAdd)
{
  this->content = SPIFFS.open(this->filename, "a");
  for(int ii = 0; ii < strlen(stringToAdd); ii++)
  {
    this->content.write((char)*(stringToAdd+ii));
  }
  this->content.close();
}

void ESPStringTemplate::add_P(PGM_P stringToAdd, const char* token, const char* string)
{
  String editString;
  for(int ii = 0; ii < strlen_P(stringToAdd); ii++)
  {
    editString += (char)pgm_read_byte(stringToAdd+ii);
  }

  editString.replace(token, string);
  this->content = SPIFFS.open(this->filename, "a");
  const char* stringPointer = editString.c_str();
  for(int ii = 0; ii < editString.length(); ii++)
  {
    this->content.write((char)*(stringPointer+ii));
  }
  this->content.close();
}


void ESPStringTemplate::add(const char* stringToAdd, const char* token, const char* string)
{
  String editString = stringToAdd;
  editString.replace(token, string);
  this->content = SPIFFS.open(this->filename, "a");
  const char* stringPointer = editString.c_str();
  for(int ii = 0; ii < editString.length(); ii++)
  {
    this->content.write((char)*(stringPointer+ii));
  }
  this->content.close();
}

void ESPStringTemplate::add_P(PGM_P stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  String editString;
  for(int ii = 0; ii < strlen_P(stringToAdd); ii++)
  {
    editString += (char)pgm_read_byte(stringToAdd+ii);
  }

  for(int ii = 0; ii < numberOfPairs; ii++)
  {
    editString.replace(pairList[ii].getToken(), pairList[ii].getString());
  }
  
  this->content = SPIFFS.open(this->filename, "a");
  const char* stringPointer = editString.c_str();
  for(int ii = 0; ii < editString.length(); ii++)
  {
    this->content.write((char)*(stringPointer+ii));
  }
  this->content.close();
}

void ESPStringTemplate::add(const char* stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  String editString = stringToAdd;
  for(int ii = 0; ii < numberOfPairs; ii++)
  {
    editString.replace(pairList[ii].getToken(), pairList[ii].getString());
  }

  this->content = SPIFFS.open(this->filename, "a");
  const char* stringPointer = editString.c_str();
  for(int ii = 0; ii < editString.length(); ii++)
  {
    this->content.write((char)*(stringPointer+ii));
  }
  this->content.close();
}

void ESPStringTemplate::clear(void)
{
  String filename;
  filename = this->content.name();
  SPIFFS.remove(filename.c_str());
  this->content = SPIFFS.open(this->filename, "w");
  this->content.close();
  return;
}

const char* ESPStringTemplate::getFilename(void)
{
  return this->filename;
}
  
