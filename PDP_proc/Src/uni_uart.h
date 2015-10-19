//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_uart.h
user uart declarations
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef _uni_uart_H
#define _uni_uart_H
#ifdef __cplusplus
 extern "C" {
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal_def.h"
#include "uni_defines.h"
	 

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
	 
	 
// Set the uart baud rate
#define SC_UART_BAUD(x)				{ SC_UART->Init.BaudRate = x; HAL_UART_Init(SC_UART); }
#define DEBUG_UART_BAUD(x)			{ DEBUG_UART->Init.BaudRate = x; HAL_UART_Init(DEBUG_UART); }
#define MAINS_UART_BAUD(x)			{ MAINS_UART->Init.BaudRate = x; HAL_UART_Init(MAINS_UART); }
	 	


void test_uart(UART_HandleTypeDef *huart);
void loopback_uart(UART_HandleTypeDef *huartmon, UART_HandleTypeDef *huarta, UART_HandleTypeDef *huartb);



HAL_StatusTypeDef uni_uart_transmit(uint8 *pData, uint16 Size);	 
	 
HAL_StatusTypeDef uni_uart_receive_init(void);	



HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);


bool log_read(uint8* rxbyte);	 
void log_(uint8* buf, uint16 size);
void log_str(const char* str);
void log_char(const char* str, char character);
void log_int(const char* str, uint32 num);
void log_hex(const char* str, uint32 num);









//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
