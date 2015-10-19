//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_command.c
user host command routines


*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "usart.h"
#include <stdio.h>

#include "uni_defines.h" // pins, ports, types, structs, commands, memory

#include "uni_command.h"
IoTcam_Command_Struct ic; // IoTcam Host command
IoTcam_Status_Struct is; // IoTcam System status


#include "uni_defines.h"
#include "uni_uart.h"

//#include "..\..\uni_host\uni_host_structs.h"
PDP_Command_Struct pc;
PDP_Status_Struct ps; // output measurements
PDP_Status_Struct ps_temp; // store changing measurements until output


#include "uni_relay.h"



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// rx buffer
// see defines in fc_host_commands.h
static uint8* pRxDest; 			// write host data to pRxDest in STM32 sram

static uint32	testDataSize;

uint8 comms_test_buf[COMMS_TEST_BUF_SIZE];



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// command execute routines 
// during I2C/USB/UART interrupt service
//
//

//
// tx ack to host to indicate that host command was received 
// or that a long command has ended.
//
void command_ack_host(void)
{
	static uint8 ack = 0x01;
	uni_uart_transmit(&ack, 1);
}
//
// Send data to host
//
// USB/UART - called to tx data to host
// I2C - called set the tx buffer location
//
void command_exec_tx(void)
{
	uni_uart_transmit(ic.txPtr, ic.txSize);
}

//
// Parse command from host
//
// called after rx from host
// execute in USB/UART/I2C rx interrupt
// do not execute slow routines here or they will hang the USB etc.
//
void command_exec_rx(uint8_t* rxBuf, uint32_t bytesRx)
{
	// default
	ic.command = FC_COM_NONE;
	ic.rxSize = 0;
	ic.rxBuf = rxBuf;
	//
	// good command
	if(bytesRx){ 
		ic.command = rxBuf[0];
		ic.rxSize = bytesRx - 1;
		ic.rxBuf = (uint8*)(rxBuf + 1);
	}
}
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//

