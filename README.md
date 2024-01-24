# WiFiLocationTracker

Three differnet **wifi location tracker** are implemented; [esp](esp_wifi_location_tracker/README.md) for esp-devices, [python](python_wifi_location_tracker/README.md) for desktop, and stm32 for stm32-chip with LR1110.

**esp_wifi_location_tracker** is testet on a esp32 developer board to retreive MAC-adresses of nearby WiFi access point and sending those to Google to approximate the location of the esp32. It uses a `GOOGLE_API_KEY` (described in **Setup**).

**python_wifi_location_tracker** can be run in a python environment and approximate the lcoation of the device. It uses a `GOOGLE_API_KEY` (described in **Setup**).

## Setup

Generate your Google API key to use the applications by following [here](https://developers.google.com/maps/documentation/geolocation/get-api-key).

Create `secret.txt` and add `GOOGLE_API_KEY = YOUR_API_KEY`
