
#include <Wire.h>    //funguje - píše furt dané číslo dokud není jiné

void setup() {
  Wire.begin(10);                // join i2c bus with address #10
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);  
}

void loop() {
   delay(100);

}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
