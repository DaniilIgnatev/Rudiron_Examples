#include "Arduino.h"
#include "rudiron/adc.h"

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  float voltage_raw = Rudiron::ADC::getADC1().read_internal_reference_voltage_source().value;
  float voltage = voltage_raw / (float)4096 * 3.3;
  Serial.print("Reference voltage = ");
  Serial.println(voltage);

  Rudiron::ADCResult temperature_raw = Rudiron::ADC::getADC1().read_temperature_sensor();
  Serial.print("temperature_raw = ");
  Serial.println(temperature_raw.value);

  float temperature = Rudiron::ADC::temperature_raw_to_celsius(temperature_raw);
  Serial.print("Temperature celsius = ");
  Serial.println(temperature);

  delay(1000);
}
