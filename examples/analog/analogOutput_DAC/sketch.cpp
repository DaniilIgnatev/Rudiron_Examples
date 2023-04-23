#include "Arduino.h"
#include "rudiron/dac.h"

void setup()
{
    Serial.begin(115200);
}

int counter = 0;
int value;

void loop()
{
    value = counter * 128;
    Serial.printf("Counter = %d, value = %d\n", counter, value);

    Rudiron::DAC::getDAC2().write_pin_single(Rudiron::PORT_PIN_E0, value);

    delay(1);
    counter += 1;
}
