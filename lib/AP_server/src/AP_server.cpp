#include "AP_server.h"

AP_server::AP_server(int port){
  server = WiFiServer(80);
}

void AP_server::init_server(){
  init_server(server, Serial);
  server.begin();
}

void AP_server::Listen(HardwareSerial& _Serial){
  WiFiClient client = server.available();
  AP_server::tackle_conn(client,_Serial);
}


// below are static func

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
            client.print("<h1>Zza simple wifi using esp32</h1>");
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

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

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BLUE, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BLUE, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    _Serial.println("Client Disconnected.");
  }
}

void AP_server::init_server(WiFiServer &server, HardwareSerial& _Serial){
  // Set these to your desired credentials.

  _Serial.println("Configuring access point...");

  const char *ssid = "ESP32_TEST";
  const char *password = nullptr;

  IPAddress local_IP(192,168,101,1);
  IPAddress gateway_IP(192,168,137,1);
  IPAddress subnet(255,255,255,0);

    // You can remove the password parameter if you want the AP to be open.
  WiFi.softAPConfig(local_IP, gateway_IP, subnet);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  _Serial.print("AP IP address: ");
  _Serial.println(myIP);
  // server.begin();
  Serial.println("Server started");
}

