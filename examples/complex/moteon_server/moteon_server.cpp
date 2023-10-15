#include "Arduino.h"
#include "Adafruit_ST7735.h"
#include "fonts/FreeSans9pt7b.h"
#include "nrf24.h"
#include "rudiron/adc.h"

using namespace Rudiron;

Adafruit_ST7735 tft(1, 4, 6);

uint16_t background_color = ST7735_BLACK;
uint16_t red_color = ST7735_RED;
uint16_t green_color = ST7735_GREEN;
uint16_t blue_color = ST7735_BLUE;

int font_width = 14;

void setup_pinout()
{
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);
}

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

    tft.initR(INITR_MINI160x80);
    tft.setTextWrap(false);

    tft.invertDisplay(1);
    tft.setRotation(3);

    tft.fillScreen(background_color);
    tft.setFont(&FreeSans9pt7b);

    tft.setCursor(tft.width() / 2, tft.height() / 2);
}

int last_i = tft.width() / 2 - 25;

void scrollText(String text, int direction)
{
    if (direction == 0)
    {
        tft.setTextColor(blue_color);
        tft.setCursor(last_i, tft.height() / 2);
        tft.print(text);
        return;
    }

    int symbols = text.length();
    int offset = font_width * symbols;
    int leftBorder = -offset;
    int rightBorder = tft.height() + offset;

    int i;
    if (direction == 1)
    {
        i = last_i + 1;
    }
    else
    {
        i = last_i - 1;
    }

    if (i < leftBorder)
    {
        i = rightBorder;
    }
    if (i > rightBorder)
    {
        i = leftBorder;
    }

    tft.setTextColor(background_color);
    tft.setCursor(last_i, tft.height() / 2);
    tft.print(text);

    tft.setTextColor(blue_color);
    tft.setCursor(i, tft.height() / 2);
    tft.print(text);

    last_i = i;
}

float last_temperature = 0;

void displayTemperature(float temperature){
    tft.setCursor(tft.width() / 2 - tft.width() / 4, tft.height() / 2);
    tft.setTextColor(background_color);
    tft.print("T=");
    tft.print(last_temperature);

    tft.setCursor(tft.width() / 2 - tft.width() / 4, tft.height() / 2);
    tft.setTextColor(red_color);
    tft.print("T=");
    tft.print(temperature);

    last_temperature = temperature;

    delay(100);
}

int last_x = 0;
int last_y = 0;
int last_z = 0;

void displayAngles(int x, int y, int z){
    tft.setCursor(0, tft.height() / 2);
    tft.setTextColor(background_color);
    tft.printf("x=%d, y=%d, z=%d", last_x, last_y, last_z);

    tft.setCursor(0, tft.height() / 2);
    tft.setTextColor(green_color);
    tft.printf("x=%d, y=%d, z=%d", x, y, z);

    last_x = x;
    last_y = y;
    last_z = z;
}

/// Первая кнопка нажата
bool pressed1 = false;

/// Вторая кнопка нажата
bool pressed2 = false;
bool pressed2_last = false;

/// Третья кнопка нажата
bool pressed3 = false;

/// Счетчик пакетов
int counter = false;

float temperature = 0;

const int package_length = 9;

/// Буфер обмена
uint8_t rx_buffer[package_length];

int x = 0;

int y = 0;

int z = 0;

int modeNumber = 0;

void loop()
{
    if (nrf24.available())
    {
        pressed2_last = pressed2;

        int read = nrf24.readBytes(rx_buffer, package_length);
        pressed1 = rx_buffer[0];
        pressed2 = rx_buffer[1];
        pressed3 = rx_buffer[2];
        counter = rx_buffer[3];

        uint16_t temp_raw = 0;
        *(((uint8_t*)&(temp_raw)) + 0) = rx_buffer[4];
        *(((uint8_t*)&(temp_raw)) + 1) = rx_buffer[5];

        ADCResult result;
        result.valid = true;
        result.override = false;
        result.channel = ADC_ChannelName::ADC_Channel_0;
        result.value = temp_raw;
        temperature = ADC::temperature_raw_to_celsius(result);
        temperature = (float)((int)(temperature * 100)) / 100;

        x = (signed char)rx_buffer[6];
        y = (signed char)rx_buffer[7];
        z = (signed char)rx_buffer[8];

        digitalWrite(LED_BUILTIN_1,  pressed1);
        digitalWrite(LED_BUILTIN_2,  pressed3);
    }

    delay(2);

    int direction = 0;
    if (pressed3)
    {
        direction = 1;
    }
    if (pressed1)
    {
        direction = -1;
    }

    if (pressed2 && !pressed2_last)
    {
        modeNumber++;
        if (modeNumber == 3)
        {
            modeNumber = 0;
        }

        tft.fillScreen(background_color);
    }

    switch (modeNumber)
    {
    case 0:
        scrollText("MOTEON", direction);
        // scrollText("INFINEON", direction);
        break;
    case 1:
        displayTemperature(temperature);
        break;
    case 2:
        displayAngles(x, y, z);
    default:
        break;
    }
}
