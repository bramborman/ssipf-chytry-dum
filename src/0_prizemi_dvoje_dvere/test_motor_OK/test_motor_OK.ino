#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Servo.h>

//definice pinů
#define ledZelSig 12
#define ledSvetla 10
#define justAnotherZem 11


LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myservo;
int pTrig = 4;
int pEcho = 5;
int pos = 0;
int otevreno = 0;
long odezva, vzdalenost;

// čísla pinů pro krokový zahradní motor
const int in1 =  8;
const int in2 = 11;
const int in3 = 13;
const int in4 = 7;
const int in5 = 3;
int pocet_kroku = 0;
int doraz;
// proměnná pro nastavení rychlosti,
// se zvětšujícím se číslem se rychlost zmenšuje
int rychlost = 1;
//
int uhel = 360;

void setup() {
 
  // inicializace výstupů k zahradnímu motoru
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //inicializace vstupu od dorazu zahradních dveří
  pinMode(in5, INPUT);
  doraz = digitalRead(in5);

  //nastavení výchozí polohy - nejdříve úplné otevření na doraz, pak zavře
  while (doraz == 1) //otevírej
    {OtevriZahradu();}
  for(int i =1;i<2*(uhel*64/45);i++)  //2* plná rotace o 360 stupňů = 2*512 volání
    {ZavriZahradu();}
  
  // myservo.attach(9);
  lcd.begin();
  lcd.backlight();
  lcd.print("   SS IPF Brno ");
  lcd.setCursor ( 3, 1 );
  lcd.print("Vitame te!!!");

  pinMode(pTrig, OUTPUT);
  pinMode(pEcho, INPUT);
  pinMode(ledZelSig, OUTPUT);
  pinMode(ledSvetla, OUTPUT);
  pinMode(justAnotherZem, OUTPUT);
  digitalWrite(ledZelSig, LOW);
  digitalWrite(ledSvetla, LOW);
  digitalWrite(justAnotherZem, LOW);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(pTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pTrig, HIGH);
  delayMicroseconds(5);
  digitalWrite(pTrig, LOW);
  odezva = pulseIn(pEcho, HIGH);
  vzdalenost = odezva / 58.31;
  Serial.print("Vzdalenost je ");
  Serial.print(vzdalenost);
  Serial.println(" cm.");
  if (vzdalenost < 25)
  {
    otevri();
  }
  if (otevreno == 1)
  {
    zavri();
    lcd.clear();
  }
  lcd.setCursor ( 1, 0 );
  lcd.print(" SS IPF Brno   ");
  lcd.setCursor ( 1, 1 );
  lcd.print(" Vitame te!!!     ");

for(int i=0;i<(uhel*64/45);i++){
    OtevriZahradu();}
  
  delay(5000);

  for(int i=0;i<(uhel*64/45);i++){
    ZavriZahradu();  }
}

void otevri()
{
  digitalWrite(ledZelSig, HIGH);
  lcd.clear();
  lcd.setCursor ( 1, 0 );
  lcd.print("Oteviram dvere.");
  lcd.setCursor ( 1, 1 );
  lcd.print("Zapinam svetla.");

  myservo.attach(9);
  for (pos = 85; pos >= 0; pos -= 1)
  {
    myservo.write(pos);
    delay(10);
  }
  myservo.detach();
  delay(5500);

  otevreno = 1;

  digitalWrite(ledZelSig, LOW);
}

void zavri()
{
  digitalWrite(ledSvetla, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Zaviram dvere!    ");
  lcd.setCursor ( 0, 1 );
  lcd.print("Zhasinam.         ");

  myservo.attach(9);
  for (pos = 0; pos < 140; pos += 1)
  {
    myservo.write(pos);
    delay(5);
    /*if (pos >138)
       {myservo.detach();}
    */
  }
  myservo.detach();
  otevreno = 0;
  delay(2000);
  digitalWrite(ledSvetla, LOW); 
  }


  // plná rotace zahradního motoru o 360 stupňů = 512 volání
  // funkce Otevri() či Zavri()

// zde následují funkce pro volání jednotlivých
// kroků pro otočení po či proti směru hodinových
// ručiček pro zahradní motor
void OtevriZahradu() {
  krok1();
  krok2();
  krok3();
  krok4();
  krok5();
  krok6();
  krok7();
  krok8();
}
void ZavriZahradu() {
  krok8();
  krok7();
  krok6();
  krok5();
  krok4();
  krok3();
  krok2();
  krok1();
}

// každý krok obsahuje výrobcem dané pořadí
// pro správné spínání motoru(zahradního) a následnou
// pauzu, kterou určujeme rychlost otáčení
void krok1(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(rychlost);
  pocet_kroku =1;
}
void krok2(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(rychlost);
  pocet_kroku =2;
}
void krok3(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(rychlost);
  pocet_kroku =3;
}
void krok4(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(rychlost);
  pocet_kroku =4;
}
void krok5(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(rychlost);
  pocet_kroku =5;
}
void krok6(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
  delay(rychlost);
  pocet_kroku =6;
}
void krok7(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(rychlost);
  pocet_kroku =7;
}
void krok8(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(rychlost);
  pocet_kroku =8;
}
