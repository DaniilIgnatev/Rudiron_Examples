#include "Arduino.h"
#include "nrf24.h"

/// Устанавливаются режимы ввода-вывода для используемых пинов
void setup_pinout()
{
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);
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
    Serial.println("РУДИРОН Бутерброд!");
}

void setup()
{
    setup_pinout();
    wellcome();

    if (!nrf24.begin(true))
    {
        Serial.println("Nrf24 Error!");
        return;
    }
}

/// Первая кнопка нажата
bool pressed1 = false;

/// Вторая кнопка нажата
bool pressed2 = false;

/// Третья кнопка нажата
bool pressed3 = false;

/// Счетчик пакетов
int counter = false;

/// Буфер обмена
uint8_t rx_buffer[4];

void loop()
{
    if (nrf24.available())
    {
        int read = nrf24.readBytes(rx_buffer, 4);
        pressed1 = rx_buffer[0];
        pressed2 = rx_buffer[1];
        pressed3 = rx_buffer[2];
        counter = rx_buffer[3];

        Serial.printf("%d, %d, %d, %d\n",pressed1, pressed2, pressed3, counter);

        digitalWrite(LED_BUILTIN_1, pressed2 || pressed1);
        digitalWrite(LED_BUILTIN_2, pressed2 || pressed3);
    }

    delay(50);
}
