#include "Arduino.h"
#include "drive.h"
#include "ultrasonic.h"
#include "servo_telegka.h"

void setup()
{

    ultrasonic_setup(); // настройка управлением ультразвуковым датчиком
    drive_init();       // начальная настройке двигателей

    servo_setup();      // настройка управления сервоприводом
    stop_telegka(3000); // останавливаем тележку
    servo_angle(90, 2000);
}

float right_distance; // расстояние справо от препятствия
float left_distance;  // расстояние слева от препятствия

void loop()
{
    if (ultrasonic_distance() > 20.00) // измеряем расстояние и определяем расстояние
    {
        forward_telegka(70, 2000); // двигаемся прямо
    }
    else
    {
        stop_telegka(1000); // если расстояние меньше 20 см то останавливаемся

        servo_angle(45, 1300);                  // поворачиваем на 45 вправо сервопривод
        right_distance = ultrasonic_distance(); // измеряем расстояние справа

        servo_angle(135, 1300);                 // поворот влево на 45 сервопривод
        left_distance = ultrasonic_distance();  // измеряем расстояние слева
        servo_angle(90, 1300);                  // устанавливаем сервопривод в нулевое положение

        if (right_distance > 20.00 && right_distance > left_distance)
        {
            rotation_telegka(1, 70, 1300); // поворачиваем тележку вправо
        }

        if (left_distance > 20.00 && left_distance > right_distance)
        {
            rotation_telegka(0, 70, 1300); // поворачиваем тележку влево
        }
    }
}
