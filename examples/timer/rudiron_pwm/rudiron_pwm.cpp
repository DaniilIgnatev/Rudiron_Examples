#include "Arduino.h"
#include "rudiron/timer.h"

#define PWM_PIN 8

using namespace Rudiron;

void setup()
{
    // конфигурация последовательного порта
    Serial.begin(115200);
    // отправка приветствия через последовательный порт
    Serial.println("Рудирон Бутерброд!");
}

void loop()
{
    // запуск ШИМ с частотой 500 Гц с заполнением 1% (1/100) на одну секунду на выводе PWM_PIN
    pwm(PWM_PIN, 490, 1, 100);
    // пауза программы на 1 секунду
    delay(1000);
    // остановка шим на выводе PWM_PIN
    pinMode(PWM_PIN, OUTPUT);
}
