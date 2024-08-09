#include "can.h"

static void formur_can_gpio_init_struct(gpio_init_type* gpio_init_struct_ptr){
	gpio_init_struct_ptr->gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct_ptr->gpio_mode           = GPIO_MODE_MUX;
	gpio_init_struct_ptr->gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct_ptr->gpio_pull           = GPIO_PULL_NONE;
}


static void can_gpio_init(CAN_INDEX INDEX){
  
	gpio_type*            GPIO_RX;
	uint16_t              GPIO_PINS_RX;
	gpio_pins_source_type GPIO_PINS_SOURCE_RX;
	gpio_mux_sel_type     GPIO_MUX_RX;
  
	gpio_type*            GPIO_TX;
	uint16_t              GPIO_PINS_TX;
	gpio_pins_source_type GPIO_PINS_SOURCE_TX;
	gpio_mux_sel_type     GPIO_MUX_TX;
    
	switch(INDEX){
	case(CAN1_INDEX):
		GPIO_RX                  = CAN1_GPIO_RX;
		GPIO_PINS_RX             = CAN1_GPIO_PINS_RX;
		GPIO_PINS_SOURCE_RX      = CAN1_GPIO_PINS_SOURCE_RX;
		GPIO_MUX_RX              = CAN1_GPIO_MUX_RX;
            
		GPIO_TX                  = CAN1_GPIO_TX;
		GPIO_PINS_TX             = CAN1_GPIO_PINS_TX;
		GPIO_PINS_SOURCE_TX      = CAN1_GPIO_PINS_SOURCE_TX;
		GPIO_MUX_TX              = CAN1_GPIO_MUX_TX;
		break;

	case(CAN2_INDEX):
		GPIO_RX                  = CAN2_GPIO_RX;
		GPIO_PINS_RX             = CAN2_GPIO_PINS_RX;
		GPIO_PINS_SOURCE_RX      = CAN2_GPIO_PINS_SOURCE_RX;
		GPIO_MUX_RX              = CAN2_GPIO_MUX_RX;
    
		GPIO_TX                  = CAN2_GPIO_TX;
		GPIO_PINS_TX             = CAN2_GPIO_PINS_TX;
		GPIO_PINS_SOURCE_TX      = CAN2_GPIO_PINS_SOURCE_TX;
		GPIO_MUX_TX              = CAN2_GPIO_MUX_TX;
		break;
	}
    
	gpio_init_type gpio_init_struct; 
	formur_can_gpio_init_struct(&gpio_init_struct);
		
	gpio_init_struct.gpio_pins = GPIO_PINS_RX;
	gpio_init(GPIO_RX, &gpio_init_struct);
  
	gpio_init_struct.gpio_pins = GPIO_PINS_TX;
	gpio_init(GPIO_TX, &gpio_init_struct);

	gpio_pin_mux_config(GPIO_RX, GPIO_PINS_SOURCE_RX, GPIO_MUX_RX);
	gpio_pin_mux_config(GPIO_TX, GPIO_PINS_SOURCE_TX, GPIO_MUX_TX);
}


static void formur_can_base_struct(can_base_type* can_base_struct_ptr){
	can_base_struct_ptr->mode_selection   = CAN_MODE_COMMUNICATE;
	can_base_struct_ptr->ttc_enable       = FALSE;
	can_base_struct_ptr->aebo_enable      = TRUE;
	can_base_struct_ptr->aed_enable       = TRUE;
	can_base_struct_ptr->prsf_enable      = FALSE;
	can_base_struct_ptr->mdrsel_selection = CAN_DISCARDING_FIRST_RECEIVED;
	can_base_struct_ptr->mmssr_selection  = CAN_SENDING_BY_ID;
}


static void formur_can_baudrate_struct(can_baudrate_type* can_baudrate_struct_ptr){
	can_baudrate_struct_ptr->baudrate_div = 12; //1000
	can_baudrate_struct_ptr->rsaw_size = CAN_RSAW_3TQ;
	can_baudrate_struct_ptr->bts1_size = CAN_BTS1_8TQ;
	can_baudrate_struct_ptr->bts2_size = CAN_BTS2_3TQ;
}


static void formur_can_filter_init_struct(can_filter_init_type* can_filter_init_struct_ptr){
	can_filter_init_struct_ptr->filter_activate_enable = TRUE;
	can_filter_init_struct_ptr->filter_mode            = CAN_FILTER_MODE_ID_MASK;
	can_filter_init_struct_ptr->filter_fifo            = CAN_FILTER_FIFO0;
	can_filter_init_struct_ptr->filter_bit             = CAN_FILTER_32BIT;
	can_filter_init_struct_ptr->filter_id_high         = 0;
	can_filter_init_struct_ptr->filter_id_low          = 0;
	can_filter_init_struct_ptr->filter_mask_high       = 0;
	can_filter_init_struct_ptr->filter_mask_low        = 0; // if ((X & 0) == 0) Always teken
}


