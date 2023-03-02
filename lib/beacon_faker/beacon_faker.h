#ifndef BEACON_FAKER_H
#define BEACON_FAKER_H
#include <esp_wifi.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>

#define BEACON_SSID_OFFSET 38
#define SRCADDR_OFFSET 10
#define BSSID_OFFSET 16
#define SEQNUM_OFFSET 22

uint8_t beacon_raw[] = {
    0x80, 0x00,							// 0-1: Frame Control
    0x00, 0x00,							// 2-3: Duration
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,				// 4-9: Destination address (broadcast)
    0xba, 0xde, 0xaf, 0xfe, 0x00, 0x06,				// 10-15: Source address
    0xba, 0xde, 0xaf, 0xfe, 0x00, 0x06,				// 16-21: BSSID
    0x00, 0x00,							// 22-23: Sequence / fragment number
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,			// 24-31: Timestamp (GETS OVERWRITTEN TO 0 BY HARDWARE)
    0x64, 0x00,							// 32-33: Beacon interval
    0x31, 0x04,							// 34-35: Capability info
    0x00, 0x00, /* FILL CONTENT HERE */				// 36-38: SSID parameter set, 0x00:length:content
    0x01, 0x08, 0x82, 0x84,	0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24,	// 39-48: Supported rates
    0x03, 0x01, 0x01,						// 49-51: DS Parameter set, current channel 1 (= 0x01),
    0x05, 0x04, 0x01, 0x02, 0x00, 0x00,				// 52-57: Traffic Indication Map
};

class beacon_faker{
    private:
        int len;

    public:
        static void send_basic_beacon(HardwareSerial &_Serial){
            esp_err_t err_info;
            err_info = esp_wifi_80211_tx(WIFI_IF_AP, beacon_raw, sizeof(beacon_raw), false);
            switch (err_info){
            case ESP_OK:
                _Serial.println("Success");
                break;

            case ESP_ERR_WIFI_IF:
                _Serial.println("Get invalid interface");
                break;

            case ESP_ERR_INVALID_ARG:
                _Serial.println("Get invalid args");
                break;

            default:
                _Serial.println("Unrecognized err");
                break;
                }
        }
};

#endif