# The HomeDing Library

This is a Arduino library to build small IoT devices without the need for cloud services or a local central device.
It includes capabilities that can be combined by configuration providing a flexible low code solution.

[![Arduino Library Format Check](https://github.com/HomeDing/HomeDing/actions/workflows/checkLibraryFormat.yml/badge.svg)](https://github.com/HomeDing/HomeDing/actions/workflows/checkLibraryFormat.yml)
[![Build Examples for ESP8266](https://github.com/HomeDing/HomeDing/actions/workflows/checks.yml/badge.svg)](https://github.com/HomeDing/HomeDing/actions/workflows/checks.yml)
[![Build Examples for ESP32](https://github.com/HomeDing/HomeDing/actions/workflows/buildESP32.yml/badge.svg)](https://github.com/HomeDing/HomeDing/actions/workflows/buildESP32.yml)
[![arduino-library-badge](https://www.ardu-badge.com/badge/HomeDing.svg?)](https://www.ardu-badge.com/HomeDing)
[![GitLicense](https://gitlicense.com/badge/HomeDing/HomeDing)](https://gitlicense.com/license/HomeDing/HomeDing)

The library includes a complete ecosystem to build a IoT device by combining IoT specific Elements that interact using events and actions.
These elements and the data flow and activities between elements can be configured without compiling a new firmware.
It is extensible by adding new specific Elements.

The devices run stand-alone only connected to the local WiFi and provide
information and control of the device using a built-in web server.

Documentation can be found in at [HomeDing documentation site](https://homeding.github.io)

This Library is published using a [BSD 3-clause license](./LICENSE).


## Documentation

The Homeding library is documented on <https://homeding.github.io>.

The library has implemented many
[Elements](https://homeding.github.io/elements/index.htm) for simple Input and Output of signals,
[Sensors](https://homeding.github.io/sensors/sensors.htm),
[Displays](https://homeding.github.io/displays/index.htm), lightning, logical processing, time support and many more.

The embedded Web Server enables configuration and control of the device.

It supports:

* a device specific UI that can be accessed by browsers and mobile devices
* web services using the REST format to control the device
* web services using the REST format to get the status of the device
* retrieving actions from other devices
* a small IDE (Integrated Development Environment) that allows changing the configuration

More documentation can be found in the  [HomeDing documentation site](https://homeding.github.io)

## Supported CPUs

The library supports the following CPUs:

* **ESP8266** with boards of 1MB and 4MB flash memory
* **ESP32** Wrover SoC
* **ESP32** Wroom SoC with PSRAM for Audio processing
* **ESP32-C3**

More will follow as the library has no strong dependencies on the processor used.


## Provided examples

The HomeDing library can be used for a broad variety of IoT devices partially showen by the provided examples.

**standard** -- Full featured firmware containing all most-used stable elements like standard I/O, sensors,
light drivers and displays. Runs well on a 4MB Flash with all supported processors

**minimal** -- A reduced set pf elements and a small footprint Web-UI for small devices and off-the-shell
devices with custom firmware like swtiches, sensors and lights. Especially support for ESP8266 with 1MB Flash.

**DevDing** -- A extended standard setup that includes hints and tools for development of new elements.

**webradio** -- A examples especially for the ESP32 Wroover with PSRAM to build a Web Radio 
by using a Aduio library. This Web Radio can be controlled through the embedded web interface.


## BSD License

The HomeDing library is open-source and uses one of the most permissive licenses so you can use it for any project:

* Private use
* Commercial use
* You can modify
* You can distribute

If you like to contribute by additions or bug fixes you are welcome to open issues or pull requests on github.

If you plan to use it in a commercial product you are welcome to contact me regarding working and planning the future.


## Included Libraries / Components

* The **BME680 sensor** Element uses the Open Source from the Arduino Library published by Bosch Sensortec GmbH <https://github.com/BoschSensortec/BSEC-Arduino-library/tree/master/src/bme680> using a BSD 3-clause license.

## See also

* [CLI for Homeding](https://homeding.github.io)