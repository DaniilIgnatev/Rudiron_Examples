#include "Arduino.h"
#include "rudiron/nrf24.h"
#include "Adafruit_ST7735.h"
#include "FreeSans9pt7b.h"

/// Устанавливаются режимы ввода-вывода для используемых пинов
void setup_pinout()
{
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);
}

Adafruit_ST7735 tft(1, 4, 6);

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

    tft.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display
    tft.invertDisplay(1);

    tft.fillScreen(ST7735_BLACK);
    delay(100);
    tft.fillScreen(ST7735_WHITE);
    delay(100);
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
float voltage;

/// Вторая кнопка нажата
float temperature_raw;

/// Третья кнопка нажата
float temperature;

/// Счетчик пакетов
int counter = false;

/// Буфер обмена
float rx_buffer[3];

void loop()
{
    if (nrf24.available())
    {
        int read = nrf24.readBytes((char *)rx_buffer, sizeof(rx_buffer));
        voltage = rx_buffer[0];
        temperature_raw = rx_buffer[1];
        temperature = rx_buffer[2];

        tft.fillScreen(ST7735_BLACK);
        delay(100);
        
        tft.setCursor(0, 16);
        tft.setFont(&FreeSans9pt7b);
        tft.setTextColor(ST7735_ORANGE);
        tft.print("V = \n");
        tft.println(voltage);
        tft.print("t_raw = \n");
        tft.println(temperature_raw);
        tft.print("t_C = \n");
        tft.println(temperature);
        delay(1000);

        digitalWrite(LED_BUILTIN_1, !digitalRead(LED_BUILTIN_1));
        digitalWrite(LED_BUILTIN_2, !digitalRead(LED_BUILTIN_2));
    }

    delay(50);
}
