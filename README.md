# Automated Greenhouse Irrigation System Using Arduino 

This repository contains code for an Arduino-based greenhouse irrigation system. The system is designed to measure various environmental parameters such as temperature, humidity, CO2 levels, soil moisture, and pH. It utilizes sensors like DHT11 for temperature and humidity, SCD30 for CO2 levels, and an analog soil moisture sensor with a 1 channel relay module. This was my final project for PHYS 310 at Capilano University. 

## Requirements

- Arduino board
- DHT11 sensor
- SCD30 CO2 sensor
- 1 channel relay module and mini water pump 
- LiquidCrystal library
- Wire library
- SparkFun_SCD30_Arduino_Library
- DHT library

## Installation

1. Connect all the required sensors to the appropriate pins on the Arduino board.
2. Make sure to install the necessary libraries mentioned above.
3. Upload the provided code to your Arduino board.

## Usage

1. Upon powering up the system, it initializes the sensors and sets up the LCD display.
2. The main loop continuously reads data from the sensors.
3. Temperature, humidity, and CO2 levels are displayed on the LCD and serial monitor.
4. Soil moisture and pH levels are also measured and displayed.
5. Based on soil moisture levels, a relay is controlled to manage watering.
6. The LCD display scrolls to accommodate all the data.

## Notes

- Ensure correct wiring of sensors to avoid any errors during initialization.
- Calibration values for pH and soil moisture may need adjustment based on environmental conditions.
- Regular maintenance may be required to ensure accurate readings from sensors.

Happy watering! 
