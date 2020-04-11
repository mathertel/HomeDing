/**
 * @file TemplateElement.cpp
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
 * Changelog:see TemplateElement.h
 */

#include <Arduino.h>
#include <Element.h>
#include <Board.h>

#include <HomeDing.h>

#include "TemplateElement.h"


/* ===== Define local constants and often used strings ===== */

// like:
// #define ANYCONSTANT 1000


/* ===== Static factory function ===== */

/**
 * @brief static factory function to create a new TemplateElement
 * @return TemplateElement* created element
 */
Element *TemplateElement::create()
{
  return (new TemplateElement());
} // create()


/* ===== Element functions ===== */

// TemplateElement::TemplateElement() {}


// maybe: overwrite the init() function.

// void TemplateElement::init(Board *board)
// {
//   Element::init(board);
//   // do something here like initialization
// } // init()


/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool TemplateElement::set(const char *name, const char *value)
{
  bool ret = true;

  if (_stricmp(name, PROP_VALUE) == 0) {
    _value = _atoi(value);

    // } else if (_stricmp(name, "onChange") == 0) {
    // save the actions
    // _changeAction = value;

    // } else if (_stricmp(name, "doAction") == 0) {
    // make something

  } else {
    ret = Element::set(name, value);
  } // if

  return (ret);
} // set()


/**
 * @brief Activate the TemplateElement.
 */
void TemplateElement::start()
{
  LOGGER_ETRACE("start()");

  // Verify parameters

  // if (parameters ok) {
  Element::start();
  // } // if

} // start()


/**
 * @brief Give some processing time to the Element to check for next actions.
 */
void TemplateElement::loop()
{
  // do something
} // loop()


/**
 * @brief push the current value of all properties to the callback.
 */
void TemplateElement::pushState(
    std::function<void(const char *pName, const char *eValue)> callback)
{
  Element::pushState(callback);
  callback(PROP_VALUE, String(_value).c_str());
} // pushState()


// maybe: overwrite the term() function,
// void Element::term()
// {
//   LOGGER_ETRACE("term()");
//   active = false;
// } // term()




/* ===== Register the Element ===== */

// As long as the Element is project specific or is a element always used
// the registration is placed here without using a register #define.

// When transferred to the HomeDing library a #define like the
// HOMEDING_INCLUDE_XXX should be used to allow the sketch to select the
// available Elements. See <HomeDing.h> the move these lines to TemplateElement.h:

// #ifdef HOMEDING_REGISTER
// Register the TemplateElement onto the ElementRegistry.
bool TemplateElement::registered =
    ElementRegistry::registerElement("template", TemplateElement::create);
// #endif

// End