/**
 * @file NeoElement.h
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
 * Changelog:
 * * 30.07.2019 created by Matthias Hertel
 * * 15.11.2019 some more modes implemented
 * * 28.12.2019 less blocked time in loop()
 */

#ifndef NEOELEMENT_H
#define NEOELEMENT_H

#include <HomeDing.h>
#include <LightElement.h>

// https://github.com/adafruit/Adafruit_NeoPixel
#include <Adafruit_NeoPixel.h>

/**
 * @brief NeoElement implements an Element to drive LED stripes with the WS2812 LEDs.
 */

class NeoElement : public LightElement
{
public:
  /**
   * @brief Factory function to create a NeoElement.
   * @return Element*
   */
  static Element *create();

  /**
   * @brief static variable to ensure registering in static init phase.
   */
  static bool registered;

  /**
   * @brief initialize a new Element.
   * @param board The board reference.
   */
  virtual void init(Board *board);

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

  enum class Mode {
    color = 0,
    wheel = 1,
    flow = 2,
    pulse = 3,
    more
  };

private:
  /**
   * @brief The actual mode.
  */
  Mode _mode;

  /** Number of pixels in the stripe */
  int _count;

  /** data output pin is taken from _pins[0]*/

  /** actual colors are stored in LightElement::value */

  /** Brightness level in 0..255 */
  int _brightness_255;

  /** set color pattern */
  void _setColors(String colList);

  Adafruit_NeoPixel *_strip = (Adafruit_NeoPixel *)NULL;

};


/* ===== Register the Element ===== */

// When transferred to the HomeDing library a #define like the
// HOMEDING_INCLUDE_XXX should be used to allow the sketch to select the
// available Elements. See <HomeDing.h> the move these lines to NeoElement.h:

#ifdef HOMEDING_REGISTER
// Register the NeoElement onto the ElementRegistry.
bool NeoElement::registered =
    ElementRegistry::registerElement("neo", NeoElement::create);
#endif

#endif