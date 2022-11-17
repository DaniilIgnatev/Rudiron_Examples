#include "Arduino.h"


void setup()
{
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    digitalWrite(LED_BUILTIN_1, OUTPUT);
    digitalWrite(LED_BUILTIN_2, OUTPUT);

    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");
}

bool pressed = false;

void loop()
{
    if (Serial.available()){
        String s = Serial.readString();
        Serial.println(s);
    }

    pressed = false;

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

    delay(100);
}
