#ifndef BOOT_DATA_WORKS_H
#define BOOT_DATA_WORKS_H

#include "J73.h"
#include "flash.h"

#include "stdint.h"

typedef struct {
	uint8_t          imageload_must_active;
	uint8_t          app_image_in_place;
	J1939_ID_t       dm14_id;
	J1939_DM14_pkg_t dm14_pkg;
	can_type*        CANX;
} boot_data_t;


void write_boot_data_struct_in_flash();
void read_boot_data_struct();
int  app_image_in_memory();
void set_info_about_image();
int  imageloader_called_from_app();
void set_flag_call_from_app();
J1939_ID_t get_dm14_id_from_boot_data();
J1939_DM14_pkg_t get_dm14_pkg_from_boot_data();
can_type* get_canx_from_boot_data();
void save_in_boot_data_init_pkg(J1939_ID_t _dm14_id, J1939_DM14_pkg_t _dm14_pkg, can_type* _CANX);
void clean_flag_call_from_app();
void clean_flag_image_in_memory();



#endif
