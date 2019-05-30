#include <Wire.h>               //ulice s posílám zpráv do Foxee
#include <NewPing.h>

#define BYTES 10
#define TIMEOUT 100
unsigned char rec_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'}; 
unsigned char send_packet[BYTES]={'0','1','2','3','4','5','6','7','8','9'};
uint8_t received;
char s1[4]={' ',' ',' ',0};
char s2[4]={' ',' ',' ',0};
char seq[6]={' ',' ',' ',' ',' ',0};

int karta = 0;

   
    int redPin1 = 13;
    int greenPin1 = 11;
    int bluePin1 = 12;

    int redPin2 = 51;
    int greenPin2 = 53;
    int bluePin2 = 52;

    int redPin3 = 48;
    int greenPin3 = 49;
    int bluePin3 = 50;
    
    int redPin4 = 9;
    int greenPin4 = 8;
    int bluePin4 = 10;
    
int pinOut [4] = {4, 5, 6, 7};
int pinStav [4] = {0,0,0,0};

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
  
     pinMode(greenPin1, OUTPUT);
     pinMode (redPin1, OUTPUT);
     pinMode (bluePin1, OUTPUT);

     pinMode(greenPin2, OUTPUT);
     pinMode (redPin2, OUTPUT);
     pinMode (bluePin2, OUTPUT);

     pinMode(greenPin3, OUTPUT);
     pinMode (redPin3, OUTPUT);
     pinMode (bluePin3, OUTPUT);

     pinMode(greenPin4, OUTPUT);
     pinMode (redPin4, OUTPUT);
     pinMode (bluePin4, OUTPUT);
 /*    
   Serial.begin(9600);  
   for (int i=0; i < 4; i++){
      pinMode(pinOut[i], INPUT);}   */

} 
void loop(){   

    
    digitalWrite (greenPin1,HIGH);
    digitalWrite (greenPin2,HIGH);
    digitalWrite (greenPin3,HIGH);
    digitalWrite (greenPin4,HIGH);


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

  if (rec_packet[2]==0x72){
      for (uint8_t n=4; n<8; n++){
        send_packet[3]='K';
        send_packet[4]='1';
        karta = 1;    //zelená žena (maminka) Karta 1
        }
     }
    else{
      if (rec_packet[2]==0x5E){
        for (uint8_t n=4; n<8; n++){
          send_packet[3]='K';
          send_packet[4]='2';
          karta = 2;        //pruhovaný muž (tatínek)  Karta 2
          }
        }
        else{
          if (rec_packet[2]==0xA5){
           for (uint8_t n=4; n<8; n++){
            send_packet[3]='K';
            send_packet[4]='3';
            karta = 3;      //bílá návštěva  Karta 3
            }
          }

          else{
           if (rec_packet[2]==0x78){
            for (uint8_t n=4; n<8; n++){
             send_packet[3]='K';
             send_packet[4]='4';
             karta = 4;     //šedý zloděj Karta 4
              }
            }
            else
            {
            if(rec_packet[2]!=0x72 ||rec_packet[2]!=0xA5 ||rec_packet[2]!=0x5E||rec_packet[2]!=0x78){
              send_packet[3]='K';
              send_packet[4]='x';
              karta = 9;
            }
          }
    }}}
   karta = 0;



/*
       
    digitalWrite (redPin1, LOW);
    digitalWrite (bluePin1, LOW);

 
  Serial.print("Stav vstupnich pinu: ");
  for (int i=0; i < 4; i++){
    
    bool stav;
    stav = !(digitalRead(pinOut[i]));
    pinStav[i] = stav;
    Serial.print(stav);
    Serial.print("  ");
   
    if (pinStav[1] == 1) //if =1 pak D1 na plošňáku má překážku, ve výpisu je to: 0 1 0 0, je od velké brány první 
  {  
    digitalWrite (greenPin2, HIGH);
    digitalWrite (redPin2, LOW);
    digitalWrite (bluePin2, LOW);
  }


    if (pinStav[2] == 1) //D2 na plošňáku, ve výpisu je to: 1 0 0 0, je od velké brány druhé
  {  digitalWrite (greenPin1, LOW);
    digitalWrite (redPin1, LOW);
    digitalWrite (bluePin1, LOW);
  }
  
 //D3 na plošňáku, ve výpisu je to: 0 0 1 0, je od velké brány třetí
      if (pinStav[3] == 1) 
  {  
    delay (2000);
    digitalWrite (greenPin2, LOW);
    digitalWrite (redPin2, LOW);
    digitalWrite (bluePin2, LOW);
  }
  }
  Serial.println();
   
 delay(1000);*/
 
 sendPacket();
  }}

/*void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin1, red);
  analogWrite(greenPin1, green);
  analogWrite(bluePin1, blue);  
  analogWrite(redPin2, red);
  analogWrite(greenPin2, green);
  analogWrite(bluePin2, blue);  

}*/
