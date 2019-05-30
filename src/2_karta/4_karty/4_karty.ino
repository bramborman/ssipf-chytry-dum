#include <Wire.h>
#include <NewPing.h>


#define BYTES 10
#define TIMEOUT 100
unsigned char rec_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'}; 
unsigned char send_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'};
uint8_t received;
char s1[4]={' ',' ',' ',0};
char s2[4]={' ',' ',' ',0};
char seq[6]={' ',' ',' ',' ',' ',0};

//char str1[3]={'&','r','Í'};

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
} 

void loop()
{    

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

      
/*  received=receivePacket();
  if (received != 10) {
    clearBuffer();
    clearPacket(rec_packet);
  }
  else {*/

  if (rec_packet[1]==0x26 && rec_packet[2]==0x72 && rec_packet[3]==0xCD){
      for (uint8_t n=4; n<8; n++){
        digitalWrite(3,HIGH);
        delay(100);
        digitalWrite(3,LOW);
        send_packet[3]='K';
        send_packet[4]='1';
      }
    
    }else{
      if (rec_packet[1]==0x26 && rec_packet[2]==0xA5 && rec_packet[3]==0x43){
      for (uint8_t n=4; n<8; n++){
        digitalWrite(3,HIGH);
        delay(100);
        digitalWrite(3,LOW);
        send_packet[3]='K';
        send_packet[4]='2';
        
      }}else{
        if (rec_packet[1]==0x26 && rec_packet[2]==0x5E && rec_packet[3]==0xE4){
      for (uint8_t n=4; n<8; n++){
        digitalWrite(3,HIGH);
        delay(100);
        digitalWrite(3,LOW);
        send_packet[3]='K';
        send_packet[4]='3';
       
    }}else
        {
          if(rec_packet[2]!=0x72 ||rec_packet[2]!=0xA5 ||rec_packet[2]!=0x5E){
          send_packet[3]='K';
          send_packet[4]='x';
          digitalWrite(3,HIGH);
         
          }
        
        }
    }}}
  

     
    
 sendPacket();   
 }
