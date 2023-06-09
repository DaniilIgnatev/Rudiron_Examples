#include "Arduino.h"
#include "rangefinder.h"

void rangefinder_setup()
{
        // Устанавливаем режим работы портов TRIG на выход и Echo на вход
        pinMode(HCSR04_TRIG_PIN, OUTPUT);
        pinMode(HCSR04_ECHO_PIN, INPUT);
}

float rangefinder_distance() // измеряем расстояние и переводим его в сантиметры
{
        pinMode(HCSR04_TRIG_PIN, OUTPUT);
        pinMode(HCSR04_ECHO_PIN, INPUT);

        // Генерируем импульс 10 мкс на выходе TRIG
        digitalWrite(HCSR04_TRIG_PIN, LOW); // выход в 0
        delayMicroseconds(5);               // пауза на 5 мкс

        digitalWrite(HCSR04_TRIG_PIN, HIGH); // подача единицы на выход TRIG
        delayMicroseconds(10);               // Пауза 10 мкс В этот момент датчик будет посылать сигналы с частотой 40 КГц.

        digitalWrite(HCSR04_TRIG_PIN, LOW); // Выход TRIG в ноль импульс в 10 мкс завершен

        // Время задержки акустического сигнала на эхолокаторе.
        int duration = pulseIn(HCSR04_ECHO_PIN, HIGH); // считываем длительность импульса

        // Расстояние в сантиметрах
        float distance = (duration / 2) / 29.1; // рассчитываем расстояние в сантиметрах

        return distance;
}
