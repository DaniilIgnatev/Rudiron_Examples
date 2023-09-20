#include "Arduino.h"
#include "driver.h"

// функция настройки портов контроллера для управления двигателями
void driver_setup()
{
        pinMode(DIR_LEFT, OUTPUT);
        pinMode(SPEED_LEFT, OUTPUT);
        pinMode(DIR_RIGHT, OUTPUT);
        pinMode(SPEED_RIGHT, OUTPUT);
}

// функция начального управления двигателями - установка нулевой скорости и вращения вперед
void driver_init()
{
        driver_setup();
        driver_lefr(DIRECTION_FORWD, 0);  // устанавливаем направление вращения - вперед
        driver_right(DIRECTION_FORWD, 0); // устанавливаем нулевую скорость вращения двигателя
}

// функция установки скорости и направления вращения левого двигателя
void driver_lefr(bool direction, int speed)
{
        digitalWrite(DIR_LEFT, direction); // задаем направление вращения двигателя
        analogWrite(SPEED_LEFT, speed);    // задаем  скорость вращения в виде сигнала шим
}

// функция установки скорости и направления вращения правого двигателя
void driver_right(bool directiont, int speed)
{
        digitalWrite(DIR_RIGHT, directiont); // задаем направление вращения двигателя
        analogWrite(SPEED_RIGHT, speed);     // задаем  скорость вращения в виде сигнала шим
}

// остановка тележки
void driver_stop(int time_sleep)
{
        driver_lefr(DIRECTION_FORWD, 0);  // выключаем левый двигатель
        driver_right(DIRECTION_FORWD, 0); // выключаем правый двигатель
        delay(time_sleep);
}

// движение вперед
void driver_forward(int speed, int time_sleep)
{
        driver_lefr(DIRECTION_FORWD, speed);  // включаем левый двигатель для движения вперед
        driver_right(DIRECTION_FORWD, speed); // включаем правый двигатель для движения вперед
        delay(time_sleep);
}

// поворот тележки
void driver_rotation(int direction, int speed, int time_sleep)
{
        if (direction == 1)
        {
                driver_lefr(DIRECTION_FORWD, speed); // включаем левый двигатель для движения вперед
                driver_right(DIRECTION_FORWD, 0);    // выключаем правый двигатель
                delay(time_sleep);
        }
        else
        {
                driver_lefr(DIRECTION_FORWD, 0);      // выключаем левый двигатель
                driver_right(DIRECTION_FORWD, speed); // включаем правый двигатель для движения вперед
                delay(time_sleep);
        }
}
