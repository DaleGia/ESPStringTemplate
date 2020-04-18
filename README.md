# ESPStringTemplate
Lightweight string templating library for building static web pages on the ESP8266

ESPStringTemplate is a lightweight library that was created to make static web page generation simple and easy to understand on the ESP8266. It was created in an attempt to make building customisable and scalable web pages fast and easy while using flash memory to store the created web pages. 

## Features
- Simple interface to add string/HTML elements to files stored in flash memory using SPIFFS.
- Ability to replace multiple string tokens in HTML elements that are added to the file.
- Ability to utilise flash memory for string/HTML elements used to build web pages.

## Installation
Download this file as a zip, and extract the resulting folder into your Arduino Libraries folder. See [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries). Alternatively, use the Arduino IDE and library manager to find and install ESPStringTemplate.

## Examples
- Simple example - Using string literals stored in RAM to build a web page that is stored in flash.
```c++
SPIFFS.begin();
/* Filename must be less than 32 characters long */
ESPStringTemplate webpage("webpage.html");
webpage.add("Hi!<br><br>");
webpage.add("This is an example of the ESPStringTemplate Library.<br><br>");
webpage.add("These strings are stored in RAM, and are appended to a file stored in flash using SPIFFS.<br><br>");
```
- Using string literals stored in flash (program memory) to build a web page that is stored in flash (filesystem).
```c++
SPIFFS.begin();
/* Filename must be less than 32 characters long */
ESPStringTemplate webpage("webpage.html");
webpage.add_P(PSTR("This is an example of the ESPStringTemplate Library.<br><br>");
webpage.add_P(PSTR("This string is stored in flash using PROGMEM, and this appended to a file stored in flash using SPIFFS."));
```
- Using reusable HTML elements with a simple token replacement.
```c++
static const char _PAGEHEADER[] PROGMEM = "<html><body>";
static const char _CONTENT[]    PROGMEM = "%CONTENT%";
static const char _PAGEFOOTER[] PROGMEM = "</body></html>";

SPIFFS.begin();
/* Filename must be less than 32 characters long */
ESPStringTemplate webpage("webpage.html");
webpage.add_P(_PAGEHEADER);
webpage.add_P(_CONTENT, "%CONTENT%", "TEST CONTENT");
webpage.add_P(_PAGEFOOTER);
```
- Using reusable HTML elements with a multiple token replacement.
```c++
static const char _PAGEHEADER[] PROGMEM = "<html><body>";
static const char _CONTENT[]   PROGMEM = "%CONTENTA% and %CONTENTB% and %CONTENTC%";
static const char _PAGEFOOTER[] PROGMEM = "</body></html>";

SPIFFS.begin();
/* Filename must be less than 32 characters long */
ESPStringTemplate webpage("webpage.html");
TokenStringPair pair[3];
webpage.add_P(_PAGEHEADER);
pair[0].setPair("%CONTENTA%", "Replacing this token");
pair[1].setPair("%CONTENTB%", "this token");
pair[2].setPair("%CONTENTC%", "this last token as well...");
webpage.add_P(_CONTENT, pair, 3);
webpage.add_P(_PAGEFOOTER);
```
- Using ESPAsyncWebServer to serve created webpage
```c++
SPIFFS.begin();
WiFi.softAP("ESPStringTemplate Example");

AsyncWebServer server(80);
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
{
  /* Filename must be less than 32 characters long */
  ESPStringTemplate webpage("webpage.html");
  webpage.add("Hi!");
  /* Send the webpage from SPIFFS where it is stored. */
  request->send(SPIFFS, webpage.getFilename(), "text/html");
}
server.begin();
```

## Further Examples  
[SimpleStaticPageBuilder.ino](examples/SimpleStaticPageBuilder/SimpleStaticPageBuilder.ino)
[StaticPageBuilder.ino](examples/StaticPageBuilder/StaticPageBuilder.ino)

