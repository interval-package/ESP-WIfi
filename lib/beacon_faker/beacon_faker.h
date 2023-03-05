#ifndef BEACON_FAKER_H
#define BEACON_FAKER_H
#include <esp_wifi.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>


#define SRCADDR_OFFSET 10
#define BSSID_OFFSET 16
#define SEQNUM_OFFSET 22
#define BEACON_SSID_OFFSET 38
#define TIM_OFFSET 52


#define MAX_BEACON_LEN 2048

class beacon_faker{
    private:
        uint8_t beacon_buffer[MAX_BEACON_LEN];
        int total_offset=0;

        uint8_t _src_address[6];
        uint8_t _BSSID[6];
        uint8_t _TIM_bit_map[2];

        HardwareSerial* mSerial;

    public:
        void set_faker_srcAddr();
        void send_basic_beacon();
        void send_imitate_beacon(uint8_t source_addr[6], uint8_t *TIM_bit_map, int map_len);
        void attack_target_null(uint8_t mmac[6]);
};

struct beacon_TIM_template{
    uint8_t ID=0x05;                // element ID
    uint8_t length=0x04;            // length from 5 - 255
    uint8_t DTIM_count=0x01;        // DTIM_count
    uint8_t DTIM_period=0x02;       // DTIM_period
    uint8_t bitmap_control=0x00;    // 0-6 bitmap_control.offset 7 bitmap_control.broadcast 
    /* bit map */
    // 这个就是变长了的
    // This comprises (length-4)×8 bits, each representing a currently-associated station. 
    // The low-order bit of the first octet represents station with association ID (bitmap_control.offset×16). 
    // Bits outside the partial bitmap are implicitly zero.
    uint8_t bitmap[251]={0xff};
    // ...partial_virtual_bitmap
};

//The MAC header structure we will use. It is in the same format as in IEEE. Some data types are in bits others are in bytes 
//so that we can take any piece of the 6 bytes and display in HEX (mainly the addresses).
//Wireshark implementation does not need this seperately, this is only for testing out the serial monitor (shown in previous project reports).
// typedef struct {
//     unsigned frame_ctrl:16;
//     unsigned duration_id:16;
//     uint8_t addr1[6]; /* receiver mac address */
//     uint8_t addr2[6]; /* original sender mac address */
//     uint8_t addr3[6]; /* BSSID */
//     unsigned sequence_ctrl:16;
// } wifi_packet_mac_hdr_t;

// //Data Frame/Packet struct to split captured packet into mac header and payload.
// typedef struct {
//     wifi_packet_mac_hdr_t hdr;
//     uint8_t payload[0];
// } wifi_captured_packet_t;


struct mac_header_template{
    uint8_t Frame_Ctrl[2] = {0x80, 0x00};
    uint8_t Duration[2] = {0x00, 0x00,};							// 2-3: Duration
    uint8_t dest_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff,};// 4-9: Destination address (broadcast)
    uint8_t scr_addr[6] = {0xaa, 0xaa, 0xbb, 0xbb, 0xbb, 0xbb,};	// 10-15: Source address
    uint8_t BSSID[6] = {0xaa, 0xaa, 0xbb, 0xbb, 0xbb, 0xbb,};	// 16-21: BSSID
    uint8_t seq[2] = {0x00, 0x00};
};

#define SSID_NUMS 0x00
struct beacon_body_template{
    /* beacon body */
    uint8_t TimeStamp[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,};
    uint8_t b_interval[2] = {0x64, 0x00,};
    uint8_t cap_info[2] = {0x31, 0x04,};
    uint8_t SSID[SSID_NUMS + 2] = {0x00, SSID_NUMS,};
    /* FILL CONTENT HERE */
    uint8_t rates[10] = 
    {0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24,};
    uint8_t DS_parmas[3] = {0x03, 0x01, 0x01,};
    struct beacon_TIM_template TIM;
};

struct beacon_template{
    struct mac_header_template head;
    struct beacon_body_template body;
};

#endif