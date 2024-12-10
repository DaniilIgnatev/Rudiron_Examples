#include "Arduino.h"
#include "gpio.h"
#include "timer.h"
#include "dac.h"

using namespace Rudiron;

#define pole_pairs 4

float phase_sin(float t_sec, float w_hz, float p_degree, float a_volt, float b_volt)
{
    float w_rad_s = w_hz * 2 * PI;
    float p_rad = p_degree * (PI / 180);

    return a_volt * sin(t_sec * w_rad_s + p_rad) + b_volt;
}

volatile float w_rad_s = 60.0 * pole_pairs * 2 * PI / 60.0;

#define InverterFreq 10000

#define PortPin_Va PORT_PIN_A1 // Va 4
#define PortPin_Vb PORT_PIN_A3 // Vb 6
#define PortPin_Vc PORT_PIN_A5 // Vc 8

uint16_t timer1_ARR = 0;

uint16_t dma_abc[3];

// void pwm_dma_interrupt_handler(Timer& timer){
//     timer.PWM_DMA_update(dma_abc, 3);
// }


#define FastTimerFreq 20000

volatile unsigned int time_us = 0;

bool led = false;

void fast_timer_isr(){
    GPIO::writeLED_1(true);
    if (time_us >= 1000000){
        time_us = 0;
        led = !led;
        digitalWrite(LED_BUILTIN_2, led);
    }

    float t_s = time_us / (float)1000000.0;
    float sin_a = 0.5 * sin(t_s * w_rad_s) + 0.5;
    float sin_b = 0.5 * sin(t_s * w_rad_s - (PI / 3)) + 0.5;
    float sin_c = 0.5 * sin(t_s * w_rad_s + (PI / 3)) + 0.5;

    dma_abc[0] = (uint16_t)(sin_a * (float)timer1_ARR);
    dma_abc[1] = (uint16_t)(sin_b * (float)timer1_ARR);
    dma_abc[2] = (uint16_t)(sin_c * (float)timer1_ARR);

    // Timer& timer1 = Timer::getTimer1();
    // timer1.PWM_DMA_update(dma_abc, 3);

    time_us += 50;
    GPIO::writeLED_1(false);
}

void setup()
{
    CLK::setCPUSpeed(CLK_Speed::high);

    // конфигурация встроенных кнопок
    // pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    // pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    // pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    // // конфигурация встроенных светодиодов
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    // // включение встроенных светодиодов
    // digitalWrite(LED_BUILTIN_1, true);
    // digitalWrite(LED_BUILTIN_2, true);

    // конфигурация последовательного порта
    // Serial.begin(115200);
    // отправка приветствия через последовательный порт
    // Serial.println("Рудирон Бутерброд!");

    // Timer& timer1 = Timer::getTimer1();
    // timer1.setup(InverterFreq);
    // timer1_ARR = timer1.get_ARR();

    // dma_abc[0] = 0;
    // dma_abc[1] = 0;
    // dma_abc[2] = 0;
    // timer1.PWM_DMA_setup_all_channels(PortPin_Va, PortPin_Vb, PortPin_Vc, dma_abc, nullptr);

    // //timer.PWM_DMA_setup(PortPin_Va, dma_abc, 1, pwm_dma_interrupt_handler);

    Timer_3_Interrupt_Handler = fast_timer_isr;
    Timer& timer3 = Timer::getTimer3();
    timer3.setup(FastTimerFreq);
    timer3.enable();

    CLK::pause_delay();
}


void loop()
{

}
