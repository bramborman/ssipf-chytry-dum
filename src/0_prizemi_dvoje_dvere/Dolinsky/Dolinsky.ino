#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Definice pinů
#define OUT_VSTUP_SERVO 9
#define OUT_LED_ZEL_SIG 12
#define OUT_LED_SVETLA 10
#define OUT_JUST_ANOTHER_ZEM 11
#define OUT_P_TRIG 4
#define IN_P_ECHO 5
// Čísla pinů pro krokový zahradní motor
#define OUT_ZAHRADA_MOTOR_1 8
#define OUT_ZAHRADA_MOTOR_2 11
#define OUT_ZAHRADA_MOTOR_3 13
#define OUT_ZAHRADA_MOTOR_4 7
#define IN_ZAHRADA_DORAZ 3

// Konstanta pro nastavení rychlosti,
// se zvětšujícím se číslem se rychlost zmenšuje
#define ZAHRADA_RYCHLOST 1
#define ZAHRADA_UHEL 360
#define ZAHRADA_KROKY_POCET 8

bool vstup_otevreno = false;
Servo vstup_servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Inicializace výstupů k zahradnímu motoru
  pinMode(OUT_ZAHRADA_MOTOR_1, OUTPUT);
  pinMode(OUT_ZAHRADA_MOTOR_2, OUTPUT);
  pinMode(OUT_ZAHRADA_MOTOR_3, OUTPUT);
  pinMode(OUT_ZAHRADA_MOTOR_4, OUTPUT);
  // Inicializace vstupu od dorazu zahradních dveří
  pinMode(IN_ZAHRADA_DORAZ, INPUT);
  pinMode(OUT_P_TRIG, OUTPUT);
  pinMode(IN_P_ECHO, INPUT);
  pinMode(OUT_LED_ZEL_SIG, OUTPUT);
  pinMode(OUT_LED_SVETLA, OUTPUT);
  pinMode(OUT_JUST_ANOTHER_ZEM, OUTPUT);

  zahrada_inicializaceDveri();

  lcd.begin();
  lcd.backlight();
  lcd.print("   SS IPF Brno ");
  lcd.setCursor(3, 1);
  lcd.print("Vitame te!!!");

  digitalWrite(OUT_LED_ZEL_SIG, LOW);
  digitalWrite(OUT_LED_SVETLA, LOW);
  digitalWrite(OUT_JUST_ANOTHER_ZEM, LOW);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(OUT_P_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(OUT_P_TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(OUT_P_TRIG, LOW);

  long odezva = pulseIn(IN_P_ECHO, HIGH);
  long vzdalenost = odezva / 58.31;

  Serial.print("Vzdalenost je ");
  Serial.print(vzdalenost);
  Serial.println(" cm.");

  if (vzdalenost < 25) {
    vstup_otevrit();
  }

  if (vstup_otevreno) {
    vstup_zavrit();
    lcd.clear();
  }

  lcd.setCursor(1, 0);
  lcd.print(" SS IPF Brno   ");
  lcd.setCursor(1, 1);
  lcd.print(" Vitame te!!!     ");

  zahrada_otevrit();
  delay(5000);
  zahrada_zavrit();
}

void vstup_otevrit() {
  digitalWrite(OUT_LED_ZEL_SIG, HIGH);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Oteviram dvere.");
  lcd.setCursor(1, 1);
  lcd.print("Zapinam svetla.");

  vstup_servo.attach(OUT_VSTUP_SERVO);

  for (int pos = 85; pos >= 0; pos -= 1) {
    vstup_servo.write(pos);
    delay(10);
  }

  vstup_servo.detach();
  delay(5500);

  vstup_otevreno = true;

  digitalWrite(OUT_LED_ZEL_SIG, LOW);
}

void vstup_zavrit() {
  digitalWrite(OUT_LED_SVETLA, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zaviram dvere!    ");
  lcd.setCursor(0, 1);
  lcd.print("Zhasinam.         ");

  vstup_servo.attach(OUT_VSTUP_SERVO);

  for (int pos = 0; pos < 140; pos++) {
    vstup_servo.write(pos);
    delay(5);
  }

  vstup_servo.detach();
  vstup_otevreno = false;
  delay(2000);
  digitalWrite(OUT_LED_SVETLA, LOW);
}

// Plná rotace zahradního motoru o 360 stupňů = 512 volání
// Funkce Otevri() či Zavri()
void zahrada_inicializaceDveri() {
  // Nastavení výchozí polohy
  //   1) úplné otevření na doraz
  //   2) zavření
  int doraz = digitalRead(IN_ZAHRADA_DORAZ);

  // Otevírej
  while (doraz == 1) {
    for (int i = 0; i < ZAHRADA_KROKY_POCET; i++) {
      zahrada_krok(i);
      doraz = digitalRead(IN_ZAHRADA_DORAZ);
    }
  }

  // 2 * plná rotace o 360 stupňů = 2 * 512 volání
  zahrada_zavrit();
  zahrada_zavrit();
}

// Funkce pro volání jednotlivých
// kroků pro otočení po či proti směru hodinových
// ručiček pro zahradní motor
void zahrada_otevrit() {
  for (int i = 0; i < (ZAHRADA_UHEL * 64 / 45); i++) {
    for (int i = 0; i < ZAHRADA_KROKY_POCET; i++) {
      zahrada_krok(i);
    }
  }
}

void zahrada_zavrit() {
  for (int i = 0; i < (ZAHRADA_UHEL * 64 / 45); i++) {
    for (int i = ZAHRADA_KROKY_POCET - 1; i >= 0; i--) {
      zahrada_krok(i);
    }
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
      {HIGH, LOW, LOW, LOW},  // 1
      {HIGH, HIGH, LOW, LOW}, // 2
      {LOW, HIGH, LOW, LOW},  // 3
      {LOW, HIGH, HIGH, LOW}, // 4
      {LOW, LOW, HIGH, LOW},  // 5
      {LOW, LOW, HIGH, HIGH}, // 6
      {LOW, LOW, LOW, HIGH},  // 7
      {HIGH, LOW, LOW, HIGH}  // 8
  };

  const int *HODNOTY = ZAHRADA_KROKY[krok];
  digitalWrite(OUT_ZAHRADA_MOTOR_1, HODNOTY[0]);
  digitalWrite(OUT_ZAHRADA_MOTOR_2, HODNOTY[1]);
  digitalWrite(OUT_ZAHRADA_MOTOR_3, HODNOTY[2]);
  digitalWrite(OUT_ZAHRADA_MOTOR_4, HODNOTY[3]);

  delay(ZAHRADA_RYCHLOST);
}
