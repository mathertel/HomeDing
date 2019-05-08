/**
 * @file RFSendElement.cpp
 * @brief Element Template class.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 *
 * This work is licensed under a BSD style license.
 * https://www.mathertel.de/License.aspx.
 *
 * More information on https://www.mathertel.de/Arduino
 *
 * Changelog:see RFSendElement.h
 */

#include <Arduino.h>
#include <Element.h>
#include <Board.h>

#include <RFSendElement.h>
#include <ElementRegistry.h>

#include <TabRF.h>
#include <intertechno2.h> // use the intertechno2 code defintions


/* ===== Define local constants and often used strings ===== */

// Define static class variables here.
int RFSendElement::_rfpin;

/* ===== Static factory function ===== */

/**
 * @brief static factory function to create a new RFSendElement
 * @return RFSendElement* created element
 */
Element *RFSendElement::create()
{
  return (new RFSendElement());
} // create()


/* ===== Element functions ===== */

/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool RFSendElement::set(const char *name, const char *value)
{
  // LOGGER_ETRACE("set(%s, %s)", name, value);
  bool ret = true;

  if (_stricmp(name, "value") == 0) {
    _lastValue = _atoi(value);

    if (active) {
      if (_lastValue) {
        LOGGER_ETRACE("send(%s)", _codeOn.c_str());
        tabRF.send(_codeOn.c_str());
      } else {
        LOGGER_ETRACE("send(%s)", _codeOff.c_str());
        tabRF.send(_codeOff.c_str());
      }
    } // if

  } else if (_stricmp(name, "pin") == 0) {
    _rfpin = _atopin(value);

  } else if (_stricmp(name, "codeOn") == 0) {
    _codeOn = value;

  } else if (_stricmp(name, "codeOff") == 0) {
    _codeOff = value;

  } else {
    ret = Element::set(name, value);
  } // if

  return (ret);
} // set()


/**
 * @brief Activate the RFSendElement.
 */
void RFSendElement::start()
{
  static bool initialized = false;
  // LOGGER_ETRACE("start()");

  // initialize the tabrf library only once.
  if (!initialized) {
    LOGGER_ETRACE("start on pin=%d", _rfpin);

    tabRF.init(NO_PIN, _rfpin); // input at D1 = pin #2 , output at D4, pin # 9
    tabRF.setupDefinition(&Intertechno2_Sequence);
    initialized = true;
  }

  Element::start();
} // start()


/**
 * @brief push the current value of all properties to the callback.
 */
void RFSendElement::pushState(
    std::function<void(const char *pName, const char *eValue)> callback)
{
  Element::pushState(callback);
  callback("value", (_lastValue == 1) ? "1" : "0");
} // pushState()


// End
