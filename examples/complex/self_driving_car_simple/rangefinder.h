#ifndef RANGEFINDER_H
#define RANGEFINDER_H

// Задаем номерам портов для подключения ультразвукового датчика
#define HCSR04_TRIG_PIN 31
#define HCSR04_ECHO_PIN 32

// инициализация ультразвукового датчика
void rangefinder_setup();     
 // измерение расстояния
float rangefinder_distance();

#endif