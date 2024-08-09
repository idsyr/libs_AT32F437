#ifndef GPIO_H
#define GPIO_H

#include "at32f435_437_conf.h"

void config_digital_output(uint16_t GPIO_PINS_LIST, gpio_type* GPIOX);
void config_digital_input(uint16_t GPIO_PINS_LIST, gpio_type* GPIOX);
void config_analog_input(uint16_t GPIO_PINS_LIST, gpio_type* GPIOX);
void enable_clock_all_gpio();
void disable_clock_all_gpio();

#endif
