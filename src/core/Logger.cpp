/**
 * @file Logger.cpp
 * @brief Implementation of a logging utility that logs to the Serial port and
 * to a buffer to support for http access to error logs.
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 *
 * This work is licensed under a BSD style license,
 * https://www.mathertel.de/License.aspx.
 *
 * More information on https://www.mathertel.de/Arduino.
 *
 * Changelog:
 * see Logger.h
 */

#include <Arduino.h>
#include <Board.h>
#include <Element.h>

#include <stdio.h>

#include <sntp.h>
#include <time.h>

#include <FS.h>

#include <Board.h>
#include <core/Logger.h>

#define LOGFILE_MAXSIZE (4 * 1024 - 200)

// String constants, only once in Memory
static const char *LOGGER_LEVELS = "iet"; // info, error, trace
static const char *LOGFILE_NAME = "/log.txt";
static const char *LOGFILE_OLD_NAME = "/log_old.txt";

/**
 * @brief Print out logging information
 */
void Logger::_print(const char *module, int level, const char *fmt,
                    va_list args)
{
  char buffer[200];
  char *p = buffer; // end of string : next segment of logline

  // timestamp into buffer, using millis() when no time is available
  time_t now = time(nullptr);
  if (!now)
    now = Board::getSeconds();
  p += strftime(p, sizeof(buffer), "%H:%M:%S ", localtime(&now)); // %T

  if (module) {
    // add module and loglevel
    p += sprintf(p, "%s:%c ", module, *(LOGGER_LEVELS + level));
  } else {
    p += sprintf(p, ">");
  }

  // message into buffer
  vsnprintf(p, sizeof(buffer) - 40, fmt, args);

#ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.println(buffer);
#endif

  delay(0);

  if ((module) && (logger_file) && (level < LOGGER_LEVEL_TRACE)) {
    _printToFile(buffer);
    delay(0);
  } // if
} // _print

/**
 * @brief Print Log entry from system (not element)
 */
void Logger::LoggerPrint(const char *module, int level, const char *fmt, ...)
{
  if (level <= logger_level) {
    va_list args;
    va_start(args, fmt);
    Logger::_print(module, level, fmt, args);
    va_end(args);
  } // if
  yield();
} // LoggerPrint


/**
 * @brief Print Log entry from element.
 */
void Logger::LoggerEPrint(Element *elem, int level, const char *fmt, ...)
{
  if ((level <= logger_level) || (level <= elem->loglevel)) {
    va_list args;
    va_start(args, fmt);
    Logger::_print(elem->id, level, fmt, args);
    va_end(args);
  } // if
  yield();
} // LoggerEPrint


void Logger::_printToFile(char *buffer)
{
  File f = SPIFFS.open(LOGFILE_NAME, "a");

  if (f.size() > LOGFILE_MAXSIZE) {
    // rename to LOGFILE_OLD_NAME
    f.close();
    SPIFFS.remove(LOGFILE_OLD_NAME);
    SPIFFS.rename(LOGFILE_NAME, LOGFILE_OLD_NAME);
    yield();
    f = SPIFFS.open(LOGFILE_NAME, "a");
  } // if
  f.println(buffer);
  f.close();
  yield();
};

// Default: Log INFO and ERROR
int Logger::logger_level = LOGGER_LEVEL_ERR;

bool Logger::logger_file = false;

// end.
