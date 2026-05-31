#include "IntroSatLib.h"
#include "Device/CC1101/CC1101.h"
#include <SPI.h>

#define CC1101_CS_PIN 2

using namespace IntroSatLib;

CC1101 radio(SPI, CC1101_CS_PIN);
uint8_t buffer[64];
uint8_t receivedLength = 0;

void setup() {
    pinMode(CC1101_CS_PIN, OUTPUT);

    Serial.begin(115200);
    Serial.println(F("Инициализация..."));
    SPI.begin();
    

    if (radio.Init() != 0) {
        Serial.println(F("Ошибка инициализации!"));
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

void loop() {
    CC1101::Status status = radio.receive(buffer, 1, &receivedLength);
        
    if (status == CC1101::STATUS_OK) {
        Serial.print(F("Received "));
        Serial.print(receivedLength);
        Serial.print(F(" bytes: "));
        Serial.write(buffer, receivedLength);
        Serial.print(" [RSSI: ");
        Serial.print(radio.getRSSI());
        Serial.println(" dBm]");
    }

    delay(1000);
}
