
// WIFI
typedef enum
{
    LR1110_WIFI_TYPE_SCAN_B     = 0x01,  //!< Wi-Fi B
    LR1110_WIFI_TYPE_SCAN_G     = 0x02,  //!< Wi-Fi G
    LR1110_WIFI_TYPE_SCAN_N     = 0x03,  //!< Wi-Fi N
    LR1110_WIFI_TYPE_SCAN_B_G_N = 0x04,  //!< Wi-Fi B and Wi-Fi G/N
} lr1110_wifi_signal_type_scan_t;

typedef enum
{
    LR1110_WIFI_NO_CHANNEL   = 0x00,
    LR1110_WIFI_CHANNEL_1    = 0x01,  //!< Channel at frequency 2.412 GHz
    LR1110_WIFI_CHANNEL_2    = 0x02,  //!< Channel at frequency 2.417 GHz
    LR1110_WIFI_CHANNEL_3    = 0x03,  //!< Channel at frequency 2.422 GHz
    LR1110_WIFI_CHANNEL_4    = 0x04,  //!< Channel at frequency 2.427 GHz
    LR1110_WIFI_CHANNEL_5    = 0x05,  //!< Channel at frequency 2.432 GHz
    LR1110_WIFI_CHANNEL_6    = 0x06,  //!< Channel at frequency 2.437 GHz
    LR1110_WIFI_CHANNEL_7    = 0x07,  //!< Channel at frequency 2.442 GHz
    LR1110_WIFI_CHANNEL_8    = 0x08,  //!< Channel at frequency 2.447 GHz
    LR1110_WIFI_CHANNEL_9    = 0x09,  //!< Channel at frequency 2.452 GHz
    LR1110_WIFI_CHANNEL_10   = 0x0A,  //!< Channel at frequency 2.457 GHz
    LR1110_WIFI_CHANNEL_11   = 0x0B,  //!< Channel at frequency 2.462 GHz
    LR1110_WIFI_CHANNEL_12   = 0x0C,  //!< Channel at frequency 2.467 GHz
    LR1110_WIFI_CHANNEL_13   = 0x0D,  //!< Channel at frequency 2.472 GHz
    LR1110_WIFI_CHANNEL_14   = 0x0E,  //!< Channel at frequency 2.484 GHz
    LR1110_WIFI_ALL_CHANNELS = 0x0F,
} lr1110_wifi_channel_t;

typedef enum
{
    LR1110_WIFI_SCAN_MODE_BEACON =
        0x01,  //!< Exposes Beacons and Probe Responses Access Points frames until Period Beacon field (Basic result)
    LR1110_WIFI_SCAN_MODE_BEACON_AND_PKT =
        0x02,  //!< Exposes some Management Access Points frames until Period Beacon field, and some other packets frame
            //!< until third Mac Address field (Basic result)
    LR1110_WIFI_SCAN_MODE_FULL_BEACON =
        0x04,  //!< Exposes Beacons and Probes Responses Access Points frames until Frame Check Sequence (FCS) field
            //!< (Extended result). In this mode, only signal type LR1110_WIFI_TYPE_SCAN_B is executed and other signal
            //!< types are silently discarded.
    LR1110_WIFI_SCAN_MODE_UNTIL_SSID = 0x05,  //!< Exposes Beacons and Probes Responses Access Points frames until the end
                                           //!< of SSID field (Extended result) - available since firmware 0x0306
} lr1110_wifi_mode_t;

