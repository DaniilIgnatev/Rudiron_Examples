#include "Arduino.h"
#include "rudiron/adc.h"

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  int temperature = Rudiron::ADC::read_temperature_sensor().value;
  Serial.print("Temperature = ");
  Serial.println(temperature);

  int voltage = Rudiron::ADC::read_internal_reference_voltage_source().value;
  Serial.print("Voltage = ");
  Serial.println(voltage);

  delay(1000);
}
