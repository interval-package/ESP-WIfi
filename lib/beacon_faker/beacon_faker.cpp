#include <beacon_faker.h>

void beacon_faker::send_basic_beacon(HardwareSerial &_Serial){
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