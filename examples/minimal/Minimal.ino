/**
 * @file Minimal.ino
 * @brief Minimal Sketch that uses the HomeDing Library to implement Things attached the
 * Internet.
 * 
 * The primary reason for this Sketch is to have a small firmware with the elements
 * for a simple switching device without displays.
 * 
 * There is minified WebUI available. 
 * 
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 *
 * This work is licensed under a BSD 3-Clause style license,
 * https://www.mathertel.de/License.aspx.
 *
 * More information on https://www.mathertel.de/Arduino
 * and https://homeding.github.io/index.htm#page=/examples/develop.md.
 *
 * Changelog:
 * * 12.11.2019 Minimal Example created from development sketch.
 * * 20.12.2019 updated from DevDing example
 * * 07.04.2019 updated from DevDing example, no sensor elements, no elements that need libraries.
 */

#define HOMEDING_REGISTER 1

#define HOMEDING_INCLUDE_SYSTEM

// Use explicit Elements of the HomeDing Library
#define HOMEDING_INCLUDE_Value
#define HOMEDING_INCLUDE_Button
#define HOMEDING_INCLUDE_Switch
#define HOMEDING_INCLUDE_DigitalIn
#define HOMEDING_INCLUDE_DigitalOut

#define HOMEDING_INCLUDE_AND
#define HOMEDING_INCLUDE_Timer
#define HOMEDING_INCLUDE_Schedule
#define HOMEDING_INCLUDE_Alarm

// #define HOMEDING_INCLUDE_REMOTE

// #define HOMEDING_INCLUDE_PMS
// #define HOMEDING_INCLUDE_LIGHT
// #define HOMEDING_INCLUDE_NEOPIXEL


#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <FS.h>

#include <Board.h>
#include <Element.h>
#include <HomeDing.h>

#include <BoardServer.h>
#include <FileServer.h>

// ===== include project specific elements =====

static const char respond404[] PROGMEM =
    R"==(<html><head><title>File not found</title></head><body>File not found</body></html>)==";

// ===== WLAN credentials =====

#include "secrets.h"

// need a WebServer
ESP8266WebServer server(80);

// ===== application state variables =====

Board mainBoard;

// ===== implement =====

void handleRedirect()
{
  LOGGER_RAW("Redirect...");

  String url;
  if (! mainBoard.isCaptiveMode()) {
    url = mainBoard.homepage;
  } else {
    url = "http://";
    url.concat(WiFi.softAPIP().toString()); // mainBoard.deviceName
    url.concat("/$setup.htm");
  }
  server.sendHeader("Location", url, true);
  server.send(302);
  server.client().stop();
} // handleRedirect()


/**
 * Setup all components and Serial debugging helpers
 */
void setup(void)
{
  Serial.begin(115200);

  Serial.setDebugOutput(false);

  LOGGER_INFO("Device starting...");

  // ----- setup the file system and load configuration -----
  SPIFFS.begin();
  mainBoard.init(&server);
  yield();

  // ----- adding web server handlers -----
  // redirect to index.htm when only domain name is given.
  server.on("/", HTTP_GET, handleRedirect);

  // Board status and actions
  server.addHandler(new BoardHandler(&mainBoard));

  // Static files in the file system.
  server.addHandler(new FileServerHandler(SPIFFS, "NO-CACHE", &mainBoard));
  // GET static files is added after network connectivity is given.

  server.onNotFound([]() {
    const char *uri = server.uri().c_str();
    LOGGER_RAW("notFound: %s", uri);

    if ((mainBoard.boardState == BOARDSTATE_RUNCAPTIVE) &&
        ((strcmp(uri, "/connecttest.txt") == 0) ||
         (strcmp(uri, "/redirect") == 0) ||
         (strcmp(uri, "/generate_204") == 0) ||
         (strcmp(uri, "/more.txt") == 0))) {
      handleRedirect();
    } else {
      // standard not found in browser.
      server.send(404, TEXT_HTML, FPSTR(respond404));
    }
  });

  LOGGER_INFO("setup done.");
} // setup


// handle all give time to all Elements and active components.
void loop(void)
{
  server.handleClient();
  mainBoard.loop();
} // loop()


// end.
