#include "Arduino.h"
#include "drive.h"

void drive_setup() // функция настройки портов контроллера для управления двигателями
{
        pinMode(DIR_LEFT, OUTPUT);
        pinMode(SPEED_LEFT, OUTPUT);
        pinMode(DIR_RIGHT, OUTPUT);
        pinMode(SPEED_RIGHT, OUTPUT);
}

void drive_init() // функция начального управления двигателями - установка нулевой скорости и вращения вперед
{
        drive_setup();
        drive_lefr(DIRECTION_FORWD, 0);  // устанавливаем направление вращения - вперед
        drive_right(DIRECTION_FORWD, 0); // устанавливаем нулевую скорость вращения двигателя
}

// функция установки скорости и направления вращения левого двигателя
void drive_lefr(bool direction, int speed)
{
        digitalWrite(DIR_LEFT, direction); // задаем направление вращения двигателя
        analogWrite(SPEED_LEFT, speed);    // задаем  скорость вращения в виде сигнала шим
}

// функция установки скорости и направления вращения правого двигателя
void drive_right(bool directiont, int speed)
{
        digitalWrite(DIR_RIGHT, directiont); // задаем направление вращения двигателя
        analogWrite(SPEED_RIGHT, speed);     // задаем  скорость вращения в виде сигнала шим
}

void drive_stop(int time_sleep) // остановка тележки
{
        drive_lefr(DIRECTION_FORWD, 0);  // выключаем левый двигатель
        drive_right(DIRECTION_FORWD, 0); // выключаем правый двигатель
        delay(time_sleep);
}

void drive_forward(int speed, int time_sleep) // движение вперед
{
        drive_lefr(DIRECTION_FORWD, speed);  // включаем левый двигатель для движения вперед
        drive_right(DIRECTION_FORWD, speed); // включаем правый двигатель для движения вперед
        delay(time_sleep);
}

void drive_rotation(int direction, int speed, int time_sleep) // поворот тележки
{
        if (direction == 1)
        {
                drive_lefr(DIRECTION_FORWD, speed); // включаем левый двигатель для движения вперед
                drive_right(DIRECTION_FORWD, 0);    // выключаем правый двигатель
                delay(time_sleep);
        }
        else
        {
                drive_lefr(DIRECTION_FORWD, 0);      // выключаем левый двигатель
                drive_right(DIRECTION_FORWD, speed); // включаем правый двигатель для движения вперед
                delay(time_sleep);
        }
}
