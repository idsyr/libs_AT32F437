#include "adc.h"


static void formur_adc_common_config_struct(adc_common_config_type* adc_common_config_struct_ptr){
	adc_common_config_struct_ptr->div                             = ADC_HCLK_DIV_4;
	adc_common_config_struct_ptr->sampling_interval               = ADC_SAMPLING_INTERVAL_5CYCLES;
	adc_common_config_struct_ptr->combine_mode                    = ADC_INDEPENDENT_MODE;

	adc_common_config_struct_ptr->common_dma_mode                 = ADC_COMMON_DMAMODE_DISABLE;
	adc_common_config_struct_ptr->common_dma_request_repeat_state = FALSE;

	adc_common_config_struct_ptr->tempervintrv_state              = FALSE;
	adc_common_config_struct_ptr->vbat_state                      = FALSE;
}


static void formur_adc_base_config_struct(adc_base_config_type* adc_base_config_struct_ptr){
	adc_base_config_struct_ptr->sequence_mode           = TRUE;
	adc_base_config_struct_ptr->repeat_mode             = TRUE;
	adc_base_config_struct_ptr->data_align              = ADC_RIGHT_ALIGNMENT;
	adc_base_config_struct_ptr->ordinary_channel_length = 3;
}


static void adc_dma_config(uint8_t LIST_NUMS, uint16_t* STORAGE_LOCATION, ADC_INDEX adc_index){

	int                  DMAX_ChannelX_IRQn;
	dma_channel_type*    DMAX_CHANNELX;
	dmamux_channel_type* DMAXMUX_CHANNELX;
	int                  DMAMUX_DMAREQ_ID_ADCX;
	adc_type*            ADCN;

	switch(adc_index){
	case(ADC1_INDEX):
		DMAX_ChannelX_IRQn    = DMA2_Channel1_IRQn;
		DMAX_CHANNELX         = DMA2_CHANNEL1;
		DMAXMUX_CHANNELX      = DMA2MUX_CHANNEL1;
		DMAMUX_DMAREQ_ID_ADCX = DMAMUX_DMAREQ_ID_ADC1;
		ADCN                  = ADC1;
		break;
        
	case(ADC2_INDEX):
		DMAX_ChannelX_IRQn    = DMA2_Channel2_IRQn;
		DMAX_CHANNELX         = DMA2_CHANNEL2;
		DMAXMUX_CHANNELX      = DMA2MUX_CHANNEL2;
		DMAMUX_DMAREQ_ID_ADCX = DMAMUX_DMAREQ_ID_ADC2;
		ADCN                  = ADC2;
		break;
        
	case(ADC3_INDEX):
		DMAX_ChannelX_IRQn    = DMA2_Channel3_IRQn;
		DMAX_CHANNELX         = DMA2_Channel3_IRQn;
		DMAXMUX_CHANNELX      = DMA2MUX_CHANNEL3;
		DMAMUX_DMAREQ_ID_ADCX = DMAMUX_DMAREQ_ID_ADC3;
		ADCN                  = ADC3;
		break;
	}

	crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

	nvic_irq_enable(DMA1_Channel7_IRQn, 0, 0);
	dma_reset(DMA1_CHANNEL7);

	dma_init_type dma_init_struct;
	dma_default_para_init(&dma_init_struct);

	dma_init_struct.buffer_size           = LIST_NUMS;
	dma_init_struct.direction             = DMA_DIR_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_base_addr      = (uint32_t)STORAGE_LOCATION;
	dma_init_struct.memory_data_width     = DMA_MEMORY_DATA_WIDTH_HALFWORD;
	dma_init_struct.memory_inc_enable     = TRUE;
	dma_init_struct.peripheral_base_addr  = (uint32_t)&(ADCN->odt);
	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
	dma_init_struct.peripheral_inc_enable = FALSE;
	dma_init_struct.priority              = DMA_PRIORITY_HIGH;
	dma_init_struct.loop_mode_enable      = TRUE;

	dma_init(DMAX_CHANNELX, &dma_init_struct);

	dmamux_enable(DMA2, TRUE);
	dmamux_init(DMAXMUX_CHANNELX, DMAMUX_DMAREQ_ID_ADCX);

	dma_interrupt_enable(DMAX_CHANNELX, DMA_FDT_INT, TRUE);
	dma_channel_enable(DMAX_CHANNELX, TRUE);
}


void adc_init(ADC_INDEX INDEX, uint8_t* ADC_CHANNEL_LIST, uint8_t LIST_NUMS, uint16_t* STORAGE_LOCATION){
    
	adc_type*             ADCN;
	crm_periph_clock_type CRM_ADCN_PERIPH_CLOCK;
  
	switch(INDEX){
	case(ADC1_INDEX):
		ADCN                  = ADC1;
		CRM_ADCN_PERIPH_CLOCK = CRM_ADC1_PERIPH_CLOCK;
		break;
        
	case(ADC2_INDEX):
		ADCN                  = ADC2;
		CRM_ADCN_PERIPH_CLOCK = CRM_ADC2_PERIPH_CLOCK;
		break;
        
	case(ADC3_INDEX):
		ADCN                  = ADC3;
		CRM_ADCN_PERIPH_CLOCK = CRM_ADC3_PERIPH_CLOCK;
		break;
	}

	crm_periph_clock_enable(CRM_ADCN_PERIPH_CLOCK, TRUE);
  
	adc_common_config_type adc_common_config_struct;
	adc_common_default_para_init(&adc_common_config_struct);
	formur_adc_common_config_struct(&adc_common_config_struct);
	adc_common_config(&adc_common_config_struct);
      
	adc_base_config_type adc_base_config_struct;
	adc_base_default_para_init(&adc_base_config_struct);
	formur_adc_base_config_struct(&adc_base_config_struct);
	adc_base_config(ADCN, &adc_base_config_struct);
    
	for(uint8_t i = 0, sz = LIST_NUMS; i<sz; ++i)
		adc_ordinary_channel_set(ADCN, ADC_CHANNEL_LIST[i], i+1, ADC_SAMPLETIME_47_5);
    
	nvic_irq_enable(ADC1_2_3_IRQn, 0, 0);
	adc_interrupt_enable(ADCN, ADC_OCCO_INT, TRUE);
     
	adc_dma_mode_enable(ADCN, TRUE);
	adc_dma_request_repeat_enable(ADCN, TRUE);
    
	adc_resolution_set(ADCN, ADC_RESOLUTION_12B);
	adc_occe_each_conversion_enable(ADCN, TRUE);
	adc_ordinary_conversion_trigger_set(ADCN, ADC_ORDINARY_TRIG_TMR1CH1, 
	                                    ADC_ORDINARY_TRIG_EDGE_NONE);
 
	adc_enable(ADCN, TRUE);      while(adc_flag_get(ADCN, ADC_RDY_FLAG) == RESET);
	adc_calibration_init(ADCN);  while(adc_calibration_init_status_get(ADCN));
	adc_calibration_start(ADCN); while(adc_calibration_status_get(ADCN));
    
	adc_dma_config(LIST_NUMS, STORAGE_LOCATION, INDEX);
}


