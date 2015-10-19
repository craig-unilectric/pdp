//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_adc.c
user adc routines
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "adc.h"
#include <stdio.h>

#include "uni_defines.h" // pins, ports, types, structs, commands, memory

#include "uni_adc.h" 
#include "uni_power.h" 

#include "uni_command.h"
extern IoTcam_Command_Struct ic; // IoTcam Host command
extern IoTcam_Status_Struct is; // IoTcam System status

//#include "..\..\uni_host\uni_host_structs.h" // PDP structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// memory
//
/*

Circular DMA adc_dma_buf[]
Accessed in two parts:
Half-full interrupt processes adc_dma_buf_1[]
Full interrupt processes adc_dma_buf_2[]

*/


#define CIRCULAR_DMA

uint16 adc_dma_buf[ADC_DMA_SAMPLES * 2];

uint16* adc_dma_buf_1 = &adc_dma_buf[0];
uint16* adc_dma_buf_2 = &adc_dma_buf[ADC_DMA_SAMPLES];




//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// single sample
//
uint16 uni_adc_singleSample(void)
{
	uint16 val16 = 0;
  ADC_ChannelConfTypeDef sConfig;
  //sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Channel = ADC_CHANNEL_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	sConfig.Rank = 1;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	if(HAL_ADC_Start(&hadc1) != HAL_OK) return 0;
	HAL_ADC_PollForConversion(&hadc1, 10); // timeout
	val16 = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	return val16;
}
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// DMA callback
//
// 
uint32 uni_adcDMA_bytesCaptured(uint32 maxsize)
{
	//uint32 bytes = maxsize - (uint32)__HAL_DMA_GET_COUNTER(hadc1.DMA_Handle); // dma in bytes
	uint32 bytes = (uint32)__HAL_DMA_GET_COUNTER(hadc1.DMA_Handle); // dma in bytes
	//uint32 bytes = maxsize - (uint32)((uint32)__HAL_DMA_GET_COUNTER(IC_IMAGECAPTURE_DMA) << 2); // dma in words
	return bytes;
}
//

