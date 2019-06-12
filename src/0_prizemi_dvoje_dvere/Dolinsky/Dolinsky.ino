#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>

// Odkomentujte následující řádek
// pro zapnutí vypisování zpráv
// do sériového monitoru
// #define SERIAL_DEBUG

// Definice pinů
#define IN_PRIZEMI_VSTUP_CIDLO 5
#define OUT_PRIZEMI_VSTUP_LED_ZELENY_SIGNAL 12
#define OUT_PRIZEMI_VSTUP_SERVO 9
#define OUT_PRIZEMI_LED_SVETLA 10
#define OUT_JUST_ANOTHER_ZEM 11
#define OUT_P_TRIG 4
#define OUT_ZAHRADA_MOTOR_1 8
#define OUT_ZAHRADA_MOTOR_2 11
#define OUT_ZAHRADA_MOTOR_3 13
#define OUT_ZAHRADA_MOTOR_4 7
#define IN_ZAHRADA_DORAZ 3

// Konstanta pro nastavení rychlosti,
// se zvětšujícím se číslem se rychlost zmenšuje
#define ZAHRADA_KROK_PAUZA 1
#define ZAHRADA_KROKY_POCET 8
#define ZAHRADA_KROKY_ZAVRENI_POCET 13000

Servo vstup_servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Inicializace pinů
  pinMode(IN_PRIZEMI_VSTUP_CIDLO, INPUT);
  pinMode(OUT_PRIZEMI_VSTUP_LED_ZELENY_SIGNAL, OUTPUT);
  pinMode(OUT_PRIZEMI_LED_SVETLA, OUTPUT);
  pinMode(OUT_JUST_ANOTHER_ZEM, OUTPUT);
  pinMode(OUT_P_TRIG, OUTPUT);
  pinMode(OUT_ZAHRADA_MOTOR_1, OUTPUT);
  pinMode(OUT_ZAHRADA_MOTOR_2, OUTPUT);
  pinMode(OUT_ZAHRADA_MOTOR_3, OUTPUT);
  pinMode(OUT_ZAHRADA_MOTOR_4, OUTPUT);
  pinMode(IN_ZAHRADA_DORAZ, INPUT);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Zapinam se");

  zahrada_otevrit();
  zahrada_zavrit();
  displej_uvitaciHlaska();

  digitalWrite(OUT_PRIZEMI_VSTUP_LED_ZELENY_SIGNAL, LOW);
  digitalWrite(OUT_PRIZEMI_LED_SVETLA, LOW);
  digitalWrite(OUT_JUST_ANOTHER_ZEM, LOW);

#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  digitalWrite(OUT_P_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(OUT_P_TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(OUT_P_TRIG, LOW);

  long odezva = pulseIn(IN_PRIZEMI_VSTUP_CIDLO, HIGH);
  long vzdalenost = odezva / 58.31;

#ifdef SERIAL_DEBUG
  Serial.print("Vzdalenost je ");
  Serial.print(vzdalenost);
  Serial.println(" cm.");
#endif

  if (vzdalenost < 25) {
    digitalWrite(OUT_PRIZEMI_VSTUP_LED_ZELENY_SIGNAL, HIGH);

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Oteviram dvere");
    vstup_otevrit();

    lcd.setCursor(1, 1);
    lcd.print("Zapinam svetla");
    digitalWrite(OUT_PRIZEMI_LED_SVETLA, HIGH);

    zahrada_otevrit();
    
    delay(5500);

    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Zhasinam");
    digitalWrite(OUT_PRIZEMI_LED_SVETLA, LOW);
    
    lcd.setCursor(1, 1);
    lcd.print("Zaviram dvere");
    digitalWrite(OUT_PRIZEMI_VSTUP_LED_ZELENY_SIGNAL, LOW);
    vstup_zavrit();

    zahrada_zavrit();
    
    displej_uvitaciHlaska();
  }
}

void displej_uvitaciHlaska() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("SSIPF Brno");
  lcd.setCursor(2, 1);
  lcd.print("Vitame te!!!");
}

void vstup_otevrit() {
  vstup_servo.attach(OUT_PRIZEMI_VSTUP_SERVO);

  for (int pos = 85; pos >= 0; pos--) {
    vstup_servo.write(pos);
    delay(10);
  }

  vstup_servo.detach();
}

void vstup_zavrit() {
  vstup_servo.attach(OUT_PRIZEMI_VSTUP_SERVO);

  for (int pos = 0; pos < 140; pos++) {
    vstup_servo.write(pos);
    delay(5);
  }

  vstup_servo.detach();
}

void zahrada_otevrit() {
  int doraz = digitalRead(IN_ZAHRADA_DORAZ);
  int krok = 0;

  while (doraz == HIGH) {
    zahrada_krok(krok);
    krok++;

    if (krok == ZAHRADA_KROKY_POCET) {
      krok = 0;
    }

    doraz = digitalRead(IN_ZAHRADA_DORAZ);
  }
}

void zahrada_zavrit() {
  int krok = ZAHRADA_KROKY_POCET - 1;
  int krokyCelkem = 0;

  while (krokyCelkem != ZAHRADA_KROKY_ZAVRENI_POCET) {
    zahrada_krok(krok);
    krok--;

    if (krok == -1) {
      krok = ZAHRADA_KROKY_POCET - 1;
    }

    krokyCelkem++;
  }
}

void zahrada_krok(int krok) {
  // Každý krok obsahuje výrobcem dané pořadí
  // pro správné spínání motoru(zahradního) a následnou
  // pauzu, kterou určujeme rychlost otáčení
  // Klíčové slovo static zajistí, že bude proměnná
  // uložena v paměti i po opuštení funkce
  // a nebude se vytvářet při každém volání funkce
  static const int ZAHRADA_KROKY[ZAHRADA_KROKY_POCET][4] = {
      {HIGH,  LOW,  LOW,  LOW},
      {HIGH, HIGH,  LOW,  LOW},
      { LOW, HIGH,  LOW,  LOW},
      { LOW, HIGH, HIGH,  LOW},
      { LOW,  LOW, HIGH,  LOW},
      { LOW,  LOW, HIGH, HIGH},
      { LOW,  LOW,  LOW, HIGH},
      {HIGH,  LOW,  LOW, HIGH}
  };

  const int *HODNOTY = ZAHRADA_KROKY[krok];
  digitalWrite(OUT_ZAHRADA_MOTOR_1, HODNOTY[0]);
  digitalWrite(OUT_ZAHRADA_MOTOR_2, HODNOTY[1]);
  digitalWrite(OUT_ZAHRADA_MOTOR_3, HODNOTY[2]);
  digitalWrite(OUT_ZAHRADA_MOTOR_4, HODNOTY[3]);

  delay(ZAHRADA_KROK_PAUZA);
}
