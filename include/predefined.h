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

uint8_t _broadcast[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t _fake[6] = {0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc};

uint8_t TPLINK_AP_Addrs[][6] = 
{
  {0xec, 0x17, 0x2f, 0xf3, 0x0b, 0x28}, // TP LINK
  {0xec, 0x17, 0x2f, 0xf3, 0x0b, 0x29}, // TP LINK
};

#define AP_ADDR_NUM sizeof(TPLINK_AP_Addrs)/6


uint8_t TPLINK_STA_Addrs[][6] = {
  {0xec, 0x26, 0x51, 0x61, 0x68, 0x75},
  // {0xDC, 0xA6, 0x32, 0xF9, 0xCF, 0xB7},
  {0x22, 0xd2, 0xcb, 0x3c, 0xc7, 0x1b},
  // {0xBC, 0x54, 0x2F, 0x9D, 0x3C, 0x8F},
  // {0xdc, 0xa6, 0x32, 0xf9, 0xcf, 0xb7},
  // {0xf8, 0xff, 0xc2, 0xa8, 0x81, 0xd6},
  // {0x06, 0x05, 0x88, 0x2f, 0xb6, 0x4d},
  // {0x10, 0x10, 0x81, 0x6e, 0x1a, 0x08}, // 只有这一台是会回应的
  // {0x40, 0x22, 0xd8, 0x3c, 0xc1, 0x81},
  // {0xd4, 0x25, 0x8b, 0x70, 0xad, 0xe1},
  // {0x01, 0x00, 0x52, 0x7f, 0xff, 0xfa},
  // {0xf8, 0x8c, 0x21, 0x8b, 0x45, 0xec},
  // {0xb8, 0x3b, 0xcc, 0x0e, 0xb0, 0x00},
  {0xec, 0x17, 0x2f, 0xf3, 0x0b, 0x28}, // TP LINK

};

#define STA_ADDR_NUM sizeof(TPLINK_STA_Addrs)/6

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
    ft_base = WIFI_PROMIS_FILTER_MASK_DATA|WIFI_PROMIS_FILTER_MASK_MGMT;
    ft_ctrl = WIFI_PROMIS_CTRL_FILTER_MASK_ACK|WIFI_PROMIS_CTRL_FILTER_MASK_ALL;
    m_sniffer.sniffer_setup(Serial, sniff_out);
    m_sniffer.set_filter(ft_base, ft_ctrl);
}

void setup_m_sniffer_ph2_getAddr(STA_sniffer& m_sniffer){
    uint32_t ft_base, ft_ctrl;
    ft_base = WIFI_PROMIS_FILTER_MASK_DATA;
    ft_ctrl = WIFI_PROMIS_CTRL_FILTER_MASK_ACK|WIFI_PROMIS_FILTER_MASK_ALL;
    set_ph2_tar(TPLINK_AP_Addrs[0]);
    m_sniffer.sniffer_setup(Serial, sniff_out_ph2_getAddr);
    m_sniffer.set_filter(ft_base, ft_ctrl);
};

void setup_m_sniffer_ph3_getTof(STA_sniffer& m_sniffer){
    uint32_t ft_base, ft_ctrl;
    set_ph3_tar(_fake);
    ft_base = WIFI_PROMIS_FILTER_MASK_DATA|WIFI_PROMIS_FILTER_MASK_CTRL|WIFI_PROMIS_FILTER_MASK_FCSFAIL|WIFI_PROMIS_FILTER_MASK_MISC|WIFI_PROMIS_FILTER_MASK_ALL;
    ft_ctrl = WIFI_PROMIS_CTRL_FILTER_MASK_ACK|WIFI_PROMIS_CTRL_FILTER_MASK_ALL;
    m_sniffer.sniffer_setup(Serial, sniff_out_ph3_getTof);
    m_sniffer.set_filter(ft_base, ft_ctrl);
};

void setup_m_server(AP_server& m_server){
    // register_func(2, "Get /Restart", url_make_header);
    m_server = AP_server(80);
    m_server.init_server();
}

uint8_t faker_src_mac[6] = {0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc};
uint8_t faker_dest_mac[6] = {0xdc,0xa6, 0x32, 0xf9, 0xcf, 0xb7};

int list_num = 7;
void setup_m_faker_null_ack(beacon_faker& m_faker){
  memcpy(faker_src_mac, _fake, 6);
  m_faker.set_faker_srcAddr(faker_src_mac);
  m_faker.setup(&Serial);
}

uint8_t faker_destAP_mac[6] = {0xdc,0xa6, 0x32, 0xf9, 0xcf, 0xb7};
uint8_t fake_bssid[6] = {0xec, 0x17, 0x2f, 0xf3, 0x0b, 0x29};
void setup_m_faker_beacon_wake(beacon_faker& m_faker){
  memcpy(faker_destAP_mac, TPLINK_AP_Addrs[0], 6);
  m_faker.set_faker_srcAddr(faker_destAP_mac);
  m_faker.setup(&Serial);
}

void setup_m_faker(beacon_faker& m_faker){
  m_faker.set_faker_srcAddr(faker_src_mac);
  m_faker.setup(&Serial);
}

void faker_loop_wakingup(beacon_faker& m_faker){
  // 发送伪造beacon唤醒设备
    delay(500);
    // m_faker.attack_beacon_wake(_broadcast);
    m_faker.send_basic_beacon();
    // delay(100);
    Serial.println("sending beacon to call up devs to send null.");
    lit_light(Serial);
    delay(500);
    unlit_light(Serial);
}

void faker_loop_call4ack(beacon_faker& m_faker){
  // 发送伪造空包，攻击设备
    uint8_t *cur;
    for (size_t i = 0; i < STA_ADDR_NUM; i++)
    {
      unlit_light(Serial);
      delay(500);
      cur = TPLINK_STA_Addrs[i];
      m_faker.attack_target_null(cur);
      Serial.printf("Send null, atk the %d sta: %02x:%02x:%02x:%02x:%02x:%02x\n", i, 
      *(cur), *(cur+1), *(cur+2), *(cur+3), *(cur+4), *(cur+5));
      lit_light(Serial);
      delay(500);
    }

    // unlit_light(Serial);
    // delay(1000);
    // m_faker.attack_target_null(TPLINK_STA_Addrs[0]);
    // lit_light(Serial);
    // delay(1000);

}

#endif