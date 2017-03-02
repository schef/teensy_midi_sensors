/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 11;      // select the pin for the LED
  // Pin 13: Arduino has an LED connected on pin 13
  // Pin 11: Teensy 2.0 has the LED on pin 11
  // Pin  6: Teensy++ 2.0 has the LED on pin 6
  // Pin 13: Teensy 3.0 has the LED on pin 13
int sensorValue = 0;  // variable to store the value coming from the sensor

#define INMIN 75
#define INMAX 400
#define OUTMIN 0
#define OUTMAX 127

#define VALUE_DIFERENCE 2
#define NUMBER_OF_SAMPLES 10

void setup() {
  // declare the ledPin as an OUTPUT:
  //Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  
}

int resultArray[NUMBER_OF_SAMPLES] = {0};
int result = 0;
int oldResult = 0;

float linearMapTo(float value, float inMin, float inMax, float outMin, float outMax)
{
  if (value > inMax) value = inMax;
  if (value < inMin) value = inMin;
  float result = (((value - inMin) * (outMax - outMin)) / (inMax - inMin)) + outMin;
  if (result > outMax) result = outMax;
  if (result < outMin) result = outMin;
  //Serial.printf("liner: value: %d, result %d\r\n", (int)value, (int)result);
  return(result);
}

int calcAverage(int *buffer)
{
  int result = 0;
  for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
  {
    result += buffer[i];
  }
  result = result / NUMBER_OF_SAMPLES;
  return(result);
}

void loop() {
  // read the value from the sensor:
  for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
  {
    sensorValue = analogRead(sensorPin);
    int tempResult = linearMapTo(sensorValue, INMIN, INMAX, OUTMIN, OUTMAX);
    //Serial.printf("value: %d, result %d\r\n", sensorValue, tempResult);
    resultArray[i] = 127 - tempResult;
    
  }
  result = calcAverage(resultArray);
  if ((result != oldResult) && (abs(result-oldResult) > VALUE_DIFERENCE))
  {
    //Serial.printf("value: %d\n", result);
    usbMIDI.sendControlChange(11, result, 1);
    oldResult = result;
             
  }
}
