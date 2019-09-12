/**
 * @file DisplaySH1106Element.h
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
 * * 18.09.2018 created by Matthias Hertel
 */

#ifndef DisplaySH1106Element_H
#define DisplaySH1106Element_H

/**
 * @brief DisplaySH1106Element implements creating an Display Adapter for a LCD
 * attached to the I2C bus.
 * @details
 * The DisplayAdapterLCD Element includes the real functionality.
 */

class DisplaySH1106Element : public Element
{
public:
  /* ===== Static factory function ===== */

  /**
   * @brief Factory function to create a DisplaySH1106Element.
   * @return Element*
   */
  static Element *create();

  /**
   * @brief static variable to ensure registering in static init phase.
   */
  static bool registered;

  /* ===== Element functions ===== */

  /**
   * @brief Constructor of a new DisplaySH1106Element.
   */
  DisplaySH1106Element();

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

private:
  int _address = 0x3c;
  int _height = 64;
  int _resetpin = -1;
};

#ifdef HOMEDING_REGISTER
// Register the DisplaySH1106Element onto the ElementRegistry.
bool DisplaySH1106Element::registered = ElementRegistry::registerElement(
    "displaySH1106", DisplaySH1106Element::create);
#endif

#endif