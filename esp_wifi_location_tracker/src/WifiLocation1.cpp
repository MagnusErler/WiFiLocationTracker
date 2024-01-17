//
//
//

#include "WifiLocation1.h"

const char* googleApisHost = "www.googleapis.com";
const char* googleApiUrl = "/geolocation/v1/geolocate";

#ifdef ARDUINO_ARCH_ESP32
const char* googleCA = "-----BEGIN CERTIFICATE-----\n" \
"MIIFljCCA36gAwIBAgINAgO8U1lrNMcY9QFQZjANBgkqhkiG9w0BAQsFADBHMQsw\n" \
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
"MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMjAwODEzMDAwMDQyWhcNMjcwOTMwMDAw\n" \
"MDQyWjBGMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
"Y2VzIExMQzETMBEGA1UEAxMKR1RTIENBIDFDMzCCASIwDQYJKoZIhvcNAQEBBQAD\n" \
"ggEPADCCAQoCggEBAPWI3+dijB43+DdCkH9sh9D7ZYIl/ejLa6T/belaI+KZ9hzp\n" \
"kgOZE3wJCor6QtZeViSqejOEH9Hpabu5dOxXTGZok3c3VVP+ORBNtzS7XyV3NzsX\n" \
"lOo85Z3VvMO0Q+sup0fvsEQRY9i0QYXdQTBIkxu/t/bgRQIh4JZCF8/ZK2VWNAcm\n" \
"BA2o/X3KLu/qSHw3TT8An4Pf73WELnlXXPxXbhqW//yMmqaZviXZf5YsBvcRKgKA\n" \
"gOtjGDxQSYflispfGStZloEAoPtR28p3CwvJlk/vcEnHXG0g/Zm0tOLKLnf9LdwL\n" \
"tmsTDIwZKxeWmLnwi/agJ7u2441Rj72ux5uxiZ0CAwEAAaOCAYAwggF8MA4GA1Ud\n" \
"DwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwEgYDVR0T\n" \
"AQH/BAgwBgEB/wIBADAdBgNVHQ4EFgQUinR/r4XN7pXNPZzQ4kYU83E1HScwHwYD\n" \
"VR0jBBgwFoAU5K8rJnEaK0gnhS9SZizv8IkTcT4waAYIKwYBBQUHAQEEXDBaMCYG\n" \
"CCsGAQUFBzABhhpodHRwOi8vb2NzcC5wa2kuZ29vZy9ndHNyMTAwBggrBgEFBQcw\n" \
"AoYkaHR0cDovL3BraS5nb29nL3JlcG8vY2VydHMvZ3RzcjEuZGVyMDQGA1UdHwQt\n" \
"MCswKaAnoCWGI2h0dHA6Ly9jcmwucGtpLmdvb2cvZ3RzcjEvZ3RzcjEuY3JsMFcG\n" \
"A1UdIARQME4wOAYKKwYBBAHWeQIFAzAqMCgGCCsGAQUFBwIBFhxodHRwczovL3Br\n" \
"aS5nb29nL3JlcG9zaXRvcnkvMAgGBmeBDAECATAIBgZngQwBAgIwDQYJKoZIhvcN\n" \
"AQELBQADggIBAIl9rCBcDDy+mqhXlRu0rvqrpXJxtDaV/d9AEQNMwkYUuxQkq/BQ\n" \
"cSLbrcRuf8/xam/IgxvYzolfh2yHuKkMo5uhYpSTld9brmYZCwKWnvy15xBpPnrL\n" \
"RklfRuFBsdeYTWU0AIAaP0+fbH9JAIFTQaSSIYKCGvGjRFsqUBITTcFTNvNCCK9U\n" \
"+o53UxtkOCcXCb1YyRt8OS1b887U7ZfbFAO/CVMkH8IMBHmYJvJh8VNS/UKMG2Yr\n" \
"PxWhu//2m+OBmgEGcYk1KCTd4b3rGS3hSMs9WYNRtHTGnXzGsYZbr8w0xNPM1IER\n" \
"lQCh9BIiAfq0g3GvjLeMcySsN1PCAJA/Ef5c7TaUEDu9Ka7ixzpiO2xj2YC/WXGs\n" \
"Yye5TBeg2vZzFb8q3o/zpWwygTMD0IZRcZk0upONXbVRWPeyk+gB9lm+cZv9TSjO\n" \
"z23HFtz30dZGm6fKa+l3D/2gthsjgx0QGtkJAITgRNOidSOzNIb2ILCkXhAd4FJG\n" \
"AJ2xDx8hcFH1mt0G/FX0Kw4zd8NLQsLxdxP8c4CU6x+7Nz/OAipmsHMdMqUybDKw\n" \
"juDEI/9bfU1lcKwrmz3O2+BtjjKAvpafkmO8l7tdufThcV4q5O8DIrGKZTqPwJNl\n" \
"1IXNDw9bg1kWRxYtnCQ6yICmJhSFm/Y3m6xv+cXDBlHz4n/FsRC6UfTd\n" \
"-----END CERTIFICATE-----\n";
#endif

String WifiLocation::MACtoString(uint8_t* macAddress) {
    uint8_t mac[6];
    char macStr[18] = { 0 };
#ifdef ARDUINO_ARCH_SAMD
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[5], macAddress[4], macAddress[3], macAddress[2], macAddress[1], macAddress[0]);
#elif defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
#endif
    return  String(macStr);
}

WifiLocation::WifiLocation(String googleKey) {
    _googleApiKey = googleKey;
}

