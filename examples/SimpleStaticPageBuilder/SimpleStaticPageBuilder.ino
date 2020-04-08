/*
ESPStringTemplate example

Copyright (c) 2020 Dale Giancono. All rights reserved..
This file is a sample application of the ESPStringTemplate class
that uses HTML string elements stored in ram and flash to build a web page
that is stored and accessed in flash using SPIFFS. 
The example serves text based webpage. 

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

#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include "ESPStringTemplate.h"

/* Create webserver instance for serving the StringTemplate example. */
AsyncWebServer server(80);

void setup() 
{
  /* Start SPIFFS so it can be used by ESPStringTemplate */
  SPIFFS.begin();
  /* Configure access point with static IP address */
  WiFi.softAPConfig(
    IPAddress(192,168,4,1),
    IPAddress(192,168,4,1),
    IPAddress(255,255,255,0));
  WiFi.softAP("StringTemplate HTMLBuilder Test");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin();
  /* Define the handler for when the server receives a GET request for the root uri. */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    /* The file name of what the template file will be called in the file system.*/
    const char* templateFileName = "/stringTemplate/template.html";
    /* Create instance (and file_ of the ESPStringTemplate */
    ESPStringTemplate testTemplate(templateFileName);

    /* Add page header string to the template. */
    testTemplate.add("Hi!<br><br>");
    testTemplate.add("Here is a super simple example of how to use the ESPStringTemplate library...<br><br>");
    testTemplate.add("This string is stored in RAM, and is appended to a file stored in flash using SPIFFS.<br><br>");
    testTemplate.add_P(PSTR("This string is stored in flash using PROGMEM, and is appended to the same file stored in flash using SPIFFS.<br><br>"));
    testTemplate.add(
      "This string has basic templating performed on it. The filename of the web page stored in the file system is %FILENAME%.<br><br>",
      "%FILENAME%",
      templateFileName);

    TokenStringPair pairs[2];
    pairs[0].setPair("%TOKEN1%", "STRING1");
    pairs[1].setPair("%TOKEN2%", "STRING2"); 
    testTemplate.add(
      "This string has more advanced templating performed on it. It uses multiple token/string pairs to perform complex templating on a single string. %TOKEN1% %TOKEN2%.",
      pairs,
      2);

    /* Send the webpage from SPIFFS where it is stored. */
    request->send(SPIFFS, testTemplate.getFilename(), "text/html");
  });

  /* Begin the web server */
  server.begin();
}

void loop() 
{
  /* Nothing needed here as async server is used. */
}
