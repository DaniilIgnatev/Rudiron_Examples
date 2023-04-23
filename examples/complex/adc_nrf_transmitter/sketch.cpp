#include "Arduino.h"
#include "rudiron/nrf24.h"
#include "rudiron/adc.h"

/// Устанавливаются режимы ввода-вывода для используемых пинов
void setup_pinout()
{
    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);
}

/// Приветствие пользователя
void wellcome()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED_BUILTIN_1, true);
        digitalWrite(LED_BUILTIN_2, true);
        delay(100);
        digitalWrite(LED_BUILTIN_1, false);
        digitalWrite(LED_BUILTIN_2, false);
        delay(100);
    }

    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");
}

void setup()
{
    setup_pinout();
    wellcome();

    if (!nrf24.begin(false))
    {
        Serial.println("Nrf24 Error!");
        return;
    }
}

/// Первая кнопка нажата
bool pressed1 = false;

/// Вторая кнопка нажата
bool pressed2 = false;

/// Третья кнопка нажата
bool pressed3 = false;

/// Счетчик пакетов
int counter = 0;

/// Буфер обмена
float package[3];

void loop()
{
    pressed1 = digitalRead(BUTTON_BUILTIN_1);
    pressed2 = digitalRead(BUTTON_BUILTIN_2);
    pressed3 = digitalRead(BUTTON_BUILTIN_3);

    float voltage_raw = Rudiron::ADC::getADC1().read_internal_reference_voltage_source().value;
    float voltage = voltage_raw / (float)4096 * 3.3;
    Serial.print("Voltage = ");
    Serial.println(voltage);

    Rudiron::ADCResult temperature_raw = Rudiron::ADC::getADC1().read_temperature_sensor();
    Serial.print("temperature_raw = ");
    Serial.println(temperature_raw.value);

    float temperature = Rudiron::ADC::temperature_raw_to_celsius(temperature_raw);
    Serial.print("Temperature = ");
    Serial.println(temperature);

    package[0] = voltage;
    package[1] = temperature_raw.value;
    package[2] = temperature;

    digitalWrite(LED_BUILTIN_1, pressed2 || pressed1);
    digitalWrite(LED_BUILTIN_2, pressed2 || pressed3);

    if (nrf24.availableForWrite())
    {
        nrf24.write((char*)package, sizeof(package));
    }

    delay(100);
}
