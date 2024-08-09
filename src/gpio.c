#include "gpio.h"


void config_analog_input(uint16_t GPIO_PINS_LIST, gpio_type* GPION){
	gpio_init_type gpio_init_struct;
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
	gpio_init_struct.gpio_pins = GPIO_PINS_LIST;
	gpio_init(GPION, &gpio_init_struct);
}


void config_digital_output(uint16_t GPIO_PINS_LIST, gpio_type* GPIOX){
	gpio_init_type gpio_init_struct;
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_pins           = GPIO_PINS_LIST;
	gpio_init_struct.gpio_mode           = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_pull           = GPIO_PULL_NONE; // ext pull down
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOX, &gpio_init_struct);
}


void config_digital_input(uint16_t GPIO_PINS_LIST, gpio_type* GPIOX){
	gpio_init_type gpio_init_struct;
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_pins           = GPIO_PINS_LIST;
	gpio_init_struct.gpio_mode           = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOX, &gpio_init_struct);
}


void enable_clock_all_gpio(){
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOH_PERIPH_CLOCK, TRUE);
}


void disable_clock_all_gpio(){
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, FALSE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, FALSE);
	crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, FALSE);
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, FALSE);
	crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, FALSE);
	crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, FALSE);
	crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, FALSE);
	crm_periph_clock_enable(CRM_GPIOH_PERIPH_CLOCK, FALSE);
}


