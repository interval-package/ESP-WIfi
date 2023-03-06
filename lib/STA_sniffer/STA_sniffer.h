#ifndef STA_SNIFFER_H
#define STA_SNIFFER_H

/*
 * Author: Abir Mojumder 
*/

#include "SPI.h"
#include <TimeLib.h>
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "inttypes.h"

#include "sniffer_structs.h"

//channel hopping interval in milliseconds
#define INTERVAL 250

//Parser method to send PCAP formated packets to serial port. (PCAP needs timestamps in sec, microsec, payload length and payload).
void serialPacket(uint32_t time_sec, uint32_t time_usec, uint32_t len, uint8_t* payload_buf);

//convert 32 bit input to 4 bytes output to serial port
//Need this method because Wireshark serial program only reads 1 byte each time and writes to a capture file.
void serialout_32bit(uint32_t input);

//convert 16 bit input to 2 bytes output to serial port
void serialout_16bit(uint16_t input);

void serialout_write(char * target, int len);

void make_addr(uint8_t* addr, char *ans);

bool addr_cmp(uint8_t* addr1, uint8_t* addr2);

void sniff_disp_base_info(uint8_t type_info, uint8_t subtype_info, wifi_pkt_rx_ctrl_t header, wifi_captured_packet_t* mac_pak);
void sniff_disp_base_info_ack(uint8_t type_info, uint8_t subtype_info, wifi_pkt_rx_ctrl_t header, wifi_captured_packet_t* mac_pak);

//Callback method to capture promiscuous packets
void sniff_out(void* buf, wifi_promiscuous_pkt_type_t type);
void sniff_out_parsed(void* buf, wifi_promiscuous_pkt_type_t type);
void sniff_out_ph2_getAddr(void* buf, wifi_promiscuous_pkt_type_t type);
void set_ph2_tar(uint8_t mac[6]);
void sniff_out_ph3_getTof(void* buf, wifi_promiscuous_pkt_type_t type);

class STA_sniffer{
    public:
        uint32_t magic_num = 0xa1b2c3d4; //The default magic number. With this magic number, every packet needs timestamp in seconds and Microseconds.
        uint16_t version_major = 2; /* major version number */
        uint16_t version_minor = 4; /* minor version number */
        uint32_t thiszone = 0; /* GMT to local correction */
        uint32_t sigfigs = 0; /* accuracy of timestamps */
        uint32_t snaplen = 65535; /* max length of captured packets, in octets */
        uint32_t network = 105; /* data link type */

        //set scanning channel (main loop changes channel in given interval)
        int channel = 1;
        int max_channel = 11;
        unsigned long lastHopTime = 0;
        boolean activate_hopping = true;

    public:
        void setupPCAP();
        void set_filter(uint32_t base_flag, uint32_t ctr_flag);
        void sniffer_start_wifi();
        void sniffer_setup(HardwareSerial& _Serial, wifi_promiscuous_cb_t cb);

        //Main Functions that run continuously:
        //This function runs if channel hopping is enabled during initialization
        void sniffer_loop();
};

#endif