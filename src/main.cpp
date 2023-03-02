#include <Arduino.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <freertos/FreeRTOS.h>

#include <HardwareSerial.h>

#include <AP_server.h>
// #include <AP_sniffer.h>
// #include <beacon_faker.h>


#ifndef LED_BLUE
#define LED_BLUE 2
#endif

AP_server m_server;

void setup() {
  pinMode(LED_BLUE, OUTPUT);

// 115299
  Serial.begin(9600);
  Serial.println();

  m_server = AP_server(80);
  m_server.init_server();
}

void loop() {

  // beacon_faker::send_basic_beacon(Serial);

  m_server.Listen(Serial);
}
