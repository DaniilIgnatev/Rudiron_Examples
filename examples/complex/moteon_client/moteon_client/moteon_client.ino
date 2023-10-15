#include "Arduino.h"
#include "nrf24.h"
#include "rudiron/adc.h"

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

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

    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("");
    delay(100);
}

/// Первая кнопка нажата
bool pressed1 = false;

/// Вторая кнопка нажата
bool pressed2 = false;

/// Третья кнопка нажата
bool pressed3 = false;

/// Счетчик пакетов
int counter = 0;

uint16_t temperature = 0;

const int package_length = 9;

/// Буфер обмена
uint8_t package[package_length];

void loop()
{
    pressed1 = digitalRead(BUTTON_BUILTIN_1);
    pressed2 = digitalRead(BUTTON_BUILTIN_2);
    pressed3 = digitalRead(BUTTON_BUILTIN_3);

    Rudiron::ADCResult temperature_raw = Rudiron::ADC::getADC1().read_temperature_sensor();
    temperature = temperature_raw.value;
    uint8_t *term_pointer = (uint8_t*)&temperature;

    package[0] = pressed1;
    package[1] = pressed2;
    package[2] = pressed3;
    package[4] = *(term_pointer + 0);
    package[5] = *(term_pointer + 1);

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    uint8_t x = (int)(a.acceleration.x * 9);
    uint8_t y = (int)(a.acceleration.y * 9);
    uint8_t z = (int)(a.acceleration.z * 9);

    package[6] = x;
    package[7] = y;
    package[8] = z;

    digitalWrite(LED_BUILTIN_1, pressed2 || pressed1);
    digitalWrite(LED_BUILTIN_2, pressed2 || pressed3);

    if (nrf24.availableForWrite())
    {
        package[3] = counter++;
        nrf24.write(package, package_length);
    }

    delay(1);
}
