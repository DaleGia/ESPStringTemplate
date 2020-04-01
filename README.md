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
- Using reusable HTML elements with a simple token replacement.
```c++
#include "ESPStringTemplate.h"
static const char _PAGEHEADER[] = "<html><body>";
static const char _CONTENT[]    = "%CONTENT%";
static const char _PAGEFOOTER[] = "</body></html>";

static char[50] buffer;
ESPStringTemplate webpage(buffer, sizeof(buffer));
TokenStringPair pair;
webpage.add(_PAGEHEADER);
pair.setPair("%CONTENT%", "TEST CONTENT");
webpage.add(_CONTENT, &pair);
webpage.add(_PAGEFOOTER);
```
- Using reusable HTML elements with a multiple token replacement.
```c++
#include "ESPStringTemplate.h"
static const char _PAGEHEADER[] = "<html><body>";
static const char _CONTENTA[]   = "%CONTENTA% and %CONTENTB% and %CONTENTC%";
static const char _PAGEFOOTER[] = "</body></html>";

static char[100] buffer;
ESPStringTemplate webpage(buffer, sizeof(buffer));
TokenStringPair pair[3];
webpage.add(_PAGEHEADER);
pair[0].setPair("%CONTENTA%", "Replacing this token");
pair[1].setPair("%CONTENTB%", "this token");
pair[2].setPair("%CONTENTC%", "this last token as well...");
webpage.add(_CONTENT, pair, 3);
webpage.add(_PAGEFOOTER);
```
