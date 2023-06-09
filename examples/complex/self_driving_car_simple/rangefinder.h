#ifndef RANGEFINDER_H
#define RANGEFINDER_H

// Задаем номерам портов для подключения ультразвукового датчика
#define HCSR04_TRIG_PIN 31
#define HCSR04_ECHO_PIN 32

void ultrasonic_setup();     // инициализация ультразвукового датчика
float ultrasonic_distance(); // измерение расстояния

#endif