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

/* Store different HTML elements in flash. Descriptions of the various 
tokens are included above each element that has tokens.*/
static const char _PAGEHEADER[] PROGMEM = "<!DOCTYPE html><html><body>";
/*%TITLE% token refers to the text of the element */
static const char _TITLE[]      PROGMEM = "<hr><h1>%TITLE%</h1><hr>";
/*%ACTION% token refers to the uri that is called from the form.  */
/*%VALUE% token refers to the text that will be inside the button
 as well as the parameter value used in the HTTP_GET request. */
static const char _BUTTON[]     PROGMEM = "<form action=\"%ACTION%\"><input type=\"submit\" value=\"%VALUE%\" name=\"event\"></form>";
/*%LINEEDITNAME% token refers to the text that will be inside the lineedit
 as well as the parameter value used in the HTTP_GET request. */
static const char _LINEEDIT[]   PROGMEM = "<form action=\"/\"><input type=\"text\" name=\"%LINEEDITNAME%\"><input type=\"Submit\"  value=\"submit\" name=\"event\"></form>";
static const char _LINE[]       PROGMEM = "<hr>";
static const char _PAGEFOOTER[] PROGMEM = "</body></html>";

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
  WiFi.softAP("ESPStringTemplate example");
  /* Define the handler for when the server receives a GET request for the root uri. */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    /* Create instance of StringTemplate, TokenStringPair, and a TokenStringPair array
    that will be used to build the site.*/
    ESPStringTemplate testTemplate(buffer, sizeof(buffer));
    TokenStringPair pair;
    TokenStringPair pairArray[2];

    /* Add page header string to the template. */
    testTemplate.add_P(_PAGEHEADER);
    /* Set tokenStringPair to replace %TITLE% token with "StringTemplate Example"*/
    /* This is set here as it is meant to be the default value */
    String title = "StringTemplate Example";
    /* Check with a parameter named "event" was included in the GET request. */ 
    if(request->hasParam("event"))
    {
      AsyncWebParameter* p = request->getParam("event");
      /* Check if the value of the "event" parameter is "lineeditsubmit". If it is, 
      get the value of the parameter amd set tokenStringPair to replace %TITLE% token with the value */
      if(p->value() == "submit")
      {
        if(request->hasParam("lineedittext"))
        {
          p = request->getParam("lineedittext");
          title = p->value();
        }
      }
    }
    /* Add the __TITLE string to the StringTemplate and use the TokenStringPair to replace
     *  specific token strings.*/
    testTemplate.add_P(_TITLE, "%TITLE%", title.c_str());
    /* Add the _LINEEDIT string to the StringTemplate and use the TokenStringPair to replace
     *  specific token strings.*/
    testTemplate.add_P(_LINEEDIT, "%LINEEDITNAME%", "lineedittext");
    /* Add the _LINE string to the StringTemplate */
    testTemplate.add_P(_LINE);
      
    /* Set the various TokenStringPairs in the pairArray to replace tokens in the _LINEEDIT string */
    pairArray[0] = TokenStringPair("%ACTION%", "/");
    pairArray[1] = TokenStringPair("%VALUE%", "reset");
    /* Add the _BUTTON string to the StringTemplate and use the TokenStringPair to replace
     *  specific token strings.*/
    testTemplate.add_P(_BUTTON, pairArray, 2);
    /* Add the _PAGEFOOTER string to the StringTemplate */
    testTemplate.add_P(_PAGEFOOTER);   
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
