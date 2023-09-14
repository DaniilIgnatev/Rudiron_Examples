#include "Arduino.h"

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  int sensorValueA0 = analogRead(A0);
  Serial.print("A0 = ");
  Serial.println(sensorValueA0);

  int sensorValueA1 = analogRead(A1);
  Serial.print("A1 = ");
  Serial.println(sensorValueA1);

  int sensorValueA2 = analogRead(A2);
  Serial.print("A2 = ");
  Serial.println(sensorValueA2);

  int sensorValueA3 = analogRead(A3);
  Serial.print("A3 = ");
  Serial.println(sensorValueA3);

  int sensorValueA4 = analogRead(A4);
  Serial.print("A4 = ");
  Serial.println(sensorValueA4);

  int sensorValueA5 = analogRead(A5);
  Serial.print("A5 = ");
  Serial.println(sensorValueA5);

  delay(100);
}
