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

ESPStringTemplate::ESPStringTemplate(const char* fileName)
{
  this->espFlash = new ESPFlash<char>(fileName);
  this->espFlash->clear();
  return;
}

ESPStringTemplate::~ESPStringTemplate()
{
  this->espFlash->clear();
  delete this->espFlash;
  return;
}
  
bool ESPStringTemplate::add_P(PGM_P stringToAdd)
{
  size_t bytesToWrite;
  bool success;
  
  success = false;
  bytesToWrite = strlen_P(stringToAdd);
  success = this->espFlash->appendElements_P(stringToAdd, bytesToWrite);

  return success;
}


bool ESPStringTemplate::add(const char* stringToAdd)
{
  size_t bytesToWrite;
  bool success;
  
  success = false;
  bytesToWrite = strlen(stringToAdd);
  success = this->espFlash->appendElements(stringToAdd, bytesToWrite);

  return success;
}

bool ESPStringTemplate::add_P(PGM_P stringToAdd, const char* token, const char* string)
{
  size_t bytesToWrite;
  bool success;

  success = false;
  bytesToWrite = strlen_P(stringToAdd);
  char buffer[bytesToWrite];
  strcpy_P(buffer, stringToAdd);
  String bufferString(buffer);
  bufferString.replace(token, string);
  success = this->espFlash->appendElements(bufferString.c_str(), bufferString.length());
  
  return success;
}


bool ESPStringTemplate::add(const char* stringToAdd, const char* token, const char* string)
{
  bool success;

  success = false;
  String bufferString(stringToAdd);
  bufferString.replace(token, string);
  success = this->espFlash->appendElements(bufferString.c_str(), bufferString.length());
  
  return success;
}

bool ESPStringTemplate::add_P(PGM_P stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  size_t bytesToWrite;
  bool success;

  success = false;
  bytesToWrite = strlen_P(stringToAdd);
  char buffer[bytesToWrite];
  strncpy_P(buffer, stringToAdd, bytesToWrite);

  String bufferString(buffer);
  for(int ii = 0; ii < numberOfPairs; ii++)
  {
    bufferString.replace(pairList[ii].getToken(), pairList[ii].getString());
  }
  success = this->espFlash->appendElements(bufferString.c_str(), bufferString.length());
  
  return success;
}

bool ESPStringTemplate::add(const char* stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  bool success;

  success = false;
  String bufferString(stringToAdd);
  for(int ii = 0; ii < numberOfPairs; ii++)
  {
    bufferString.replace(pairList[ii].getToken(), pairList[ii].getString());
  }
  success = this->espFlash->appendElements(bufferString.c_str(), bufferString.length());
  
  return success;
}

void ESPStringTemplate::clear(void)
{
  this->espFlash->clear();
  return;
}

const char* ESPStringTemplate::getFileName(void)
{
  return this->espFlash->getFileName();
}
  
