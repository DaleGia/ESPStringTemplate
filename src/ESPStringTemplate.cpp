/*
ESPStringTemplate.cpp
Copyright (c) 2020 Dale Giancono. All rights reserved..
This file defines the ESPStringTemplate class. It can be used
to build the conent of statically allocated strings and have "tokens" 
within those strings replaced with substrings. It is particularly 
useful as a static web page generator.

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

static const char overflowString[] PROGMEM = "<html><body>overflow detected. Try increasing ESPStringTemplate buffer size.</body></html>";

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

ESPStringTemplate::ESPStringTemplate(char* templateBuffer, uint32_t templateBufferSize)
{
  this->buffer = templateBuffer;
  this->bufferEndPointer = templateBuffer+templateBufferSize-1;
  this->bufferLength = templateBufferSize;
  this->overflowFlag = false;
  this->bufferIndex = 0;
  clear();
  return;
}

bool ESPStringTemplate::add_P(PGM_P stringToAdd)
{
  size_t stringToAddLength;
  if(!this->overflowFlag)
  {
    stringToAddLength = strlen_P(stringToAdd);
    if(stringToAddLength < bufferLeft())
    {
      strcat_P(this->buffer, stringToAdd);
      this->bufferIndex += stringToAddLength;
    }
    else
    {
      handleOverflow();
    }
  }
  
  return this->overflowFlag;
}


bool ESPStringTemplate::add(const char* stringToAdd)
{
  size_t stringToAddLength;
  if(!this->overflowFlag)
  {
    stringToAddLength = strlen(stringToAdd);
    if(stringToAddLength < bufferLeft())
    {
      strcat(this->buffer, stringToAdd);
      this->bufferIndex += stringToAddLength;
    }
    else
    {
      handleOverflow();
    }
  }
  
  return this->overflowFlag;
}

bool ESPStringTemplate::add_P(PGM_P stringToAdd, const char* token, const char* string)
{  
  char* destPointer;   
  size_t stringToAddLength;
  if(!this->overflowFlag)
  {
    stringToAddLength = strlen_P(stringToAdd);
    if(stringToAddLength < bufferLeft())
    {
      destPointer = strcat_P(this->buffer, stringToAdd);
      this->bufferIndex += stringToAddLength;
      replace(destPointer, token, string);
    }
    else
    {
      handleOverflow();
    }
  }  
  return this->overflowFlag;
}


bool ESPStringTemplate::add(const char* stringToAdd, const char* token, const char* string)
{
  char* destPointer;   
  size_t stringToAddLength;
  if(!this->overflowFlag)
  {
    stringToAddLength = strlen(stringToAdd);
    if(stringToAddLength < bufferLeft())
    {
      destPointer = strcat(this->buffer, stringToAdd);
      this->bufferIndex += stringToAddLength;
      replace(destPointer, token, string);
    }
    else
    {
      handleOverflow();
    }
  }  
  return this->overflowFlag;
}

bool ESPStringTemplate::add_P(PGM_P stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  char* destPointer;
  size_t stringToAddLength;
  int ii;   
  if(!this->overflowFlag)
  {
    stringToAddLength = strlen(stringToAdd);
    if(stringToAddLength < bufferLeft())
    {
      destPointer = 
        strcat_P(this->buffer, stringToAdd);
      this->bufferIndex += stringToAddLength;
      for(ii = 0; ii < numberOfPairs; ii++)
      {
        replace(
          destPointer, 
          pairList[ii].getToken(), 
          pairList[ii].getString()); 
        if(this->overflowFlag)
        {
          break;
        }
      }
    }
    else
    {
      handleOverflow();
    }
  }     
  return this->overflowFlag;
}


bool ESPStringTemplate::add(const char* stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  char* destPointer;
  size_t stringToAddLength;
  int ii;   
  if(!this->overflowFlag)
  {
    stringToAddLength = strlen(stringToAdd);

    if(stringToAddLength < bufferLeft())
    {
      destPointer = 
        strcat(this->buffer, stringToAdd);
      this->bufferIndex += stringToAddLength;
      for(ii = 0; ii < numberOfPairs; ii++)
      {
        replace(
          destPointer, 
          pairList[ii].getToken(), 
          pairList[ii].getString()); 
        if(this->overflowFlag)
        {
          break;
        }
      }
    }
    else
    {
      handleOverflow();
    }
  }     
  return this->overflowFlag;
}

void ESPStringTemplate::clear(void)
{
  memset(buffer, 0, this->bufferLength);
  this->bufferIndex = 0;
  this->overflowFlag = false;
  return;
}

const char* ESPStringTemplate::get(void)
{
  return buffer;
}

bool ESPStringTemplate::isOverflow(void)
{
  return this->overflowFlag;
}

size_t ESPStringTemplate::bufferLeft(void)
{
  size_t bufferLeft;
  bufferLeft = this->bufferLength - this->bufferIndex;
  return bufferLeft;
}

void ESPStringTemplate::handleOverflow(void)
{
  memset(this->buffer, 0, this->bufferLength);
  add_P(overflowString);
  this->overflowFlag = true;
  return;
}  

bool ESPStringTemplate::replace(char* stringToEdit, const char* oldSubstring, const char* newSubstring)
{
  size_t oldSize;
  size_t newSize;
  size_t newBufferLeft;
  int moveSize;
  char* foundPointer;
  if(!this->overflowFlag)
  {
    /* Get the key and string size so we can correctly calculate
     *  the correct pointer posisitons to write to the string array.
     */
    oldSize = strlen(oldSubstring);
    newSize = strlen(newSubstring);
    
    foundPointer = strstr(stringToEdit, oldSubstring);
    while(NULL != foundPointer)
    {
      newBufferLeft = bufferLeft()-(newSize-oldSize);
      if((0 < newBufferLeft) && (this->bufferIndex < newBufferLeft))
      {
        moveSize = 
          (this->bufferEndPointer-foundPointer) - newSize;
       /* Move data found after the found substring so it will be in
        *  the correct position for the size of the replacement
        *  substring.
        */
        memmove(foundPointer+newSize, foundPointer+oldSize, moveSize);
       /* Copies the string in to the space that was just created
        *  by the above operation
        */
        memcpy(foundPointer, newSubstring, newSize);
        this->bufferIndex += newSize-oldSize;
        foundPointer = strstr(stringToEdit, oldSubstring);
      }
      else
      {
        handleOverflow();
        break;
      }
    }
  }
  return this->overflowFlag;
}
