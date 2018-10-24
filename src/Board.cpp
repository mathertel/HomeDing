/**
 * @file board.cpp
 *
 * @brief Implementation of the Board class for the HomeDing Library
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
 * Changelog: see Board.h
 */

#include <Board.h>
#include <Element.h>
#include <ElementRegistry.h>

#include "MicroJsonParser.h"

#include "sntp.h"

// time_t less than this value is assumed as not initialized.
#define MIN_VALID_TIME (30 * 24 * 60 * 60)

extern const char *ssid;
extern const char *password;

#define NetMode_AUTO 3
#define NetMode_PSK 2
#define NetMode_PASS 1

#define SYS_LED 2 // TODO: make configurable

/**
 * @brief Initialize a blank board.
 */
void Board::init(ESP8266WebServer *s)
{
  LOGGER_TRACE("init()");
  server = s;
  boardState = BOARDSTATE_NONE;
  deviceName = WiFi.hostname(); // use mac based default device name
} // init()


/**
 * @brief Add and config the Elements defined in the config files.
 *
 */
void Board::addElements()
{
  LOGGER_TRACE("addElements()");
  Element *_lastElem = NULL; // last created Element
  MicroJson *mj;

  mj = new MicroJson(
      [this, &_lastElem](int level, char *path, char *name, char *value) {
        // LOGGER_TRACE("callback %d %s", level, path);

        if (level == 3) {
          if (name == NULL) {
            LOGGER_TRACE(" new Element %s", path);
            // path = <elem-type>/<elem-name>

            // create that element using the typename
            char tmp[32];
            strncpy(tmp, path, sizeof(tmp));
            tmp[31] = '\0'; // force termination
            char *p = strchr(tmp, MICROJSON_PATH_SEPARATOR);
            if (p)
              *p = '\0'; // cut at first path separator. The type remains in the
                         // buffer.

            _lastElem = ElementRegistry::createElement(tmp);
            if (_lastElem == NULL) {
              LOGGER_ERR("Cannot create Element type %s", tmp);

            } else {
              // add to the list of elements
              _add(path, _lastElem);
            } // if

          } else if (_lastElem != NULL) {
            // add a parameter to the last Element
            LOGGER_TRACE(" add %s=%s", name, value);
            _lastElem->set(name, value);
          } // if
        } // if
      });

  // config the thing to the local network
  mj->parseFile(ENV_FILENAME);

  // config the Elements of the thing
  mj->parseFile(CONF_FILENAME);

} // addElements()


void Board::start(Element_StartupMode startupMode)
{
  LOGGER_TRACE("start()");

  // make elements active that match
  Element *l = _list2;
  while (l != NULL) {

    if ((!l->active) && (l->startupMode <= startupMode)) {
      // start element when not already active
      LOGGER_TRACE("starting %s...", l->id);
      l->start();
    } // if

    l = l->next;
  } // while

  active = true;
  _next2 = NULL;
} // start()


// switch to a new state
void Board::_newState(BoardState newState)
{
  // LOGGER_TRACE("do BoardState %d", newState);
  boardState = newState;
}


