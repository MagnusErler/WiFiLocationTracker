# WiFi and Satellite Scanner for Geolocation

Two different **wifi location tracker** are implemented; [ESP](other_platforms/esp/README2.md) for ESP-devices, [Python](other_platforms/python/README.md) for desktop, and **wifi and GNSS location tracker** [STM32 ](stm32/README.md)for STM32-chip with LR1110.

###### Esp

Tested on a ESP32 developer board to retrieve MAC-addresses of nearby WiFi access point and sending those to Google to approximate the location of the ESP32. It uses a `GOOGLE_API_KEY` (described in [Setup](#setup)).

###### Python

Able to run in a Python environment and approximate the location of the device. It uses a `GOOGLE_API_KEY` (described in [Setup](#setup)).

###### STM32

View [README](stm32/README.md) for more info.

## Web application for LR1110 device**

View [README](web_application/README.md)

## Setup for **wifi location tracker**

Generate your Google API key to use the applications by following [here](https://developers.google.com/maps/documentation/geolocation/get-api-key).

Create `secret.txt` and add `GOOGLE_API_KEY = YOUR_API_KEY`

Choose either **ESP** or **Python** and read the respectively README.md
