#include "AP_server.h"

AP_server::AP_server(int port){
  local_IP    = IPAddress(192,168,1,1);
  gateway_IP  = IPAddress(192,168,137,1);
  subnet      = IPAddress(255,255,255,0);
  server      = WiFiServer(80);
}

void AP_server::init_server(){
  init_server(server, Serial);
  server.begin();
}

void AP_server::Listen(HardwareSerial& _Serial){
  WiFiClient client = server.available();
  tackle_conn(client,_Serial);
}


void AP_server::tackle_conn(WiFiClient &client, HardwareSerial& _Serial){
if (client) {                             // if you get a client,
    _Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
            char c = client.read();             // read a byte, then
            _Serial.write(c);                    // print it out the serial monitor
            if (c == '\n') {                    // if the byte is a newline character

                // if the current line is blank, you got two newline characters in a row.
                // that's the end of the client HTTP request, so send a response:
                if (currentLine.length() == 0) {
                    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                    // and a content-type so the client knows what's coming, then a blank line:
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println();

                    // the content of the HTTP response follows the header:
                    client.print("<h1>Zza simple wifi using esp32</h1>\n");
                            // Check actions:
                    for (size_t i = 0; i < url_nums; i++)
                    {
                        client.printf("Click <a href=\"%s\">here</a>\t %s<br>\n", url_patterns[i].url+4, url_patterns[i].doc);
                    }

                    client.print("</br>\n");

                    disp_wifi_info(client);

                    // The HTTP response ends with another blank line:
                    client.println();
                    // break out of the while loop:
                    break;
                } else {    // if you got a newline, then clear currentLine:
                    currentLine = "";
              }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check actions:
        for (size_t i = 0; i < url_nums; i++)
        {
          if (currentLine.endsWith(url_patterns[i].url)) {
            (*(url_patterns[i].func))(_Serial);
          }
        }
        }
    }
    // close the connection:
    client.stop();
    _Serial.println("Client Disconnected.");
}
}


void AP_server::disp_wifi_info(WiFiClient &client){
    /*
    <table>
    <thead>
        <tr>
            <th>The table header</th>
            <th>The table header</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>The table body</td>
            <td>with two columns</td>
        </tr>
    </tbody>
    </table>
    */
    wifi_sta_list_t wifi_list;
    esp_wifi_ap_get_sta_list(&wifi_list);
    client.print("<table>\n"
                 "  <thead>\n"
                 "      <tr>\n"
                 "          <th>id</th>\n"
                 "          <th>mac</th>\n"
                 "          <th>rssi</th>\n"
                 "      </tr>\n"
                 "  </thead>\n"
                 "  <tbody>\n");

    for (size_t i = 0; i < wifi_list.num; i++)
    {
      client.printf("<tr><td>%d</td> <td>%02x:%02x:%02x:%02x:%02x:%02x</td> <td>%d</td></tr>\n",
      i,
      wifi_list.sta[i].mac[0],
      wifi_list.sta[i].mac[1],
      wifi_list.sta[i].mac[2],
      wifi_list.sta[i].mac[3],
      wifi_list.sta[i].mac[4],
      wifi_list.sta[i].mac[5],
      wifi_list.sta[i].rssi);
    }

    client.print("  </tbody>\n"
                 "</table>\n");
}

/*
  @attention: this func will not begin server, please begin it
*/ 
void AP_server::init_server(WiFiServer &server, HardwareSerial& _Serial){
    // Set these to your desired credentials.

    _Serial.println("Configuring access point...");

    // You can remove the password parameter if you want the AP to be open.
    WiFi.softAPConfig(local_IP, gateway_IP, subnet);
    WiFi.softAP(ssid, password);

    esp_wifi_set_mac(WIFI_IF_AP, mmac);

    IPAddress myIP = WiFi.softAPIP();
    _Serial.print("AP IP address: ");
    _Serial.println(myIP);
    // server.begin();
    Serial.println("Server started");
}

void AP_server::register_func(int idx, const char * url, void (*func)(HardwareSerial& _Serial)){
    url_patterns[idx].url = url;
    url_patterns[idx].func = func;
}

void lit_light(HardwareSerial& _Serial){
    digitalWrite(LED_BLUE, HIGH);
}

void unlit_light(HardwareSerial& _Serial){
    digitalWrite(LED_BLUE, LOW);
}

void url_nope(HardwareSerial& _Serial){
}

