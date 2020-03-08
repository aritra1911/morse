#include <WiFi.h>
#include <ArduinoJson.h>
#include "morse_utils.h"
#include "network_handler.h"

#define LED_BUILTIN 2  // for ESP32
#define BUZZER 25
#define DEFAULT_WPM 15
#define WAIT_TIME 5000

String morse_code;
int previous_character;
int wpm, fwpm;
double unit, f_unit;
int i;  // the control variable
bool changing_state;

WiFiServer server(80);
WiFiClient client;
StaticJsonDocument<20000> doc;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(115200);
  connect_to_network();

  morse_code = ".-. . .- -.. -.--";  // ready

  unit = 60 / (50.0 * DEFAULT_WPM);
  f_unit = ((60.0 / DEFAULT_WPM) - (31.0 * unit)) / 19.0;

  i = 0;
  changing_state = false;
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
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, json);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      wpm = doc["wpm"];
      fwpm = doc["fwpm"];
      morse_code = doc["code"].as<String>();
      Serial.print("WPM: ");
      Serial.println(wpm);
      Serial.print("FWPM: ");
      Serial.println(fwpm);
      Serial.print("Code: ");
      Serial.println(morse_code);
      changing_state = true;
      recalculate_units();
      return;
    }

    if (WiFi.status() != WL_CONNECTED) {
      connect_to_network();
      return;
    }
}

void recalculate_units() {
  unit = 6.0 / (5.0 * wpm);
  f_unit = ((60.0 / fwpm) - (31.0 * unit)) / 19.0;
}

void reset() {
  i = 0;
  my_delay(WAIT_TIME);
}
