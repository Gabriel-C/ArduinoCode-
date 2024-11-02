const int buttonPin = 8;  // the number of the pushbutton pin
// Which pin on the Arduino is connected to the NeoPixels?

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize the pushbutton pin as an output:
  pinMode(buttonPin, OUTPUT);
}

void loop() 
{
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) 
  {
    // turn LED on:
    digitalWrite(LED_BUILTIN, HIGH);
  } 
  else 
  {
    // turn LED off:
    digitalWrite(LED_BUILTIN, LOW);
  }
}
