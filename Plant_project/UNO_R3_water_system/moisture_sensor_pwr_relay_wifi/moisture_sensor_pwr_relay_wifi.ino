#include <SoftwareSerial.h>

#define RX 10
#define TX 11

String AP = "WiFi-2.4-66CF";       // CHANGE ME
String PASS = "8AA2B4949D"; // CHANGE ME

String API = "VMQBG42EGAZA44OO";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";

int relayPin = 7;

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

SoftwareSerial esp8266(RX,TX);

void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  
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

  String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(sensorValue);

  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");
  
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

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
