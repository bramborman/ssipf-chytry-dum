#include <Wire.h>

// Odkomentujte následující řádek
// pro zapnutí vypisování testovacích
// zpráv do sériového monitoru
// #define SERIAL_DEBUG

#define LED_ARRAY_SIZE 3
#define LED_ARRAY_RED 0
#define LED_ARRAY_GREEN 1
#define LED_ARRAY_BLUE 2

// Definice pinů pomocí polí pro jednodušší práci
// Piny MUSÍ být v pořadí RGB
const int OUT_LED_1[LED_ARRAY_SIZE] = { 13, 11, 12 };
const int OUT_LED_2[LED_ARRAY_SIZE] = { 51, 53, 52 };
const int OUT_LED_3[LED_ARRAY_SIZE] = { 48, 49, 50 };
const int OUT_LED_4[LED_ARRAY_SIZE] = {  9,  8, 10 };

int a = 0;
int karta = 0;
int c1 = 0;

void setup() {
  // join i2c bus with address #10
  Wire.begin(10);
  Wire.onReceive(receiveEvent);

#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif

  for (int i = 0; i < LED_ARRAY_SIZE; i++) {
    pinMode(OUT_LED_1[i], OUTPUT);
    pinMode(OUT_LED_2[i], OUTPUT);
    pinMode(OUT_LED_3[i], OUTPUT);
    pinMode(OUT_LED_4[i], OUTPUT);
  }

  lampa_vypnout(OUT_LED_1);
  lampa_vypnout(OUT_LED_2);
  lampa_vypnout(OUT_LED_3);
  lampa_vypnout(OUT_LED_4);

  c1 = 0;
  karta = 0;
}

void loop() {
  delay(10);
  karta = a;

  if (karta == c1) {
    lampa_vypnout(OUT_LED_1);
    lampa_vypnout(OUT_LED_2);
    lampa_vypnout(OUT_LED_3);
    lampa_vypnout(OUT_LED_4);
  }
  else {
    lampa_zapnout(OUT_LED_3, karta);
    lampa_zapnout(OUT_LED_4, karta);

    delay(9000);
    c1 = karta;

    lampa_zapnout(OUT_LED_1, c1);
    lampa_zapnout(OUT_LED_2, c1);

    delay(1000);
    lampa_vypnout(OUT_LED_3);
    lampa_vypnout(OUT_LED_4);

    delay(8500);
    lampa_vypnout(OUT_LED_1);
    lampa_vypnout(OUT_LED_2);

    delay(1000);
  }

  karta = 0;
}

void lampa_zapnout(const int led[LED_ARRAY_SIZE], const int b) {
  if (b == 1) {
    digitalWrite(led[LED_ARRAY_GREEN], HIGH);
  }
  else if (b == 2) {
    digitalWrite(led[LED_ARRAY_RED], HIGH);
    digitalWrite(led[LED_ARRAY_GREEN], HIGH);
  }
  else if (b == 3) {
    digitalWrite(led[LED_ARRAY_GREEN], HIGH);
    digitalWrite(led[LED_ARRAY_BLUE], HIGH);
  }
}

void lampa_vypnout(const int led[LED_ARRAY_SIZE]) {
  digitalWrite(led[LED_ARRAY_RED], LOW);
  digitalWrite(led[LED_ARRAY_GREEN], LOW);
  digitalWrite(led[LED_ARRAY_BLUE], LOW);
}

void receiveEvent(int howMany) {
  a = Wire.read();

#ifdef SERIAL_DEBUG
  Serial.println(a);
#endif
}
