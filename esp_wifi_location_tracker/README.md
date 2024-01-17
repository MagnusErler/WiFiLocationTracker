# ESPWiFiLocationTracker

## Setup

Google requires using secure TLS connection in order to use GeoLocation API. `WifiLocation1.cpp` includes GoogleCA certificate to check server chain. This certificate may expire.

You may get current certificate using this command (if you are using Linux)

```
openssl s_client -servername www.googleapis.com -showcerts \
 -connect www.googleapis.com:443 < /dev/null \
 |  awk '/^-----BEGIN CERTIFICATE-----/,/^-----END CERTIFICATE-----/{if(++m==1)n++;if(n==2)print;if(/^-----END CERTIFICATE-----/)m=0}' \
 > googleCA.cer
```

Then, you can copy file content and overwrite line 11-42 of `WifiLocation1.cpp`.

## Usage

Upload `GoogleLocation.cpp` to ESP board and monitor
