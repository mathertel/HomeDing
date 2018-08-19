/**
 * @file DisplayDotElement.h
 * @brief Output Element for controlling a binary output on a display.
 *
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
 * * 29.04.2018 created by Matthias Hertel
 * * 24.06.2018 no problems when no display is available.
 */


#ifndef DISPLAYDOTELEMENT_H
#define DISPLAYDOTELEMENT_H

#include <Arduino.h>
#include "Board.h"
#include "Element.h"

#include "DisplayAdapter.h"

/**
 * @brief The DisplayDotElement is an Element that allows to create information
 * on the display based on actions.
 *
 * The parameters specify how the information from the action will be displayed.
 */
class DisplayDotElement : public Element
{
public:
  /**
   * @brief Factory function to create a ButtonElement.
   * @return Element*
   */
  static Element *create();

  /**
   * @brief static variable to ensure registering in static init phase.
   */
  static bool registered;


  /**
   * @brief Set a parameter or property to a new value or start an action.
   * @param name Name of property.
   * @param value Value of property.
   * @return true when property could be changed and the corresponding action
   * could be executed.
   */
  virtual bool set(const char *name, const char *value);

  /**
   * @brief Activate the DisplayDotElement.
   * @return true when activation was good.
   * @return false when activation failed.
   */
  virtual void start();

  /**
   * @brief push the current value of all properties to the callback.
   * @param callback callback function that is used for every property.
   */
  virtual void pushState(
      std::function<void(const char *pName, const char *eValue)> callback);


private:
  /**
   * @brief displayed value
   */
  String _value;

  /**
   * @brief This variable corresponds to the x parameter.
   */
  int _x = 0;

  /**
   * @brief This variable corresponds to the y parameter.
   */
  int _y = 0;

  int _w = 11;
  int _h = 11;

  DisplayAdapter *_display = NULL;
};

#ifdef HOMEDING_REGISTER
// Register the DisplayDotElement onto the ElementRegistry.
bool DisplayDotElement::registered =
    ElementRegistry::registerElement("displaydot", DisplayDotElement::create);
#endif

#endif // DISPLAYDOTELEMENT_H