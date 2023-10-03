#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN A4  // Cambia esto al pin SDA que estés utilizando
#define SCL_PIN A5  // Cambia esto al pin SCL que estés utilizando
#define led 13
const uint8_t arrayB[4] = { 0x7, 0x94, 0x64, 0xB5 };

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  Serial.println("Hello!");

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();

  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1)
      ;
  }

  nfc.SAMConfig();
  Serial.println("Waiting for an NFC card...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (success) {
    Serial.println("Found an NFC card!");

    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x");
      Serial.print(uid[i], HEX);
    }
    Serial.println("");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (uint8_t index = 0; index < uidLength; index++) {
      if (uid[index] != arrayB[index]) {
        Serial.print("NEGADO");
        digitalWrite(led,LOW);
      }
    }
    for (uint8_t index = 0; index < uidLength; index++) {
      if (uid[index] != arrayB[index]) {
        Serial.print("ABRIENDO");
        digitalWrite(led,HIGH);
      }
       }
    delay(1000);
  }
}