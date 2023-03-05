#ifndef AP_SERVER_H
#define AP_SERVER_H
#include <WiFi.h>
#include <esp_wifi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Arduino.h>

#define LED_BLUE 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

struct url_action{
    const char* doc;
    const char* url;
    void (*func)(HardwareSerial& _Serial);
};

void lit_light(HardwareSerial& _Serial);
void unlit_light(HardwareSerial& _Serial);
void url_nope(HardwareSerial& _Serial);

class AP_server{
    private:
        const char *ssid = "ESP32_TEST_1";
        const char *password = "67184002";

        IPAddress local_IP;
        IPAddress gateway_IP;
        IPAddress subnet;
        uint8_t mmac[6] = {
            0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc
        };

        // IPAddress local_IP(192,168,101,1);
        // IPAddress gateway_IP(192,168,137,1);
        // IPAddress subnet(255,255,255,0);

        struct url_action url_patterns[256] = {
            {
                .doc = "Unlit led.",
                .url = "GET /L",
                .func = unlit_light,
            },
            {
                .doc = "Lit led.",
                .url = "GET /H",
                .func = lit_light,
            },
            {
                .doc = "nothing.",
                .url = "GET /Restart",
                .func = url_nope,
            },
        };

        int url_nums=3;

    public:
        WiFiServer server;
        AP_server(int port=80);
        void init_server();
        void Listen(HardwareSerial& _Serial);
        void tackle_conn(WiFiClient &client, HardwareSerial& _Serial);
        void init_server(WiFiServer &server, HardwareSerial& _Serial);

        void register_func(int idx, const char * url, void (*func)(HardwareSerial& _Serial));

        void disp_wifi_info(WiFiClient &client);
};



#endif