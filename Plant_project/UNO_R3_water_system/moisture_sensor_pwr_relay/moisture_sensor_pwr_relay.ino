#include <SoftwareSerial.h>

#define RX 10
#define TX 11

int relayPin = 7;

void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  pinMode(relayPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.print("MOISTURE LEVEL: ");
  Serial.println(sensorValue);
  if (sensorValue < 300)
  {
    pmp_prog();
  }
  delay(5000);        // delay in between reads for stability
}
 
void pmp_prog() 
{
  digitalWrite(relayPin, HIGH);
  Serial.println ("Pump ON");
  delay(1000);
  digitalWrite(relayPin, LOW);
  Serial.println("Pump OFF");
}
