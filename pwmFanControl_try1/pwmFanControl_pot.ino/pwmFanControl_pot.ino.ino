#include "AVR_PWM.h"

#define POT A0
#define WHITE 3
#define BLUE 4
#define GREEN 5
#define YELLOW 6
#define RED 7

#define RPM 2
#define PWM 9

#define DEBOUNCE 1 //0 is fine for most fans, crappy fans may require 10 or 20 to filter out noise
#define FANSTUCK_THRESHOLD 500 //if no interrupts were received for 500ms, consider the fan as stuck and report 0 RPM

int val;

int potValue = 0;

unsigned long lastButtonPress = 0;
unsigned int ledState = 0;
int currentRPM;
int fanVal;

int fanSpeed = 0;
int prevFanSpeed = 0;


//Interrupt handler. Stores the timestamps of the last 2 interrupts and handles debouncing
unsigned long volatile ts1=0,ts2=0;
void tachISR() {
    unsigned long m=millis();
    if((m-ts2)>DEBOUNCE){
        ts1=ts2;
        ts2=m;
    }
}
//Calculates the RPM based on the timestamps of the last 2 interrupts. Can be called at any time.
unsigned long calcRPM(){
  unsigned long ts1_copy, ts2_copy;
  noInterrupts();
  ts1_copy = ts1;
  ts2_copy = ts2;
  interrupts();
  if(millis()-ts2_copy<FANSTUCK_THRESHOLD&&ts2_copy!=0){
    return (60000/(ts2_copy-ts1_copy))/2;
  }else return 0;
}

void analogWrite25k(int pin, int value)
{
  switch (pin)
  {
    case 9:
      OCR1A = value;
      break;
      
    case 10:
      OCR1B = value;
      break;
      
    default: // no other pin will work
      break;
  }
}

void setup() {

	// Set encoder pins as inputs
  pinMode(POT,INPUT);
  pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(RPM, INPUT_PULLUP);

	// Setup Serial Monitor
	Serial.begin(9600);

  //attachInterrupt(digitalPinToInterrupt(RPM),tachISR,FALLING); //set tachISR to be triggered when the signal on the sense pin goes low

	// Read the initial pot value
	analogWrite25k(PWM, 0);
  TCCR1A = 0;            //clear timer registers
  TCCR1B = 0;
  TCNT1 = 0;
  TCCR1B |= _BV(CS10);   //no prescaler
  ICR1 = 320;            //PWM mode counts up 320 then down 320 counts (25kHz)

  OCR1A = 64;          //0-320 = 0-100% duty cycle
  TCCR1A |= _BV(COM1A1); //output A clear rising/set falling

  OCR1B = 64;          //0-320 = 0-100% duty cycle
  TCCR1A |= _BV(COM1B1); //output B clear rising/set falling

  TCCR1B |= _BV(WGM13);  //PWM mode with ICR1 Mode 10
  TCCR1A |= _BV(WGM11);  //WGM13:WGM10 set 1010

  ledSeries(0);
}

void loop() {
    potValue = analogRead(POT);
    //Serial.println(potValue);
    fanSpeed = map(potValue, 0, 1020, 0, 10);
    if(prevFanSpeed != fanSpeed){
      prevFanSpeed = fanSpeed;
      //Serial.print(" | Color: ");
      //val = map(fanSpeed, 0, 10, 0, 767);
      //Serial.println(val);
      //showRGB(val);
      fanVal = map(fanSpeed, 0, 10, 0, 320);
      Serial.print("Fan val:");
      Serial.println(fanVal);

      analogWrite25k(PWM, fanVal);

      Serial.print("Fan speed:");
      Serial.println(fanSpeed);
      Serial.print("Fan val:");
      Serial.print(fanVal);

      ledSeries(fanSpeed);
      Serial.print("Fan speed:");
      Serial.print(fanSpeed*10);
      Serial.println(" %"); 
    }
    //Serial.print("Rpm:");
    //Serial.print (calcRPM());
    //Serial.println(" rpm");
	// Put in a slight delay to help debounce the reading
	delay(10);
}

