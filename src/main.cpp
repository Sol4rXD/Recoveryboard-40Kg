#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_u-blox_GNSS_v3.h"
#include "sd_card_functions.h"
#include "definitions.h"
#include "vt_tools"
#include "vt_lora"
#include "BotleticsSIM7000.h"

using namespace vt;

smart_delay sd1(1000, millis);
smart_delay sd2(2000, millis);

// SD Card
SPIClass SPI_SD(PIN_SPI_MOSI1, PIN_SPI_MISO1, PIN_SPI_SCK1);
SdFat32 sd0;
File32 sd0_file;
String sd0_filename;

lora_e32 lora(SerialLoRa, PIN_LORA_M0, PIN_LORA_M1);

int release_alitude = 500;  // Fix ur release altitude

char replybuffer[255];
Botletics_modem_LTE modem = Botletics_modem_LTE();

enum state {
  PREPARE,
  RELEASE,
  DEFLATE,
  PHONEGPS
};

const String allstate[] = { "PRE", "REL", "DEF", "PHGPS" };

enum state current_state = PREPARE;

extern void take_GPS();
extern void updateState();
extern void save_sd();
extern void send_sms();
extern void send_sms_test();

void setup() {
  delay(10000);
  Serial.begin();
  delay(1000);

  while (!sd0.begin(SdSpiConfig(PIN_SPI_CS_SD_INT, SHARED_SPI, SD_SCK_MHZ(SPI_SPEED_SD_MHZ), &SPI_SD))) {
      Serial.println("SD not found! Retrying...");
      delay(500);
  }
  make_new_filename(sd0, sd0_filename, "mcu1_data_sd0_", ".csv");
  open_for_append(sd0, sd0_file, sd0_filename);
  delay(1000);
  Wire.setSCL(PB6);
  Wire.setSDA(PB7);
  Wire.setClock(400*1000);
  Wire.begin();
  myGNSS.begin();
  myGNSS.setI2COutput(COM_TYPE_UBX, VAL_LAYER_RAM_BBR);
  myGNSS.setNavigationFrequency(2, VAL_LAYER_RAM_BBR);  
  myGNSS.setAutoPVT(true, VAL_LAYER_RAM_BBR);
  myGNSS.setDynamicModel(DYN_MODEL_AIRBORNE2g, VAL_LAYER_RAM_BBR);
  delay(100);
  lora.begin(115200);
  delay(100);
  modem.powerOn(PWRKEY);  
  modemSS.begin(115200);  // Set baud rate sim7000
  modemSS.println("AT+IPR=115200"); 
  delay(100);
  modem.begin(modemSS);
  type = modem.type();
  modem.setFunctionality(1); 
  delay(2000);
  send_sms_test();
}

void loop() {
  static uint32_t timer = millis();
  if (sd2) {
    updateState();
    previous_altitude = altitude;
  }

  if (sd1) {
    take_GPS();
    GPSpacket = String(latitude, 6) + "," +
                String(longitude, 6) + "," +
                String(altitude) + "," +
                String(SIV) + "," +
                String(gps_time) + "," +
                String(packet_counter) + "," +
                allstate[current_state];
    Serial.println(GPSpacket);
    SerialLoRa.println(GPSpacket);
    save_sd();
    packet_counter++;
  }

  if (current_state != PHONEGPS && (millis() - timer > operation_time)) {
    current_state = PHONEGPS;
  }
}

void take_GPS() {
  if (myGNSS.getPVT()) {
    latitude = static_cast<double>(myGNSS.getLatitude()) * 1e-7;
    longitude = static_cast<double>(myGNSS.getLongitude()) * 1e-7;
    altitude = static_cast<double>(myGNSS.getAltitude()) * 1e-3;
    gps_time = myGNSS.getUnixEpoch();
    SIV = myGNSS.getSIV();
  }
}

void updateState() {
  switch (current_state) {
    case PREPARE:
      if (altitude > release_alitude) {
        current_state = RELEASE;
      }
      break;
    case RELEASE:
      if (previous_altitude > altitude) {
        conditionCounter++;
        if (conditionCounter >= 5) {
          current_state = DEFLATE;
          conditionCounter = 0;
        }
      } else {
        conditionCounter = 0;
      }
      break;
    case DEFLATE:
      if (altitude <= 420) {
        conditionCounter++;
        if (conditionCounter >= 5) {
          current_state = PHONEGPS;
          conditionCounter = 0;
        }
      } else {
        conditionCounter = 0;
      }
      break;
    case PHONEGPS:
      GPSpacketSMS = String(latitude, 6) + "," +
                     String(longitude, 6) + "," +
                     String(altitude);
      send_sms();
      break;
  }
}

void save_sd() {
  sd0_file.println(GPSpacket);
  sd0_file.flush();
}

void send_sms() {
  modem.sendSMS(phone_number.c_str(), GPSpacketSMS.c_str());
}

void send_sms_test() {
  modem.sendSMS(phone_number.c_str(), sms_test.c_str());
}




