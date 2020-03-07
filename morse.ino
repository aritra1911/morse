#include <WiFi.h>
#include "morse_utils.h"
#include "network_handler.h"

#define LED_BUILTIN 2  // for ESP32
#define BUZZER 25
#define WPM 40
#define WAIT_TIME 5000

String morse_code;
double unit;
int i;  // the control variable
bool changing_state;

WiFiServer server(80);
WiFiClient client;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(115200);
  connect_to_network();

  morse_code = ".-. . .- -.. -.--";  // ready
  i = 0;
  changing_state = false;

  unit = 60 / (50.0 * WPM);
}

void loop() {
  if (changing_state) {
    changing_state = false;
    reset();
  }
  
  write_morse(morse_code.charAt(i));
  if (++i == morse_code.length()) reset();
}

void write_state(double duration, bool state) {
  if (changing_state) return;
  digitalWrite(LED_BUILTIN, state);
  digitalWrite(BUZZER, state);
  my_delay(duration*1000);
}

void my_delay(int duration) {
  int start_time = millis();
  while ((millis() - start_time) < duration)
    if (client = server.available()) {
      String json = get_request_data();
      Serial.println(json);
      morse_code = json;
      changing_state = true;
      return;
    }

    if (WiFi.status() != WL_CONNECTED) {
      connect_to_network();
      changing_state = true;
      return;
    }
}

void reset() {
  i = 0;
  my_delay(WAIT_TIME);
}
