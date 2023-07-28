#include <Arduino.h>
#include "vt_lora"

using namespace vt;

lora_e32 lora(Serial1, 5, 6);

void setup() {
    // Begin configuration (sleep) mode.
    lora.config();
    lora.set_param(0,               // Address
                   9600,          // Operational baud rate
                   vt::LoRaParity::PARITY_8N1,           // Serial parity
                   9600,         // Air data rate
                   53,             // Frequency channel
                   vt::LoRaTxPower::TX_MAX,               // Tx Power
                   false,        // Enable Forward Error Correction?
                   true);     // Save configuration for next boot?

    // Begin operational mode.
    lora.begin(115200);
}

void loop() {
}