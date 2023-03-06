#ifndef SNIFFER_STRUCT_H
#define SNIFFER_STRUCT_H

#define TYPE_MGMT 0
#define TYPE_CTRL 1
#define TYPE_DATA 2
#define TYPE_MISC 3

#define SUBTYPE_MGMT_BEACON 0b1000
// #define SUBTYPE_MGMT_

#define SUBTYPE_CTRL_ACK 0b1101
// #define SUBTYPE_CTRL_

#define SUBTYPE_DATA_DATA 0b0000
#define SUBTYPE_DATA_NULL 0b0100


//The MAC header structure we will use. It is in the same format as in IEEE. Some data types are in bits others are in bytes 
//so that we can take any piece of the 6 bytes and display in HEX (mainly the addresses).
//Wireshark implementation does not need this seperately, this is only for testing out the serial monitor (shown in previous project reports).
typedef struct {
    unsigned frame_ctrl:16;
    unsigned duration_id:16;
    uint8_t addr1[6]; /* receiver mac address */
    uint8_t addr2[6]; /* original sender mac address */
    uint8_t addr3[6]; /* BSSID */
    unsigned sequence_ctrl:16;
} wifi_packet_mac_hdr_t;

//Data Frame/Packet struct to split captured packet into mac header and payload.
typedef struct {
    wifi_packet_mac_hdr_t hdr;
    uint8_t payload[0];
} wifi_captured_packet_t;


#endif