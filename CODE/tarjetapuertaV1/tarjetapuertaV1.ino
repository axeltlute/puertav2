#include <Wire.h>
#include <Adafruit_PN532.h>
#include <liquidcrystal_i2c.h>

#define SDA_PIN A4
#define SCL_PIN A5
#define LED_PIN 13

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");
  pinMode(LED_PIN, OUTPUT);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1)
      ;
  }

  nfc.SAMConfig();
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

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
    if (checkCardUID(uid)) {
      digitalWrite(LED_PIN, HIGH);
      Serial.print("pendido");
      delay(1000);
      digitalWrite(LED_PIN, LOW);
    }
  }
  nfc.SAMConfig();
  delay(1000);
}
bool checkCardUID(uint8_t *uid) {
  bool state = false;
  byte contador = 0;
    uint8_t allowedUID[][4] = { { 0x7, 0x94, 0x64, 0xB5 }, { 0x5A, 0x5A, 0x53, 0x1A }, { 0x8A, 0xEF, 0xEB, 0x81 }, { 0x4, 0xB5, 0x88, 0xDa } };
  for (int x = 0; x < 5; x++) {
    for (int i = 0; i < 5; i++) {
      if (*(uid + i) == allowedUID[x][i]) {
        contador++;
      }
    }
    if (contador == 4) {
      state = 1;
      contador = 0;
    } else {
      contador = 0;
    }
  }
  return state;
}