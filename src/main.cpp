#include "predefined.h"

AP_server m_server;
beacon_faker m_faker;
STA_sniffer m_sniffer;

// uint8_t fake_addr = "10:10:81:6e:1a:08";

void setup() {
  pinMode(LED_BLUE, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println();
  Serial.println("Testing...............");
  delay(5000);
  lit_light(Serial);

  setup_m_sniffer(m_sniffer);
  setup_m_server(m_server);
  setup_m_faker(m_faker);

}

void loop() {

  m_server.Listen(Serial1);

  // beacon_faker::send_basic_beacon(Serial);


  // if your wifi is hopping enable this.
  // m_sniffer.sniffer_loop();
}

