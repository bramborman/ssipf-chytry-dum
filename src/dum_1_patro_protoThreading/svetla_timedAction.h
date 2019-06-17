#ifndef SVETLA_TIMED_ACTION_H
#define SVETLA_TIMED_ACTION_H

#include <VariableTimedAction.h>
#include "utils/AnalogoveCidlo.h"

// Definice pinů
#define IN_A_FOTOREZISTOR A0
#define OUT_RELAY_ZAROVKY 8
#define IN_A_TERMISTOR A1
#define OUT_RELAY_CHLAZENI 9

#define SVETLA_VYCHOZI_DOBA_CEKANI_MS 1000
#define SVETLA_FOTOREZISTOR_KALIBRACE_PRODLEVA_MS 60000
#define SVETLA_FOTOREZISTOR_ROZDIL_HODNOT 10
#define SVETLA_TERMISTOR_ROZDIL_NAHRATI 6
#define SVETLA_TERMISTOR_ROZDIL_CHLAZENI 4

class SvetlaTimedAction : public VariableTimedAction {
private:
  int _krok = 0;
  int _dalsiKalibraceFotorezistoru = millis();
  int _kalibraceFotorezistoru;
  int _kalibraceTermistoru;

  unsigned long run() {
    if (_krok == 0) {
      kalibrovatFotorezistor();

      const int hodnotaFotorezistoru = analogRead(IN_A_FOTOREZISTOR);

      if (hodnotaFotorezistoru >= _kalibraceFotorezistoru + SVETLA_FOTOREZISTOR_ROZDIL_HODNOT) {
        _kalibraceTermistoru = analogRead(IN_A_TERMISTOR);
#ifdef SERIAL_DEBUG
        Serial.print("Termistor zkalibrován. Aktuální hodnota: ");
        Serial.println(_kalibraceTermistoru);
        Serial.println("Zapínám žárovky");
        Serial.println("Čekám na nahřátí žárovek");
#endif
        digitalWrite(OUT_RELAY_ZAROVKY, HIGH);
        _krok++;
      }
    }
    else if (_krok == 1) {
      // Počkáme, než se žárovky dostatečně nahřejí
      // Se zvyšující se teplotou, se hodnota vrácená
      // z termistoru SNIŽUJE
      const int hodnotaTermistoru = analogRead(IN_A_TERMISTOR);

      if (hodnotaTermistoru <= _kalibraceTermistoru - SVETLA_TERMISTOR_ROZDIL_NAHRATI) {
#ifdef SERIAL_DEBUG
        Serial.println("Vypínám žárovky");
        Serial.println("Zapínám chlazení");
        Serial.println("Čekám na zchlazení žárovek");
#endif
        digitalWrite(OUT_RELAY_ZAROVKY, LOW);
        digitalWrite(OUT_RELAY_CHLAZENI, HIGH);
        _krok++;
      }
    }
    else {
      // Počkáme, než se žárovky zchladí na původní
      // teplotu. Se snižující se teplotou, se hodnota
      // vrácená z termistoru ZVYŠUJE
      const int hodnotaTermistoru = analogRead(IN_A_TERMISTOR);

      if (hodnotaTermistoru >= _kalibraceTermistoru) {
#ifdef SERIAL_DEBUG
        Serial.println("Vypínám chlazení");
#endif
        digitalWrite(OUT_RELAY_CHLAZENI, LOW);
        _krok = 0;
      }
    }

    return 0;
  }

  void kalibrovatFotorezistor() {
    if (millis() >= _dalsiKalibraceFotorezistoru) {
      _kalibraceFotorezistoru = analogRead(IN_A_FOTOREZISTOR);
      _dalsiKalibraceFotorezistoru = millis() + SVETLA_FOTOREZISTOR_KALIBRACE_PRODLEVA_MS;

#ifdef SERIAL_DEBUG
      Serial.print("Fotorezistor zkalibrován. Aktuální hodnota: ");
      Serial.println(_kalibraceFotorezistoru);
#endif
    }
  }

public:
  SvetlaTimedAction() {
    pinMode(IN_A_FOTOREZISTOR, INPUT);
    pinMode(OUT_RELAY_ZAROVKY, OUTPUT);
    pinMode(IN_A_TERMISTOR, INPUT);
    pinMode(OUT_RELAY_CHLAZENI, OUTPUT);

    kalibrovatFotorezistor();
  }
};

#endif