uint32 adcDma_halfcnt = 0xff;
uint32 adcDma_fullcnt = 0xff;


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	//SCOPE_1_TOGGLE();	

	
	adcDma_fullcnt = uni_adcDMA_bytesCaptured(hadc1.Init.NbrOfConversion);
		
	
	#ifdef CIRCULAR_DMA
	
	//is.dma2_complete_flag = true;
	power_from_ADC(adc_dma_buf_2); // ****** use is.adc_samples_per_cycle here?
	
	
	// continues automatically
	#else
	HAL_ADC_Stop_DMA(&hadc1);  
	// call uni_adc_start(); to restart
	#endif			
}


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{

	//SCOPE_1_TOGGLE();
	
	//SCOPE_1_LOW();	
	//SCOPE_2_LOW();
	//SCOPE_2_TOGGLE();
	
	adcDma_halfcnt = uni_adcDMA_bytesCaptured(hadc1.Init.NbrOfConversion);
	

	#ifdef CIRCULAR_DMA
	
	
	//is.dma1_complete_flag = true;
	power_from_ADC(adc_dma_buf_1); // ****** use is.adc_samples_per_cycle here?
	
	
	
	#endif
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Error callback
//

// HAL_ADC_ErrorCallback()
// HAL_ADC_Stop_DMA()

//HAL_ADC_ErrorCallback(hadc); 


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// ADC DMA init
//

//
// uni_adc_start(); 
// do not restart DMA when using circular buffer
//
bool uni_adc_start(void)
{
	
	
	//SCOPE_1_HIGH();
	
	
	//log_int("adc channels: ", hadc1.Init.NbrOfConversion);
	//if(HAL_ADC_Start(&hadc1) != HAL_OK) return 0; 
	//if(HAL_ADC_Start_DMA(&hadc1, (uint32*)adc_dma_buf, ADC_DMA_SAMPLES) != HAL_OK) return 0;
	
	
	//
	// *** set DMA length - by cycle length ???
	//
	
	
	// if the number of samples is fixed
	if(HAL_ADC_Start_DMA(&hadc1, (uint32*)adc_dma_buf, ADC_DMA_SAMPLES * 2) != HAL_OK) return 0; // x2 for circular
	
	
	// no does not work for circular with half buffers
	// if the number of samples is set by measuring a cycle using zero-crossing etc.
	//if(HAL_ADC_Start_DMA(&hadc1, (uint32*)adc_dma_buf, is.adc_samples_per_cycle * 2) != HAL_OK) return 0; // x2 for circular

	
	
	// disable half-full interrupt
	//__HAL_DMA_DISABLE_IT(hadc1.DMA_Handle, DMA_IT_HT); 
	
	
	return 1;
}


bool uni_adc_stop(void)
{
	//if(HAL_ADC_Stop(&hadc1) != HAL_OK) return 0; 
	if(HAL_ADC_Stop_DMA(&hadc1) != HAL_OK) return 0;
	return 1;
}

/* bad, use hal_adc_deinit..., not HAL_ADC_MspDeInit()
void uni_adc_deinit(void)
{
	HAL_ADC_MspDeInit(&hadc1);
	hadc->Lock = HAL_UNLOCKED;
}
*/
/*

uni_adc_init() is for convenience,
user can change most ADC options here.

ADC clock source is APB2

uint32 timerclk = HAL_RCC_GetPCLK2Freq();

*/

void uni_adc_init(void)
{

  ADC_ChannelConfTypeDef sConfig;
	
	
	//-------------------------------------
	
	
	HAL_DMA_DeInit(hadc1.DMA_Handle);
	HAL_ADC_DeInit(&hadc1);
	
	
	//-------------------------------------
	

	// Configure the global features of the ADC 
	// (Clock, Resolution, Data Alignment and number of conversion) 
  
	
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION12b;

  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;

	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 16;
  hadc1.Init.DMAContinuousRequests = ENABLE; 
  hadc1.Init.EOCSelection = EOC_SINGLE_CONV;

  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;

	#ifdef CIRCULAR_DMA
	// circular buffer with HAL_ADC_ConvCpltCallback() 
	// and (if not disabled) HAL_ADC_ConvHalfCpltCallback()
	// Note: set hdma_adc1.Init.Mode = DMA_CIRCULAR; in adc.c
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1; // dummy value
	#else
	// one buffer sample with HAL_ADC_ConvCpltCallback()
	// Note: set hdma_adc1.Init.Mode = DMA_NORMAL; in adc.c
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	#endif
	
  HAL_ADC_Init(&hadc1);
	
	
	sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;


    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
	sConfig.Channel = ADC_CHANNEL_0; 
  sConfig.Rank = 1;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  //sConfig.Channel = ADC_CHANNEL_TEMPSENSOR; 
 //sConfig.Channel = ADC_CHANNEL_VBAT;
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  //sConfig.Channel = ADC_CHANNEL_VREFINT;
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = 3;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 6;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 7;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 8;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 9;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 10;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 11;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 12;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 13;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 14;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 15;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = 16;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);


}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
11.8.1 Using the DMA
Since converted regular channel values are stored into a unique data register, it is useful to
use DMA for conversion of more than one regular channel. This avoids the loss of the data
already stored in the ADC_DR register.
When the DMA mode is enabled (DMA bit set to 1 in the ADC_CR2 register), after each
conversion of a regular channel, a DMA request is generated. This allows the transfer of the
converted data from the ADC_DR register to the destination location selected by the
software.
Despite this, if data are lost (overrun), the OVR bit in the ADC_SR register is set and an
interrupt is generated (if the OVRIE enable bit is set). DMA transfers are then disabled and
DMA requests are no longer accepted. In this case, if a DMA request is made, the regular
conversion in progress is aborted and further regular triggers are ignored. It is then
necessary to clear the OVR flag and the DMAEN bit in the used DMA stream, and to reinitialize
both the DMA and the ADC to have the wanted converted channel data transferred
to the right memory location. Only then can the conversion be resumed and the data
transfer, enabled again. Injected channel conversions are not impacted by overrun errors.
When OVR = 1 in DMA mode, the DMA requests are blocked after the last valid data have
been transferred, which means that all the data transferred to the RAM can be considered
as valid.
At the end of the last DMA transfer (number of transfers configured in the DMA controller’s
DMA_SxNTR register):
• No new DMA request is issued to the DMA controller if the DDS bit is cleared to 0 in the
ADC_CR2 register (this avoids generating an overrun error). However the DMA bit is
not cleared by hardware. It must be written to 0, then to 1 to start a new transfer.
• Requests can continue to be generated if the DDS bit is set to 1. This allows
configuring the DMA in double-buffer circular mode.
To recover the ADC from OVR state when the DMA is used, follow the steps below:
1. Reinitialize the DMA (adjust destination address and NDTR counter)
2. Clear the ADC OVR bit in ADC_SR register
3. Trigger the ADC to start the conversion.
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
