// define pins
int LED1 = 13;
int LED2 = 12;
int LED3 = 11;
int zarovka = 8;
int fotorez = A0;     //fotorezistor
int termi = A1;       //termistor
int PIR = 3;
int pin_salina =10;
//int salina = HIGH;//vyhodit až bude drát od jelena

// knihovny pro LCD přes I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// nastavení adresy I2C (0x27 v mém případě),
// a dále počtu znaků a řádků LCD, zde 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(PIR, INPUT);
pinMode (pin_salina, INPUT);


  lcd.begin();
  lcd.backlight();
  lcd.print("     VSE OK");
}

void loop() {
  
int valuePIR = digitalRead(PIR);
int salina = digitalRead(pin_salina);
if (salina == HIGH)
  {
    digitalWrite(LED3, HIGH);
    lcd.setCursor ( 0, 0 );
    lcd.print("Salina se blizi.");// vytisknutí hlášky na první řádek
    lcd.setCursor ( 0, 1 );
    lcd.print("Prchej na salinu!");
    delay (2000);
    salina == LOW;
    digitalWrite(LED3, LOW);
  }
  else
  {
    if (valuePIR == HIGH){
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      lcd.setCursor ( 0, 0 );
      lcd.print("POZOR, VETRELEC!");// vytisknutí hlášky na první řádek
      lcd.setCursor ( 0, 1 );
      lcd.print(" Volam policii!!");
      delay(300);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      delay(300);
    }
    else {  
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      lcd.setCursor ( 0, 0 );
      lcd.print("     VSE OK             ");
      lcd.setCursor ( 0, 1 );
      lcd.print("--------------------");
      }
  }//vypnout displej od šaliny
  delay (1000);
//  salina = LOW;
}

void termistor ()
{
  int termiValue = analogRead(termi);
  Serial.print("sensor = ");
  Serial.print(LDRValue);
  delay(30);
  if (termiValue <=489) 
    {
      digitalWrite(LED, HIGH);    //poslat vypnutí žárovky
      digitalWrite(relay, HIGH);  //poslat zapnutí motorku
      Serial.println("Vypni světlo, zapni ventilaci");
    }
  else 
    { 
      digitalWrite(relay, LOW);     //poslat vypnutí motorku - nebo doplnit if motorek high, pak vypni
      Serial.println("Ventilace vypnuta");
  
  }
