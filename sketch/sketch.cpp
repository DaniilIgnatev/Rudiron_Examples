#include "Arduino.h"
#include "rudiron/gpio.h"
#include "rudiron/timer.h"

using namespace Rudiron;

Timer* timer_led1 = nullptr;

Timer* timer_led2 = nullptr;


void setup()
{
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    timer_led1 = Timer::getTimerForPinName(GPIO::pinMap[LED_BUILTIN_1]);
    timer_led1->start();
    timer_led1->PWM_setup();

    timer_led2 = Timer::getTimerForPinName(GPIO::pinMap[LED_BUILTIN_2]);
    timer_led2->start();
    timer_led2->PWM_setup();

    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");
}

uint8_t percentage = 0;
bool negative = false;
bool pressed = false;

void loop()
{
    if (Serial.available()){
        String s = Serial.readString();
        Serial.println(s);
    }

    if (!pressed){
        if (negative){
            percentage -= 1;
        }
        else{
            percentage += 1;
        }

        if (percentage >= 100){
            negative = true;
        }
        else if (percentage <= 0){
            negative = false;
        }

        timer_led1->PWM_start(GPIO::pinMap[LED_BUILTIN_1], percentage);
        timer_led2->PWM_start(GPIO::pinMap[LED_BUILTIN_2], percentage);
    }


    if (digitalRead(BUTTON_BUILTIN_1)){
        pressed = true;
        pinMode(LED_BUILTIN_1, OUTPUT);
        digitalWrite(LED_BUILTIN_1, true);
        pinMode(LED_BUILTIN_2, OUTPUT);
        digitalWrite(LED_BUILTIN_2, false);
    }
    if (digitalRead(BUTTON_BUILTIN_2)){
        pressed = true;
        pinMode(LED_BUILTIN_1, OUTPUT);
        digitalWrite(LED_BUILTIN_1, false);
        pinMode(LED_BUILTIN_2, OUTPUT);
        digitalWrite(LED_BUILTIN_2, false);
    }
    if (digitalRead(BUTTON_BUILTIN_3)){
        pressed = true;
        pinMode(LED_BUILTIN_1, OUTPUT);
        digitalWrite(LED_BUILTIN_1, false);
        pinMode(LED_BUILTIN_2, OUTPUT);
        digitalWrite(LED_BUILTIN_2, true);
    }

    delay(10);
}
