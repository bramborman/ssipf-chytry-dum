#include <Wire.h>               //kam narvat karta=0  ???????
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define ledZelSig 12
#define ledSvetla 10
#define justAnotherZem 11

#define BYTES 10
#define TIMEOUT 100
unsigned char rec_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'}; 
unsigned char send_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'};
uint8_t received;
char s1[4]={' ',' ',' ',0};
char s2[4]={' ',' ',' ',0};
char seq[6]={' ',' ',' ',' ',' ',0};

//char str1[3]={'&','r','Í'};

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myservo;
int pTrig = 4;
int pEcho = 5;
int pos = 0;
int otevreno = 0;
int karta = 0;
long odezva, vzdalenost;


uint8_t clearBuffer(void){
  uint8_t n=0;
  while (Serial.available()){
    Serial.read();
    n++;
  }
  return n;
}

void clearPacket(unsigned char *packet){
  packet[0]=0;
  for (uint8_t n=1; n<BYTES; n++){
    packet[n]=' ';
  }
}

uint8_t receivePacket(void){
  return Serial.readBytes(rec_packet,BYTES);
}

void sendPacket(void){
 Serial.write(send_packet, BYTES);
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(TIMEOUT);

  lcd.begin();
  lcd.backlight();
  lcd.print("   Vila Fiesta   ");
  lcd.setCursor ( 3, 1 );
  lcd.print("  SS IPF Brno ");

  pinMode(pTrig, OUTPUT);
  pinMode(pEcho, INPUT);
  pinMode(ledZelSig, OUTPUT);
  pinMode(ledSvetla, OUTPUT);
  pinMode(justAnotherZem, OUTPUT);
  digitalWrite(ledZelSig, LOW);
  digitalWrite(ledSvetla, LOW);
  digitalWrite(justAnotherZem, LOW);
} 

void loop()
{  
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
  
  received=receivePacket();


  
  /*if (received != 10) {
    clearBuffer();
    clearPacket(rec_packet);
    }
  else {

    clearPacket(send_packet);
    send_packet[0]=rec_packet[0];
    if (rec_packet[1]=='S' && rec_packet[2]=='1'){
      for (uint8_t n=2; n<8; n++){
        send_packet[1]=rec_packet[1];
        send_packet[2]=rec_packet[2];
        send_packet[n+1]=s1[n-2];
        }
      }
       
    if (rec_packet[1]=='S' && rec_packet[2]=='2'){
      for (uint8_t n=2; n<8; n++){
        send_packet[1]=rec_packet[1];
        send_packet[2]=rec_packet[2];
        send_packet[n+1]=s2[n];
        }
      }
*/
 if (rec_packet[2]==0x72){
      for (uint8_t n=4; n<8; n++){
       // send_packet[3]='K';
       // send_packet[4]='1';
        karta = 1;            //zelená žena (maminka) Karta 1
      }
     }
    else{
      if (rec_packet[2]==0x5E){
        for (uint8_t n=4; n<8; n++){
         // send_packet[3]='K';
         // send_packet[4]='2';
          karta = 2;        //pruhovaný muž (tatínek)  Karta 2
          }
        }
        else{
          if (rec_packet[2]==0xA5){
           for (uint8_t n=4; n<8; n++){
           //send_packet[3]='K';
           // send_packet[4]='3';
            karta = 3;      //bílá návštěva  Karta 3
            }
          }

          else{
           if (rec_packet[2]==0x78){
            for (uint8_t n=4; n<8; n++){
            // send_packet[3]='K';
            // send_packet[4]='4';
             karta = 4;     //šedý zloděj Karta 4
              }
            }
            else
            {
            if(rec_packet[2]!=0x72 ||rec_packet[2]!=0xA5 ||rec_packet[2]!=0x5E||rec_packet[2]!=0x78){
             // send_packet[3]='K';
             // send_packet[4]='x';
              karta = 99;

            }
          }
    }}}


  if (vzdalenost < 25)
  {
    if(karta==1){
    otevri_Z();
    }
    else {
      if(karta==2){
      otevri_M();
       }
       else{
        if(karta==3){
          otevri_N();
        }
       }
    }
  }
  if (otevreno == 1&&karta==1)
  {
    zavri_Z();
    lcd.clear();
  }
  if (otevreno == 1&&karta==2)
  {
    zavri_M();
    lcd.clear();
  }
  if (otevreno == 1&&karta==3)
  {
    zavri_N();
    lcd.clear();
  }
  lcd.setCursor ( 1, 0 );
  lcd.print("  Vila Fiesta   ");
  lcd.setCursor ( 1, 1 );
  lcd.print("  SS IPF Brno   ");
   }
     
 //sendPacket();   
 
 
 void otevri_Z()
{
  digitalWrite(ledZelSig, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Vitej doma pani.");
  lcd.setCursor ( 1, 1 );
  lcd.print("Uz mame hlad...");

  myservo.attach(9);
  for (pos = 85; pos >= 0; pos -= 1)
  {
    myservo.write(pos);
    delay(10);
  }
  myservo.detach();
  delay(3500);

  otevreno = 1;

  digitalWrite(ledZelSig, LOW);
}

 void otevri_M()
{
  digitalWrite(ledZelSig, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Vitej muj pane.");
  lcd.setCursor ( 0, 1 );
  lcd.print("Veceri si uvar!");

  myservo.attach(9);
  for (pos = 85; pos >= 0; pos -= 1)
  {
    myservo.write(pos);
    delay(10);
  }
  myservo.detach();
  delay(3500);

  otevreno = 1;

  digitalWrite(ledZelSig, LOW);
}

 void otevri_N()
{
  digitalWrite(ledZelSig, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Radi te vidime.");
  lcd.setCursor ( 1, 1 );
  lcd.print("Pojd na kafe!");

  myservo.attach(9);
  for (pos = 85; pos >= 0; pos -= 1)
  {
    myservo.write(pos);
    delay(10);
  }
  myservo.detach();
  delay(3500);

  otevreno = 1;

  digitalWrite(ledZelSig, LOW);
}
void zavri_M()
{
  digitalWrite(ledSvetla, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Vypni televizi!");
  lcd.setCursor ( 0, 1 );
  lcd.print("Nebo ji vypnu JA!");  //

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
  delay(2000);
 }

void zavri_Z()
{
  digitalWrite(ledSvetla, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Deti jdou spat.");
  lcd.setCursor ( 0, 1 );
  lcd.print("Zhasinam. Dobrou");

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
  delay(2000);
}

void zavri_N()
{
  digitalWrite(ledSvetla, HIGH);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  lcd.print("Mej se hezky.");
  lcd.setCursor ( 0, 1 );
  lcd.print("Stastnou cestu!");  //

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
  delay(2000);
}
