#ifndef DRIVER_H
#define DRIVER_H

#define DIRECTION_FORWD true //  направление движения вперед
#define DIRECTION_BACK false //  направление движение назад

#define DIR_LEFT 4    // направление вращения левого двигателя
#define SPEED_LEFT 7  // скорость вращения левого двигателя
#define DIR_RIGHT 6   // направление вращения правого двигателя
#define SPEED_RIGHT 8 // скорость вращения левого двигателя

#define LEFT_ROTATION 0  // константа поворота налево
#define RIGHT_ROTATION 1 // константа поворота направо

// функция настройки портов контроллера для управления двигателями
void driver_setup();
// функция начального управления двигателями - установка нулевой скорости и вращения вперед
void driver_init();
// функция установки скорости и направления вращения левого двигателя
void driver_lefr(bool direction, int speed);
// функция установки скорости и направления вращения правого двигателя
void driver_right(bool driection, int speed);

// остановка тележки
void driver_stop(int time_sleep);
// движение вперед
void driver_forward(int speed, int time_sleep);
// поворот тележки
void driver_rotation(int direction, int speed, int time_sleep);

#endif