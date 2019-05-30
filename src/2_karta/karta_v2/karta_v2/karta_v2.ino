#include <Wire.h>
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

//char str1[3]={'&','r','Í'};

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
int pTrig = 4;
int pEcho = 5;
int pos = 0;
int otevreno = 0;
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
  pinMode(3,OUTPUT);
  digitalWrite(3,LOW);

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



   received=receivePacket();
  if (received != 10) {
  //  lcd.setCursor(0, 0);
  //  lcd.print("Receive timeout!");
    clearBuffer();
    clearPacket(rec_packet);
  }
  else {
//    printPacket();
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

      
  received=receivePacket();
  if (received != 10) {
    clearBuffer();
    clearPacket(rec_packet);
  }
  else {

  if (rec_packet[1]==0x26 && rec_packet[2]==0x72 && rec_packet[3]==0xCD){
      for (uint8_t n=4; n<8; n++){
        digitalWrite(3,HIGH);
        delay(100);
        digitalWrite(3,LOW);
        send_packet[3]='K';
        send_packet[4]='1';
       lcd.setCursor ( 1, 0 );
       lcd.print(" Dodavatel   ");
       lcd.setCursor ( 1, 1 );
       lcd.print(" Cekame na tebe!!!");
       if (vzdalenost < 25)
  {
    otevri();
  }
  if (otevreno == 1)
  {
    zavri();
    lcd.clear();
  }
      }
    
    }else{
      if (rec_packet[1]==0x26 && rec_packet[2]==0xA5 && rec_packet[3]==0x43){
      for (uint8_t n=4; n<8; n++){
        digitalWrite(3,HIGH);
        delay(100);
        digitalWrite(3,LOW);
        send_packet[3]='K';
        send_packet[4]='2';
      lcd.setCursor ( 1, 0 );
      lcd.print(" Odberatel ");
      lcd.setCursor ( 1, 1 );
      lcd.print("Uz se tesim na jidlo!!!");
      if (vzdalenost < 25)
  {
    otevri();
  }
  if (otevreno == 1)
  {
    zavri();
    lcd.clear();
  }
        
      }}else{
        if (rec_packet[1]==0x26 && rec_packet[2]==0x5E && rec_packet[3]==0xE4){
      for (uint8_t n=4; n<8; n++){
        digitalWrite(3,HIGH);
        delay(100);
        digitalWrite(3,LOW);
        send_packet[3]='K';
        send_packet[4]='3';
      lcd.setCursor ( 1, 0 );
      lcd.print(" ZLODEJ ");
      lcd.setCursor ( 1, 1 );
      lcd.print("  Dovnitr nepujdes!!!");
      
       
    }}else
        {
          if(rec_packet[2]!=0x72 ||rec_packet[2]!=0xA5 ||rec_packet[2]!=0x5E){
          send_packet[3]='K';
          send_packet[4]='x';
          digitalWrite(3,HIGH);
                lcd.setCursor ( 1, 0 );
      lcd.print(" navsteva ");
      lcd.setCursor ( 1, 1 );
      lcd.print("Pozadej maitele o vstup");
      if (vzdalenost < 25)
  {
    otevri();
  }
  if (otevreno == 1)
  {
    zavri();
    lcd.clear();
  }
         
          }
        
        }
    }}}
  
   
 sendPacket();   
  } }

  
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
  delay(3000);

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
  delay(1000);
  digitalWrite(ledSvetla, LOW);
  delay(2000);
}
