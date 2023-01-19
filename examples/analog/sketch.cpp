/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/

#include "Arduino.h"

// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop()
{
    // int sensorValueA0 = analogRead(A0);
    int sensorValueA1 = analogRead(A1);
    int sensorValueA2 = analogRead(A2);
    int sensorValueA3 = analogRead(A3);
    // int sensorValueA4 = analogRead(A4);
    // int sensorValueA5 = analogRead(A5);
    Serial.printf("A0: %d, A1: %d, A2: %d, A3: %d, A4: %d, A5: %d\n", 0, sensorValueA1, sensorValueA2, sensorValueA3, 0, 0);
    delay(100);
}
