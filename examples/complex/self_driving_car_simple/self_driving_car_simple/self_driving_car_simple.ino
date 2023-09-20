#include "Arduino.h"
#include "driver.h"
#include "rangefinder.h"
#include "rotator.h"

void setup()
{
    rotator_setup(); // настройка управлением ультразвуковым датчиком
    driver_init();   // начальная настройка двигателей

    rotator_setup();   // настройка управления сервоприводом
    driver_stop(3000); // останавливаем тележку
    rotator_angle(90, 2000);
}

float right_distance; // расстояние справо до препятствия
float left_distance;  // расстояние слева до препятствия

void loop()
{
    if (rangefinder_distance() > 20.00) // измеряем расстояние
    {
        driver_forward(70, 2000); // двигаемся прямо
    }
    else
    {
        driver_stop(1000); // если расстояние меньше 20 см, то останавливаемся

        rotator_angle(45, 1300);                 // поворачиваем сервопривод на 45 градусов направо
        right_distance = rangefinder_distance(); // измеряем расстояние справа

        rotator_angle(135, 1300);               // поворачиваем сервопривод на 45 градусов налево
        left_distance = rangefinder_distance(); // измеряем расстояние слева
        rotator_angle(90, 1300);                // устанавливаем сервопривод в положение прямо

        if (right_distance > 20.00 && right_distance > left_distance)
        {
            driver_rotation(1, 70, 1300); // поворачиваем тележку направо
        }

        if (left_distance > 20.00 && left_distance > right_distance)
        {
            driver_rotation(0, 70, 1300); // поворачиваем тележку налево
        }
    }
}
