/**
 * @file RotaryElement.cpp
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
 * Changelog:see RotaryElement.h
 */

#include <Arduino.h>
#include <Board.h>
#include <Element.h>

#include <RotaryElement.h>

#include <RotaryEncoder.h>


/* ===== statics ===== */

/**
 * @brief The Instance of the Rotary Encoder library.
 * Because of the interrupt implementation ther can be only ONE rotary encoder used at the same time.
 */
RotaryEncoder *__encoder;

/**
 * @brief The interrupt service routine to check the signals from the rotary encoder 
 */
ICACHE_RAM_ATTR void __checkPosition()
{
  __encoder->tick(); // just call tick() to check the state.
}


/* ===== Static factory function ===== */

/**
 * @brief static factory function to create a new RotaryElement
 * @return RotaryElement* created element
 */
Element *RotaryElement::create()
{
  return (new RotaryElement());
} // create()


/* ===== Element functions ===== */

/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool RotaryElement::set(const char *name, const char *value)
{
  bool ret = true;

  if (_stricmp(name, "step") == 0) {
    _step = _atoi(value);

  } else if (_stricmp(name, "pin1") == 0) {
    _pin1 = _atopin(value);

  } else if (_stricmp(name, "pin2") == 0) {
    _pin2 = _atopin(value);

  } else if (_stricmp(name, ACTION_ONVALUE) == 0) {
    _valueAction = value;

  } else {
    ret = Element::set(name, value);
  } // if

  return (ret);
} // set()


/**
 * @brief Activate the RotaryElement.
 */
void RotaryElement::start()
{
  LOGGER_ETRACE("start()");

  // Verify parameters
  if ((_pin1 < 0) || (_pin2 < 0)) {
    LOGGER_EERR("pin values are required.");

  } else {
    // LOGGER_ETRACE("connect %d %d\n", _pin1, _pin2);
    __encoder = new RotaryEncoder(_pin1, _pin2);
    pinMode(_pin1, INPUT_PULLUP);
    attachInterrupt(_pin1, __checkPosition, CHANGE);
    pinMode(_pin2, INPUT_PULLUP);
    attachInterrupt(_pin2, __checkPosition, CHANGE);
    Element::start();
  } // if
} // start()


/**
 * @brief Give some processing time to the Element to check for next actions.
 */
void RotaryElement::loop()
{
  // do something
  __encoder->tick(); // just call tick() to check the state.

  long newPos = __encoder->getPosition();
  if (newPos != _value) {
    // send an action with the delta
    _board->dispatch(_valueAction, _step * (newPos - _value));
  }
  _value = newPos;
} // loop()


/**
 * @brief push the current value of all properties to the callback.
 */
void RotaryElement::pushState(
    std::function<void(const char *pName, const char *eValue)> callback)
{
  Element::pushState(callback);
  callback(PROP_VALUE, String(_value).c_str());
} // pushState()

// End
