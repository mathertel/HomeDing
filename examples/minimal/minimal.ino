/**
 * @file Minimal.ino
 * @brief Minimal Sketch that uses the HomeDing Library to implement Things attached the
 * Internet.
 * 
 * The use-case covered with this Sketch is to compile a small firmware
 * with the elements for a devices with 1MByte flash without displays like
 * * switchable plugs and sockets including capturing power consumption
 * * bulbs
 * * temperature sensors
 * * led stripe controllers with single color
 * * led stripe controllers with neopixel
 *  
 * Compile with
 * * Board: Generic ESP8266 module
 * * Flash Size: 1M (FS:128kb, OTA:~438KB)
 * * Flash Mode: DOUT 
 * * Debug Port: "Disabled"
 * * Debug Level: "None"
 * * MMU: 32+32 balanced
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
 * * 10.02.2022 remove using SPIFFS in favour of LittleFS - saving code space.
 */

// ----- activatable debug options

// #define DBG_TRACE // trace level for all elements
// #define NET_DEBUG // show network event in output

// ===== HomeDing Configuration : Enable Elements for the firmware

#define HOMEDING_REGISTER 1

// Enable the Core Elements of the HomeDing Library
#define HOMEDING_INCLUDE_SYSTEM

// Enable some Core Elements for small devices
// This collection may be used in e.g. remote controllable plugs.
#define HOMEDING_INCLUDE_Value
#define HOMEDING_INCLUDE_Button
#define HOMEDING_INCLUDE_Switch
#define HOMEDING_INCLUDE_DigitalIn
#define HOMEDING_INCLUDE_DigitalOut

#define HOMEDING_INCLUDE_AND
#define HOMEDING_INCLUDE_REFERENCE
#define HOMEDING_INCLUDE_Timer
#define HOMEDING_INCLUDE_Schedule
#define HOMEDING_INCLUDE_Alarm
#define HOMEDING_INCLUDE_REMOTE


// Enable some Sensor Elements
#define HOMEDING_INCLUDE_DHT
#define HOMEDING_INCLUDE_BL0937

// Enable Elements for LIGHT control
#define HOMEDING_INCLUDE_COLOR
#define HOMEDING_INCLUDE_LIGHT
#define HOMEDING_INCLUDE_NEOPIXEL
#define HOMEDING_INCLUDE_MY9291

#include <Arduino.h>
#include <HomeDing.h>

#include <FS.h> // File System for Web Server Files
#include <LittleFS.h> // File System for Web Server Files

#include <BuiltinHandler.h> // Serve Built-in files
#include <BoardServer.h> // Web Server Middleware for Elements
#include <FileServer.h> // Web Server Middleware for UI


// ===== WLAN credentials =====

#include "secrets.h"

// WebServer on port 80 to reach Web UI and services
WebServer server(80);

// HomeDing core functionality
Board mainBoard;


// ===== implement =====

/**
 * Setup all components and Serial debugging helpers
 */
void setup(void) {
  Serial.begin(115200);

#ifdef NET_DEBUG
  Serial.setDebugOutput(true);
#else
  Serial.setDebugOutput(false);
#endif

#ifdef DBG_TRACE
  // wait so the serial monitor can capture all output.
  delay(3000);
  // sometimes configuring the logger_level in the configuration is too late. Then patch loglevel here:
  Logger::logger_level = LOGGER_LEVEL_TRACE;
#endif

  // ----- setup the platform with webserver and file system -----

  mainBoard.init(&server, &LittleFS, "minimal");

  // ----- adding web server handlers -----

  // Builtin Files
  server.addHandler(new BuiltinHandler(&mainBoard));

  // Board status and actions
  server.addHandler(new BoardHandler(&mainBoard));

  // UPLOAD and DELETE of static files in the file system.
  server.addHandler(new FileServerHandler(*mainBoard.fileSystem, &mainBoard));

  LOGGER_INFO("setup done.");
} // setup


// handle all give time to all Elements and active components.
void loop(void) {
  server.handleClient();
  mainBoard.loop();
} // loop()


// end.
