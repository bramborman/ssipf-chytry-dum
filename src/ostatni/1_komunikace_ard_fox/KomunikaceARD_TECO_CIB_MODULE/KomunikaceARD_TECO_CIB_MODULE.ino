#include <Wire.h>
//#include <LiquidCrystal_PCF8574.h>
#include <NewPing.h>

#define TRIGGER_PIN_1  12 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_1     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_2  10 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_2     9  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define BYTES 10
#define TIMEOUT 100
unsigned char rec_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'}; 
unsigned char send_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'};
uint8_t dist1,dist2,received;
char s1[4]={' ',' ',' ',0};
char s2[4]={' ',' ',' ',0};
char seq[6]={' ',' ',' ',' ',' ',0};



NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//LiquidCrystal_PCF8574 lcd(0x3f);  // set the LCD address to 0x3f for a 16 chars and 2 line display


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
  //packet[0]='A';
  //packet[1]='A';
  Serial.write(send_packet, BYTES);
}
void printPacket(void){
 // lcd.setCursor(0,0);
 // lcd.print("                ");
 // lcd.setCursor(0,0);
 // sprintf(seq, "%03d: ", rec_packet[0]);
 // lcd.print(seq);
 // for (uint8_t n=1; n<BYTES; n++){
 //   lcd.write(rec_packet[n]);
 // }
}

void setup()
{
 // lcd.begin(16, 2); // initialize the lcd
 // lcd.setBacklight(255);
 // lcd.home(); lcd.clear();
 // lcd.print("Init...");
 // delay(1000);
 // lcd.clear();
  Serial.begin(9600);
  Serial.setTimeout(TIMEOUT);
 pinMode(3,OUTPUT);
} // setup()

void loop()
{

  dist1=sonar1.ping_cm();
 // delay(100);
  dist2=sonar2.ping_cm();
//  delay(100);
  
  sprintf(s1, "%03d", dist1);
  sprintf(s2, "%03d", dist2);
    
  received=receivePacket();
  if (received != 10) {
  //  lcd.setCursor(0, 0);
  //  lcd.print("Receive timeout!");
    clearBuffer();
    clearPacket(rec_packet);
  }
  else {
    printPacket();
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
      
  
 // if (rec_packet[1]!='S'){
  //  send_packet[0]=rec_packet[0];
 //   for (uint8_t n=1; n<8; n++){
 //       send_packet[n]=rec_packet[n];
 //     }
 // }
 //podmínky pro akci arduino Karta 1 s posláním zpět do Mosaic
  if (rec_packet[1]==0x26 && rec_packet[2]==0x72 && rec_packet[3]==0xCD){
      for (uint8_t n=4; n<8; n++){
        digitalWrite(3,LOW);
       // send_packet[1]=rec_packet[1];
       // send_packet[2]=rec_packet[2];
        send_packet[3]='K';
        send_packet[4]='1';
       // send_packet[n+1]=s2[n];
      }
    }
    if (rec_packet[1]==0x26 && rec_packet[2]==0xA5 && rec_packet[3]==0x43){
      for (uint8_t n=4; n<8; n++){
        digitalWrite(3,HIGH);
     //  send_packet[1]=rec_packet[1];
       // send_packet[2]=rec_packet[2];
        send_packet[3]='K';
        send_packet[4]='2';
      //  send_packet[5]='-';
      //  send_packet[n+1]=s2[n];
      }
    }  
    
 sendPacket();   
  }
  
  
 
// printPacket();
// delay(500);
// clearPacket(rec_packet);
// printPacket();
 // lcd.setCursor(0, 1);
 // lcd.print("S1:");
 // lcd.print(s1);
 // lcd.print(" S2:");
 // lcd.print(s2);
  
}
