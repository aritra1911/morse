#include <WiFi.h>
#include <HardwareSerial.h>
#include "network_handler.h"

#define SSID "ray"
#define PASSWORD "unlocknet"

extern WiFiServer server;
extern WiFiClient client;

void connect_to_network() {
  Serial.print("Connecting to ");
  Serial.print(SSID);
  
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("\nConnected.");

  Serial.print("\nIP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

String get_request_data() {
  String client_IP = client.remoteIP().toString();
  String header="", current_line="", content="";
  int content_length=-1;
  bool header_transmitted = false;
  
  Serial.print("New Client : ");
  Serial.println(client_IP);

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (!header_transmitted)
        header += c;
      else {
        content += c;
        if (!--content_length) {
          client.stop();
          Serial.print(client_IP);
          Serial.println(" disconnected");
        }
      }
      
      if (c == '\n') {
          if (!current_line.length()) {
            header_transmitted = true;
            continue;
          } else if (current_line.startsWith("Content-Length:"))
            content_length = current_line.substring(16).toInt();
          current_line = "";
      } else if (c != '\r')
        current_line += c;
    }
  }

  return content;
}
