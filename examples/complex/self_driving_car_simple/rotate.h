#ifndef ROTATE_H
#define ROTATE_H

#define SERVO_PIN 5 // даем имя порту 5 по управлению сервоприводом

void rotate_setup();                        // инициализация шима для управления серовприводом
void rotate_angle(int angle, int delay_ms); // установка необходимого угла с временной задержкой

#endif