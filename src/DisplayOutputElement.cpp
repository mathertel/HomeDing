/**
 * @file DisplayOutputElement.cpp
 * @brief Base Output Element for a display.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 * This work is licensed under a BSD 3-Clause style license, see https://www.mathertel.de/License.aspx
 *
 * More information on https://www.mathertel.de/Arduino.
 *
 * Changelog, see DisplayOutputElement.h.
 */

#include <Arduino.h>
#include <HomeDing.h>

#include <DisplayOutputElement.h>

#define TRACE(...)  // LOGGER_ETRACE(__VA_ARGS__)

/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool DisplayOutputElement::set(const char *name, const char *value) {
  TRACE("set %s=%s", name, value);
  bool ret = true;

  if (Element::set(name, value)) {
    // done

  } else if (_stricmp(name, "redraw") == 0) {
    this->draw();

  } else if (_stricmp(name, "page") == 0) {
    _page = _atoi(value);

  } else if (_stricmp(name, "x") == 0) {
    _x = _atoi(value);

  } else if (_stricmp(name, "y") == 0) {
    _y = _atoi(value);

  } else if ((_stricmp(name, "h") == 0) || (_stricmp(name, "fontsize") == 0)) {
    _h = _atoi(value);

  } else if (_stricmp(name, "color") == 0) {
    _color = _atoColor(value);

  } else if (_stricmp(name, "background") == 0) {
    _background = _atoColor(value);

  } else {
    ret = false;
  }  // if
  return (ret);
}  // set()


/**
 * @brief Activate the DisplayOutputElement.
 */
void DisplayOutputElement::start() {
  DisplayAdapter *d = _board->display;

  if (d == NULL) {
    LOGGER_EERR("no display found");

  } else {
    if (_color == COLOR_UNDEFINED) {
      _color = d->getColor();  // get standard draw/text color from display.
    }
    _display = d;
    if (_page > d->maxpage) {
      d->maxpage = _page;
    }
    Element::start();
    _neededraw = true;
  }  // if
}  // start()


/**
 * @brief check the state of the DHT values and eventually create actions.
 */
void DisplayOutputElement::loop() {
  if (_neededraw) {
    if (_display->page == _page) {
      draw();
      _display->flush();
    }
    _neededraw = false;
  }  // if
}  // loop()


/**
 * @brief Set a parameter or property to a new value or start an action.
 */
void DisplayOutputElement::draw() {
  if (_display) {
    _display->setColor(_color);
    _display->setBackgroundColor(_background);
  }
}

// End
