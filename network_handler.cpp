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
  String content="";
  int b=0;

  Serial.print("New Client : ");
  Serial.println(client_IP);

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.print((char)c);
      // capturing one json object
      if (c == '{') b++;
      if (b) content += c;
      if (c == '}' && --b == 0) {
        client.stop();
        Serial.print(client_IP);
        Serial.println(" disconnected");
      }
    }
  }

  return content;
}
