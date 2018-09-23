/**
 * @file DisplayAdapterSSD1306.h
 *
 * @brief DisplayAdapter implementation for the HomeDing library
 * adapting OLED displays using the SSD1306 chip.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 * -----
 * * 25.07.2018 created by Matthias Hertel
 */

#ifndef DisplayAdapterSSD1306_H
#define DisplayAdapterSSD1306_H

#include <Arduino.h>
#include <displays/DisplayAdapter.h>

#include <SSD1306Wire.h>

class DisplayAdapterSSD1306 : DisplayAdapter
{
public:
  /**
   * @brief Construct a new Display Adapter for a SSD1306 display
   * using common parameters.
   * I2C defaults to 4(SDA) and 5(SCL) for most boards.
   */
  DisplayAdapterSSD1306()
  {
    // use parameters of a common display board.
    DisplayAdapterSSD1306(0x3c, 5, 4, 64);
  } // DisplayAdapterSSD1306()


  /**
   * @brief Construct a new Display Adapter for a SSD1306 display
   * using specific parameters.
   */
  DisplayAdapterSSD1306(int address, int sda, int scl, int h)
      : _address(address), _sda(sda), _scl(scl)
  {
    if (h == 32)
      _resolution = GEOMETRY_128_32;
    else if (h == 64)
      _resolution = GEOMETRY_128_64;
  } // DisplayAdapterSSD1306()


  // define the default line height
  int lineHeight = 10;

  // define the default character width
  int charWidth = 8;


  bool init()
  {
    // test if a display device is attached
    Wire.begin(_sda, _scl);
    Wire.beginTransmission(_address);
    int error = Wire.endTransmission();

    if (error != 0) {
      Serial.printf("display error %d\n", error);
      display = NULL;
      return (false);

    } else {
      Serial.printf("setupDisplay...\n");
      display = new SSD1306Wire(_address, _sda, _scl, _resolution);

      display->init();
      display->connect();

      display->flipScreenVertically();
      display->setTextAlignment(TEXT_ALIGN_LEFT);
      display->setFont(ArialMT_Plain_10);

      display->drawString(0, 0, "HomeDing...");
      display->display();
      delay(100);

    } // if
    return (true);
  }; // init()


  /**
   * @brief Clear all displayed information from the display.
   */
  void clear()
  {
    display->clear();
    _dirty = true;
  }; // clear()


  /**
   * @brief Clear information from the display in this area.
   * @param x x-position or offset of the text.
   * @param y y-position of the area.
   * @param w width of the area.
   * @param h height of the area.
   */
  void clear(int16_t x, int16_t y, int16_t w, int16_t h)
  {
    display->setColor(BLACK);
    display->fillRect(x, y, w, h);
    _dirty = true;
  }; // clear()


  /**
   * @brief Draw a text at this position using the specific height.-
   * @param x x-position or offset of the text.
   * @param y y-position of the text.
   * @param h height of the characters. Use 0 to use standard height;
   * @param text the text.
   */
  int drawText(int16_t x, int16_t y, int16_t h, String &text)
  {
    if ((h == 0) || (h == 10)) {
      display->setFont(ArialMT_Plain_10);
    } else if (h == 16) {
      display->setFont(ArialMT_Plain_16);
    } else if (h == 24) {
      display->setFont(ArialMT_Plain_24);
    } // if

    display->setColor(WHITE);
    display->drawString(x, y, text);
    _dirty = true;
    return (display->getStringWidth(text));
  }; // drawText()


  int drawText(int16_t x, int16_t y, int16_t h, const char *text)
  {
    String s_text(text);
    return (drawText(x, y, h, s_text));
  }


  int drawDot(int16_t x, int16_t y, int16_t h, bool fill)
  {
    int r = h / 2;

    display->setColor(WHITE);
    if (fill) {
      display->fillCircle(x + r, y + r, r);
    } else {
      display->drawCircle(x + r, y + r, r);
    }
    return (h);
  }; // drawDot()


  void flush()
  {
    if (_dirty) {
      display->display();
      _dirty = false;
    }
  }; // flush()

private:
  /**
   * @brief Reference to the used library object
   */
  SSD1306Wire *display;

  /**
   * @brief I2C Deisplay device address.
   */
  int _address;

  /**
   * @brief GPIO pin for the I2C SCL Line.
   */
  int _scl;

  /**
   * @brief GPIO pin for the I2C SDA Line.
   */
  int _sda;


  bool _dirty;

  /**
   * @brief Number of vertical pixels of the display.
   * The chip supports 32 and 64 vertical pixels.
   */
  OLEDDISPLAY_GEOMETRY _resolution;
};

#endif // DisplayAdapterSSD1306_H