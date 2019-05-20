/**
 * @file ValueElement.cpp
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
 * Changelog:see ValueElement.h
 */

#include <Arduino.h>
#include <Board.h>
#include <Element.h>

#include <ElementRegistry.h>
#include <ValueElement.h>

/**
 * @brief static factory function to create a new ValueElement
 * @return ValueElement* created element
 */
Element *ValueElement::create()
{
  return (new ValueElement());
} // create()


/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool ValueElement::set(const char *name, const char *value)
{
  LOGGER_ETRACE("set(%s, %s)", name, value);
  bool ret = true;

  if (_stricmp(name, "value") == 0) {
    _setValue(_atoi(value));

  } else if (_stricmp(name, "up") == 0) {
    _setValue(_value += _atoi(value) * _step);

  } else if (_stricmp(name, "down") == 0) {
    _setValue(_value -= _atoi(value) * _step);

  } else if (_stricmp(name, "min") == 0) {
    _min = _atoi(value);

  } else if (_stricmp(name, "max") == 0) {
    _max = _atoi(value);

  } else if (_stricmp(name, "step") == 0) {
    _step = _atoi(value);

  } else if (_stricmp(name, "label") == 0) {
    _label = value;

  } else if (_stricmp(name, "onvalue") == 0) {
    _valueAction = value;

  } else if (_stricmp(name, "onchange") == 0) { // deprecated: use onValue
    _valueAction = value;

  } else {
    ret = Element::set(name, value);
  } // if

  return (ret);
} // set()


void ValueElement::start()
{
  Element::start();

  // Generate label from id if not given
  if (_label.length() == 0) {
    _label = strchr(id, ELEM_ID_SEPARATOR) + 1;
  }

  // send out the actual defined value
  _setValue(_value);
} // start()


/**
 * @brief push the current value of all properties to the callback.
 */
void ValueElement::pushState(
    std::function<void(const char *pName, const char *eValue)> callback)
{
  Element::pushState(callback);
  callback("value", String(_value).c_str());
} // pushState()

/** return actual value */
int ValueElement::getValue()
{
  return (_value);
}

/** return actual value */
const char *ValueElement::getLabel()
{
  return (_label.c_str());
}

// set a new value, maybe adjust to range
void ValueElement::_setValue(int newValue)
{
  _value = newValue;

  if (_value < _min)
    _value = _min;

  if (_value > _max)
    _value = _max;

  if ((active) && (_valueAction.length() > 0))
    _board->dispatch(_valueAction, _value);

} // _setValue()

// End
