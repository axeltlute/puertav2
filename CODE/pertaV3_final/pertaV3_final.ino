#include <Adafruit_PN532.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define SDA_PIN 22
#define SCL_PIN 21
#define LED_PIN 13
#define motorA1 18
#define motorA2 5
#define motorb1 4
#define motorb2 2

LiquidCrystal_I2C lcd(0x27, 20, 4);

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.println("INGRESE TARJETA  ");
  nfc.begin();
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorb1, OUTPUT);
  pinMode(motorb2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    lcd.setCursor(0, 0);
    lcd.print("no se detecto nada");
    while (1)
      ;
  }

  nfc.SAMConfig();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (success) {
    if (checkCardUID(uid)) {
      digitalWrite(LED_PIN, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("TARJETA CORRECTA");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("ABRIENDO");
      digitalWrite(LED_PIN, LOW);
      abrir();
      delay(10000);
      STOP();
      delay(50000);
      lcd.setCursor(0, 1);
      lcd.print("CERRANDO");
      cerrar();
      delay(10000);
      STOP();
    } else {
      lcd.setCursor(0, 0);
      lcd.print("TARJETA ERRONEA  ");
      cerrar();
      lcd.setCursor(0, 1);
      lcd.print("CERRANDO");
      delay(10000);
      STOP();
    }
  }
  nfc.SAMConfig();
  delay(1000);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void abrir() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorb1, LOW);
  digitalWrite(motorb2, LOW);
}
void cerrar() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorb1, LOW);
  digitalWrite(motorb2, LOW);
}
void STOP() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorb1, LOW);
  digitalWrite(motorb2, LOW);
}