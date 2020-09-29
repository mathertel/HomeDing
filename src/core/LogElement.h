/**
 * @file LogElement.h
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
 * * 30.07.2018 created by Matthias Hertel
 * * 10.04.2020 filesize property added.
 */

#ifndef LOGELEMENT_H
#define LOGELEMENT_H

#include <HomeDing.h>

/**
 * @brief LogElement implements...
 * @details
@verbatim

The LogElement can ...

@endverbatim
 */

class LogElement : public Element
{
public:
  /**
   * @brief Factory function to create a LogElement.
   * @return Element*
   */
  static Element *create();

  /**
   * @brief static variable to ensure registering in static init phase.
   */
  static bool registered;

  /**
   * @brief Construct a new LogElement
   */
  LogElement();


  /**
   * @brief initialize a new Element.
   * @param board The board reference.
   */
  virtual void init(Board *board) override;

  /**
   * @brief Set a parameter or property to a new value or start an action.
   * @param name Name of property.
   * @param value Value of property.
   * @return true when property could be changed and the corresponding action
   * could be executed.
   */
  virtual bool set(const char *name, const char *value) override;

  /**
   * @brief Activate the Element.
   * @return true when the Element could be activated.
   * @return false when parameters are not usable.
   */
  virtual void start() override;

  /**
   * @brief Give some processing time to the timer to check for next action.
   */
  virtual void loop() override;

  /**
   * @brief push the current value of all properties to the callback.
   * @param callback callback function that is used for every property.
   */
  virtual void pushState(
      std::function<void(const char *pName, const char *eValue)> callback) override;

private:
  void _logToFile();

  /**
   * @brief The actual value to be logged.
   */
  String _value;
 
  /**
   * @brief The actual timestamp of the last value.
   */
  time_t _timestamp;

  /**
   * @brief Flag when value has been changed since last logging.
   */
  bool _changed;


  uint16_t _filesize;

  String _logfileName;
  String _logfileOldName;
};


/* ===== Register the Element ===== */

#ifdef HOMEDING_REGISTER
bool LogElement::registered =
    ElementRegistry::registerElement("log", LogElement::create);
#endif

#endif