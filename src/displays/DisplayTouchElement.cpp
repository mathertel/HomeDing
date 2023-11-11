/**
 * @file DisplayTouchElement.cpp
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 * This work is licensed under a BSD 3-Clause style license, see https://www.mathertel.de/License.aspx
 *
 * More information on https://www.mathertel.de/Arduino
 *
 * Changelog:see DisplayTouchElement.h
 */

#include <Arduino.h>
#include <HomeDing.h>

#include <displays/DisplayButtonElement.h>

#include "DisplayTouchElement.h"

#include <Wire.h>


#define TRACE(...) LOGGER_ETRACE(__VA_ARGS__)

/* ===== Private functions ===== */

/* ===== Element functions ===== */

DisplayTouchElement::DisplayTouchElement() {
  // adjust startupMode when Network (default) is not applicable.
  // startupMode = Element_StartupMode::System;
}

bool DisplayTouchElement::pullSensorData() {
  return (false);
}


void DisplayTouchElement::init(Board *board) {
  TRACE("init()");
  Element::init(board);
  // do something here like initialization
}  // init()


/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool DisplayTouchElement::set(const char *name, const char *value) {
  bool ret = true;

  if (Element::set(name, value)) {
    // done.

  } else if (_stricmp(name, "address") == 0) {
    _address = _atoi(value);

  } else if (_stricmp(name, "width") == 0) {
    _width = _atoi(value);

  } else if (_stricmp(name, "height") == 0) {
    _height = _atoi(value);

  } else if (_stricmp(name, "rotation") == 0) {
    _rotation = _atoi(value);

  } else if (_stricmp(name, "interruptpin") == 0) {
    _interruptPin = _atopin(value);

  } else if (_stricmp(name, "resetpin") == 0) {
    _resetPin = _atopin(value);

  } else {
    ret = false;
  }  // if

  return (ret);
}  // set()


/**
 * @brief Activate the DisplayTouchElement.
 */
void DisplayTouchElement::start() {
  TRACE("DisplayTouchElement::start()");

  Element::start();
  nextRead = millis() + 50;
  _bFound = nullptr;
}  // start()


/// @brief Poll the touch controller for touch points
void DisplayTouchElement::loop() {
  Element::loop();
  unsigned long now = millis();

  if (now > nextRead) {

    if (pullSensorData()) {
      // got lastX and lastY

      // as of now only interested in the first.
      TRACE("-touch %d/%d", lastX, lastY);

      if (!_bFound) {
        // find displaybutton at x/y
        _board->forEach("displaybutton", [this](Element *e) {
          if (!_bFound) {
            DisplayButtonElement *be = (DisplayButtonElement *)e;
            if (be->touchStart(lastX, lastY)) {
              _bFound = be;
            }
          }
        });

      } else {
        bool over = _bFound->touchStart(lastX, lastY);
        if (!over) {
          _bFound = nullptr;
        }
      }

    } else if (_bFound) {
      TRACE("-end");
      // call touchEnd to found button
      _bFound->touchEnd(lastX, lastY);
      _bFound = nullptr;

    }  // if

    nextRead = millis() + 50;
  }

  //   GDTpoint_t points[GT911_MAX_CONTACTS];
  //   uint8_t count = tp->getTouchPoints(points);

  //   if (count > 0) {
  //     // as of now only interested in the first.
  //     TRACE("-touch %d/%d", points[0].x, points[0].y);

  //     if (!_bFound) {
  //       // find displaybutton at x/y
  //       _board->forEach("displaybutton", [this](Element *e) {
  //         if (!_bFound) {
  //           DisplayButtonElement *be = (DisplayButtonElement *)e;
  //           if (be->touchStart(lastX, lastY)) {
  //             _bFound = be;
  //           }
  //         }
  //       });

  //     } else {
  //       bool over = _bFound->touchStart(lastX, lastY);
  //       if (!over) {
  //         _bFound = nullptr;
  //       }
  //     }

  //   } else if (_bFound) {
  //     TRACE("-end");
  //     // call touchEnd to found button
  //     _bFound->touchEnd(lastX, lastY);
  //     _bFound = nullptr;

  //   }  // if

  // }
}  // loop()


/**
 * @brief push the current value of all properties to the callback.
 */
void DisplayTouchElement::pushState(
  std::function<void(const char *pName, const char *eValue)> callback) {
  Element::pushState(callback);
  // callback(PROP_VALUE, _printInteger(_value));
}  // pushState()


void DisplayTouchElement::term() {
  TRACE("term()");
  Element::term();
}  // term()


// End
