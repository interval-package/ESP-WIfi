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

//channel hopping interval in milliseconds
#define INTERVAL 250

//Initialize phase (setup PCAP file headers and ESP32 Wifi configuration)
//PCAP file required headers and values (information in https://wiki.wireshark.org/Development/LibpcapFileFormat)

// uint32_t magic_num = 0xa1b2c3d4; //The default magic number. With this magic number, every packet needs timestamp in seconds and Microseconds.
// uint16_t version_major = 2; /* major version number */
// uint16_t version_minor = 4; /* minor version number */
// uint32_t thiszone = 0; /* GMT to local correction */
// uint32_t sigfigs = 0; /* accuracy of timestamps */
// uint32_t snaplen = 65535; /* max length of captured packets, in octets */
// uint32_t network = 105; /* data link type */

// //set scanning channel (main loop changes channel in given interval)
// int channel = 1;
// int max_channel = 11;
// unsigned long lastHopTime = 0;
// boolean activate_hopping = true;


//Callback method to capture promiscuous packets
void sniff_out(void* buf, wifi_promiscuous_pkt_type_t type);

//Parser method to send PCAP formated packets to serial port. (PCAP needs timestamps in sec, microsec, payload length and payload).
void serialPacket(uint32_t time_sec, uint32_t time_usec, uint32_t len, uint8_t* payload_buf);

//convert 32 bit input to 4 bytes output to serial port
//Need this method because Wireshark serial program only reads 1 byte each time and writes to a capture file.
void serialout_32bit(uint32_t input);

//convert 16 bit input to 2 bytes output to serial port
void serialout_16bit(uint16_t input);

void serialout_write(char * target, int len);

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

        //The MAC header structure we will use. It is in the same format as in IEEE. Some data types are in bits others are in bytes 
        //so that we can take any piece of the 6 bytes and display in HEX (mainly the addresses).
        //Wireshark implementation does not need this seperately, this is only for testing out the serial monitor (shown in previous project reports).
        //typedef struct {
        //  unsigned frame_ctrl:16;
        //  unsigned duration_id:16;
        //  uint8_t addr1[6]; /* receiver mac address */
        //  uint8_t addr2[6]; /* original sender mac address */
        //  uint8_t addr3[6]; /* BSSID */
        //  unsigned sequence_ctrl:16;
        //} wifi_packet_mac_hdr_t;

        //Data Frame/Packet struct to split captured packet into mac header and payload.
        //typedef struct {
        //  wifi_packet_mac_hdr_t hdr;
        //  uint8_t payload[0];
        //} wifi_captured_packet_t;

        void sniffer_setup(HardwareSerial& _Serial, wifi_promiscuous_cb_t cb);

        //Main Functions that run continuously:
        //This function runs if channel hopping is enabled during initialization
        void sniffer_loop();
};

#endif