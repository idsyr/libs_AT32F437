#ifndef ADC_H
#define ADC_H

#include "at32f435_437_conf.h"
#include "mcu_periph.h"
#include "xprintf.h"

typedef enum {ADC1_INDEX, ADC2_INDEX, ADC3_INDEX} ADC_INDEX;
typedef enum {ADC_POLLING, ADC_DMA} ADC_MODE;

void adc_init(ADC_INDEX INDEX, uint8_t* ADC_CHANNEL_LIST, uint8_t PINS_LIST_N, uint16_t* STORAGE_LOCATION);

#endif
