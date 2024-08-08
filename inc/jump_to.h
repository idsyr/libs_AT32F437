#ifndef JUMP_TO_H
#define JUMP_TO_H

#include "stdint.h"

typedef void (*app_signature)();
typedef void (*bootloader_signature)();

//              _FLASH.ld format 

// FLASH     (rx)  : ORIGIN = 0x08000000, LENGTH = X1K
// BOOT_DATA (rx)  : ORIGIN = 0x0800C000, LENGTH = X2K
// APP       (rx)  : ORIGIN = 0x08010000, LENGTH = X3K
// RAM       (xrw) : ORIGIN = 0x20000000, LENGTH = X4K

// _sapp      = ORIGIN(APP);
// _eapp      = ORIGIN(APP) + LENGTH(APP);
// _sbootdata = ORIGIN(BOOT_DATA);
// _sboot     = ORIGIN(FLASH);

extern uint8_t _sapp;
extern uint8_t _eapp;
extern uint8_t _sbootdata;
extern uint8_t _sboot;

#define FLASH_APP_START_ADDRESS        (uint32_t) & _sapp
#define FLASH_APP_END_ADDRESS          (uint32_t) & _eapp
#define FLASH_BOOT_DATA_ADDRESS        (uint32_t) & _sbootdata
#define FLASH_BOOTLOADER_START_ADDRESS (uint32_t) & _sboot

/**  _____________________
  * | 0x00     SP         |
  * |_____________________|
  * | 0x04   Reset()      |
  * |_____________________|
 */

// this function is called from the jumped image
void set_interrupt_table_addr(uint32_t addr);
void jump_to_app();
// if bootloader addr is 0x08000000 you can use nvic_system_reset()
void jump_to_bootloader(); 









#endif
