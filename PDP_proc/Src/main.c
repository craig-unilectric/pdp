/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

//#include "..\..\uni_host\fc_host_commands.h" // PDP commands, flash addresses

#include "uni_defines.h" // pins, ports, types, structs, commands, memory
#include "uni_uart.h"
#include "uni_adc.h"
#include "uni_relay.h"
#include "uni_voltage.h"
#include "uni_power.h"
#include "uni_eeprom.h"


#include "uni_command.h"
extern IoTcam_Command_Struct ic; // IoTcam Host command
extern IoTcam_Status_Struct is; // IoTcam System status


//#include "..\..\uni_host\uni_host_structs.h" // PDP structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;
extern PDP_Status_Struct ps_temp;




/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	
	
	uint8 val8;
	uint32 i;
	uint32 val32;		
	
	//
	// init structs
	//

	//
	// system control struct
	//

	//
	// system status struct
	//
	is.flashSize = *(__IO uint16_t*)STM32REG_FLASHSIZE_ADDR;
	is.UDID1 = *(__IO uint32_t*)STM32REG_UDID1_ADDR;
	is.UDID2 = *(__IO uint32_t*)STM32REG_UDID2_ADDR;
	is.UDID3 = *(__IO uint32_t*)STM32REG_UDID3_ADDR;
	is.flashRDPreg = *(__IO uint16_t*)STM32REG_RDP_ADDR;
	is.flashWRP1reg = *(__IO uint16_t*)STM32REG_WRP1_ADDR;
	is.chip_id = *(__IO uint32_t*)STM32REG_IDCODE_ADDR;
	

	is.adc_buf_bytes_per_cycle = 0;
	is.adc_channel_bytes_per_cycle = 0;
	
	// 
	// PDP command struct
	//
	pc.channel_number = 0;
	

	//
	// init Status
	//

	for(i = 0;i < PDP_RELAY_CHANNELS;i++){
		ps_temp.dc[i] = 0;	
		ps_temp.pk[i] = 0;	
		ps_temp.rms[i] = 0;		
		ps_temp.real[i] = 0;		
		ps_temp.apparent[i] = 0;	
		ps_temp.pf[i] = 0;	
    ps_temp.relay_state[i] = 0;	// *** read state from flash?
	}
	
	//
	// PDP status struct
	//
	
	ps_temp.voltage_cycle_time = 0;
	ps_temp.voltage_cycle_samples = CYCLE_SAMPLES_DEFAULT;

	

	
	//
	// init power math
	//
	// init with ADC start instead
	//power_init_math();
	
	
	
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();

  /* USER CODE BEGIN 2 */
	//--------------------------------------------
	//
	// init
	//

	LED_1_OFF();
	LED_2_OFF();

	// init relay control signals
	relay_init();

	// init uarts
	DEBUG_UART_BAUD(DEBUG_UART_BAUD_RATE); // debug uart
	SC_UART_BAUD(SC_UART_BAUD_RATE); // host uart
	MAINS_UART_BAUD(MAINS_UART_BAUD_RATE); // mains uart

	// init host (Site Controller) command rx
	uni_uart_receive_init(); // start PDP command rx

	//
	// ADC init
	//
	// re-init adc with user values
	uni_adc_init();

	//
	// debug monitor
	//
	log_str("//----------------------");
	log_str("");
	log_str("hello PDP");
	log_str("");
	
	
	voltage_timer_init();
	
	HAL_Delay(100);
	

	log_int("APB1 timers HAL_RCC_GetPCLK1Freq: ", VOLTAGE_TIMERCLK());
	log_int("ADC clk: ", ADC_CLK());
	
	

	
/*	
		while(1){
			
			
			val32 = is.adc_samples_per_cycle;		
			log_int("  adc samples ", val32);
			
			val32 = ps.voltage_cycle_time;		
			log_int("  voltage timer ", val32);
			val32 = ps.voltage_cycle_samples;		
			log_int("  voltage samples ", val32);
			log_str("");
			HAL_Delay(2000);
			
		}
	
*/	
	
	
	
	//
	// LED test
	//
	/*
	while(1){
		LED_2_ON();
		SCOPE_2_HIGH();	
		HAL_Delay(1000);
		LED_2_OFF();
		SCOPE_2_LOW();	
		HAL_Delay(1000);	
	}
	*/
	
	//
	// UART tests
	//
	
	//test_uart(DEBUG_UART);
	// turn off SC_UART callback() vefore SC_UART test
	//test_uart(SC_UART);
	//loopback_uart(SC_UART, DEBUG_UART, MAINS_UART);
	//loopback_uart(SC_UART, MAINS_UART, DEBUG_UART);
		
	//
	// I2C test
	//
	/*
	log_str("eeprom test");

	val8 = 0x25;
	//eeprom_write(uint8 channel, uint8 addr, uint8* data, uint8 size)
	eeprom_write(0, 0, &val8, 1);
	val8 = 0x62;
	eeprom_write(9, 1, &val8, 1);
	
	while(1){

		//eeprom_read(uint8 channel, uint8 addr, uint8* data, uint8 size)
		eeprom_read(0, 0, &val8, 1);
		log_hex("  eeprom read 0 ", val8);
		eeprom_read(9, 1, &val8, 1);
		log_hex("  eeprom read 1 ", val8);
		eeprom_read(9, 0, &val8, 1);
		log_hex("  eeprom read 00 ", val8);

		
		LED_1_TOGGLE();
		HAL_Delay(3000);
		
	}
	*/
	
	//
	// relay test
	//
	/*
	pc.channel_number = 1;

	while(1){
	
		LED_1_TOGGLE();
		uni_relay_on();		
		HAL_Delay(2000);
		
		LED_1_TOGGLE();
		uni_relay_off();		
		HAL_Delay(2000);
		
	}
	*/
	
	//
	// clock test
	//
	/*
	log_int("HAL_RCC_GetHCLKFreq: ", HAL_RCC_GetHCLKFreq());
	log_int("APB1 timers HAL_RCC_GetPCLK1Freq: ", HAL_RCC_GetPCLK1Freq() * 2);
	log_int("APB2 timers HAL_RCC_GetPCLK2Freq: ", HAL_RCC_GetPCLK2Freq());
	log_str("");
	log_int("Command struct size: ", sizeof(PDP_Command_Struct));
	log_int("Status struct size: ", sizeof(PDP_Status_Struct));
	log_str("");
	*/	
	
	//
	// single adc test
	//
	/*
	uni_adc_init();
	HAL_Delay(1000);
	val32 = uni_adc_singleSample();
	log_int("  single sample  ", val32);
	while(1);
	*/
	

	// adc start
	power_init_math();
	uni_adc_start();
	


	// sync to zero crossing

	
	
	
	ic.enable = true;




	//
	// end init
	//
	//--------------------------------------------
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//-----------------------------------------------
	//-----------------------------------------------
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	


			
	if(ic.command){			
		uni_command_exec(); // execute host command
		
		
		
		
	}
			
	
	if(ic.enable){
		LED_1_TOGGLE();
	}
	else{
		LED_1_OFF();
	}
		
	HAL_Delay(100);		



	//uni_adc_start(); // do not restart DMA when using circular buffer
 		

  }
	//-----------------------------------------------
	//-----------------------------------------------
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

/* USER CODE BEGIN 4 */
/*
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}
*/
/*
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}
*/
/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
