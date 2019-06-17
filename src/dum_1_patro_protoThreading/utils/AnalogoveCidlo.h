#ifndef ANALOGOVE_CIDLO_H
#define ANALOGOVE_CIDLO_H

// Použito ve funkci analogoveCidlo_cekatNaHodnotu
#define BEZ_CASOVEHO_OMEZENI -1

void analogoveCidlo_cekatNaHodnotu(const uint8_t pin, const int hodnota);
bool analogoveCidlo_cekatNaHodnotu(const uint8_t pin, const int pozadovanaHodnota, const int maxDobaCekani);
inline bool _analogoveCidlo_hodnotaMensiNezPozadovana(const int hodnota, const int pozadovanaHodnota);
inline bool _analogoveCidlo_hodnotaVetsiNezPozadovana(const int hodnota, const int pozadovanaHodnota);
inline bool _analogoveCidlo_porovnavatCas(const int aktualniMs);
inline bool _analogoveCidlo_neporovnavatCas(const int aktualniMs);

// Čeká, dokud nepřijde z čidla požadovaná hodnota
void analogoveCidlo_cekatNaHodnotu(const uint8_t pin, const int hodnota) {
  analogoveCidlo_cekatNaHodnotu(pin, hodnota, BEZ_CASOVEHO_OMEZENI);
}

// Čeká, dokud nepřijde z čidla požadovaná hodnota, nebo dokud nevyprší čas
// Vrací:
// - true: hodnota z čidla dosáhla požadované hodnoty
// - false: čas vypršel dříve, než hodnota z čidla dosáhla požadované hodnoty
bool analogoveCidlo_cekatNaHodnotu(const uint8_t pin, const int pozadovanaHodnota, const int maxDobaCekani) {
  int hodnota = analogRead(pin);
  bool (*const porovnavaniHodnot)(const int, const int) = hodnota < pozadovanaHodnota ? _analogoveCidlo_hodnotaMensiNezPozadovana : _analogoveCidlo_hodnotaVetsiNezPozadovana;
  bool (*const porovnavaniCasu)(const int) = maxDobaCekani == BEZ_CASOVEHO_OMEZENI ? _analogoveCidlo_neporovnavatCas : _analogoveCidlo_porovnavatCas;
  int ms = maxDobaCekani;

#ifdef SERIAL_DEBUG
  Serial.print("  hodnota: ");
  Serial.println(hodnota);
#endif

  while (porovnavaniCasu(ms) && porovnavaniHodnot(hodnota, pozadovanaHodnota)) {
    delay(1000);
    ms -= 1000;

    hodnota = analogRead(pin);
#ifdef SERIAL_DEBUG
    Serial.print("  hodnota: ");
    Serial.println(hodnota);
#endif
  }

#ifdef SERIAL_DEBUG
  if (porovnavaniHodnot(hodnota, pozadovanaHodnota)) {
    Serial.print("Čas ");
    Serial.print(maxDobaCekani);
    Serial.println("ms vypršel a hodnota z čidla nedosáhla požadované hodnoty");

    return false;
  }
  else {
    Serial.println("Hodnota z čidla dosáhla požadované hodnoty");
    return true;
  }
#endif
}

inline bool _analogoveCidlo_hodnotaMensiNezPozadovana(const int hodnota, const int pozadovanaHodnota) {
  return hodnota < pozadovanaHodnota;
}

inline bool _analogoveCidlo_hodnotaVetsiNezPozadovana(const int hodnota, const int pozadovanaHodnota) {
  return hodnota > pozadovanaHodnota;
}

inline bool _analogoveCidlo_porovnavatCas(const int aktualniMs) {
  return aktualniMs > 0;
}

inline bool _analogoveCidlo_neporovnavatCas(const int aktualniMs) {
  return true;
}

#undef BEZ_CASOVEHO_OMEZENI

#endif