#include "IntroSatLib.h"
#include "Device/CC1101/CC1101.h"
#include <SPI.h>

#define CC1101_CS_PIN 2

using namespace IntroSatLib;

CC1101 radio(SPI, CC1101_CS_PIN);

void setup() {
  pinMode(CC1101_CS_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println(F("Starting..."));
  SPI.begin();
  

  if (radio.Init() == CC1101::STATUS_CHIP_NOT_FOUND) {
    Serial.println(F("Chip not found!"));
    while (true) { delay(1000); }
  }

  radio.setModulation(CC1101::MOD_2FSK);
  radio.setFrequency(433.8);
  radio.setDataRate(1.5);
  radio.setOutputPower(0);

  radio.setPacketLengthMode(CC1101::PKT_LEN_MODE_VARIABLE);
  radio.setAddressFilteringMode(CC1101::ADDR_FILTER_MODE_NONE);
  radio.setPreambleLength(64);
  radio.setSyncWord(0x1234);
  radio.setSyncMode(CC1101::SYNC_MODE_16_16);
  radio.setCrc(true);
  radio.setDataWhitening(true);
  radio.setManchester(false);
  radio.setFEC(false);
}

int counter = 0;

void loop() {
  String data = "Hello #" + String(counter++);

  Serial.print(F("Transmitting: "));
  Serial.print(data);
  Serial.print(F(" "));
  CC1101::Status status = radio.transmit((uint8_t *)data.c_str(), data.length());

  if (status == CC1101::STATUS_OK) {
    Serial.println(F("[OK]"));
  } else {
    Serial.print("[ERROR ");
    Serial.print(status);
    Serial.println("]");
  }

  delay(1000);
}
