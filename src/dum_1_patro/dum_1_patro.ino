#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Odkomentujte následující řádek
// pro zapnutí vypisování testovacích
// zpráv do sériového monitoru
#define SERIAL_DEBUG

// Definice pinů
#define IN_SALINA 10
#define OUT_LED_SALINA 11
#define IN_PIR 3
#define OUT_LED_MAJAK_1 13
#define OUT_LED_MAJAK_2 12
#define IN_A_FOTOREZISTOR A0
#define OUT_RELAY_ZAROVKY 8
#define IN_A_TERMISTOR A1
#define OUT_RELAY_CHLAZENI 9

#define FOTOREZISTOR_ROZDIL_HODNOT 10
#define TERMISTOR_ROZDIL_NAHRATI 6
#define TERMISTOR_ROZDIL_CHLAZENI 4
// Použito ve funkci analogoveCidlo_cekatNaHodnotu
#define BEZ_CASOVEHO_OMEZENI -1

// Nastavení adresy I2C (0x27 v mém případě),
// a dále počtu znaků a řádků LCD, zde 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif

  pinMode(IN_SALINA, INPUT);
  pinMode(OUT_LED_SALINA, OUTPUT);
  pinMode(IN_PIR, INPUT);
  pinMode(OUT_LED_MAJAK_1, OUTPUT);
  pinMode(OUT_LED_MAJAK_2, OUTPUT);
  pinMode(IN_A_FOTOREZISTOR, INPUT);
  pinMode(OUT_RELAY_ZAROVKY, OUTPUT);
  pinMode(IN_A_TERMISTOR, INPUT);
  pinMode(OUT_RELAY_CHLAZENI, OUTPUT);

  lcd.begin();
  lcd.backlight();
  displej_vseOk();
}

void loop() {
  const int salinaBlizko = digitalRead(IN_SALINA);
  const int pirHodnota = digitalRead(IN_PIR);

  if (salinaBlizko == HIGH) {
#ifdef SERIAL_DEBUG
    Serial.println("Přijíždí šalina");
#endif
    digitalWrite(OUT_LED_SALINA, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("Salina se blizi");
    lcd.setCursor(0, 1);
    lcd.print("Prchej na salinu");

    delay(2000);
#ifdef SERIAL_DEBUG
    Serial.println("Šalina odjela");
#endif
    digitalWrite(OUT_LED_SALINA, LOW);
    displej_vseOk();
  }
  else if (pirHodnota == HIGH) {
#ifdef SERIAL_DEBUG
    Serial.println("Zaznamenán vetřelec");
#endif
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("POZOR, VETRELEC!");
    lcd.setCursor(0, 1);
    lcd.print("Volam policii!!!");

    majak_blikat(4000);
    displej_vseOk();

#ifdef SERIAL_DEBUG
    Serial.println("Čekám na zakrytí fotorezistoru (max 10s)");
    Serial.print("Původní hodnota z fotorezistoru: ");
    Serial.println(analogRead(IN_A_FOTOREZISTOR));
#endif
    
    if (analogoveCidlo_cekatNaHodnotu(IN_A_FOTOREZISTOR, analogRead(IN_A_FOTOREZISTOR) + FOTOREZISTOR_ROZDIL_HODNOT, 10000)) {
#ifdef SERIAL_DEBUG
      Serial.println("Zapínám žárovky");
#endif
      digitalWrite(OUT_RELAY_ZAROVKY, HIGH);

      // Počkáme, než se žárovky dostatečně nahřejí
      // Se zvyšující se teplotou, se hodnota vrácená
      // z termistoru SNIŽUJE
#ifdef SERIAL_DEBUG
      Serial.println("Čekám na nahřátí žárovek");
      Serial.print("Původní hodnota z termistoru: ");
      Serial.println(analogRead(IN_A_TERMISTOR));
#endif
      analogoveCidlo_cekatNaHodnotu(IN_A_TERMISTOR, analogRead(IN_A_TERMISTOR) - TERMISTOR_ROZDIL_NAHRATI);

      digitalWrite(OUT_RELAY_ZAROVKY, LOW);
#ifdef SERIAL_DEBUG
      Serial.println("Vypínám žárovky");
      Serial.println("Zapínám chlazení");
#endif
      digitalWrite(OUT_RELAY_CHLAZENI, HIGH);

      analogoveCidlo_cekatNaHodnotu(IN_A_TERMISTOR, analogRead(IN_A_TERMISTOR) + TERMISTOR_ROZDIL_CHLAZENI);

      digitalWrite(OUT_RELAY_CHLAZENI, LOW);
#ifdef SERIAL_DEBUG
      Serial.println("Vypínám chlazení");
#endif
    }
  }

  delay(1000);
}

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
  bool (*porovnavaniHodnot)(const int, const int) = hodnota < pozadovanaHodnota ? _analogoveCidlo_hodnotaMensiNezPozadovana : _analogoveCidlo_hodnotaVetsiNezPozadovana;
  bool (*porovnavaniCasu)(const int) = maxDobaCekani == BEZ_CASOVEHO_OMEZENI ? _analogoveCidlo_neporovnavatCas : _analogoveCidlo_porovnavatCas;
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

void displej_vseOk() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("VSE V PORADKU");
  lcd.setCursor(0, 1);
  lcd.print("----------------");
}

// Parametr ms určuje, jak dlouho má
// maják ZHRUBA blikat v milisekundách
void majak_blikat(int ms) {
  if (ms <= 0) {
    return;
  }

  while (true) {
    digitalWrite(OUT_LED_MAJAK_1, HIGH);
    digitalWrite(OUT_LED_MAJAK_2, LOW);
    delay(300);
    ms -= 300;

    if (ms <= 0) {
      break;
    }

    digitalWrite(OUT_LED_MAJAK_1, LOW);
    digitalWrite(OUT_LED_MAJAK_2, HIGH);
    delay(300);
    ms -= 300;

    if (ms <= 0) {
      break;
    }
  }

  digitalWrite(OUT_LED_MAJAK_1, LOW);
  digitalWrite(OUT_LED_MAJAK_2, LOW);
}
