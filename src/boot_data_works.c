#include "boot_data_works.h"

boot_data_t boot_data_struct_ptr[1];
#define TRUELEAN 42

int app_image_in_memory(){
	return boot_data_struct_ptr->app_image_in_place == TRUELEAN; 
}


void read_boot_data_struct(){
	flash_read(FLASH_BOOT_DATA_ADDRESS, sizeof(boot_data_t)/sizeof(uint8_t), (uint8_t*)boot_data_struct_ptr);
}


void set_info_about_image(){
	boot_data_struct_ptr->app_image_in_place = TRUELEAN;
}


int imageloader_called_from_app(){
	return boot_data_struct_ptr->imageload_must_active == TRUELEAN; 
}


void set_flag_call_from_app(){
	boot_data_struct_ptr->imageload_must_active = TRUELEAN;
}


void write_boot_data_struct_in_flash(){
	flash_write(FLASH_BOOT_DATA_ADDRESS, sizeof(boot_data_t)/sizeof(uint8_t), (uint8_t*)boot_data_struct_ptr);
}


J1939_ID_t get_dm14_id_from_boot_data(){
	return boot_data_struct_ptr->dm14_id;
}


J1939_DM14_pkg_t get_dm14_pkg_from_boot_data(){
	return boot_data_struct_ptr->dm14_pkg;
}


can_type* get_canx_from_boot_data(){
	return boot_data_struct_ptr->CANX;
}


void save_in_boot_data_init_pkg(J1939_ID_t _dm14_id, J1939_DM14_pkg_t _dm14_pkg, can_type* _CANX){
	boot_data_struct_ptr->dm14_id  = _dm14_id;
	boot_data_struct_ptr->dm14_pkg = _dm14_pkg;
	boot_data_struct_ptr->CANX     = _CANX;
}


void clean_flag_call_from_app(){
	boot_data_struct_ptr->imageload_must_active = 0;
}


void clean_flag_image_in_memory(){
	boot_data_struct_ptr->app_image_in_place = 0;
}
