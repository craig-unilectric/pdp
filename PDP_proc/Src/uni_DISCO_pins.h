//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_DISCO_pins.h
STM32F411 Discover board definitions
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef _uni_DISCO_pins_H
#define _uni_DISCO_pins_H
#ifdef __cplusplus
 extern "C" {
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 


#define LED_1_ON()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET)
#define LED_1_OFF()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET)
#define LED_1_TOGGLE()			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12)

#define LED_2_ON()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET)
#define LED_2_OFF()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET)
#define LED_2_TOGGLE()			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13)

#define LED_3_ON()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)
#define LED_3_OFF()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)
#define LED_3_TOGGLE()			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14)

#define LED_4_ON()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)
#define LED_4_OFF()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)
#define LED_4_TOGGLE()			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15)


	
#define SCOPE_1_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET)
#define SCOPE_1_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)
#define SCOPE_1_TOGGLE()			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0)

#define SCOPE_2_HIGH()			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)
#define SCOPE_2_LOW()			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)
#define SCOPE_2_TOGGLE()		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2)



// UART
#define DEBUG_UART	 			((UART_HandleTypeDef*)&huart6) // Debug monitor UART
#define DEBUG_UART_BAUD_RATE	115200


#define UART_TXRX_TIMEOUT		10 // timeout for blocking version of uart tx/rx	


#define SC_UART	 			((UART_HandleTypeDef*)&huart2) // Debug monitor UART
#define SC_UART_BAUD_RATE	115200


#define MAINS_UART	 			((UART_HandleTypeDef*)&huart1) // Debug monitor UART
#define MAINS_UART_BAUD_RATE	115200



// Relay timer
#define RELAY_TIMER	 			((TIM_HandleTypeDef*)&htim1)

/*	

Timers 1, 8, 9, 10, 11, 15, 16, 17
Timer clk = 2 * APB1 clk source
Timers 2, 3, 4, 5, 6, 7, 12, 13, 14 
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

#define RELAY_TIMERCLK()	(2 * HAL_RCC_GetPCLK1Freq())
//#define RELAY_TIMERCLK()	HAL_RCC_GetPCLK2Freq()




//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
