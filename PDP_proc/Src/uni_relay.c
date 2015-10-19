//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_relay.c
user relay routines
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "tim.h"
#include <stdio.h>

#include "uni_defines.h" // pins, ports, types, structs, commands, memory

#include "uni_relay.h" 

#include "uni_command.h"
extern IoTcam_Command_Struct ic; // IoTcam Host command
extern IoTcam_Status_Struct is; // IoTcam System status

//#include "..\..\uni_host\uni_host_structs.h" // PDP structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;
extern PDP_Status_Struct ps_temp;



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 

// always use timer
// uncomment this define
#define USE_TIMER_FOR_RELAY_PULSE

// relay pulse duration in mS
#define RELAY_PULSE_MS	80
//#define RELAY_PULSE_MS	1000





//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//
//
void end_pulse(void)
{

	LED_2_OFF();
	SCOPE_2_LOW();	
	
	
	// clear relay control signals
	//R_DISABLE();
	R_DRIVE_OPEN_HIGH();
	R_DRIVE_CLOSE_HIGH();
	
}
//
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == RELAY_TIMER){
		// end pulse
		end_pulse();
	}
}
//
/*	

Timers 2, 3, 4, 5, 6, 7, 12, 13, 14 
Timer clk = 2 * APB1 clk source
Timers 1, 8, 9, 10, 11, 15, 16, 17
Timer clk = APB2 clk source - stm32f411
Timer clk = 2 * APB2 clk source - stm32f407

log_int("HAL_RCC_GetHCLKFreq: ", HAL_RCC_GetHCLKFreq());
log_int("APB1 timers HAL_RCC_GetPCLK1Freq: ", 2 * HAL_RCC_GetPCLK1Freq());
log_int("APB2 timers HAL_RCC_GetPCLK2Freq: ", HAL_RCC_GetPCLK2Freq());

stm32f411
uint32 timerclk = 2 * HAL_RCC_GetPCLK1Freq();
uint32 timerclk = HAL_RCC_GetPCLK2Freq();

stm32f407
uint32 timerclk = 2 * HAL_RCC_GetPCLK1Freq();
uint32 timerclk = 2 * HAL_RCC_GetPCLK2Freq();

*/
//
void start_pulse(void)
{
	//
	const uint16 mS_delay = RELAY_PULSE_MS;//1000; // set mS timer period
	//
	// get real timer clk
	uint32 timerclk = RELAY_TIMERCLK();
	//
	const uint32 divmS = 1000;
	const uint32 div10 = 10;
	uint32 prescaler = timerclk/divmS; // clks/mS (> 16 bits)
	prescaler = prescaler/div10; // clks/mS/10 = 16 bit integer
	//
	// converts sysclk to mS
	//	
	// Period
  RELAY_TIMER->Instance->ARR = (mS_delay * div10) - 1; // counts down to 0, so subtract 1
	// Prescalar
  //RELAY_TIMER->Init.Prescaler = 10000 - 1; // counts down to 0, so subtract 1
  RELAY_TIMER->Instance->PSC = prescaler - 1; // counts down to 0, so subtract 1	
	//
	// start timer
	__HAL_TIM_CLEAR_IT(RELAY_TIMER, 0xff);
	HAL_TIM_Base_Start_IT(RELAY_TIMER);
	//		
}
//
void relay_init(void)
{
	// clear control signals
	R_DISABLE();
	R_DRIVE_OPEN_HIGH();
	R_DRIVE_CLOSE_HIGH();
	// init timer pulse with first call
	start_pulse();
}
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//
void relay_addr(uint8 addr)
{
	// clear relay control signals
	//R_DISABLE();
	R_DRIVE_OPEN_HIGH();
	R_DRIVE_CLOSE_HIGH();

	// relay mux
	if(addr & 0x08) R_SEL_3_HIGH();
	else R_SEL_3_LOW();
	if(addr & 0x04) R_SEL_2_HIGH();
	else R_SEL_2_LOW();
	if(addr & 0x02) R_SEL_1_HIGH();
	else R_SEL_1_LOW();
	if(addr & 0x01) R_SEL_0_HIGH();
	else R_SEL_0_LOW();
	
	//
	// ******* fix PDP schematic error, mux channels scrambled
	//
	if(addr == 0x01){
		R_SEL_0_LOW();
		R_SEL_1_HIGH();
	}
	else if(addr == 0x02){
		R_SEL_0_HIGH();
		R_SEL_1_LOW();
	}


}
//
void uni_relay_on(void)
{
	// select pc.channel_number
	// select "on" relay coil
	// start pulse
	
	// set address
	relay_addr(pc.channel_number);
	// latch address
	R_ALATCH_CLOSE_HIGH();
	HAL_Delay(1);	
	R_ALATCH_CLOSE_LOW();

	// set relay close signal
	R_DRIVE_CLOSE_LOW();
	
	// enable pulse to relay coil
	// timer interrupt will disable pulse after delay
	LED_2_ON();
	SCOPE_2_HIGH();	
	#ifdef USE_TIMER_FOR_RELAY_PULSE
	start_pulse();
	#else
	HAL_Delay(RELAY_PULSE_MS);
	// clear relay control signals
	//R_DISABLE();
	R_DRIVE_OPEN_HIGH();
	R_DRIVE_CLOSE_HIGH();
	LED_2_OFF();
	SCOPE_2_LOW();	
	#endif
	
	// store relay state
	ps_temp.relay_state[pc.channel_number] = 1;	
}
//
void uni_relay_off(void)
{
	// select pc.channel_number
	// select "off" relay coil
	// start pulse

	// set address
	relay_addr(pc.channel_number);
	// latch address
	R_ALATCH_OPEN_HIGH();
	HAL_Delay(1);		
	R_ALATCH_OPEN_LOW();
	
	// set relay open signal
	R_DRIVE_OPEN_LOW();

	// enable pulse to relay coil
	// timer interrupt will disable pulse after delay
	LED_2_ON();
	SCOPE_2_HIGH();	
	#ifdef USE_TIMER_FOR_RELAY_PULSE
	start_pulse();
	#else
	HAL_Delay(RELAY_PULSE_MS);
	// clear relay control signals
	//R_DISABLE();
	R_DRIVE_OPEN_HIGH();
	R_DRIVE_CLOSE_HIGH();
	LED_2_OFF();
	SCOPE_2_LOW();	
	#endif

	// store relay state
	ps_temp.relay_state[pc.channel_number] = 0;
}
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//





//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//
//



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
