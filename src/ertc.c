#include "ertc.h"

void ertc_timestamp_config(){  
	exint_init_type exint_init_struct;

	nvic_irq_enable(TAMP_STAMP_IRQn, 0, 0);
 
	exint_default_para_init(&exint_init_struct);
	exint_init_struct.line_enable   = TRUE;
	exint_init_struct.line_mode     = EXINT_LINE_INTERRUPUT;
	exint_init_struct.line_select   = EXINT_LINE_21;
	exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
	exint_init(&exint_init_struct);
    
	ertc_timestamp_valid_edge_set(ERTC_TIMESTAMP_EDGE_FALLING);
	ertc_timestamp_enable(TRUE);
    
	ertc_interrupt_enable(ERTC_TS_INT, TRUE);
}


void ertc_config(){
	crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE); 
	pwc_battery_powered_domain_access(TRUE);             
	crm_battery_powered_domain_reset(TRUE); crm_battery_powered_domain_reset(FALSE);
	crm_clock_source_enable(CRM_CLOCK_SOURCE_LEXT, TRUE); 
	while(crm_flag_get(CRM_LEXT_STABLE_FLAG) == RESET){}
    
	crm_ertc_clock_select(CRM_ERTC_CLOCK_LEXT);
	crm_ertc_clock_enable(TRUE); ertc_reset(); ertc_wait_update();
	ertc_divider_set(127, 255); //128*256 = 32768
	ertc_hour_mode_set(ERTC_HOUR_MODE_24);
	ertc_date_set(24, 4, 9, 2);
	ertc_time_set(0, 0, 0, ERTC_AM);
                     
	ertc_timestamp_config();
}

