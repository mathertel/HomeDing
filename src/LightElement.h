/**
 * @file LightElement.h
 * @brief Output Element to drive LEDs on GPIO pins.
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
 * Changelog:
 * * 10.03.2020 created by Matthias Hertel
 */

#ifndef LIGHTELEMENT_H
#define LIGHTELEMENT_H

#include <Arduino.h>
#include <HomeDing.h>

/**
 * @brief LightElement implements an Element to drive a single LED.
 */

class LightElement : public Element
{
public:
  static const int MAXPINS = 4;

  /**
   * @brief Factory function to create a LightElement.
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
   * @brief Activate the Element.
   * @return true when the Element could be activated.
   * @return false when parameters are not usable.
   */
  virtual void start();

  /**
   * @brief Give some processing time to the timer to check for next action.
   */
  virtual void loop();

  /**
   * @brief stop all activities and go inactive.
   */
  // virtual void term();

  /**
   * @brief push the current value of all properties to the callback.
   * @param callback callback function that is used for every property.
   */
  virtual void pushState(
      std::function<void(const char *pName, const char *eValue)> callback);

protected:
  /**
   * @brief The actual / new value of the output value.
   */
  String value;

  /**
   * @brief The last / old value of the output value.
   */
  String oldValue;

  /** @brief Brightness level in percent. */
  int brightness = 50; // percent

  /** duration of animation / transition in msecs */
  unsigned long duration = 4000;

  /** data output pins (R,G,B) or pins[0] only. */
  int _pins[4]; //  = {D8, D6, D7, 0};

  /** set color pattern */
  virtual void setOutput(String value);

  /**
   * @brief set to true after a color has been prepares to be send to the pixels next loop().
  */
  bool needUpdate = false;


private:
  /** Number of pixels in the stripe, set by the number of GPIO pins in then pin property. */
  int _count = 0;

};

/* ===== Register the Element ===== */

#ifdef HOMEDING_REGISTER
// Register the LightElement onto the ElementRegistry.
bool LightElement::registered =
    ElementRegistry::registerElement("light", LightElement::create);
#endif


#endif