// showRGB()

// This function translates a number between 0 and 767 into a
// specific color on the RGB LED. If you have this number count
// through the whole range (0 to 767), the LED will smoothly
// change color through the entire spectrum.

// The "base" numbers are:
// 0   = pure red
// 255 = pure green
// 511 = pure blue
// 767 = pure red (again)

// Numbers between the above colors will create blends. For
// example, 640 is midway between 512 (pure blue) and 767
// (pure red). It will give you a 50/50 mix of blue and red,
// resulting in purple.

// If you count up from 0 to 767 and pass that number to this
// function, the LED will smoothly fade between all the colors.
// (Because it starts and ends on pure red, you can start over
// at 0 without any break in the spectrum).
void showRGB(int color)
{
  int redIntensity;
  int greenIntensity;
  int blueIntensity;

  // Here we'll use an "if / else" statement to determine which
  // of the three (R,G,B) zones x falls into. Each of these zones
  // spans 255 because analogWrite() wants a number from 0 to 255.

  // In each of these zones, we'll calculate the brightness
  // for each of the red, green, and blue LEDs within the RGB LED.

  if (color <= 255)          // zone 1
  {
    redIntensity = 255 - color;    // red goes from on to off
    greenIntensity = color;        // green goes from off to on
    blueIntensity = 0;             // blue is always off
  }
  else if (color <= 511)     // zone 2
  {
    redIntensity = 0;                     // red is always off
    greenIntensity = 255 - (color - 256); // green on to off
    blueIntensity = (color - 256);        // blue off to on
  }
  else // color >= 512       // zone 3
  {
    redIntensity = (color - 512);         // red off to on
    greenIntensity = 0;                   // green is always off
    blueIntensity = 255 - (color - 512);  // blue on to off
  }

  // Now that the brightness values have been set, command the LED
  // to those values

  analogWrite(RED, redIntensity);
  analogWrite(BLUE, blueIntensity);
  analogWrite(GREEN, greenIntensity);
  ledState = 1;
}

void ledSeries(int dutyCycle){
  if (dutyCycle == 0){
    digitalWrite(WHITE, LOW);
    digitalWrite(BLUE, LOW);
	  digitalWrite(GREEN, LOW);
	  digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    Serial.println("0");
  }
  else if (dutyCycle == 1 || dutyCycle == 2 ){
    digitalWrite(WHITE, HIGH);
    digitalWrite(BLUE, LOW);
	  digitalWrite(GREEN, LOW);
	  digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    Serial.println("1");
  }
  else if (dutyCycle == 3 || dutyCycle == 4 ){
    digitalWrite(WHITE, HIGH);
    digitalWrite(BLUE, HIGH);
	  digitalWrite(GREEN, LOW);
	  digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    Serial.println("3");
  }
  else if (dutyCycle == 5 || dutyCycle == 6 ){
    digitalWrite(WHITE, HIGH);
    digitalWrite(BLUE, HIGH);
	  digitalWrite(GREEN, HIGH);
	  digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    Serial.println("5");
  }
  else if (dutyCycle == 7 || dutyCycle == 8 ){
    digitalWrite(WHITE, HIGH);
    digitalWrite(BLUE, HIGH);
	  digitalWrite(GREEN, HIGH);
	  digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    Serial.println("7");
  }
   else if (dutyCycle == 9 || dutyCycle == 10 ){
    digitalWrite(WHITE, HIGH);
    digitalWrite(BLUE, HIGH);
	  digitalWrite(GREEN, HIGH);
	  digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, HIGH);
    Serial.println("9");
  }
}

void switchLed(){
	if (ledState == 1)
	{
		digitalWrite(RED, LOW);
		digitalWrite(GREEN, LOW);
		digitalWrite(BLUE, LOW);
		ledState = 0;
	}
	else
	{
		digitalWrite(RED, HIGH);
		digitalWrite(GREEN, HIGH);
		digitalWrite(BLUE, HIGH);
		ledState = 1;
	}
}