#include "Arduino.h"
#include "rudiron/nrf24.h"

/// Устанавливаются режимы ввода-вывода для используемых пинов
void setup_pinout()
{
    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

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

/// Режим приемника/передатчика
bool isReceiver = true;

void initRadio(bool becomeReceiver)
{
    isReceiver = becomeReceiver;
    Serial.printf("Инициализируем модуль как %s\n", isReceiver ? "Приемник" : "Передатчик");

    if (!nrf24.begin(isReceiver))
    {
        Serial.println("Nrf24 Error!");
        return;
    }
}

void setup()
{
    setup_pinout();
    wellcome();

    initRadio(true);
}

/// Первая кнопка нажата
bool pressed1 = false;

/// Предыдущее значение второй кнопки
bool last_pressed2 = false;

/// Вторая кнопка нажата
bool pressed2 = false;

/// Третья кнопка нажата
bool pressed3 = false;

/// Счетчик пакетов
int counter = 0;

/// Буфер приемника
uint8_t rx_buffer[4];

/// Буфер передатчика
uint8_t tx_buffer[4];

void loop()
{
    pressed1 = digitalRead(BUTTON_BUILTIN_1);
    pressed2 = digitalRead(BUTTON_BUILTIN_2);
    pressed3 = digitalRead(BUTTON_BUILTIN_3);

    if (pressed2 && !last_pressed2)
    {
        initRadio(!isReceiver);
    }
    last_pressed2 = pressed2;

    if (nrf24.availableForWrite())
    {
        tx_buffer[0] = pressed1;
        tx_buffer[1] = pressed2;
        tx_buffer[2] = pressed3;

        digitalWrite(LED_BUILTIN_1, pressed2 || pressed1);
        digitalWrite(LED_BUILTIN_2, pressed2 || pressed3);

        tx_buffer[3] = counter++;
        nrf24.write(tx_buffer, 4);
    }

    if (nrf24.available())
    {
        int read = nrf24.readBytes(rx_buffer, 4);
        pressed1 = rx_buffer[0];
        pressed2 = rx_buffer[1];
        pressed3 = rx_buffer[2];
        counter = rx_buffer[3];

        Serial.printf("%d, %d, %d, %d\n", pressed1, pressed2, pressed3, counter);

        digitalWrite(LED_BUILTIN_1, pressed2 || pressed1);
        digitalWrite(LED_BUILTIN_2, pressed2 || pressed3);
    }

    delay(isReceiver ? 50 : 100);
}
