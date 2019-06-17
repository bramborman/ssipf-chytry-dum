#ifndef VETRELEC_TIMED_ACTION_H
#define VETRELEC_TIMED_ACTION_H

#include <LiquidCrystal_I2C.h>
#include <VariableTimedAction.h>
#include "utils/LCD_I2C_StridaniTextu.h"
#include "utils/StridaniAkci.h"

// Definice pinů
#define IN_PIR 3
#define OUT_LED_MAJAK_1 13
#define OUT_LED_MAJAK_2 12

#define VETRELEC_VYCHOZI_DOBA_CEKANI_MS 1000
#define VETRELEC_DOBA_ALARMU_MS 4000
#define VETRELEC_DOBA_ZOBRAZENI_JEDNE_HLASKY_MS 500
#define VETRELEC_DOBA_JEDNE_FAZE_MAJAKU_MS VETRELEC_DOBA_ZOBRAZENI_JEDNE_HLASKY_MS / 2

class VetrelecTimedAction : public VariableTimedAction {
private:
  LiquidCrystal_I2C *const _lcd;
  const uint8_t _radekNaLcd;
  LCD_I2C_StridaniTextu _stridaniHlasek;
  StridaniAkci<VetrelecTimedAction> _blikaniMajaku;

  bool _zaznamenanVetrelec = false;
  bool _vystridatHlasky = false;
  unsigned long _konecAlarmu = 0L;

  unsigned long run() {
    if (!_zaznamenanVetrelec) {
      const int pirHodnota = digitalRead(IN_PIR);

      if (pirHodnota == HIGH) {
  #ifdef SERIAL_DEBUG
          Serial.println("Zaznamenán vetřelec");
  #endif
          _stridaniHlasek.vystridat();
          _blikaniMajaku.vystridat();

          _zaznamenanVetrelec = true;
          _konecAlarmu = millis() + VETRELEC_DOBA_ALARMU_MS;
          return VETRELEC_DOBA_JEDNE_FAZE_MAJAKU_MS;
      }
    }
    else {
      if (millis() >= _konecAlarmu) {
#ifdef SERIAL_DEBUG
        Serial.println("Vypínám alarm");
#endif
        displej_vseOk();
        _stridaniHlasek.resetovat();

        // Vypnout maják
        digitalWrite(OUT_LED_MAJAK_1, LOW);
        digitalWrite(OUT_LED_MAJAK_2, LOW);
        _blikaniMajaku.resetovat();

        _zaznamenanVetrelec = false;
        _vystridatHlasky = false;
        return VETRELEC_VYCHOZI_DOBA_CEKANI_MS;
      }

      // Měníme vždy každou druhou fázi blikání
      // majáku, protože maják se střídá 2x častěji
      if (_vystridatHlasky) {
        _stridaniHlasek.vystridat();
      }

      _vystridatHlasky = !_vystridatHlasky;

      _blikaniMajaku.vystridat();
      return 0;
    }
  }

  void displej_vseOk() {
    _lcd->setCursor(0, _radekNaLcd);
    // Mezery na začátku a na konci mažou
    // případné znaky na daném místě na displeji
    _lcd->print(" VSE V PORADKU  ");
  }

  void majak_faze1() {
    digitalWrite(OUT_LED_MAJAK_1, HIGH);
    digitalWrite(OUT_LED_MAJAK_2, LOW);
  }

  void majak_faze2() {
    digitalWrite(OUT_LED_MAJAK_1, LOW);
    digitalWrite(OUT_LED_MAJAK_2, HIGH);
  }

public:
  VetrelecTimedAction(LiquidCrystal_I2C *const lcd, const uint8_t radekNaLcd)
    : _lcd(lcd),
      _radekNaLcd(radekNaLcd),
      _stridaniHlasek(LCD_I2C_StridaniTextu("POZOR, VETRELEC!", 0, "Volam policii!!!", 0, lcd, radekNaLcd, false)),
      _blikaniMajaku(StridaniAkci<VetrelecTimedAction>(this, &VetrelecTimedAction::majak_faze1, &VetrelecTimedAction::majak_faze2, false)) {
    pinMode(IN_PIR, INPUT);
    pinMode(OUT_LED_MAJAK_1, OUTPUT);
    pinMode(OUT_LED_MAJAK_2, OUTPUT);

    displej_vseOk();
  }
};

#endif
