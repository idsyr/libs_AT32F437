#ifndef CAN_H
#define CAN_H

#include "at32f435_437_conf.h"
#include <stdint.h>

typedef enum { CAN1_INDEX = 0, CAN2_INDEX = 1, CAN3_INDEX = 2} CAN_INDEX;

void can_init(CAN_INDEX can_index);
void push_packet_to_can(uint32_t ext_id, uint8_t* data, uint8_t DLC, can_type* CANX){


#endif
 
