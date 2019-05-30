
   #include <Wire.h>

   int a = 0;
   int karta = 0;
 //  char b = '0';
 int c1 = 0;
 int c2 = 0;
 int c3 = 0;
 
    //lampa 1
    int redPin1 = 13;
    int greenPin1 = 11;
    int bluePin1 = 12;

    //lampa 2
    int redPin2 = 51;
    int greenPin2 = 53;
    int bluePin2 = 52;

    //lampa 3
    int redPin3 = 48;
    int greenPin3 = 49;
    int bluePin3 = 50;

    //lampa 4
    int redPin4 = 9;
    int greenPin4 = 8;
    int bluePin4 = 10;


void setup(){
     
     Wire.begin(10);                // join i2c bus with address #10
     Wire.onReceive(receiveEvent);
     Serial.begin(9600);

 //pinMode(48,OUTPUT);
     
     
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

     digitalWrite (greenPin1,LOW);
     digitalWrite (greenPin2,LOW);
     digitalWrite (redPin1,LOW);
     digitalWrite (redPin2,LOW); 
     digitalWrite (bluePin1,LOW);
     digitalWrite (bluePin2,LOW);
     digitalWrite (greenPin3,LOW);
     digitalWrite (greenPin4,LOW);
     digitalWrite (redPin3,LOW);
     digitalWrite (redPin4,LOW); 
     digitalWrite (bluePin3,LOW);
     digitalWrite (bluePin4,LOW);
     zhasni12();
     zhasni34();
}

void loop() {

 delay(100);
  
 //karta = random(0,3) + 1; //vybere se barva
 
  karta = a;
  
  lampa34(karta);
  karta = 0;//rožne se lampa 3 a 4
  delay (2000);
  lampa12(c1); //rožne se lampa 1 a 2
  delay (500);
  c1 = 0;
  zhasni34(); //zhasne se lampa 3 a 4
  delay (2000);
  zhasni12(); //zhasne se lampa 1 a 2
  delay (2000);
}

void lampa34(int b){
  if (b == 1){
   // Serial.println("lampa34 barva G");
    digitalWrite (greenPin3,HIGH);
    digitalWrite (greenPin4,HIGH);
  }
  else if (b == 2){
    //Serial.println("lampa34 barva RB");
    digitalWrite (redPin3,HIGH);
    digitalWrite (redPin4,HIGH);
    digitalWrite (bluePin3,HIGH);
    digitalWrite (bluePin4,HIGH);
  }
  else {
    if (b == 3){
   // Serial.println("lampa34 barva GB");
    digitalWrite (greenPin3,HIGH);
    digitalWrite (greenPin4,HIGH);
    digitalWrite (bluePin3,HIGH);
    digitalWrite (bluePin4,HIGH);
    }
    
  } c1 = b;
}

void lampa12(int b){
  if (b == 1){
   // Serial.println("lampa12 barva G");
     digitalWrite (greenPin1,HIGH);
     digitalWrite (greenPin2,HIGH);
  }
  else if (b == 2){
  // Serial.println("lampa12 barva RB");
   digitalWrite (redPin1,HIGH);
   digitalWrite (redPin2,HIGH);
   digitalWrite (bluePin1,HIGH);
   digitalWrite (bluePin2,HIGH);
  }
  else {
    if (b == 3){
   // Serial.println("lampa12 barva GB");
    digitalWrite (greenPin1,HIGH);
    digitalWrite (greenPin2,HIGH);
    digitalWrite (bluePin1,HIGH);
    digitalWrite (bluePin2,HIGH);
    }
  }
 }

void zhasni34(){
   digitalWrite (greenPin3,LOW);
   digitalWrite (greenPin4,LOW);
   digitalWrite (redPin3,LOW);
   digitalWrite (redPin4,LOW); 
   digitalWrite (bluePin3,LOW);
   digitalWrite (bluePin4,LOW);     
 }

void zhasni12(){
  digitalWrite (greenPin1,LOW);
  digitalWrite (greenPin2,LOW);
  digitalWrite (redPin1,LOW);
  digitalWrite (redPin2,LOW); 
  digitalWrite (bluePin1,LOW);
  digitalWrite (bluePin2,LOW);
}


void receiveEvent(int howMany) {
  //while (1 < Wire.available()) { // loop through all but the last
  //   b = Wire.read(); // receive byte as a character
  //  Serial.print(b);         // print the character
  //}
   a = Wire.read();    // receive byte as an integer
  Serial.println(a); // print the integer
  delay (1000);

}
