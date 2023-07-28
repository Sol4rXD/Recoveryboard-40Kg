#include <Arduino.h>

HardwareSerial &ss = Serial1;

// SoftwareSerial ss(3, 2);

void setup() {
  Serial.begin(115200);
  ss.begin(115200);
  Serial.println("Ready to recieve");
}

void loop() {
  while (ss.available()) {
    Serial.write(ss.read());
  }
}