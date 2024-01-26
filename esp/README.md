# ESP WiFiLocationTracker

## Setup

Google requires using secure TLS connection in order to use GeoLocation API. `WifiLocation1.cpp` includes GoogleCA certificate to check server chain. This certificate may expire.

You may get current certificate using this command (if you are using Linux)

```bash
openssl s_client -servername www.googleapis.com -showcerts \
 -connect www.googleapis.com:443 < /dev/null \
 |  awk '/^-----BEGIN CERTIFICATE-----/,/^-----END CERTIFICATE-----/{if(++m==1)n++;if(n==2)print;if(/^-----END CERTIFICATE-----/)m=0}' \
 > googleCA.cer
```

Then, you can copy file content and overwrite line 11-42 of `WifiLocation1.cpp`.

---

Generate your Google API key to use the applications by following [here](https://developers.google.com/maps/documentation/geolocation/get-api-key).

Create `secret.h` inside `src` and add

```c
// secret.h
#ifndef SECRET_H
#define SECRET_H

const char* GOOGLE_API_KEY = "YOUR_GOOGLE_API_KEY";
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_SSID_PASSWORD";

#endif  // SECRET_H
```

## Usage

Upload `GoogleLocation.cpp` to ESP board and monitor the serial prints

## Credit

[WifiLocation](https://github.com/gmag11/WifiLocation) by gmag11
