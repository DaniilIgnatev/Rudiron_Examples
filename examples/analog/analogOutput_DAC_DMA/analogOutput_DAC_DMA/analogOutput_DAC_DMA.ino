#include "Arduino.h"
#include "rudiron/dac.h"

const int sine12bit_length = 32;

const uint16_t sine12bit[sine12bit_length] = {
        2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
        3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
        599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

uint16_t sine12bit_primary[sine12bit_length] = {
        2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
        3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
        599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

uint16_t sine12bit_alternate[sine12bit_length] = {
        2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
        3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
        599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

uint32_t freq = 500000;

bool switched = false;

void dac_dma_interrupt_handler(Rudiron::DAC &dac) {
    if (dac.DMA_done()) {
        dac.DMA_repeat();

        switched = false;
        for (int i = 0; i < sine12bit_length; ++i) {
            sine12bit_alternate[i] = 4095 - sine12bit[i];
//            sine12bit_alternate[i] = sine12bit[i];
        }
    } else if (!switched && dac.DMA_is_alternate_active()) {
        switched = true;
        for (int i = 0; i < sine12bit_length; ++i) {
//            sine12bit_primary[i] = 4095 - sine12bit[i];
            sine12bit_primary[i] = sine12bit[i];
        }
    }
}

void setup() {
    Rudiron::DAC &dac = Rudiron::DAC::getDAC2();
    Serial.begin(115200);

    dac.DMA_setup(Rudiron::Timer_1,
                   freq,
                   Rudiron::PORT_PIN_E0,
                   sine12bit_primary,
                   sine12bit_length,
                   &dac_dma_interrupt_handler,
                   Rudiron::DMA_Number_Continuous_Transfers_1,
                   true,
                   sine12bit_alternate,
                   sine12bit_length,
                   Rudiron::DMA_Number_Continuous_Transfers_1
    );
}

void loop() {
    delay(1000);
}

