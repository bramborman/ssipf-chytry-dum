
#include <Wire.h>    //funguje - píše furt dané číslo dokud není jiné
int a;

void setup() {
  Wire.begin(10);  
    pinMode(13,OUTPUT);            
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);  
}

void loop() {
   delay(100);
if (a==1){digitalWrite(13, HIGH);}
   else if (a==2){digitalWrite(13, LOW);}
}

void receiveEvent(int howMany) {
 // while ( Wire.available()>0) { // loop through all but the last
 //   a = Wire.read(); // receive byte as a character
//    Serial.print(a);         // print the character
 // }
   a = Wire.read();    // receive byte as an integer
  Serial.println(a);         // print the integer
}
