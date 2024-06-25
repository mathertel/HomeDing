/**
 * @file DisplayESP32PanelAdapter.h
 *
 * @brief Display Element for TFT displays using the ESP32-S3 specific
 * 16-bit Panel Data interface.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 * -----
 * * 25.07.2018 created by Matthias Hertel
 * * 02.04.2023 using Arduino GFX library.
 */

#pragma once

#include <displays/DisplayAGFXAdapter.h>

class DisplayESP32PanelAdapter : public DisplayAGFXAdapter {

  bool start() override {

#if defined(ESP32) && defined(CONFIG_IDF_TARGET_ESP32S3) && (ESP_ARDUINO_VERSION_MAJOR < 3)
    int pinCount = ListUtils::length(conf->busPins);
    int8_t pins[16];

    if (pinCount != 16) {
      LOGGER_ERR("ESP32Panel requires 16 pin definitions");
    } else {
      for (int n = 0; n < 16; n++) {
        pins[n] = Element::_atopin(ListUtils::at(conf->busPins, n).c_str());
      }
    }

    // configure: "buspins": "45,48,47,21,14,5,6,7,15,16,4,8,3,46,9,1"

    // from: <https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration#esp32-8048s043>
    Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
      40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
      pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[6], pins[7],
      pins[8], pins[9], pins[10], pins[11], pins[12], pins[13], pins[14], pins[15],

      0 /* hsync_polarity */, 18 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
      0 /* vsync_polarity */, 18 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */
    );

    gfx = new Arduino_RGB_Display(
      800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, false /* auto_flush */);

    DisplayAGFXAdapter::start();
#endif

    return (gfx != nullptr);
  };  // start()
};
