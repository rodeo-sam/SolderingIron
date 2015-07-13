---
layout: default
title:  "Pinheader Overview"
date:   2015-07-14 16:00:00
---

This is an overview of the pinouts of the board connectors.

##J1

|PIN|Signal|Description|
|---|---------|-----------|
| 1 |MISO     | Master In Slave Out|
| 2 |SENSOR0  | temperature sensor input|
| 3 |VOUT     | Heater supply PWM output|
| 4 |VOUT     | Heater supply PWM output|
| 5 |GND      | Heater supply ground|
| 6 |GND      | Heater supply ground|
| 7 |GND      | Heater supply ground|
| 8 |GND      | Supply voltage ground|

##J2

|PIN|Signal|Description|
|---|----|-----------|
| 1 |ADC3| Analog/Digital converter channel 3 input|
| 2 |ADC2| Analog/Digital converter channel 2 input|
| 3 |GND| Supply voltage ground|

##PWR_INPUT

|PIN|Signal|Description|
|---|------|-----------|
| 1 |+12V  | Supply voltage input|
| 2 |+12V  | Supply voltage input|
| 3 |GND   | Supply voltage ground|
| 4 |GND   | Supply voltage ground|

##ISP

|PIN|Signal|Description|
|---|------|-----------|
| 1 |MISO  | Master In Slave Out|
| 2 |VCC   | Internal regulated supply voltage|
| 3 |SCK   | ISP/SPI clock|
| 4 |MOSI  | Master Out Slave In|
| 5 |RST   | MCU Reset|
| 6 |GND   | Supply voltage ground|

##I2C/TWI

|PIN|Signal|Description|
|---|---|-----------|
| 1 |SDA| I2C/TWI data signal|
| 2 |SCL| I2C/TWI clock signal|
| 3 |GND| Supply voltage ground|

##UART

|PIN|Signal|Description|
|---|---|-----------|
| 1 |TXD| Transmit output|
| 2 |RXD| Receive Input|
| 3 |GND| Supply voltage ground|

[back to Hardware](Hardware)
