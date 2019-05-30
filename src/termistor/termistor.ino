int termPin = A0;      // Analogový pin, ke kterému je termistor připojen
int termNom = 10000;  // Referenční odpor termistoru
int refTep = 25;      // Teplota pro referenční odpor
int beta = 3977;      // Beta faktor
int rezistor = 10000; // hodnota odporu v sérii

void setup(void) {
  Serial.begin(9600);
  //použití externího pinu AREF jako referenčního napětí pro A/D převodník
  analogReference(EXTERNAL);
}

void loop(void) {
  float napeti;
  //změření napětí na termistoru
  napeti = analogRead(termPin);

  // Konverze změřené hodnoty na odpor termistoru
  napeti = 1023 / napeti - 1;
  napeti = rezistor / napeti;

  //Výpočet teploty podle vztahu pro beta faktor
  float teplota;
  teplota = napeti / termNom;         // (R/Ro)
  teplota = log(teplota);             // ln(R/Ro)
  teplota /= beta;                    // 1/B * ln(R/Ro)
  teplota += 1.0 / (refTep + 273.15); // + (1/To)
  teplota = 1.0 / teplota;            // Převrácená hodnota
  teplota -= 273.15;                  // Převod z Kelvinů na stupně Celsia

  Serial.print("Teplota je: ");
  Serial.print(teplota);
  Serial.println(" *C");

  delay(1000);
}
