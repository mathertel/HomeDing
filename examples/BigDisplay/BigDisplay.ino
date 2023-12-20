/**
 * @file The BigDisplay.ino
 * @brief The BigDisplay Sketch that uses the HomeDing Library to implement Things attached the
 * Internet.
 * This example includes a driver for some RGB panel that works with the DMA interface of a ESP32-S3 module.
 * All default from the standard sketch are included by default.
 *
 * See README.md for more details.
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
 * * 01.01.2021 created from standard sketch.
 */

#define DBG_TRACE  // trace level for all elements

// ===== HomeDing Configuration : Enable Elements for the firmware

#define HOMEDING_REGISTER 1

// Enable the following element groups of the HomeDing Library
#define HOMEDING_INCLUDE_SYSTEM
#define HOMEDING_INCLUDE_CORE
#define HOMEDING_INCLUDE_FULL_SYSTEM

// Enable some Sensor Elements
#define HOMEDING_INCLUDE_DHT
#define HOMEDING_INCLUDE_AM2320
#define HOMEDING_INCLUDE_SHT20
#define HOMEDING_INCLUDE_AHT20
#define HOMEDING_INCLUDE_DALLAS
#define HOMEDING_INCLUDE_BMP280
#define HOMEDING_INCLUDE_BME680
#define HOMEDING_INCLUDE_BH1750
#define HOMEDING_INCLUDE_SCD4X

// The PMS uses SoftwareSerial Library that requires more IRAM.
// When using, please switch the MMU: Options to give more IRAM
// #define HOMEDING_INCLUDE_PMS

// Enable some INPUT Elements
#define HOMEDING_INCLUDE_ROTARY
#define HOMEDING_INCLUDE_MENU

// Enable some TIME Elements
#define HOMEDING_INCLUDE_DSTIME

// Enable Elements for Displays
#define HOMEDING_INCLUDE_DISPLAY // all elements that can be displayed
// #define HOMEDING_INCLUDE_DISPLAYLCD
// #define HOMEDING_INCLUDE_DISPLAYSSD1306
// #define HOMEDING_INCLUDE_DISPLAYSH1106

// enable these lines to get more displays supported
#define HOMEDING_INCLUDE_DISPLAYGC9A01
#define HOMEDING_INCLUDE_DISPLAYST7796
#define HOMEDING_INCLUDE_DISPLAYESP32PANEL
#define HOMEDING_INCLUDE_DISPLAYST7789

// #define HOMEDING_INCLUDE_DISPLAYST7735
// #define HOMEDING_INCLUDE_DISPLAYMAX7219
#define HOMEDING_INCLUDE_DISPLAYTOUCHGT911
#define HOMEDING_INCLUDE_DISPLAYTOUCHFT6336
#define HOMEDING_INCLUDE_DISPLAYTOUCHCST816
// Enable simple display Elements
// #define HOMEDING_INCLUDE_TM1637

// Enable Elements for LIGHT control
#define HOMEDING_INCLUDE_COLOR
#define HOMEDING_INCLUDE_LIGHT
#define HOMEDING_INCLUDE_NEOPIXEL
#define HOMEDING_INCLUDE_APA102
#define HOMEDING_INCLUDE_MY9291

// Network Services
#define HOMEDING_INCLUDE_MQTT
#define HOMEDING_INCLUDE_WEATHERFEED
#define HOMEDING_INCLUDE_SDMMC
#define HOMEDING_INCLUDE_SD

#include <Arduino.h>
#include <HomeDing.h>

#include "esp_partition.h"

#include <FS.h>
#include <FFat.h>      // File System for Web Server Files
#include <LittleFS.h>  // File System for Web Server Files

#include <BuiltinHandler.h>  // Serve Built-in files
#include <BoardServer.h>     // Web Server Middleware for Elements
#include <FileServer.h>      // Web Server Middleware for UI

// ===== WLAN credentials =====

// WebServer on port 80 to reach Web UI and services
WebServer server(80);

// ===== implement =====

/**
 * Setup all components and Serial debugging helpers
 */
void setup(void) {
  fs::FS *fs = nullptr;
  Serial.begin(115200);

#ifdef DBG_TRACE
  // wait so the serial monitor can capture all output.
  delay(3000);
  // sometimes configuring the logger_level in the configuration is too late. Then patch loglevel here:
  Logger::logger_level = LOGGER_LEVEL_TRACE;
#endif

  Serial.setDebugOutput(false);


#if defined(ESP8266)
  fs = &LittleFS;

#elif defined(ESP32)
  // ----- check partitions for finding the fileystem type -----
  esp_partition_iterator_t i;

  if (i = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, nullptr)) {
    fs = &FFat;
  } else if (i = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, nullptr)) {
    fs = &LittleFS;
  }
  esp_partition_iterator_release(i);
#endif


  // ----- setup the platform with webserver and file system -----
  homeding.init(&server, fs, "BigDisplay");

  // ----- adding web server handlers -----

  // Builtin Files
  server.addHandler(new BuiltinHandler(&homeding));

  // Board status and actions
  server.addHandler(new BoardHandler(&homeding));

  // UPLOAD and DELETE of static files in the file system.
  server.addHandler(new FileServerHandler(&homeding));

  LOGGER_INFO("setup done");
}  // setup


// handle all give time to all Elements and active components.
void loop(void) {
  server.handleClient();
  homeding.loop();
}  // loop()


// end.
