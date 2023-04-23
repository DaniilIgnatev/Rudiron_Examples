#include "Arduino.h"
#include "rudiron/timer.h"

uint16_t PWM_LOW;

uint16_t PWM_HIGH;

const uint8_t pin = 8;

Rudiron::PortPinName pinName;

const uint16_t BUF_DMA_LENGTH = 5;

uint16_t BUF_DMA_FIRST[BUF_DMA_LENGTH];

void DMA_Interrupt_Handler(Rudiron::Timer &timer)
{
    // Асинхронный вариант. Увеличивает производительность, предпочтительнее по умолчанию
    if (timer.PWM_DMA_done())
    {
        timer.disable(); // без остановки таймера, шим продолжает работу
    }

    // Синхронный вариант. Блокирует выполнение основного потока, но более стабилен на высоких частотах
    // timer.PWM_DMA_wait_done(); // без ожидания происходит рассинхронизация
    // timer.disable();           // без остановки таймера выходной сигнал выдается DMA больше положеного
}

const uint16_t frequency_step = 1000;
const uint16_t frequency_max = 10000;
uint16_t frequency = 5000;

void setup()
{
    // конфигурация встроенных кнопок
    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    // конфигурация встроенных светодиодов
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    // конфигурация последовательного порта
    Serial.begin(115200);
    // отправка приветствия через последовательный порт
    Serial.println("Рудирон Бутерброд!");

    pinName = Rudiron::GPIO::get_rudiron_gpio(pin);
}

bool pressed1 = false;

bool pressed2 = false;

bool pressed3 = false;

void loop()
{
    // чтение встроенных кнопок, true = есть нажатие
    bool pressed1_new = digitalRead(BUTTON_BUILTIN_1);
    if (!pressed1 && pressed1_new)
    {
        if (frequency <= frequency_step)
        {
            frequency = 0;
        }
        else
        {
            frequency -= frequency_step;
        }
    }
    pressed1 = pressed1_new;

    bool pressed2_new = digitalRead(BUTTON_BUILTIN_2);
    if (!pressed2 && pressed2_new)
    {
        Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_pinName(pinName);
        timer.setup(frequency);

        uint16_t ARR = timer.get_ARR();
        PWM_LOW = ARR / 3 - 1;
        PWM_HIGH = ARR - PWM_LOW - 1;
        BUF_DMA_FIRST[0] = PWM_LOW;
        BUF_DMA_FIRST[1] = PWM_HIGH;
        BUF_DMA_FIRST[2] = PWM_LOW;
        BUF_DMA_FIRST[3] = PWM_HIGH;
        BUF_DMA_FIRST[4] = 0;

        timer.PWM_DMA_setup(pinName, BUF_DMA_FIRST, BUF_DMA_LENGTH, &DMA_Interrupt_Handler);
    }
    pressed2 = pressed2_new;

    bool pressed3_new = digitalRead(BUTTON_BUILTIN_3);
    if (!pressed3 && pressed3_new)
    {
        if (frequency_max - frequency_step <= frequency)
        {
            frequency = frequency_max;
        }
        else
        {
            frequency += frequency_step;
        }
    }
    pressed3 = pressed3_new;

    delay(10);
}
