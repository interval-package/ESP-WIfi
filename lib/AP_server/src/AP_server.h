#ifndef AP_SERVER_H
#define AP_SERVER_H
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Arduino.h>

#define LED_BLUE 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

class AP_server{
    private:
        const char *ssid = "ESP32_TEST";
        const char *password = nullptr;
        // IPAddress local_IP(192,168,101,1);
        // IPAddress gateway_IP(192,168,137,1);
        // IPAddress subnet(255,255,255,0);

    public:
        WiFiServer server;
        AP_server(int port=80);
        void init_server();
        void Listen(HardwareSerial& _Serial);
        void static tackle_conn(WiFiClient &client, HardwareSerial& _Serial);
        void static init_server(WiFiServer &server, HardwareSerial& _Serial);
};

#endif