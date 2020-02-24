#include <SoftwareSerial.h>

#define RX 10
#define TX 11

String AP = "WiFi-2.4-66CF";
String PASS = "8AA2B4949D";

String API = "VMQBG42EGAZA44OO";
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";

int relayPin = 7;

int countTrueCommand;
int countTimeCommand;
boolean found = false;

SoftwareSerial esp8266(RX,TX);

void setup() {
  // initialize serial communication at 9600 bps
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
  
  pinMode(relayPin, OUTPUT);
}

void loop() {
  delay(5000);
  // read the input on analog pin 0
  int sensorValue = analogRead(0);
  // print out the value you read
  Serial.print("MOISTURE LEVEL: ");
  Serial.println(sensorValue);

  String getData = "GET /update?api_key=" + API + "&" + field + "=" + String(sensorValue);

  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");

  delay(1000);

  String getRequest = "GET /channels/547199/fields/2/last.json?api_key=M0KP3X6U6718DMW0&results=1";
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getRequest.length() + 4), 4, ">");
  esp8266.println(getRequest);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");

  delay(1000);

  if (esp8266.available()) {
    Serial.write(esp8266.read());
    Serial.println(esp8266.read());
  }
  
}

void sendCommand(String command, int maxTime, char readReplay[]){
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command); //at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }
    countTimeCommand++;
  }
  if (found == true)
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
}

void pmp_prog()
{
  digitalWrite(relayPin, HIGH);
  Serial.println("PUMP ON");
  delay(2000);
  digitalWrite(relayPin, LOW);
  Serial.println("PUMP OFF");
}
