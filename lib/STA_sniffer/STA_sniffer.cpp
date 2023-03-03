#include "STA_sniffer.h"

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

HardwareSerial *mSerial;

void serialout_write(char * target, int len){
    mSerial->write(target, len);
}

void STA_sniffer::setupPCAP(){
    serialout_32bit(magic_num);
    serialout_16bit(version_major);
    serialout_16bit(version_minor);
    serialout_32bit(thiszone);
    serialout_32bit(sigfigs);
    serialout_32bit(snaplen);
    serialout_32bit(network);
}

void STA_sniffer::sniffer_setup(HardwareSerial& _Serial, wifi_promiscuous_cb_t cb) {
    mSerial = &_Serial;

    //store serial buffer before flushing
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

    wifi_promiscuous_filter_t base_filter, ctr_filter;
    base_filter.filter_mask = WIFI_PROMIS_FILTER_MASK_CTRL;
    // esp_wifi_get_promiscuous_filter(&base_filter);
    esp_wifi_set_promiscuous_filter(&base_filter);
    ctr_filter.filter_mask = WIFI_PROMIS_CTRL_FILTER_MASK_ACK;
    // esp_wifi_get_promiscuous_ctrl_filter(&ctr_filter);
    esp_wifi_set_promiscuous_ctrl_filter(&ctr_filter);


    // 开始记录数据
    _Serial.println();
    _Serial.println("<<START>>"); 
    // Wireshark program needs this line to indicate that ESP32 is capturing packets.
    setupPCAP(); //sends PCAP header information once to the wireshark program
    //setting promiscuous mode here
    esp_wifi_set_promiscuous(true);

    // 这个函数是最重要的，监听到所有的包之后，就会调用cb这个函数，我们的接收就是要依靠这个函数
    //CSI function
    esp_wifi_set_promiscuous_rx_cb(cb);

    //Set primary and secondary channel(not used)
    wifi_second_chan_t secondchannel = (wifi_second_chan_t)NULL;
    esp_wifi_set_channel(channel,secondchannel);
}

//This function runs if channel hopping is enabled during initialization
void STA_sniffer::sniffer_loop() {

  if(activate_hopping){
      unsigned long currentTime = millis();
      if(currentTime - lastHopTime >= INTERVAL){ //if last channel changed greater than interval, change the channel
      lastHopTime = currentTime;
      channel++; //go to next channel if possible
      if(channel > max_channel) {
        channel = 1;
      }
      wifi_second_chan_t secondchannel = (wifi_second_chan_t)NULL;
      esp_wifi_set_channel(channel,secondchannel);
    }
  }

}

//Main Functions that run continuously:


//Callback method to capture promiscuous packets
void sniff_out(void* buf, wifi_promiscuous_pkt_type_t type){
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

//Parser method to send PCAP formated packets to serial port. (PCAP needs timestamps in sec, microsec, payload length and payload).
void serialPacket(uint32_t time_sec, uint32_t time_usec, uint32_t len, uint8_t* payload_buf){

    uint32_t orig_len = len; // number of octets of packet saved in file
    uint32_t incl_len = len; // actual length of packet

    //if received packet is greater than snaplen, limit it, otherwise malforms the wireshark output.
    // if(incl_len > snaplen){
    //     incl_len = snaplen;
    // }

    //send data to serial port in the order time in seconds > time in usec > incl_len > orig_len > payload
    serialout_32bit(time_sec);
    serialout_32bit(time_usec);
    serialout_32bit(incl_len);
    serialout_32bit(orig_len);
    mSerial->write(payload_buf, incl_len);
}

//convert 32 bit input to 4 bytes output to serial port
//Need this method because Wireshark serial program only reads 1 byte each time and writes to a capture file.
void serialout_32bit(uint32_t input){
    uint8_t val[4];

    //shift bits to the right to get every chunk of bytes
    val[0] = input;
    val[1] = input >> 8;
    val[2] = input >> 16;
    val[3] = input >> 24;
    mSerial->write(val, 4);
}

//convert 16 bit input to 2 bytes output to serial port
void serialout_16bit(uint16_t input){
    uint8_t val[2];

    //shift bits to the right to get every chunk of bytes
    val[0] = input;
    val[1] = input >> 8;
    mSerial->write(val, 2);
}