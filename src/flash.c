#include "flash.h"

void flash_read(uint32_t read_addr, uint32_t nums, uint8_t* buf){
	for(uint32_t i = 0; i < nums; ++i){
		buf[i] = *(uint8_t*)(read_addr);
		read_addr += 1;
	}
}


static error_status flash_write_nocheck(uint32_t write_addr, uint32_t nums, uint8_t* buf){
	flash_status_type status = FLASH_OPERATE_DONE;
	for(uint32_t i = 0; i < nums; ++i){
		status = flash_byte_program(write_addr, buf[i]);
		if(status != FLASH_OPERATE_DONE) return ERROR;
		write_addr += 1;
	}
	return SUCCESS;
}
 

static int sector_is_clean(uint32_t sector_number, uint8_t* sector_container, uint32_t sector_offset, uint32_t sector_remain){
	flash_read(sector_number * SECTOR_SIZE + FLASH_BASE, SECTOR_SIZE, sector_container);
	for(uint32_t i = 0; i<sector_remain; ++i){
		if(sector_container[sector_offset + i] != 0xFF) return 0;
	}
	return 1;
}


static error_status flash_write_in_nonclean_sector(uint32_t sector_number, uint32_t sector_offset, uint32_t nums, uint8_t* buf, uint8_t* sector_container, uint32_t sector_remain){
	flash_status_type status = flash_operation_wait_for(ERASE_TIMEOUT);
	if(status == FLASH_PROGRAM_ERROR || status == FLASH_EPP_ERROR)
		flash_flag_clear(FLASH_PRGMERR_FLAG | FLASH_EPPERR_FLAG);
	else if(status == FLASH_OPERATE_TIMEOUT) return ERROR;

	status = flash_sector_erase(sector_number * SECTOR_SIZE + FLASH_BASE);
	if(status != FLASH_OPERATE_DONE) return ERROR;
	
	for(uint32_t i = 0; i<sector_remain; ++i)
		sector_container[i + sector_offset] = buf[i];
	
	return flash_write_nocheck(sector_number * SECTOR_SIZE + FLASH_BASE, 
	                           SECTOR_SIZE, 
	                           sector_container);
}


static error_status flash_write_in_clean_sector(uint32_t sector_number,uint32_t sector_offset, uint32_t nums, uint8_t* buf){
	return flash_write_nocheck(sector_number * SECTOR_SIZE + FLASH_BASE + sector_offset, nums, buf);
}


static error_status flash_write_porcion(uint32_t write_addr, uint32_t nums, uint8_t* buf){
	uint32_t offset_addr   = write_addr - FLASH_BASE;
	uint32_t sector_number = offset_addr / SECTOR_SIZE;
	uint32_t sector_offset = offset_addr % SECTOR_SIZE;
	uint32_t sector_remain = SECTOR_SIZE - sector_offset;
	uint8_t  sector_container[SECTOR_SIZE];

	if(nums < sector_remain) sector_remain = nums;
    
	if(sector_is_clean(sector_number, sector_container, sector_offset, sector_remain))
		return flash_write_in_clean_sector(sector_number, sector_offset, nums, buf);
	else
		return flash_write_in_nonclean_sector(sector_number, sector_offset, nums, buf, sector_container, sector_remain);
}


error_status flash_write(uint32_t write_addr, uint32_t nums, uint8_t* buf){
	flash_unlock();
	for(uint32_t i = 0, full_porcion_num = nums / SECTOR_SIZE; i < full_porcion_num; ++i){
		if(flash_write_porcion(write_addr, nums, buf) == ERROR) return ERROR;
		write_addr += SECTOR_SIZE;
		buf        += SECTOR_SIZE;
		nums       -= SECTOR_SIZE;
	}
	if(flash_write_porcion(write_addr, nums, buf) == ERROR) return ERROR;
	flash_lock();
	return SUCCESS;
}
