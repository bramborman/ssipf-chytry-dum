#ifndef SALINA_TIMED_ACTION_H
#define SALINA_TIMED_ACTION_H

#include <LiquidCrystal_I2C.h>
#include <VariableTimedAction.h>
#include "utils/LCD_I2C_StridaniTextu.h"

// Definice pinů
#define IN_SALINA 10
#define OUT_LED_SALINA 11

#define SALINA_VYCHOZI_DOBA_CEKANI_MS 1000
#define SALINA_DOBA_ZOBRAZOVANI_HLASEK_MS 5000
#define SALINA_DOBA_ZOBRAZENI_JEDNE_HLASKY_MS 500

class SalinaTimedAction : public VariableTimedAction {
private:
  LiquidCrystal_I2C *const _lcd;
  const uint8_t _radekNaLcd;
  LCD_I2C_StridaniTextu _stridaniHlasek;

  bool _salinaPrijizdi = false;
  unsigned long _konecZobrazovaniHlasek = 0L;

  unsigned long run() {
    if (!_salinaPrijizdi) {
      const int salinaHodnota = digitalRead(IN_SALINA);
      
      if (salinaHodnota == HIGH) {
#ifdef SERIAL_DEBUG
        Serial.println("Přijíždí šalina");
#endif
        digitalWrite(OUT_LED_SALINA, HIGH);
        _stridaniHlasek.vystridat();

        _salinaPrijizdi = true;
        _konecZobrazovaniHlasek = millis() + SALINA_DOBA_ZOBRAZOVANI_HLASEK_MS;
        return SALINA_DOBA_ZOBRAZENI_JEDNE_HLASKY_MS;
      }

      return 0;
    }
    else {
      if (millis() >= _konecZobrazovaniHlasek) {
#ifdef SERIAL_DEBUG
        Serial.println("Šalina odjela");
#endif
        digitalWrite(OUT_LED_SALINA, LOW);
        // Smazat hlášku na displeji
        _lcd->setCursor(0, _radekNaLcd);
        _lcd->print("                ");
        _stridaniHlasek.resetovat();

        _salinaPrijizdi = false;
        return SALINA_VYCHOZI_DOBA_CEKANI_MS;
      }

      _stridaniHlasek.vystridat();
      return 0;
    }
  }

public:
  SalinaTimedAction(LiquidCrystal_I2C *const lcd, const uint8_t radekNaLcd)
    : _lcd(lcd),
      _radekNaLcd(radekNaLcd),
      _stridaniHlasek(LCD_I2C_StridaniTextu("Salina se blizi", 0, "Prchej na salinu", 0, lcd, radekNaLcd, false)) {
    pinMode(IN_SALINA, INPUT);
    pinMode(OUT_LED_SALINA, OUTPUT);
  }
};

#endif
