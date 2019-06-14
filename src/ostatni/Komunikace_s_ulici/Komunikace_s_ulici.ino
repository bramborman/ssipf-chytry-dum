#include <Wire.h>

#define BYTES 10
#define TIMEOUT 100
unsigned char rec_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'}; 
unsigned char send_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'};
uint8_t received;
char s1[4]={' ',' ',' ',0};
char s2[4]={' ',' ',' ',0};
char seq[6]={' ',' ',' ',' ',' ',0};
byte karta = 0;



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
   Wire.begin(); 

} 

void loop()
{
  received=receivePacket();
 if (received != 10) {
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
  
  
 //podmínky pro akci arduino Karta 1 s posláním zpět do Mosaic
  if (rec_packet[1]==0x26 && rec_packet[2]==0x72 && rec_packet[3]==0xCD){
      for (uint8_t n=4; n<8; n++){
        send_packet[3]='K';
        send_packet[4]='1';
        karta = 1;
       }
    }

     if (rec_packet[1]==0x26 && rec_packet[2]==0x5E && rec_packet[3]==0xE4){
      for (uint8_t n=4; n<8; n++){
        send_packet[3]='K';
        send_packet[4]='2';
        karta = 2;
      }
    }  
    if (rec_packet[1]==0x26 && rec_packet[2]==0xA5 && rec_packet[3]==0x43){
      for (uint8_t n=4; n<8; n++){
        send_packet[3]='K';
        send_packet[4]='3';
        karta = 3;
      }
    }  
     if (rec_packet[1]==0x26 && rec_packet[2]==0xA5 && rec_packet[3]==0x43){
      for (uint8_t n=4; n<8; n++){
        send_packet[3]='K';
        send_packet[4]='9';
        karta = 9;
      }
    }  
  

  //x++;
 //   
 sendPacket(); 
   
}
 Wire.beginTransmission(10); // transmit to device #10
  //Wire.write("x is ");        // sends five bytes
  Wire.write(karta);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  delay(500);
}
