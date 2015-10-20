//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_PDP_pins.h
PDP board definitions
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef _uni_PDP_pins_H
#define _uni_PDP_pins_H
#ifdef __cplusplus
 extern "C" {
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 


#define LED_1_ON()				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET)
#define LED_1_OFF()				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET)
#define LED_1_TOGGLE()			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8)

#define LED_2_ON()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)
#define LED_2_OFF()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)
#define LED_2_TOGGLE()			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2)
	 
/* not on the PDP
#define LED_3_ON()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)
#define LED_3_OFF()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)
#define LED_3_TOGGLE()			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14)

#define LED_4_ON()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)
#define LED_4_OFF()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)
#define LED_4_TOGGLE()			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15)
*/

#define SCOPE_1_HIGH()				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)
#define SCOPE_1_LOW()				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)
#define SCOPE_1_TOGGLE()			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8)

#define SCOPE_2_HIGH()			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET)
#define SCOPE_2_LOW()			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET)
#define SCOPE_2_TOGGLE()		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9)


/* PDP buttons	
#define SCOPE_1_HIGH()				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET)
#define SCOPE_1_LOW()				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET)
#define SCOPE_1_TOGGLE()			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10)

#define SCOPE_2_HIGH()			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET)
#define SCOPE_2_LOW()			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET)
#define SCOPE_2_TOGGLE()		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11)
*/

//
// UART
//

#define DEBUG_UART	 			((UART_HandleTypeDef*)&huart6) // Debug monitor UART
//#define DEBUG_UART	 			((UART_HandleTypeDef*)&huart2) // SC monitor UART

#define DEBUG_UART_BAUD_RATE	115200

#define UART_TXRX_TIMEOUT		10 // timeout for blocking version of uart tx/rx	

#define SC_UART	 			((UART_HandleTypeDef*)&huart2) // SC UART
#define SC_UART_BAUD_RATE	115200

#define MAINS_UART	 			((UART_HandleTypeDef*)&huart1) // Mains UART
#define MAINS_UART_BAUD_RATE	115200


//
// I2C
//


// PDP I2C mux

#define I2CMUX_ADDR_0 0xe0
#define I2CMUX_ADDR_1 0xe2

// eeprom

#define EEPROM_I2C	 						((I2C_HandleTypeDef*)&hi2c1)
#define EEPROM_I2C_TIMEOUT  		100
 
#define EEPROM_I2C_PORT					GPIOB
#define EEPROM_I2C_PIN_SCL			GPIO_PIN_8
#define EEPROM_I2C_PIN_SDA			GPIO_PIN_7

#define EEPROM_I2C_PDPMUX_ENABLE()				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET)
#define EEPROM_I2C_PDPMUX_DISABLE()				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET)

#define EEPROM_I2C_ADDR 				0xa0




// PDP eeprom write protect
#define EEPROM_0_WRITE_ENABLE()			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET)
#define EEPROM_0_WRITE_DISABLE()			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET)




//
// Relay
//

// address - rsel_[3:0] - PD[11-8]
#define R_SEL_3_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET)
#define R_SEL_3_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET)
#define R_SEL_2_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET)
#define R_SEL_2_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET)
#define R_SEL_1_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET)
#define R_SEL_1_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)
#define R_SEL_0_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET)
#define R_SEL_0_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET)

// address latch
#define R_ALATCH_CLOSE_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)
#define R_ALATCH_CLOSE_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)
#define R_ALATCH_OPEN_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET)
#define R_ALATCH_OPEN_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET)

// drive command
#define R_DRIVE_CLOSE_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)
#define R_DRIVE_CLOSE_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)
#define R_DRIVE_OPEN_HIGH()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET)
#define R_DRIVE_OPEN_LOW()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET)

// enable - R_EN_n - active low
#define R_ENABLE()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET)
#define R_DISABLE()				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET)



// Timers
#define RELAY_TIMER	 			((TIM_HandleTypeDef*)&htim1)
#define VOLTAGE_TIMER	 			((TIM_HandleTypeDef*)&htim2)
#define VOLTAGE_CYCLE_TIMER	 			((TIM_HandleTypeDef*)&htim3)


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

//#define RELAY_TIMERCLK()	(2 * HAL_RCC_GetPCLK1Freq())
// stm32F411
//#define RELAY_TIMERCLK()	(HAL_RCC_GetPCLK2Freq())
// stm32F407
#define RELAY_TIMERCLK()	(2 * HAL_RCC_GetPCLK2Freq())
#define VOLTAGE_TIMERCLK()	(2 * HAL_RCC_GetPCLK1Freq())
#define VOLTAGE_CYCLECLK()	(2 * HAL_RCC_GetPCLK1Freq())

// peripheral clk, not timer clk
#define ADC_CLK()	(HAL_RCC_GetPCLK2Freq())



//
// Voltage
//


#define VOLTAGE_ZERO_CROSSING_PIN			GPIO_PIN_10

#define VOLTAGE_ADC_CHANNEL						0

#define CYCLE_SAMPLES_MIN							800
#define CYCLE_SAMPLES_DEFAULT					811
#define CYCLE_SAMPLES_MAX							820













//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
