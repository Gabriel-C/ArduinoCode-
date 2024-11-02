# 1 "\\\\192.168.50.234\\work\\Coding\\Arduino\\RotaryEncoderRgbLed\\RotaryEncoderRgbLed.ino"
// Rotary Encoder Inputs
// Name: RotaryEncoderRgbLed







int val;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
unsigned int ledState = 0;

void setup() {

 // Set encoder pins as inputs
 pinMode(2,0x0);
 pinMode(3,0x0);
 pinMode(4, 0x2);
 pinMode(5, 0x1);
 pinMode(6, 0x1);
 pinMode(7, 0x1);

 // Setup Serial Monitor
 Serial.begin(9600);

 // Read the initial state of CLK
 lastStateCLK = digitalRead(2);
}

void loop() {

 // Read the current state of CLK
 currentStateCLK = digitalRead(2);

 // If last and current state of CLK are different, then pulse occurred
 // React to only 1 state change to avoid double count
 if (currentStateCLK != lastStateCLK && currentStateCLK == 1){

  // If the DT state is different than the CLK state then
  // the encoder is rotating CCW so decrement
  if (digitalRead(3) != currentStateCLK) {
   counter --;
   currentDir ="CCW";
  } else {
   // Encoder is rotating CW so increment
   counter ++;
   currentDir ="CW";
  }

  Serial.print("Direction: ");
  Serial.print(currentDir);
  Serial.print(" | Counter: ");
  Serial.print(counter);
  Serial.print(" | Color: ");
        val = map(counter, -99, 99, 0, 767);
  Serial.println(val);
  showRGB(val);
 }

 // Remember last CLK state
 lastStateCLK = currentStateCLK;

 // Read the button state
 int btnState = digitalRead(4);

 //If we detect LOW signal, button is pressed
 if (btnState == 0x0) {
  //if 50ms have passed since last LOW pulse, it means that the
  //button has been pressed, released and pressed again
  if (millis() - lastButtonPress > 50) {
   Serial.println("Button pressed!");
   switchLed();
  }

  // Remember last button press event
  lastButtonPress = millis();
 }

 // Put in a slight delay to help debounce the reading
 delay(1);
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

  if (color <= 255) // zone 1
  {
    redIntensity = 255 - color; // red goes from on to off
    greenIntensity = color; // green goes from off to on
    blueIntensity = 0; // blue is always off
  }
  else if (color <= 511) // zone 2
  {
    redIntensity = 0; // red is always off
    greenIntensity = 255 - (color - 256); // green on to off
    blueIntensity = (color - 256); // blue off to on
  }
  else // color >= 512       // zone 3
  {
    redIntensity = (color - 512); // red off to on
    greenIntensity = 0; // green is always off
    blueIntensity = 255 - (color - 512); // blue on to off
  }

  // Now that the brightness values have been set, command the LED
  // to those values

  analogWrite(7, redIntensity);
  analogWrite(5, blueIntensity);
  analogWrite(6, greenIntensity);
  ledState = 1;
}

void switchLed(){
 if (ledState == 1)
 {
  digitalWrite(7, 0x0);
  digitalWrite(6, 0x0);
  digitalWrite(5, 0x0);
  ledState = 0;
 }
 else
 {
  digitalWrite(7, 0x1);
  digitalWrite(6, 0x1);
  digitalWrite(5, 0x1);
  ledState = 1;
 }
}
