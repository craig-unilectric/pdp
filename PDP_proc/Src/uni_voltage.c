//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_voltage.c
user voltage routines
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "tim.h"
#include <stdio.h>

#include "uni_defines.h" // pins, ports, types, structs, commands, memory

#include "uni_voltage.h" 
 


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
// memory
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
// EXTI callback
//
// 
//
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
//
//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	
  if(GPIO_Pin == VOLTAGE_ZERO_CROSSING_PIN)
  {

			HAL_TIM_Base_Stop(VOLTAGE_TIMER);
			ps_temp.voltage_cycle_time = __HAL_TIM_GET_COUNTER(VOLTAGE_TIMER);
			__HAL_TIM_SET_COUNTER(VOLTAGE_TIMER, 0);
			HAL_TIM_Base_Start(VOLTAGE_TIMER);
		
			HAL_TIM_Base_Stop(VOLTAGE_CYCLE_TIMER);
			ps_temp.voltage_cycle_samples = __HAL_TIM_GET_COUNTER(VOLTAGE_CYCLE_TIMER);
			__HAL_TIM_SET_COUNTER(VOLTAGE_CYCLE_TIMER, 0);
			HAL_TIM_Base_Start(VOLTAGE_CYCLE_TIMER);


		//SCOPE_2_TOGGLE();
		
  }
	
}
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//


// VOLTAGE_TIMERCLK()

// Current = TIM_GetCapture1(TIM3);

//VOLTAGE_CYCLECLK
//VOLTAGE_CYCLE_TIMER

void voltage_timer_init(void)
{

	//
  // Voltage Timer
	//
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  VOLTAGE_TIMER->Init.Prescaler = VOLTAGE_TIMERCLK()/100000; // clk/100000 for 10uS // clk/1000000; for uS
  VOLTAGE_TIMER->Init.CounterMode = TIM_COUNTERMODE_UP;
  VOLTAGE_TIMER->Init.Period = 0xffff;
  VOLTAGE_TIMER->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(VOLTAGE_TIMER);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(VOLTAGE_TIMER, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(VOLTAGE_TIMER, &sMasterConfig);
	
	//
  // Voltage Cycle Count
	//
	//TIM_ClockConfigTypeDef sClockSourceConfig;
  //TIM_MasterConfigTypeDef sMasterConfig;
	
	//
	// TBD
	// We want to somehow make timer cycles agree with ADC cycles
	// otherwise use trial and error
	//
	//val32 = 4 * 15 * 15 * 16; adc cycles per 16 channels
	//log_int("ADC clks per sample: ", val32);
  //
	VOLTAGE_CYCLE_TIMER->Init.Prescaler = 1727; // by trial and error
  VOLTAGE_CYCLE_TIMER->Init.CounterMode = TIM_COUNTERMODE_UP;
  VOLTAGE_CYCLE_TIMER->Init.Period = 0xffff;
  VOLTAGE_CYCLE_TIMER->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(VOLTAGE_CYCLE_TIMER);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(VOLTAGE_CYCLE_TIMER, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(VOLTAGE_CYCLE_TIMER, &sMasterConfig);
	
}

//



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//


/*
	
			val32 = is.adc_samples_per_cycle;		
			log_int("  adc samples ", val32);
			
			val32 = ps.voltage_cycle_time;		
			log_int("  voltage timer ", val32);
			val32 = ps.voltage_cycle_samples;		
			log_int("  voltage samples ", val32);
			log_str("");
	
			HAL_Delay(2000);	
*/	


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
