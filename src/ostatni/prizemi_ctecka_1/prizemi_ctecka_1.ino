#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Servo.h>

//definice pinů
#define ledZelSig 12
#define ledSvetla 10


LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myservo;
int nahoda = 0;
int pos = 0;
int otevreno = 0;
long odezva, vzdalenost;
int x = 0;


void setup() {
  
  lcd.begin();
  lcd.backlight();
  lcd.print("   SS IPF Brno ");
  lcd.setCursor ( 3, 1 );
  lcd.print("Vitame te!!!");


  pinMode(ledZelSig, OUTPUT);
  pinMode(ledSvetla, OUTPUT);
  
  digitalWrite(ledZelSig, LOW);
  digitalWrite(ledSvetla, LOW);
  
  Serial.begin(9600);

  Wire.begin(20);
  Wire.onReceive(receiveEvent);
}

void loop() {
  //nahoda = random(0,3);
 // Serial.print(nahoda);

 nahoda = x;
 
  if (nahoda == 1){
    otevri1();}
    else
      {}
      
  if (otevreno == 1 && nahoda == 1)
    {delay(3000);
    zavri1();
    lcd.clear();
    }

  if (nahoda == 2){
    otevri2();}
    else
      {}
  if (otevreno == 1 && nahoda == 2)
    {delay(3000);
    zavri2();
    lcd.clear();
    }

  lcd.setCursor ( 1, 0 );
  lcd.print(" Vila Fiesta  ");
  lcd.setCursor ( 1, 1 );
  lcd.print("SS IPF Vas vita!");
}

void otevri1()
{
  digitalWrite(ledZelSig, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Vitej doma mami");
  lcd.setCursor ( 0, 1 );
  lcd.print("Uz mame hlad!");

  myservo.attach(9);
  for (pos = 85; pos >= 0; pos -= 1)
  {
    myservo.write(pos);
    delay(10);
  }
  myservo.detach();
  delay(3000);

  otevreno = 1;

  digitalWrite(ledZelSig, LOW);
}

void zavri1()
{
  digitalWrite(ledSvetla, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Diky za obed.");
  lcd.setCursor ( 0, 1 );
  lcd.print("A co skola?!!!");

  myservo.attach(9);
  for (pos = 0; pos < 140; pos += 1)
  {
    myservo.write(pos);
    delay(5);
  }
  myservo.detach();
  otevreno = 0;
  delay(2000);
  digitalWrite(ledSvetla, LOW);
  delay(1000);
}

void otevri2()
{
  digitalWrite(ledZelSig, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Ahoj tati!");
  lcd.setCursor ( 0, 1 );
  lcd.print("Pujcis mi auto?");

  myservo.attach(9);
  for (pos = 85; pos >= 0; pos -= 1)
  {
    myservo.write(pos);
    delay(10);
  }
  myservo.detach();
  delay(3000);

  otevreno = 1;

  digitalWrite(ledZelSig, LOW);
}

void zavri2()
{
  digitalWrite(ledSvetla, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Nepujcim.");
  lcd.setCursor ( 0, 1 );
  lcd.print("Moc utracis!");

  myservo.attach(9);
  for (pos = 0; pos < 140; pos += 1)
  {
    myservo.write(pos);
    delay(5);
  }
  myservo.detach();
  otevreno = 0;
  delay(2000);
  digitalWrite(ledSvetla, LOW);
  delay(1000);
}

void receiveEvent(int howMany) {
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

