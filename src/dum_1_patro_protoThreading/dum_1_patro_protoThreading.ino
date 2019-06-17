#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "salina_timedAction.h"
#include "svetla_timedAction.h"
#include "vetrelec_timedAction.h"

// Odkomentujte následující řádek
// pro zapnutí vypisování testovacích
// zpráv do sériového monitoru
// #define SERIAL_DEBUG

// Nastavení adresy I2C (0x27 v mém případě),
// a dále počtu znaků a řádků LCD, zde 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);
SalinaTimedAction salina(&lcd, 1);
SvetlaTimedAction svetla;
VetrelecTimedAction vetrelec(&lcd, 0);

void setup() {
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif

  lcd.begin();
  lcd.backlight();

  salina.start(SALINA_VYCHOZI_DOBA_CEKANI_MS, false);
  svetla.start(SVETLA_VYCHOZI_DOBA_CEKANI_MS, false);
  vetrelec.start(VETRELEC_VYCHOZI_DOBA_CEKANI_MS, false);
}

void loop() {
  VariableTimedAction::updateActions();
  delay(10);
}