void uni_command_exec(void)
{
	uint32 i, index;
	static uint8 val8;
	static uint8 saved_val8;
	static uint16 val16;
	static uint32 val32;

	uint8 new_command = ic.command;
	
	ic.command = FC_COM_NONE; // clear command
	
	
	switch(new_command)
	{

		// host null command
		case FC_COM_NONE: 
			break;

		
		//
		// host sets FC_COM_WR_CHANNEL_NUMBER
		//
		
		case FC_COM_RD_RELAY_STATE: { 
			ic.txPtr = (uint8*)&ps.relay_state[pc.channel_number];
			ic.txSize = 1;
			command_exec_tx();
			break;}
		case FC_COM_WR_RELAY_ON: { // call function		
			uni_relay_on();
			command_ack_host(); // tx ack to host to indicate command has ended
			break;}
		case FC_COM_WR_RELAY_OFF: { // call function		
			uni_relay_off();
			command_ack_host(); // tx ack to host to indicate command has ended
			break;}
		
		
			
			
				
		case FC_COM_WR_COMMAND_STRUCT: { // write to struct
			pRxDest = (uint8*)&pc;
			ic.command = FC_COM_NONE;
			command_ack_host(); // tx ack to host to indicate command has ended
			break;}

		case FC_COM_RD_STATUS_STRUCT: { // read from struct
			ic.txPtr = (uint8*)&ps;
			ic.txSize = sizeof(PDP_Status_Struct);
			
			command_exec_tx();
			break;}

			
		//
		// select PDP channel
		//
		case FC_COM_WR_CHANNEL_NUMBER: { 
			val8 = *(uint32_t*)&ic.rxBuf[0];
			pc.channel_number = val8;	
			command_ack_host(); // tx ack to host to indicate command has ended
			break;}
		
		//
		// ADC channel data
		//
		case FC_COM_RD_CHANNEL_I_RAW_SIZE: { 
			is.adc_channel_bytes_per_cycle = ps.voltage_cycle_samples * ADC_DMA_BYTES_PER_SAMPLE;
			ic.txPtr = (uint8*)&is.adc_channel_bytes_per_cycle;
			ic.txSize = 4;
			command_exec_tx();
			break;}
		case FC_COM_RD_CHANNEL_I_RAW: // read from buffer
			/*
			// extract channel data from dma buf
			for(i = 0;i < is.adc_samples_per_cycle;i++){
				
				index = ic.channel_number + (i * 16);
				adc_channel_buf[i] = adc_dma_buf[index];
				
			}
			*/
			ic.txPtr = (uint8*)adc_channel_buf; 
			ic.txSize = is.adc_channel_bytes_per_cycle;
			command_exec_tx();
			break;

		
		
		case FC_COM_RD_CHANNEL_I_FILTERED: // read from buffer
			ic.txPtr = (uint8*)adc_channel_buf_filtered; 
			ic.txSize = is.adc_channel_bytes_per_cycle;
			command_exec_tx();
			break;

		
		
		//
		// ADC raw data
		//
		case FC_COM_RD_I_RAW_SIZE: { 
			is.adc_buf_bytes_per_cycle = ps.voltage_cycle_samples * ADC_DMA_CHANNELS * ADC_DMA_BYTES_PER_SAMPLE;
			ic.txPtr = (uint8*)&is.adc_buf_bytes_per_cycle;
			ic.txSize = 4;
			command_exec_tx();
			break;}
		case FC_COM_RD_I_RAW: // read from buffer
			ic.txPtr = (uint8*)adc_dma_buf; 
			ic.txSize = is.adc_buf_bytes_per_cycle;
			command_exec_tx();
			break;
		
		
		
		
		//
		// host wr enable command to fc
		//
		case FC_COM_WR_ENABLE: // enable IoTcam
			ic.enable = true;
			command_ack_host(); 
			break;
		case FC_COM_WR_DISABLE: // disable IoTcam
			ic.enable = false;
			command_ack_host(); 
			break;


		//
		// host wr and rd a 4Cam data buffer	
		//
		// Use this method from host:
		// FC_writeAddr(FC_COM_WR_COMMS_TEST);
		// FC_writeBuf(FC_COM_WR_CONTINUE, size, data_out);
		//
		case FC_COM_WR_COMMS_TEST: // write to buffer (pRxDest in STM32 sram)
			pRxDest = comms_test_buf;
			testDataSize = 0;			
			//if(ic.rxSize == 0) command_ack_host(); // tx ack to host to indicate command has ended
			//break;
		// continue writing and increment buffer index
		case FC_COM_WR_CONTINUE: // continue writing rx data to pRxDest
			for(i = 0;i < ic.rxSize;i++){
				pRxDest[i] = ic.rxBuf[i];
			}
			pRxDest += ic.rxSize;
			testDataSize += ic.rxSize;
			command_ack_host();
			break;
		// host read buffer from fc	
		case FC_COM_RD_COMMS_TEST: // read from buffer
			ic.txPtr = comms_test_buf; 
			ic.txSize = testDataSize;
			command_exec_tx();
			break;

			


		
		
		
			
		// host rd regs from stm32
		case FC_COM_RD_CHIP_ID: { // read reg
			// read from the chip reg
			//val32 = *(__IO uint32_t*)STM32REG_IDCODE_ADDR;
			//ic.txPtr = (uint8*)&val32;
			// read from struct
			ic.txPtr = (uint8*)&is.chip_id;
			// either way
			ic.txSize = 4;
			command_exec_tx();
			break;}
		//
		// Read UDID registers from main() before MPU is enabled.
		// Then later the host can access the UDID here.
		//
		case FC_COM_RD_UDID_1: { // read reg
			ic.txPtr = (uint8*)&is.UDID1;
			ic.txSize = 4;
			command_exec_tx();
			break;}
		case FC_COM_RD_UDID_2: { // read reg
			ic.txPtr = (uint8*)&is.UDID2;
			ic.txSize = 4;
			command_exec_tx();
			break;}
		case FC_COM_RD_UDID_3: { // read reg
			ic.txPtr = (uint8*)&is.UDID3;
			ic.txSize = 4;
			command_exec_tx();
			break;}
		//
		// Read flash registers before MPU is enabled
		//
		case FC_COM_RD_FLASH_SIZE: { // read reg
			ic.txPtr = (uint8*)&is.flashSize;
			ic.txSize = 2;
			command_exec_tx();
			break;}
		case FC_COM_RD_RDP: { // read reg
			ic.txPtr = (uint8*)&is.flashRDPreg;
			ic.txSize = 2;
			command_exec_tx();
			break;}
		case FC_COM_RD_WRP1: { // read reg
			ic.txPtr = (uint8*)&is.flashWRP1reg;
			ic.txSize = 2;
			command_exec_tx();
			break;}
		/*
		// Warning: do not access unless STM32F42x or STM32F43x
		case FC_COM_RD_WRP2: { // read reg
			ic.txPtr = (uint8*)&fc.flashWRP2reg;
			ic.size = 2;
			command_exec_tx();
			break;}
		*/

		
		
		

		
		// host command error
		default: 
			command_ack_host(); // debug
			break;		
	}   
	
	
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//









//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------




