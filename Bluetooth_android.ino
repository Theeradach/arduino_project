#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
 
int led = 13;


// Pins used for inputs and outputs********************************************************
const int analogInPin0 = A0;// Analog input pins
const int analogInPin1 = A1;
const int analogInPin2 = A2;
const int analogInPin3 = A3;
 
//Arrays for the 4 inputs**********************************************
float sensorValue[4] = {0,0,0,0};
float voltageValue[4] = {0,0,0,0};
 
//Char used for reading in Serial characters
char inbyte = 0;
//*******************************************************************************************
 
void setup() {
  // initialise serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  ads.begin();
}
 
void loop() {
  

  // read the analog in value to the sensor array
  sensorValue[0] = analogRead(analogInPin0);
  sensorValue[1] = analogRead(analogInPin1);
  sensorValue[2] = analogRead(analogInPin2);
  sensorValue[3] = analogRead(analogInPin3);

  for (int x = 0; x < 4; x++)
  {
    voltageValue[x] = sensorValue[x];
  }
  sendAndroidValues();
  
  //when serial values have been received this will be true
  if (Serial.available() > 0)
  {
    inbyte = Serial.read();
    if (inbyte == '0')
    {
      //LED off
      digitalWrite(led, LOW);
    }
    if (inbyte == '1')
    {
      //LED on
      digitalWrite(led, HIGH);
    }
  }
  //delay by 2s. Meaning we will be sent values every 2s approx
  //also means that it can take up to 2 seconds to change LED state
  delay(2000);
}
 

//sends the values from the sensor over serial to BT module
void sendAndroidValues()
 {
  //puts # before the values so our app knows what to do with the data
  Serial.print('#');
  //for loop cycles through 4 sensors and sends values via serial
  for(int k=0; k<4; k++) 
  {
    Serial.print(voltageValue[k]);
    Serial.print('+');
    //technically not needed but I prefer to break up data values
    //so they are easier to see when debugging
  }
 Serial.print('~'); //used as an end of transmission character - used in app for string length
 Serial.println();
 delay(10);        //added a delay to eliminate missed transmissions
}
 
 

