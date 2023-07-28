#include <Arduino.h>

// SD card
#define PIN_SPI_MOSI1 PA7
#define PIN_SPI_MISO1 PA6
#define PIN_SPI_SCK1 PA5
#define PIN_SPI_CS_SD_INT PB1
#define SPI_SPEED_SD_MHZ (20)

// Lora 
HardwareSerial Serial1(PA_10, PA_9);
HardwareSerial &SerialLoRa = Serial1;

#define PIN_LORA_M0 PB15
#define PIN_LORA_M1 PB14

// Sim7000
String phone_number = "+66958976187";
String sms_test = "Sim does work";
#define PWRKEY 6
#define SIMCOM_7000
HardwareSerial &modemSS = Serial2;
HardwareSerial *modemSerial = &modemSS;
uint8_t type;
char imei[16] = {0}; 

// GPS 
double latitude;
double longitude;
double altitude;

SFE_UBLOX_GNSS myGNSS;

uint32_t gps_time;
uint8_t SIV;

String GPSpacket;
String GPSpacketSMS;

// Other declare
uint32_t lastTime_1 = 0;
uint32_t lastTime_2 = 0;
uint32_t packet_counter = 1;
uint32_t operation_time = (2ul * 60ul * 60ul + 50ul * 60ul) * 1000ul; // 2 hr 50 minutes

int previous_altitude = 0;
int conditionCounter = 0;


















