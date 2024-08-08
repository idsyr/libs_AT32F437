#include "jump_to.h"

void set_interrupt_table_addr(uint32_t addr){
	SCB->VTOR = addr; // legitim, no need add 4
	__enable_irq();
}


void jump_to_app(){
	__disable_irq();
	uint32_t app_reset_handler_addr = *(uint32_t*) (FLASH_APP_START_ADDRESS + 4); 
	app_signature app_reset_handler = (app_signature) app_reset_handler_addr;
	
	__set_MSP(*(uint32_t*) FLASH_APP_START_ADDRESS );
	app_reset_handler();
}


void jump_to_bootloader(){
	__disable_irq();
	uint32_t bootloader_reset_handler_addr = *(uint32_t*) (FLASH_BOOTLOADER_START_ADDRESS + 4);
	bootloader_signature bootloader_reset_handler = (bootloader_signature) bootloader_reset_handler_addr;

	__set_MSP(*(uint32_t*) FLASH_BOOTLOADER_START_ADDRESS);
	bootloader_reset_handler();
}
