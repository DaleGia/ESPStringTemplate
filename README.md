# ESPStringTemplate
Lightweight string templating library for building static web pages on the ESP8266

ESPStringTemplate is a lightweight library created to make static web page generation simple and easy to understand on the ESP8266. It was created in an attempt to make building customisable and scalable web pages fast, easy, and with more dertermanistic characteristics by utilising statically allocated buffers. 

## Features
- Simple interface to add string/HTML elements to statically allocated buffer with overflow detection (no dynamic memory allocation).
- Abiility to replace multiple string tokens in HTML elements that are added to buffer.
- Ability to utilise flash memory for string/HTML elements used to build web pages.
- Only requires <WString.h> (for the __FlashStringHelper class). 

## Installation
Download this file as a zip, and extract the resulting folder into your Arduino Libraries folder. See [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries). 

## Example
- Simple example - Using string literals to build a page in a statically allocated buffer.
```c++
#include "ESPStringTemplate.h"
/* Statically allocated buffer that will be filled with web page string */
static char[50] buffer;
ESPStringTemplate webpage(buffer, sizeof(buffer));
webpage.add("<html><body>");
webpage.add("<h1>Hello World!</h1>");
webpage.add("</body></html>");
```
- Using string literals stored in flash memory to build the same page.
```c++
#include "ESPStringTemplate.h"
static char[50] buffer;
ESPStringTemplate webpage(buffer, sizeof(buffer));
webpage.add(F("<html><body>"));
webpage.add(F("<h1>Hello World!</h1>"));
webpage.add(F("</body></html>"));
```
```c++
- Using reusable HTML elements stored in flash with a simple token replacement.
#include "ESPStringTemplate.h"
static const char _PAGEHEADER[] PROGMEM = "<html><body>";
static const char _CONTENT[]      PROGMEM = "%CONTENT%";
static const char _PAGEFOOTER[] PROGMEM = "</body></html>";

static char[50] buffer;
ESPStringTemplate webpage(buffer, sizeof(buffer));
TokenStringPair pair;
webpage.add(FPSTR(_PAGEHEADER));
pair.setPair("%CONTENT%", "TEST CONTENT");
webpage.add(FPSTR(_CONTENT), &pair);
webpage.add(FPSTR(_PAGEFOOTER));
```
