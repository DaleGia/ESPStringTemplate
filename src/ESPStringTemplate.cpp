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
  clear();
  return;
}

bool ESPStringTemplate::add(const __FlashStringHelper* stringToAdd)
{
  if(!this->overflowFlag)
  {
    if(strlen_P((PGM_P)stringToAdd) < bufferLeft())
    {
      strcat_P(this->buffer, (PGM_P)stringToAdd);
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
  if(!this->overflowFlag)
  {
    if(strlen(stringToAdd) < bufferLeft())
    {
      strcat(this->buffer, stringToAdd);
    }
    else
    {
      handleOverflow();
    }
  }
  
  return this->overflowFlag;
}

bool ESPStringTemplate::add(const __FlashStringHelper* stringToAdd, TokenStringPair* pair)
{
  char* destPointer;   
  if(!this->overflowFlag)
  {
    if(strlen_P((PGM_P)stringToAdd) < bufferLeft())
    {
      destPointer = strcat_P(this->buffer, (PGM_P)stringToAdd);
      replace(destPointer, pair->getToken(), pair->getString());
    }
    else
    {
      handleOverflow();
    }
  }  
  return this->overflowFlag;
}  

bool ESPStringTemplate::add(const char* stringToAdd, TokenStringPair* pair)
{
  char* destPointer;   
  if(!this->overflowFlag)
  {
    if(strlen_P((PGM_P)stringToAdd) < bufferLeft())
    {
      destPointer = strcat(this->buffer, (PGM_P)stringToAdd);
      replace(destPointer, pair->getToken(), pair->getString());
    }
    else
    {
      handleOverflow();
    }
  }  
  return this->overflowFlag;
}  

bool ESPStringTemplate::add(const __FlashStringHelper* stringToAdd, TokenStringPair pairList[], size_t numberOfPairs)
{
  char* destPointer;
  int ii;   
  if(!this->overflowFlag)
  {
    if(strlen_P((PGM_P)stringToAdd) < bufferLeft())
    {
      destPointer = 
        strcat_P(this->buffer, (PGM_P)stringToAdd);
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
  int ii;   
  if(!this->overflowFlag)
  {
    if(strlen(stringToAdd) < bufferLeft())
    {
      destPointer = 
        strcat(this->buffer, stringToAdd);
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
  size_t currentBufferSize;
  size_t bufferLeft;
  
  currentBufferSize = strlen(this->buffer);
  bufferLeft = this->bufferLength - currentBufferSize;
  return bufferLeft;
}

void ESPStringTemplate::handleOverflow(void)
{
  memset(this->buffer, 0, this->bufferLength);
  add(PSTR("overflow detected"));
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

      if(newBufferLeft > 0)
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
  
