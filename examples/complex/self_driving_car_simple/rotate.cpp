#include "Arduino.h"
#include "Servo.h" // подключаем дополнительную программу по управлению сервоприводом
#include "rotate.h"

Servo servo; // создаем переменную типа сервопривод

void rotate_setup()
{
    pinMode(SERVO_PIN, OUTPUT);
    servo.attach(SERVO_PIN, 450, 2500); // устанавливаем параметры сервы порт, и значения мин и макс заполнения длительности импульсов ШИМ

    servo.write(0);
    delay(1000);

    servo.write(45);
    delay(1000);

    servo.write(90);
    delay(1000);

    servo.write(180);
    delay(1000);

    servo.write(0);
    delay(1000);
}

void rotate_angle(int angle, int delay_ms)
{
    pinMode(SERVO_PIN, OUTPUT);
    servo.attach(SERVO_PIN, 450, 2500);
    
    servo.write(angle); // устанавливаем необходимый градус на сервоприводе
    delay(delay_ms);
}