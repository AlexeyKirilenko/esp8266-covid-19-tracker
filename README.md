# esp8266 get weather and time from yandex
e

This tutorial shows how to interface ESP8266 NodeMCU (ESP-12E) board with ILI9341 TFT display and get time and weather data from yandex.

### Display
The ILI9341 TFT module contains a display controller with the same name: ILI9341. It’s a color display that uses SPI interface protocol and requires 4 or 5 control pins, it’s low cost and easy to use.
The resolution of this TFT display is 240 x 320 which means it has 76800 pixels. This module works with 3.3V only and it doesn’t support 5V (not 5V tolerant).

TFT: Thin-Film Transistor.
SPI: Serial Peripheral Interface.

### Project Hardware Required:

NodeMCU board
ILI9341 TFT display module (2.2″, 2.4″, 2.8″ …)
Micro USB cable (for programming and powering the whole circuit)
Breadboard
Jumper wires

###  NodeMCU with ILI9341 TFT display circuit:

Project circuit schematic diagram is shown below.

The ILI9341 TFT display board which is shown in project circuit diagram has 14 pins, the first 9 pins are for the display and the other 5 pins are for the touch module.

So, the display part pins are numbered from 1 to 9 (from left to right): VCC (5V), GND (ground), CS (chip select), RST (reset), DC (or D/C: data/command), MOSI (or SDI), SCK (clock), BL (back light LED) and MISO (or SDO).
MOSI: master-out slave-in.
SDI: serial data in.
MISO: master-in slave-out.
SDO: serial data out.

![ESP8266 NodeMCU ILI9341 TFT display](https://github.com/AlexeyKirilenko/esp8266-weather-and-time-/blob/master/nodemcu-ili9341-color-tft-1024x576.jpg)


![display is connected]((https://github.com/AlexeyKirilenko/esp8266-weather-and-time-/blob/master/esp8266-nodemcu-ili9341-tft-display-768x742.png)

The ILI9341 TFT display is connected to the NodeMCU board as follows:
CS pin is connected to D2 (ESP8266EX GPIO4),
RST pin is connected to D3 (ESP8266EX GPIO0),
D/C pin is connected to D4 (ESP8266EX GPIO2),
MOSI pin is connected to D7 (ESP8266EX GPIO13),
SCK pin is connected to D5 (ESP8266EX GPIO14),
VCC and BL are connected to pin 3V3,
GND is connected to pin GND of the NodeMCU board.

Pins D5 (GPIO14) and D7 (GPIO13) are hardware SPI module pins of the ESP8266EX microcontroller respectively for SCK (serial clock) and MOSI (master-out slave-in).
