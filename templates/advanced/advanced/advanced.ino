#include "Arduino.h"
#include "rudiron/tasks_timer.h"

void setup_pinout();
void setup_tasks();
void wellcome();

/// Первая кнопка нажата
bool pressed1 = false;

/// Вторая кнопка нажата
bool pressed2 = false;

/// Третья кнопка нажата
bool pressed3 = false;

/// Задача моргания первым светодиодом
bool task_led1(void *)
{
    digitalWrite(LED_BUILTIN_1, !digitalRead(LED_BUILTIN_1));
    return true;
}

/// Задача моргания вторым светодиодом
bool task_led2(void *)
{
    digitalWrite(LED_BUILTIN_2, !digitalRead(LED_BUILTIN_2));
    return true;
}

void setup()
{
    setup_pinout();
    setup_tasks();
    wellcome();
}

/// @brief Обработчик нажатия первой кнопки
void BUTTON_1_callback()
{
    Serial.println("Нажата кнопка 1");
}

/// @brief Обработчик нажатия второй кнопки
void BUTTON_2_callback()
{
    Serial.println("Нажата кнопка 2");
}

/// @brief Обработчик нажатия третьей кнопки
void BUTTON_3_callback()
{
    Serial.println("Нажата кнопка 3");
}

/// Устанавливаются режимы ввода-вывода для используемых пинов
void setup_pinout()
{
    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUTTON_BUILTIN_1), BUTTON_1_callback);

    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUTTON_BUILTIN_2), BUTTON_2_callback);

    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUTTON_BUILTIN_3), BUTTON_3_callback);

    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);
}

/// Настройка расписания задач
void setup_tasks()
{
    tasksTimer.start_every_millis(1000, task_led1);
    tasksTimer.start_every_millis(2000, task_led2);
}

/// Приветствие пользователя
void wellcome()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED_BUILTIN_1, true);
        digitalWrite(LED_BUILTIN_2, true);
        delay(100);
        digitalWrite(LED_BUILTIN_1, false);
        digitalWrite(LED_BUILTIN_2, false);
        delay(100);
    }

    Serial.begin(115200);
    Serial.println("Рудирон Бутерброд!");

    Serial1.begin(115200);
    Serial1.println("Рудирон Бутерброд!");
}

void loop()
{
}