# WiFiLocationTracker

Three different **wifi location tracker** are implemented; [esp](esp/README.md) for esp-devices, [python](python/README.md) for desktop, and stm32 for stm32-chip with LR1110.

###### Esp

Tested on a ESP32 developer board to retrieve MAC-addresses of nearby WiFi access point and sending those to Google to approximate the location of the esp32. It uses a `GOOGLE_API_KEY` (described in **Setup**).

###### Python

Able to run in a python environment and approximate the location of the device. It uses a `GOOGLE_API_KEY` (described in **Setup**).

###### **STM32**

Still in progress and is not currently working (View [README](stm32/README.md) for more info).

## Setup

Generate your Google API key to use the applications by following [here](https://developers.google.com/maps/documentation/geolocation/get-api-key).

Create `secret.txt` and add `GOOGLE_API_KEY = YOUR_API_KEY`

Choose either **esp_wifi_location_tracker** or **python_wifi_location_tracker** and read the respectively README.md
