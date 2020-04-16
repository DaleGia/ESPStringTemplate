/*
ESPStringTemplate.h
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
#ifndef ESPSTRINGTEMPLATE_H_
#define ESPSTRINGTEMPLATE_H_

#include <WString.h>

/* 
  The TokenStringPair class can be used with the StringTemplate class
  to provide a simple way to replace "tokens" within a string with other
  strings. Pointers to TokenStringPair instances are used as an argument
  in many StringTemplate methods.
*/
class TokenStringPair
{
  public:
    TokenStringPair();
    TokenStringPair(const char* token, const char* string);
    void setPair(const char* token, const char* string);
    void setToken(char*  token);
    void setString(char*  string);
    const char* getToken(void);
    const char* getString(void);
    
  private:
     const char*  token;
     const char*  string;
};

/*
 * The StringTemplate class can be used to add strings to a statically
 * allocated buffer. "Tokens" within added strings can be replaced with 
 * other strings.
 */
class ESPStringTemplate
{
  public:
    ESPStringTemplate(char* templateBuffer, uint32_t templateBufferSize);
  
    bool add_P(PGM_P  stringToAdd);
    bool add(const char* stringToAdd);
    bool add(const char* stringToAdd, const char* token, const char* string);
    bool add_P(PGM_P stringToAdd, const char* token, const char* string);

    bool add(const char* stringToAdd, TokenStringPair pairList[], size_t numberOfPairs);
    bool add_P(PGM_P stringToAdd, TokenStringPair pairList[], size_t numberOfPairs);

    void clear(void);
    const char* get(void);
    bool isOverflow(void);
    
  private:
    char* buffer;
    size_t bufferIndex;
    char* bufferEndPointer;
    size_t bufferLength;
    bool overflowFlag;
  
    size_t bufferLeft(void);
    void handleOverflow(void);
    bool replace(char* stringToEdit, const char* oldSubstring, const char* newSubstring);
};

#endif /*ESPSTRINGTEMPLATE_H_*/