// loop next element, only one at a time!
void Board::loop()
{
  unsigned long now = millis();

  if (boardState == BOARDSTATE_NONE) {
    _newState(BOARDSTATE_CONFIG);

  } else if (boardState == BOARDSTATE_CONFIG) {
    // load all config files and create+start elements
    addElements();
    start(Element_StartupMode::System);
    // LOGGER_TRACE("SYS Elements started.");

    LOGGER_INFO("devicename=%s", deviceName.c_str());
    WiFi.hostname(deviceName);

    if (display) {
      display->drawText(0, 0, 0, "HomeDing...");
      display->flush();
    } // if

    _newState(BOARDSTATE_CONNECT);
    netMode = NetMode_AUTO;

    // wait at least 6 seconds for offering config mode
    configPhaseEnd = now + (4 * 1000); // TODO: make configurable

  } else if (boardState == BOARDSTATE_CONNECT) {
    bool autoCon = WiFi.getAutoConnect();
    LOGGER_TRACE("autoconnect =(%d)", autoCon);

    if ((!autoCon) && (netMode == NetMode_AUTO))
      netMode = NetMode_PSK;

    // connect to a same network as before ?
    wl_status_t wifi_status = WiFi.status();
    LOGGER_TRACE("wifi status=(%d)", wifi_status);

    if (netMode == NetMode_AUTO) {
      // give autoconnect the chance to do it.
      LOGGER_TRACE("start NetMode_AUTO");

    } else if (netMode == NetMode_PSK) {
      LOGGER_TRACE("start NetMode_PSK");
      WiFi.mode(WIFI_STA);
      WiFi.begin();

    } else if (netMode == NetMode_PASS) {
      LOGGER_TRACE("start NetMode_PASS");
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
    } // if

    _newState(BOARDSTATE_CONFWAIT);
    pinMode(SYS_LED, OUTPUT);
    digitalWrite(SYS_LED, HIGH);

    // wait max 30 seconds for connecting to the network
    connectPhaseEnd = now + (30 * 1000); // TODO: make configurable

  } else if (boardState == BOARDSTATE_CONFWAIT) {

    if (now < configPhaseEnd) {
      // just continue waiting, Blink and wait for clicks...

      // MAKE SYS LED blink
      if (((configPhaseEnd - now) % 700) > 350) {
        digitalWrite(SYS_LED, HIGH);
      } else {
        digitalWrite(SYS_LED, LOW);
      }
      delay(100);

    } else {
      // stop using system LED
      digitalWrite(SYS_LED, HIGH);
      pinMode(SYS_LED, INPUT);
      _newState(BOARDSTATE_WAIT);
    }

  } else if (boardState == BOARDSTATE_WAIT) {
    wl_status_t wifi_status = WiFi.status();

    if (wifi_status == WL_CONNECTED) {
      LOGGER_TRACE("connected.");
      _newState(BOARDSTATE_GREET);

    } else if ((wifi_status == WL_NO_SSID_AVAIL) ||
               (wifi_status == WL_CONNECT_FAILED) || (now >= connectPhaseEnd)) {
      netMode -= 1;
      LOGGER_TRACE("next connect method = %d", netMode);
      if (netMode) {
        _newState(BOARDSTATE_CONNECT);
      } else {
        LOGGER_INFO("no-net restarting...");
        delay(10000);
        ESP.restart();
      }
    } else {
      delay(100);
    }

  } else if (boardState == BOARDSTATE_GREET) {
    LOGGER_TRACE("Connected to: %s", WiFi.SSID().c_str());
    LOGGER_TRACE("  as: %s", WiFi.hostname().c_str());

    String ipStr = WiFi.localIP().toString();
    LOGGER_TRACE("  IP: %s", ipStr.c_str());
    LOGGER_TRACE(" MAC: %s", WiFi.macAddress().c_str());

    if (display) {
      display->clear();
      display->drawText(0, 0, 0, deviceName);
      display->drawText(0, display->lineHeight, 0, ipStr.c_str());
      display->flush();
      delay(1600);
      display->clear();
      display->flush();
    } // if

    server->begin();
    start(Element_StartupMode::Network);
    _newState(BOARDSTATE_START);

  } else if (boardState == BOARDSTATE_START) {

    if (!validTime) {
      // check if time is valid now -> start all elements with Element_StartupMode::Time
      uint32 current_stamp = sntp_get_current_timestamp();
      if (current_stamp > MIN_VALID_TIME) {
        start(Element_StartupMode::Time);
        validTime = true;
        return;
      } // if
    }

    // dispatch next action if any
    if (_actionList.length() > 0) {
      String _lastAction;

      // extract first action
      int pos = _actionList.indexOf(ACTION_SEPARATOR);
      if (pos > 0) {
        _lastAction = _actionList.substring(0, pos);
        _actionList.remove(0, pos + 1);
      } else {
        _lastAction = _actionList;
        _actionList = (const char *)NULL;
      } // if
      _dispatchSingle(_lastAction);
      return;
    } // if

    // give some time to next active element
    if (_next2 == NULL) {
      _next2 = _list2;
    } // if
    if (_next2) {
      if (_next2->active) {
        _next2->loop();
      }
      _next2 = _next2->next;
    } // if
  } // if
} // loop()


Element *Board::findById(String &id)
{
  return (findById(id.c_str()));
}

Element *Board::findById(const char *id)
{
  // LOGGER_TRACE("findById(%s)", id);

  Element *l = _list2;
  while (l != NULL) {
    if (strcmp(l->id, id) == 0) {
      // LOGGER_TRACE(" found:%s", l->id);
      break; // while
    } // if
    l = l->next;
  } // while
  return (l);
} // findById


void Board::_dispatchSingle(String evt)
{
  // LOGGER_TRACE("dispatchSingle(%s)", evt.c_str());

  int pos1 = evt.indexOf(ELEM_PARAMETER);
  int pos2 = evt.indexOf(ELEM_VALUE);

  if (pos1 <= 0) {
    LOGGER_ERR("No action given: %s", evt.c_str());

  } else {
    String targetName, name, value;

    targetName = evt.substring(0, pos1);
    targetName.toLowerCase();

    Element *target = findById(targetName);

    if (!target) {
      LOGGER_ERR("dispatch target %s not found", targetName.c_str());

    } else {
      if (pos2 > 0) {
        name = evt.substring(pos1 + 1, pos2);
        value = evt.substring(pos2 + 1);
      } else {
        name = evt.substring(pos1 + 1);
        value = "";
      }
      bool ret = target->set(name.c_str(), value.c_str());
      if (!ret)
        LOGGER_ERR("Event '%s' was not handled", evt.c_str());
    }
  }
} // _dispatchSingle()