void can_init(CAN_INDEX INDEX){
  
	crm_periph_clock_type CRM_CANX_PERIPH_CLOCK;
	IRQn_Type             CANX_SE_IRQn;
	IRQn_Type             CANX_RX0_IRQn;
	can_type*             CANX;
	uint8_t               filter_num;

	switch(INDEX){
	case(CAN1_INDEX):
		CRM_CANX_PERIPH_CLOCK = CRM_CAN1_PERIPH_CLOCK;
		CANX_SE_IRQn          = CAN1_SE_IRQn;
		CANX_RX0_IRQn         = CAN1_RX0_IRQn;
		CANX                  = CAN1;
		filter_num            = 0;
		break;
        
	case(CAN2_INDEX):
		CRM_CANX_PERIPH_CLOCK = CRM_CAN2_PERIPH_CLOCK;
		CANX_SE_IRQn          = CAN2_SE_IRQn;
		CANX_RX0_IRQn         = CAN2_RX0_IRQn;
		CANX                  = CAN2;
		filter_num            = 14;
		break;
	}
    
	can_gpio_init(INDEX);
	crm_periph_clock_enable(CRM_CANX_PERIPH_CLOCK, TRUE);
  
	can_base_type can_base_struct;
	can_default_para_init(&can_base_struct);
	formur_can_base_struct(&can_base_struct);
	can_base_init(CANX, &can_base_struct);
		
	can_baudrate_type can_baudrate_struct;
	formur_can_baudrate_struct(&can_baudrate_struct);
	can_baudrate_set(CANX, &can_baudrate_struct);
    
	can_filter_init_type can_filter_init_struct;
	formur_can_filter_init_struct(&can_filter_init_struct);
	can_filter_init_struct.filter_number = filter_num;
	can_filter_init(CANX, &can_filter_init_struct);
	
	nvic_irq_enable(CANX_SE_IRQn,  0x00, 0x00);
	nvic_irq_enable(CANX_RX0_IRQn, 0x00, 0x00);
		
	can_interrupt_enable(CANX, CAN_RF0MIEN_INT, TRUE);
	can_interrupt_enable(CANX, CAN_ETRIEN_INT,  TRUE);
	can_interrupt_enable(CANX, CAN_EOIEN_INT,   TRUE);   
    
}


void push_packet_to_can(uint32_t ext_id, uint8_t* data, uint8_t DLC, can_type* CANX){
	uint8_t transmit_mailbox;
	can_tx_message_type tx_msg_struct;

	tx_msg_struct.standard_id = 0;
	tx_msg_struct.extended_id = ext_id;

	tx_msg_struct.id_type = CAN_ID_EXTENDED;
	tx_msg_struct.frame_type = CAN_TFT_DATA;
	tx_msg_struct.dlc = DLC;

	memcpy(tx_msg_struct.data, data, DLC * sizeof(uint8_t));
	
	transmit_mailbox = can_message_transmit(CANX, &tx_msg_struct);
	
	while(can_transmit_status_get(CANX, (can_tx_mailbox_num_type)transmit_mailbox) != CAN_TX_STATUS_SUCCESSFUL);
}


CAN_RX_Handler(can_type* CANX, uint8_t mailbox_num){
	can_rx_message_type rx_message_struct;
	
	if(mailbox_num == 0 && can_interrupt_flag_get(CANX, CAN_RF0MN_FLAG) != RESET)
		can_message_receive(CANX, CAN_RX_FIFO0, &rx_message_struct);
	if(mailbox_num == 1 && can_interrupt_flag_get(CANX, CAN_RF1MN_FLAG) != RESET)
		can_message_receive(CANX, CAN_RX_FIFO1, &rx_message_struct);
	
	uint32_t ext_id    = rx_message_struct.extended_id;
	uint32_t PGN_      = (ext_id & 0x00FFFF00) >> 8;
	uint8_t  priority_ = ext_id >> 26;
	uint8_t  SA_       = ext_id;
	uint8_t  DA_       = ext_id >> 8;
	uint8_t  DLC_      = rx_message_struct.dlc;
  
	// memcpy(.payload, rx_message_struct.data, DLC_ * sizeof(uint8_t));
	
}


void CAN1_RX0_IRQHandler(void){
	CAN_RX_Handler(CAN1, 0);
}


void CAN1_SE_IRQHandler(void){
	volatile uint32_t err_index = 0;
	if(can_interrupt_flag_get(CAN1, CAN_ETR_FLAG) != RESET){
		err_index = CAN1->ests & 0x70;
		can_flag_clear(CAN1, CAN_ETR_FLAG);
		if(err_index == 0x00000010){
			// data error occur
		}
	}
}



