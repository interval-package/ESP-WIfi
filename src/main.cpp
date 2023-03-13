#include "predefined.h"

// uint8_t fake_addr = "10:10:81:6e:1a:08";

// #define COMPLE_TYPE_SERVER
// #define COMPLE_TYPE_SNIFF
// #define COMPLE_TYPE_PH2_SNIFF
// #define COMPLE_TYPE_PH3_SNIFF
#define COMPLE_TYPE_PH2_ATK
// #define COMPLE_TYPE_PH3_ATK


#ifdef COMPLE_TYPE_SERVER
AP_server m_server;
#endif

#if defined(COMPLE_TYPE_PH2_ATK) || defined(COMPLE_TYPE_PH3_ATK)
beacon_faker m_faker;
#endif

#if defined(COMPLE_TYPE_SNIFF) || defined(COMPLE_TYPE_PH2_SNIFF) || defined(COMPLE_TYPE_PH3_SNIFF)
STA_sniffer m_sniffer;
#endif


void setup() {
  pinMode(LED_BLUE, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println();
  Serial.println("Testing...............");
  delay(5000);
  lit_light(Serial);

  #ifdef COMPLE_TYPE_SERVER
  setup_m_server(m_server);
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
  faker_loop_wakingup(m_faker);
  #endif

  #ifdef COMPLE_TYPE_PH3_ATK
  faker_loop_call4ack(m_faker);
  #endif

  #if defined(COMPLE_TYPE_SNIFF) || defined(COMPLE_TYPE_PH2_SNIFF) || defined(COMPLE_TYPE_PH3_SNIFF)
  // if your wifi is hopping enable this.
  m_sniffer.sniffer_loop();
  #endif
}

