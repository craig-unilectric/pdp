//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
ic_command.h
user host command declarations
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef _ic_command_H
#define _ic_command_H
#ifdef __cplusplus
 extern "C" {
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal_def.h"
#include "uni_defines.h"

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------



void command_exec_rx(uint8_t* rxBuf, uint32_t bytesRx);
	 
void uni_command_exec(void);	 
	 

	 
 

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Peeple command struct
//	 
/*
typedef struct  
{
	//---------------------------
	//
	// command parsing
	//



	//
	//---------------------------
	//	
	// commands
	//



	//	
	// DMA
	//

		
		
	//	
	// camera
	//


	
	// Peeple	
	uint8        	singleCapture;        
	uint32        	knockDelay;        
		
	
} Peeple_Command_Struct;

//
// Peeple status struct
//	 
typedef struct  
{
		
	uint8        	accelKnock;        
	
	uint8        	cameraReady;        
	uint8        	wifiReady;        

	
} Peeple_Status_Struct;
*/

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Host command
//	 

typedef struct  
{
	//---------------------------
	//
	// command parsing
	//
	uint8        	command;        
	//uint8        	val8;        
	//uint16      	val16;        
	//uint32      	val32;        
	//uint64      	val64;        
	uint8*   			txPtr;        
	uint32       	txSize;        
	uint8*   			rxPtr;        
	uint32       	rxSize;        
	uint8*     		rxBuf; 
	//
	//---------------------------
	//	
	// commands
	//
	uint8        	enable; // enable main() loop   

	
				

		
		
		
		
	
} IoTcam_Command_Struct;


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// IoTcam status
//	 
typedef struct  
{
	
     
	//uint8				dma1_complete_flag;
	//uint8				dma2_complete_flag;

     
	//uint32       	adc_samples_per_cycle;    
	//uint8       	num_ADC_bytes_per_sample;    
	//uint8       	num_ADC_channels;    
	uint32       	adc_buf_bytes_per_cycle;    
	uint32       	adc_channel_bytes_per_cycle;    

	


	
	// STM32 register values
	// write to this struct before MPU is enabled
	uint32       	chip_id;    
	uint16       	flashSize;    
	uint32       	UDID1;    
	uint32       	UDID2;    
	uint32       	UDID3;    
	uint16       	flashRDPreg;    
	uint16       	flashWRP1reg;    







} IoTcam_Status_Struct;
 

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------


void command_exec_rx(uint8_t* rxBuf, uint32_t bytesRx);

void ic_command_exec(void);


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif /* _ic_command_H */
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
