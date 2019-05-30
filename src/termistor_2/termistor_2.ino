#define relay 10
int LED = 13;
int LDR = A0;   //pin analog 0

void setup() 
{
Serial.begin(9600);
pinMode(LED, OUTPUT);
pinMode(relay, OUTPUT);
pinMode(LDR, INPUT);
}

void loop() {
int LDRValue = analogRead(LDR);
Serial.print("sensor = ");
Serial.print(LDRValue);
delay(300);
if (LDRValue <=489) 
{
digitalWrite(LED, HIGH);
digitalWrite(relay, HIGH);
Serial.println("It's Dark Outside; Lights status: ON");
}
else 
{
digitalWrite(LED, LOW);
digitalWrite(relay, LOW);
Serial.println("It's Bright Outside; Lights status: OFF");
}
}
