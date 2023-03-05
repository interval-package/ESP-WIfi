#ifndef PREDEFINED_H
#define PREDEFINED_H

#ifndef LED_BLUE
#define LED_BLUE 2
#endif

#include <Arduino.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <freertos/FreeRTOS.h>

#include <HardwareSerial.h>

#include <AP_server.h>
#include <STA_sniffer.h>
#include <beacon_faker.h>

bool start_flag=false;
char start_word[] = "<<START>>";

void m_sniff_out(void* buf, wifi_promiscuous_pkt_type_t type){
  if(start_flag){
    start_flag = false;
    serialout_write(start_word, strlen(start_word));
  }
  //received packet
  wifi_promiscuous_pkt_t* pak = (wifi_promiscuous_pkt_t*) buf;

  //received control header (extract from received packet):
  wifi_pkt_rx_ctrl_t header = (wifi_pkt_rx_ctrl_t)pak->rx_ctrl;

  //time of receiving packet
  uint32_t time_sec = now(); //current timestamp 
  uint32_t time_usec = (unsigned int)(micros() - millis() * 1000);

  /* Two structs below were for testing the parsing only*/
  //const wifi_captured_packet_t *pkt_frame = (wifi_captured_packet_t *)pak->payload;
  //const wifi_packet_mac_hdr_t *mac_hdr = &pkt_frame->hdr;

  serialPacket(time_sec, time_usec, header.sig_len, pak->payload);
}

void url_make_header(HardwareSerial& _Serial){
  _Serial.println("Get to restart.");
  start_flag=true;
}

void setup_m_sniffer(STA_sniffer& m_sniffer){
    uint32_t ft_base, ft_ctrl;
    ft_base = WIFI_PROMIS_FILTER_MASK_DATA|WIFI_PROMIS_FILTER_MASK_CTRL;
    ft_ctrl = WIFI_PROMIS_CTRL_FILTER_MASK_ACK;
    m_sniffer.set_filter(ft_base, ft_ctrl);
    m_sniffer.sniffer_setup(Serial, sniff_out_parsed);
}

void setup_m_sniffer_ph2_getAddr(STA_sniffer& m_sniffer){
    uint32_t ft_base, ft_ctrl;
    ft_base = WIFI_PROMIS_FILTER_MASK_DATA;
    ft_ctrl = WIFI_PROMIS_CTRL_FILTER_MASK_ACK;
    m_sniffer.set_filter(ft_base, ft_ctrl);
    m_sniffer.sniffer_setup(Serial, sniff_out_parsed);
};

void setup_m_sniffer_ph3_getTof(STA_sniffer& m_sniffer){
    uint32_t ft_base, ft_ctrl;
    ft_base = WIFI_PROMIS_FILTER_MASK_DATA|WIFI_PROMIS_FILTER_MASK_CTRL;
    ft_ctrl = WIFI_PROMIS_CTRL_FILTER_MASK_ACK;
    m_sniffer.set_filter(ft_base, ft_ctrl);
    m_sniffer.sniffer_setup(Serial, sniff_out_parsed);
};

void setup_m_server(AP_server& m_server){
    // register_func(2, "Get /Restart", url_make_header);
    m_server = AP_server(80);
    m_server.init_server();
}

void setup_m_faker(beacon_faker& m_faker){

}




#endif