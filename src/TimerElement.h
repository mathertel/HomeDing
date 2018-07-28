/**
 * @file TimerElement.h
 * @brief Element for creating timer based actions.
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
 * * 14.04.2018 created by Matthias Hertel
 * * 27.04.2018 Based on Element class.
 * * 15.05.2018 set = properties and actions interface.
 */

#ifndef TIMERELEMENT_H
#define TIMERELEMENT_H

#include "Board.h"
#include "Element.h"
#include <Arduino.h>

/**
 * @brief Timer Element that produces actions based on timing conditions.
 * @details
@verbatim

The Timer Element can automatically produce 2 actions after a specified timing.

                +--> "on" action  +--> "off" action
                |                 |
________________/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\______________

<-- waittime --> <-- pulsetime -->
<------------- cycletime ----------------------->
<- _state=0  --> <- _state=1 ----> <- _state=2 -> <- _state=3 (no LOOP) ...

When timer type = "LOOP" the timer automatically starts from the begining
after finishing the cycletime. The cycletime is atomatically adjusted to
minimum of waittime+pulsetime. The units are in minutes

"onon": "device.main:log:start relais...",
"onoff": "device.main:log:stop relais..."

"onon": "display.active:dot:1",
"onoff": "display.active:dot:0"


@endverbatim
 */

class TimerElement : public Element
{
public:
  /**
   * @brief Factory function to create a TimerElement.
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
   * @brief push the current value of all properties to the callback.
   * @param callback callback function that is used for every property.
   */
  virtual void pushState(
      std::function<void(const char *pName, const char *eValue)> callback);

  /**
   * @brief stop all activities of the timer and go inactive.
   */
  virtual void term();

private:
  /**
   * @brief type of timer like LOOP
   */
  int _type;

  /**
   * @brief time of a complete timer cycle.
   *
   * This variable corresponds to the "cycletime" parameter.
   * When not specified the _cycleTime is calculated by waittime + pulsetime.
   */
  uint16_t _cycleTime = 0;

  /**
   * @brief time before "on" action.
   *
   * This variable corresponds to the "waittime" parameter.
   */
  uint16_t _waitTime = 0; // minutes to wait until next water will be started

  /**
   * @brief time between "on" and "off" action.
   *
   * This variable corresponds to the "pulsetime" parameter.
   */
  uint16_t _pulseTime = 0; // minutes when water will be served

  /**
   * @brief The _onAction holds the actions that is submitted when the pulse
   * period starts.
   */
  String _onAction;

  /**
   * @brief The _offAction holds the actions that is submitted when the pulse
   * period ends.
   *
   */
  String _offAction;

  /**
   * @brief The effective time (in seconds) the timer has started.
   */
  unsigned long _startTime;

  /**
   * @brief current state of the timer.
   * 0: while waittime is running.
   * 1: while pulsetime is running.
   * 2: while cycletime is not over.
   * 3: cycletime ended but type != LOOP.
   */
  int _state = 0;

  /**
   * @brief start the timer from the beginning.
   */
  void _startTimer();

  /**
   * @brief stop the timer and set to off state.
   */
  void _stopTimer();
};

#endif