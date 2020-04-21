/*
ESPStringTemplate example

Copyright (c) 2020 Dale Giancono. All rights reserved..
This file is a sample application of the ESPStringTemplate class
that uses HTML string elements stored flash to build web page stored
and accessed using SPIFFS. The example serves a webpage with a title, 
a line edit, and a submit button. The title of the page can be 
changed by submiting a string with the line edit. Each time the 
submit button is pressed the webpage is statically created using 
the submited line edit string as the title.

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
char buffer[1000];
void setup() 
{
  /* Configure access point with static IP address */
  WiFi.softAPConfig(
    IPAddress(192,168,4,1),
    IPAddress(192,168,4,1),
    IPAddress(255,255,255,0));
  WiFi.softAP("StringTemplate HTMLBuilder Test");
  /* Define the handler for when the server receives a GET request for the root uri. */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    /* Create instance (and file_ of the ESPStringTemplate */
    ESPStringTemplate testTemplate(buffer, sizeof(buffer));

    /* Add page header string to the template. */
    testTemplate.add("Hi!<br><br>");
    testTemplate.add("Here is a super simple example of how to use the ESPStringTemplate library...<br><br>");
    testTemplate.add("This string is stored in RAM, and is added to a statically allocated buffer.<br><br>");
    testTemplate.add_P(PSTR("This string is stored in flash using PROGMEM, and is added to the same statically allocated buffer.<br><br>"));
    testTemplate.add(
      "This string has basic templating performed on it. The size of the statically allocated buffer is %BUFFERSIZE%.<br><br>",
      "%BUFFERSIZE%",
      String(sizeof(buffer)).c_str());

    TokenStringPair pairs[2];
    pairs[0].setPair("%TOKEN1%", "STRING1");
    pairs[1].setPair("%TOKEN2%", "STRING2"); 
    testTemplate.add(
      "This string has more advanced templating performed on it. It uses multiple token/string pairs to perform complex templating on a single string. %TOKEN1% %TOKEN2%.",
      pairs,
      2);

    /* Send the webpage from SPIFFS where it is stored. */
    request->send(200, "text/html", buffer);
  });

  /* Begin the web server */
  server.begin();
}

void loop() 
{
  /* Nothing needed here as async server is used. */
}
