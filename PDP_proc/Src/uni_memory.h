//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_memory.h
user memory
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef _uni_memory_H
#define _uni_memory_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Memory allocations
//

	 
//
// ADC
//	 

#define PDP_RELAY_CHANNELS  						16 

	 
// ADC_DMA_SAMPLES = # channels * # samples 
#define ADC_DMA_CHANNELS  						16 
#define ADC_DMA_SAMPLES_PER_CHANNEL  	2000 
#define ADC_DMA_SAMPLES  							ADC_DMA_CHANNELS * ADC_DMA_SAMPLES_PER_CHANNEL 

extern uint16 adc_dma_buf[ADC_DMA_SAMPLES * 2]; // x2 for circular

extern uint16* adc_dma_buf_1; // circular buffer pointer 1
extern uint16* adc_dma_buf_2; // circular buffer pointer 2
	 
#define ADC_DMA_BYTES_PER_SAMPLE  		2
#define ADC_DMA_BUF_SIZE  						ADC_DMA_SAMPLES * ADC_DMA_BYTES_PER_SAMPLE 
	 
//
// Power
//
	 
extern uint16* adc_dma_buf_filtered; 
//extern uint16 adc_dma_buf_filtered[ADC_DMA_SAMPLES]; 
	 
extern uint16 adc_channel_buf[ADC_DMA_SAMPLES_PER_CHANNEL];
extern int16 adc_channel_buf_filtered[ADC_DMA_SAMPLES_PER_CHANNEL];

 
	 

//
// UARTS
//	 


	 
// 64-byte rx buffer for packets from UART rx
// 64 byte MAX_RX_SIZE makes the UART have the same packet size as the USB port
#define SC_UART_RX_BUF_SIZE  64 	// max rx bytes for each command sent from host
extern uint8 sc_uart_rx_buf[SC_UART_RX_BUF_SIZE]; // in uni_uart.c
extern char textstr[100]; // in uni_uart.c


	 
//
// Commands
//	 
// comms_test	 
#define COMMS_TEST_BUF_SIZE  1000
extern uint8 comms_test_buf[COMMS_TEST_BUF_SIZE]; // in uni_command.c	 
	 
	 

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
	 
 






//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------








//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------


