//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_EVAL_pins.h
STM32F411 Discover board definitions
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef _uni_EVAL_pins_H
#define _uni_EVAL_pins_H
#ifdef __cplusplus
 extern "C" {
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
  /* Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	 
#define LED4_PIN                         GPIO_PIN_12
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK_ENABLE()           __GPIOD_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()          __GPIOD_CLK_DISABLE()  

#define LED3_PIN                         GPIO_PIN_13
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK_ENABLE()           __GPIOD_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()          __GPIOD_CLK_DISABLE()  
  
#define LED5_PIN                         GPIO_PIN_14
#define LED5_GPIO_PORT                   GPIOD
#define LED5_GPIO_CLK_ENABLE()           __GPIOD_CLK_ENABLE()  
#define LED5_GPIO_CLK_DISABLE()          __GPIOD_CLK_DISABLE()  

#define LED6_PIN                         GPIO_PIN_15
#define LED6_GPIO_PORT                   GPIOD
#define LED6_GPIO_CLK_ENABLE()           __GPIOD_CLK_ENABLE()  
#define LED6_GPIO_CLK_DISABLE()          __GPIOD_CLK_DISABLE()  




	 
#define LED_1_ON()				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define LED_1_OFF()				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define LED_1_TOGGLE()			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)

#define LED_2_ON()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define LED_2_OFF()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define LED_2_TOGGLE()		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7)



	
#define SCOPE_1_HIGH()				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define SCOPE_1_LOW()				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)
#define SCOPE_1_TOGGLE()			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3)

#define SCOPE_2_HIGH()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)
#define SCOPE_2_LOW()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define SCOPE_2_TOGGLE()		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15)



// UART
#define DEBUG_UART	 			((UART_HandleTypeDef*)&huart2) // Debug monitor UART
#define DEBUG_UART_BAUD_RATE	115200


#define UART_TXRX_TIMEOUT		10 // timeout for blocking version of uart tx/rx	


 

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
