#include "predefined.h"

AP_server m_server;
beacon_faker m_faker;
STA_sniffer m_sniffer;

// uint8_t fake_addr = "10:10:81:6e:1a:08";

// #define COMPLE_TYPE_SERVER
// #define COMPLE_TYPE_SNIFF
#define COMPLE_TYPE_PH2_SNIFF
// #define COMPLE_TYPE_PH3_SNIFF
// #define COMPLE_TYPE_PH2_ATK
// #define COMPLE_TYPE_PH3_ATK


void setup() {
  pinMode(LED_BLUE, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println();
  Serial.println("Testing...............");
  delay(5000);
  lit_light(Serial);

  #ifdef COMPLE_TYPE_SERVER
  setup_m_sniffer_ph3_getTof(m_sniffer);
  #endif

  #ifdef COMPLE_TYPE_SNIFF
  setup_m_sniffer(m_sniffer);
  #endif

  #ifdef COMPLE_TYPE_PH2_SNIFF
  setup_m_sniffer_ph2_getAddr(m_sniffer);
  #endif

  #ifdef COMPLE_TYPE_PH3_SNIFF
  setup_m_sniffer_ph3_getTof(m_sniffer);
  #endif

  #ifdef COMPLE_TYPE_PH2_ATK
  setup_m_faker_beacon_wake(m_faker);
  #endif

  #ifdef COMPLE_TYPE_PH3_ATK
  setup_m_faker_null_ack(m_faker);
  #endif
}

void loop() {

  #ifdef COMPLE_TYPE_SERVER
  m_server.Listen(Serial);
  #endif

  #ifdef COMPLE_TYPE_PH2_ATK
  faker_loop_wake(m_faker);
  #endif

  #ifdef COMPLE_TYPE_PH3_ATK
  faker_loop(m_faker);
  #endif

  // if your wifi is hopping enable this.
  // m_sniffer.sniffer_loop();
}

