#include "IntroSatLib.h"
#include "Device/CC1101/CC1101.h"
#include <SPI.h>

#define CC1101_CS_PIN 4

using namespace IntroSatLib;

CC1101 radio(SPI, CC1101_CS_PIN);

void setup() {
  pinMode(CC1101_CS_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println(F("Starting..."));
  SPI.begin();
  

  if (radio.Init() != 0) {
    Serial.println(F("Chip not found!"));
    while (true) { delay(1000); }
  }
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