/**
 * @brief prepare sending all the actions to the right elements.
 */
void Board::dispatch(String &action, const char *value)
{
  dispatch(action.c_str(), value);
} // dispatch


void Board::dispatch(const char *action, const char *value)
{
  String tmp = action;

  if (value != NULL)
    tmp.replace("$v", value);

  // append to existing _actionList
  if (_actionList.length() > 0)
    _actionList.concat(ACTION_SEPARATOR);
  _actionList.concat(tmp);
} // dispatch


void Board::getState(String &out, String path)
{
  // LOGGER_TRACE("getState(%s)", path.c_str());
  String ret = "{";
  const char *cPath = path.c_str();

  Element *l = _list2;
  while (l != NULL) {
    // LOGGER_TRACE("  ->%s", l->id);
    if ((cPath[0] == '\0') || (strcmp(l->id, cPath) == 0)) {
      ret += "\"";
      ret += l->id;
      ret += "\": {";
      l->pushState([&ret](const char *name, const char *value) {
        // LOGGER_TRACE("->%s=%s", name, value);
        ret.concat('\"');
        ret.concat(name);
        ret.concat("\":\"");
        ret.concat(value);
        ret.concat("\",");
      });

      // remove last comma
      if (ret.charAt(ret.length() - 1) == ',') {
        ret.remove(ret.length() - 1);
      }
      ret += "},";
    } // if

    l = l->next;
  } // while

  // remove last comma
  if (ret.charAt(ret.length() - 1) == ',') {
    ret.remove(ret.length() - 1);
  }

  // close root object
  ret += "}";

  // prettify somehow
  ret.replace("},", "},\n");

  out = ret;
} // getState


void Board::setState(String &path, String property, String value)
{
  // LOGGER_TRACE("setState(%s, %s, %s)", path.c_str(), property.c_str(), value.c_str());
  Element *e = findById(path);
  if (e != NULL) {
    e->set(property.c_str(), value.c_str());
  }
}

// ===== Time functionality =====

// fill the time structure from a timestamp;
void Board::getTime(struct tm *time)
{
  uint32 current_stamp = sntp_get_current_timestamp();
  if (current_stamp > MIN_VALID_TIME) {
    struct tm *tmp = localtime((time_t *)(&current_stamp));
    memcpy(time, tmp, sizeof(struct tm));
  } else {
    memset(time, 0, sizeof(struct tm));
  }
} // getTime()


// return the seconds since 1.1.1970 00:00:00
time_t Board::getTime()
{
  uint32 current_stamp = sntp_get_current_timestamp();
  if (current_stamp <= MIN_VALID_TIME) {
    current_stamp = 0;
  } // if
  return ((time_t)(current_stamp));
} // getTime()


// return the seconds of today.
time_t Board::getTimeOfDay()
{
  uint32 current_stamp = sntp_get_current_timestamp();
  if (current_stamp > MIN_VALID_TIME) {
    current_stamp = current_stamp % (24 * 60 * 60);
  } else {
    current_stamp = 0;
  }
  return ((time_t)(current_stamp));
} // getTimeOfDay()

/**
 * @brief Get a Element by typename. Returns the first found element.
 */
Element *Board::getElement(const char *elementTypeName)
{
  // LOGGER_TRACE("getElement(%s)", elementTypeName);

  String tn = elementTypeName;
  tn.concat(ELEM_ID_SEPARATOR);
  int tnLength = tn.length();

  Element *l = _list2;
  while (l != NULL) {
    if (String(l->id).substring(0, tnLength).equalsIgnoreCase(tn)) {
      break; // while
    } // if
    l = l->next;
  } // while
  // LOGGER_TRACE("found: %d", l);
  return (l);
} // getElement


/**
 * @brief Add another element to the board into the list of created elements.
 */
void Board::_add(const char *id, Element *e)
{
  // LOGGER_TRACE("_add(%s)", id);

  strcpy(e->id, id);
  e->next = NULL;
  Element *l = _list2;

  // append to end of list.
  if (l == NULL) {
    // first Element.
    _list2 = e;
  } else {
    // search last Element.
    while (l->next != NULL)
      l = l->next;
    l->next = e;
  } // if
  e->init(this);
} // _add()
// End