// Function to get a list of surrounding WiFi signals in JSON format to get location via Google Location API
String WifiLocation::getSurroundingWiFiJson() {
    String wifiArray = "[\n";

    int8_t numWifi = WiFi.scanNetworks();
#ifdef DEBUG_WIFI_LOCATION
    Serial.println(String(numWifi) + " WiFi networks found");
#endif // DEBUG_WIFI_LOCATION
    for (uint8_t i = 0; i < numWifi; i++) {//numWifi; i++) {
      //Serial.print("WiFi.BSSID(i) = ");
      //Serial.println((char *)WiFi.BSSID(i));
#ifdef ARDUINO_ARCH_SAMD
        byte bssid[6];

        wifiArray += "{\"macAddress\":\"" + MACtoString(WiFi.BSSID(i,bssid)) + "\",";
#elif defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
        wifiArray += "{\"macAddress\":\"" + MACtoString(WiFi.BSSID(i)) + "\",";
#else
#error Only ESP8266 and SAMD platforms are supported
#endif
        wifiArray += "\"signalStrength\":" + String(WiFi.RSSI(i)) + ",";
        wifiArray += "\"channel\":" + String(WiFi.channel(i)) + "}";
        if (i < (numWifi - 1)) {
            wifiArray += ",\n";
        }
    }
#if defined ARDUINO_ARCH_ESP8266 || defined ARDUINO_ARCH_ESP32
    WiFi.scanDelete();
#endif
    wifiArray += "]";
#ifdef DEBUG_WIFI_LOCATION
    Serial.println("WiFi list :\n" + wifiArray);
#endif // DEBUG_WIFI_LOCATION
    return wifiArray;
}

// Calls Google Location API to get current location using surrounding WiFi signals inf
location_t WifiLocation::getGeoFromWiFi() {

    location_t location;
    String response = "";
#ifdef ARDUINO_ARCH_ESP32
    _client.setCACert(googleCA);
#endif
    if (_client.connect(googleApisHost, 443)) {
#ifdef DEBUG_WIFI_LOCATION
        Serial.println("Connected to API endpoint");
#endif // DEBUG_WIFI_LOCATION
    } else {
#ifdef DEBUG_WIFI_LOCATION
        Serial.println("HTTPS error");
#endif // DEBUG_WIFI_LOCATION
        return location;
    }

    String body = "{\"wifiAccessPoints\":" + getSurroundingWiFiJson() + "}";
#ifdef DEBUG_WIFI_LOCATION
    Serial.println("requesting URL: " + String(googleApiUrl) + "?key=" + _googleApiKey);
#endif // DEBUG_WIFI_LOCATION
    String request = String("POST ") + String(googleApiUrl);
    if (_googleApiKey != "")
        request += "?key=" + _googleApiKey;
    request += " HTTP/1.1\r\n";
    request += "Host: " + String(googleApisHost) + "\r\n";
    request += "User-Agent: ESP8266\r\n";
    request += "Content-Type:application/json\r\n";
    request += "Content-Length:" + String(body.length()) + "\r\n";
    request += "Connection: close\r\n\r\n";
    request += body;
#ifdef DEBUG_WIFI_LOCATION
    Serial.println("request: \n" + request);
#endif // DEBUG_WIFI_LOCATION

    _client.println(request);
#ifdef DEBUG_WIFI_LOCATION
    Serial.println("request sent");
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
#endif // DEBUG_WIFI_LOCATION
    int timer = millis();

    // Wait for response
    while (millis() - timer < MAX_CONNECTION_TIMEOUT) {
        if (_client.available())
            break;
        yield ();
    }
    while (_client.available()) {
#ifdef DEBUG_WIFI_LOCATION
        Serial.print(".");
#endif // DEBUG_WIFI_LOCATION
        response += _client.readString();
        yield ();
    }
#ifdef DEBUG_WIFI_LOCATION
    Serial.println();
#endif // DEBUG_WIFI_LOCATION
    if (response != "") {
#ifdef DEBUG_WIFI_LOCATION
        Serial.println("Got response:");
        Serial.println(response);
#endif // DEBUG_WIFI_LOCATION

        int index = response.indexOf("\"lat\":");
        if (index != -1) {
            String tempStr = response.substring(index);
            //Serial.println(tempStr);
            tempStr = tempStr.substring(tempStr.indexOf(":") + 1, tempStr.indexOf(","));
            //Serial.println(tempStr);
            location.lat = tempStr.toFloat();
#ifdef DEBUG_WIFI_LOCATION
            Serial.println("\nLat: " + String(location.lat, 7));
#endif // DEBUG_WIFI_LOCATION
        }

        index = response.indexOf("\"lng\":");
        if (index != -1) {
            String tempStr = response.substring(index);
            //Serial.println(tempStr);
            tempStr = tempStr.substring(tempStr.indexOf(":") + 1, tempStr.indexOf(","));
            //Serial.println(tempStr);
            location.lon = tempStr.toFloat();
#ifdef DEBUG_WIFI_LOCATION
            Serial.println("Lon: " + String(location.lon, 7));
#endif // DEBUG_WIFI_LOCATION
        }

        index = response.indexOf("\"accuracy\":");
        if (index != -1) {
            String tempStr = response.substring(index);
            //Serial.println(tempStr);
            tempStr = tempStr.substring(tempStr.indexOf(":") + 1, tempStr.indexOf("\n"));
            //Serial.println(tempStr);
            location.accuracy = tempStr.toFloat();
#ifdef DEBUG_WIFI_LOCATION
            Serial.println("Accuracy: " + String(location.accuracy) + "\n");
#endif // DEBUG_WIFI_LOCATION
        }
    }

    return location;
}
