#include <Arduino.h>
#include "BotleticsSIM7000.h"

#define PWRKEY 6

char replybuffer[255];

#define SIMCOM_7000

String phone_number = "+66958976187";
String sms_message = "youngohm";

HardwareSerial &modemSS = Serial2;
HardwareSerial *modemSerial = &modemSS;

Botletics_modem_LTE modem = Botletics_modem_LTE();

uint8_t type;
char imei[16] = {0}; 

extern void send_sms();

void setup() {
  delay(10000);
  modem.powerOn(PWRKEY);  
  Serial.begin(9600);
  modemSS.begin(115200);  // Set baud rate
  modemSS.println("AT+IPR=115200"); // Set baud rate
  delay(100);  
  modem.begin(modemSS);
  type = modem.type();
  modem.setFunctionality(1);
  delay(5000);
}

void loop() {
  send_sms();
  delay(5000);
}

void send_sms() {
  modem.sendSMS(phone_number.c_str(), sms_message.c_str());
}



