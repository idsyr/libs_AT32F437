#ifndef FLASH_H
#define FLASH_H

#include "stdint.h"
#include "at32f435_437_clock.h"
#include "at32f435_437_conf.h"

void flash_read(uint32_t read_addr, uint32_t nums, uint8_t* buf);
error_status flash_write(uint32_t write_addr, uint32_t nums, uint8_t* buf);


#endif
