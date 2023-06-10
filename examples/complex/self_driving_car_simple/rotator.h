#ifndef ROTATOR_H
#define ROTATOR_H

#define SERVO_PIN 5 // задаем имя по управлению сервоприводом 5-му порту

// инициализация шима для управления серовприводом
void rotator_setup();
// установка необходимого угла с временной задержкой
void rotator_angle(int angle, int delay_ms);

#endif