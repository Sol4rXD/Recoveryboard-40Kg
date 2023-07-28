#include <Arduino.h>
#include "vt_lora"

using namespace vt;

HardwareSerial Serial1(PA_10, PA_9);
HardwareSerial &SerialLoRa = Serial1;

#define PIN_M0 PB3
#define PIN_M1 PB4

int32_t packet_counter = 1;

lora_e32 lora(SerialLoRa, PIN_M0, PIN_M1);

void setup(){
    lora.begin(115200);
}

void loop(){
    SerialLoRa.println(packet_counter);
    delay(1000);
    packet_counter++;
}