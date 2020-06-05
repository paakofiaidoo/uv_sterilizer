/*
 this app allow you to create an automatic door sterilizer which also seves as a porable
 surace sterilizer for surfaces 
 */


const int uvPin = D6;         // for uv leds
const int ledPin = D5;        // FOR LED PIN
const int sensor = D7;        // for obstacle sensor pin
const int buttonPin = D8;     //for push button pin


int uvState = LOW;        //uv led state
int ledState = 0;         // current state of the led

int sensorData = 1;          // obstacle sensor data 
int lastSensorState = 1;     // previous state of the obstacle sensor

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 10000;           // interval at which to uv to be turn on (milliseconds)
const long intervalLed = 10000;        // interval at which led to be turn on(milliseconds )



void setup() {
  pinMode(sensor, INPUT);
  pinMode(uvPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;  
    } 
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


  //the device has three basic functions thus the 3 state 
  //this first state is use for the portabilty feature which turns on the uv light only
  if (buttonPushCounter % 3 == 0) {
    uvState = HIGH;
    ledState =LOW;
    digitalWrite(ledPin,ledState);
    digitalWrite(uvPin,uvState);
  } 
  // this second state is used to turn of the device
  if (buttonPushCounter % 3 == 1) {
    uvState = LOW;
    ledState =LOW;
    digitalWrite(ledPin,ledState);
    digitalWrite(uvPin,uvState);
  }
  //this state is use to run the door strilizer function
  if (buttonPushCounter % 3 == 2) {

    sensorData = digitalRead(sensor);
    Serial.println (sensorData);
    unsigned long currentMillis = millis();

    //upon detection of obstacle (hand) let the let turn on telling user that the hand has been detected
    if (sensorData != lastSensorState) {
      Serial.println("door has been used");
      ledState = HIGH;
      uvState = LOW;
      digitalWrite(ledPin,ledState);
      digitalWrite(uvPin,uvState);
      previousMillis = currentMillis;
  
    }

  lastSensorState = sensorData;
  
  // turn off the uv after set time interval has passed
  if (currentMillis - previousMillis >= interval && uvState == HIGH ) {
      uvState = LOW;   
      digitalWrite(uvPin, uvState);
      previousMillis = currentMillis;
    }

  //turn off led after set time inerval has passed while turning on uv 
  if (currentMillis - previousMillis >= intervalLed && ledState == HIGH )
    {
      uvState = HIGH;
      ledState =LOW;
      digitalWrite(ledPin,ledState);
      digitalWrite(uvPin,uvState);
      previousMillis = currentMillis;
    }
  }
}
