#include "Arduino.h"
#include "rudiron/timer.h"

#define PWM_PIN 8

void setup()
{
    // конфигурация последовательного порта
    Serial.begin(115200);
    // отправка приветствия через последовательный порт
    Serial.println("Рудирон Бутерброд!");
}

void loop()
{
    // Ссылка на первй таймер
    Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_name(Rudiron::Timer_1);
    // Настройка таймера на частоту 1500 Гц
    timer.setup(1500);
    // запуск ШИМ сзаполнением 50% на одну секунду на выводе PWM_PIN
    timer.PWM_start(Rudiron::GPIO::get_rudiron_gpio(PWM_PIN), Rudiron::Timer::PPM_MEAN);
    // пауза программы на 1 секунду
    delay(1000);
    // остановка шим на выводе PWM_PIN
    timer.PWM_stop(Rudiron::GPIO::get_rudiron_gpio(PWM_PIN));
}
