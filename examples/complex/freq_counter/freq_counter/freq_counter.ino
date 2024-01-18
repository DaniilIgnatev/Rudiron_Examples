#include "Arduino.h"

#define PERIOD_MS 10 
#define LED_PIN 8

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

bool led_out = false;
unsigned long last_led_ms = 0;
int last_sensor_value = 0;
float last_deriv = 0;
int pulses_counter = 0;
unsigned long last_adc_micros = 0;
unsigned long freq = 0;
unsigned long freq_last_s = 0;

bool sign_changed(float number1, float number2){
    if (number1 > 0 && number2 < 0){
        return true;
    }

    if (number1 < 0 && number2 > 0){
        return true;
    }

    return false;
}

void loop() {
  unsigned long time_us = micros();
  unsigned long time_ms = time_us / 1000;
  unsigned long time_s = time_ms / 1000;

  if ((last_led_ms != time_ms) && (time_ms % (PERIOD_MS / 2) == 0)){
    led_out = !led_out;
    digitalWrite(LED_PIN, led_out);
    last_led_ms = time_ms;
  }

  int sensorValue = analogRead(A0);
  float deriv = (float)(sensorValue - last_sensor_value)/(float)(time_us - last_adc_micros);
  last_adc_micros = time_us;
  last_sensor_value = sensorValue;

  if (sign_changed(last_deriv, deriv)){
    pulses_counter++;
  }
  last_deriv = deriv;

  if (time_s != freq_last_s){
    freq = pulses_counter / 2;
    pulses_counter = 0;
    freq_last_s = time_s;
  }

  delayMicroseconds(150);

  if (time_ms % 1000 == 0){
    Serial.println(freq);
  }
}
