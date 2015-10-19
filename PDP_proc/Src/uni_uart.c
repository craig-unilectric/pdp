//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_uart.c
user uart routines
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "usart.h"
#include <stdio.h>

#include "uni_defines.h" // pins, ports, types, structs, commands, memory

#include "uni_uart.h" // user defines
#include "uni_command.h" // user defines


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// memory
//


// 64-byte rx buffer for packets from UART rx
// 64 byte MAX_RX_SIZE makes the UART have the same packet size as the USB port
//#define RX_BUF_SIZE  64 	// max rx bytes for each command sent from host
uint8 sc_uart_rx_buf[SC_UART_RX_BUF_SIZE];
static bool rxdPacket = false;	
char textstr[100];





//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// HAL drivers include error interrupts which trigger for non-errors.
//
// Rewrite HAL_UART_Transmit_IT(), HAL_UART_Receive_IT() below with no error interrupts.
//
//
/**
  * @brief  Sends an amount of data in non blocking mode.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  pData: Pointer to data buffer
  * @param  Size: Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  uint32_t tmp = 0;
  
  tmp = huart->State;
  if((tmp == HAL_UART_STATE_READY) || (tmp == HAL_UART_STATE_BUSY_RX))
  {
    if((pData == NULL ) || (Size == 0)) 
    {
      return HAL_ERROR;
    }
    
    /* Process Locked */
    __HAL_LOCK(huart);
    
    huart->pTxBuffPtr = pData;
    huart->TxXferSize = Size;
    huart->TxXferCount = Size;

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    /* Check if a receive process is ongoing or not */
    if(huart->State == HAL_UART_STATE_BUSY_RX) 
    {
      huart->State = HAL_UART_STATE_BUSY_TX_RX;
    }
    else
    {
      huart->State = HAL_UART_STATE_BUSY_TX;
    }

	//
	// ST UART error interrupts are bad, do not enable them.
	//
    /* Enable the UART Parity Error Interrupt */
    //__HAL_UART_ENABLE_IT(huart, UART_IT_PE);
    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    //__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

    /* Process Unlocked */
    __HAL_UNLOCK(huart);

    /* Enable the UART Transmit data register empty Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;   
  }
}
//
/**
  * @brief  Receives an amount of data in non blocking mode 
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  pData: Pointer to data buffer
  * @param  Size: Amount of data to be received
  * @retval HAL status
  */
HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  uint32_t tmp = 0;
  
  tmp = huart->State;  
  if((tmp == HAL_UART_STATE_READY) || (tmp == HAL_UART_STATE_BUSY_TX))
  {
    if((pData == NULL ) || (Size == 0)) 
    {
      return HAL_ERROR;
    }
    
    /* Process Locked */
    __HAL_LOCK(huart);
    
    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;
    huart->RxXferCount = Size;
    
    huart->ErrorCode = HAL_UART_ERROR_NONE;
    /* Check if a transmit process is ongoing or not */
    if(huart->State == HAL_UART_STATE_BUSY_TX) 
    {
      huart->State = HAL_UART_STATE_BUSY_TX_RX;
    }
    else
    {
      huart->State = HAL_UART_STATE_BUSY_RX;
    }
    
	//
	// ST UART error interrupts are bad, do not enable them.
	//
    /* Enable the UART Parity Error Interrupt */
    //__HAL_UART_ENABLE_IT(huart, UART_IT_PE);
    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    //__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);
    
    /* Process Unlocked */
    __HAL_UNLOCK(huart);
    
    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// uart TX
//

HAL_StatusTypeDef uni_uart_transmit(uint8 *pData, uint16 Size)
{
	HAL_StatusTypeDef status;
	/*
	// blocking, requires timeout timer
	HAL_ResumeTick(); // timout timer 
	status = HAL_UART_Transmit(IC_UART, pData, (uint16)Size, UART_TXRX_TIMEOUT);
	HAL_SuspendTick();
	*/
	// uart IT
	// wait for previous tx to be complete
	while(!((SC_UART->State == HAL_UART_STATE_READY) || (SC_UART->State == HAL_UART_STATE_BUSY_RX)));
	status = UART_Transmit_IT(SC_UART, pData, (uint16)Size);
	//
	return status;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// uart RX
//

//
// rx first byte of packet
// first byte is packet size
HAL_StatusTypeDef uni_uart_receive_init(void)
{	
	HAL_StatusTypeDef status;
	rxdPacket = false; // ready to receive first byte (size byte) of packet
	status = UART_Receive_IT(SC_UART, &sc_uart_rx_buf[0], 1); // 2 bytes if using a terminal
	return status;
}

//
// rx data part of packet
HAL_StatusTypeDef uart_receive_data(uint16 dataSize)
{
	HAL_StatusTypeDef status;
	/*
	// blocking, requires timeout timer
	HAL_ResumeTick();  
	status = HAL_UART_Receive(IC_UART, &rx_buf[1], dataSize, UART_TXRX_TIMEOUT);
	HAL_SuspendTick();
	// Call command_exec_rx() directly.
	command_exec_rx(&rx_buf[1], dataSize); // prepare to execute command from host
	// Wait for next command from host.	
	ic_uart_receive_init();
	*/
	//
	// uart IT
	rxdPacket = true; // ready to receive data portion of packet
	status = UART_Receive_IT(SC_UART, &sc_uart_rx_buf[1], dataSize);	
	//
	return status;
}
//
// uart RX interrupt callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8 packetSize = sc_uart_rx_buf[0]; // size byte = packetSize
	uint8 dataSize = packetSize - 1; // packet = size byte + data bytes
	// test for valid packet size byte
	if((packetSize < 2) || (packetSize > SC_UART_RX_BUF_SIZE)) dataSize = 0;
	//
	if(huart == SC_UART)
	{
		// rx'd size
		// first byte of packet before command and data
		if(!rxdPacket && (dataSize > 0)){	
			uart_receive_data(dataSize);
		}
		// rx'd size bytes
		// complete packet
		else if(rxdPacket){
			// Host waited for ACK from the previous completed command
			// before sending new command, 			
			// so (host.command == FC_COM_NONE),
			// ready to load new command.
			command_exec_rx(&sc_uart_rx_buf[1], dataSize); // prepare to execute command from host
			// Wait for next command from host.
			uni_uart_receive_init(); 
		}
		// rx'd NULL byte from host to reset rx UART state,
		// or	rx'd bad packet (rx_size != packetSize)
		else{ 
			uni_uart_receive_init();
		}
	}	
}	
// 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// other uart unterrupt callbacks
//

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

	
}



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Error callback
//


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == SC_UART){
		//ic_log_int(" error loop: ", (uint32)debugloop);
		log_hex(" error SC_UART: ", (uint8)huart->ErrorCode);
		// reset uart
		SC_UART_BAUD(SC_UART_BAUD_RATE); // host uart
		uni_uart_receive_init(); 
	}
	if(huart == DEBUG_UART){
		log_hex(" *** error DEBUG_UART: ", (uint8)huart->ErrorCode);
		// reset uart
		DEBUG_UART_BAUD(DEBUG_UART_BAUD_RATE);
		//debug_uart_receive_init(); 
	}
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// user debug monitor routines 
//
// usage: 
// log(" hello ");
// log((const char*)&rx_buf[0]);
//
//

//
// Receive
// Read one byte directly from UART port
// No interrupts, no blocking, no timeouts
//
bool log_read(uint8* rxbyte)
{
	*rxbyte = 0;
	// read the port flags once for rx not empty
	if(__HAL_UART_GET_FLAG(DEBUG_UART, UART_FLAG_RXNE) == RESET){
		return 0;
	}
	// read one byte
	*rxbyte = (uint8)(DEBUG_UART->Instance->DR & (uint8)0xFF);
	return 1;
}


void log_(uint8* buf, uint16 size)
{
	// IT version
	while(!((DEBUG_UART->State == HAL_UART_STATE_READY) || (DEBUG_UART->State == HAL_UART_STATE_BUSY_RX)));
	HAL_UART_Transmit_IT(DEBUG_UART, (uint8*)buf, size);

	// blocking version
	//HAL_UART_Transmit(DEBUG_UART, (uint8*)buf, size, UART_TXRX_TIMEOUT);
}

void log_char(const char* str, char character)
{
	uint32 charcnt;

	// IT version
	while(!((DEBUG_UART->State == HAL_UART_STATE_READY) || (DEBUG_UART->State == HAL_UART_STATE_BUSY_RX)));
	charcnt = sprintf(textstr, "%s%c\r\n", str, character);
	HAL_UART_Transmit_IT(DEBUG_UART, (uint8_t*)textstr, charcnt);
	
	// blocking version
	//charcnt = sprintf(textstr, "%s%c\r\n", str, character);
	//HAL_UART_Transmit(DEBUG_UART, (uint8_t*)textstr, charcnt, UART_TXRX_TIMEOUT);
}

void log_str(const char* str)
{
	uint32 charcnt;
	
	// IT version
	while(!((DEBUG_UART->State == HAL_UART_STATE_READY) || (DEBUG_UART->State == HAL_UART_STATE_BUSY_RX)));
	charcnt = sprintf(textstr, "%s\r\n", str);
	HAL_UART_Transmit_IT(DEBUG_UART, (uint8_t*)textstr, charcnt);
	
	// blocking version
	//charcnt = sprintf(textstr, "%s\r\n", str);
	//HAL_UART_Transmit(DEBUG_UART, (uint8_t*)textstr, charcnt, UART_TXRX_TIMEOUT);
}

void log_int(const char* str, uint32 num)
{
	uint32 charcnt;
	
	// IT version
	while(!((DEBUG_UART->State == HAL_UART_STATE_READY) || (DEBUG_UART->State == HAL_UART_STATE_BUSY_RX)));
	charcnt = sprintf(textstr, "%s%d\r\n", str, num);
	HAL_UART_Transmit_IT(DEBUG_UART, (uint8_t*)textstr, charcnt);
	
	// blocking version
	//charcnt = sprintf(textstr, "%s%u\r\n", str, num); // unsigned // "%s%d\r\n", str, num); // signed 
	//HAL_UART_Transmit(DEBUG_UART, (uint8_t*)textstr, charcnt, UART_TXRX_TIMEOUT);
}

void log_hex(const char* str, uint32 num)
{
	uint32 charcnt;
	
	// IT version
	while(!((DEBUG_UART->State == HAL_UART_STATE_READY) || (DEBUG_UART->State == HAL_UART_STATE_BUSY_RX)));
	charcnt = sprintf(textstr, "%s0x%x\r\n", str, num);
	HAL_UART_Transmit_IT(DEBUG_UART, (uint8_t*)textstr, charcnt);
	
	// blocking version
	//charcnt = sprintf(textstr, "%s0x%x\r\n", str, num);
	//HAL_UART_Transmit(DEBUG_UART, (uint8_t*)textstr, charcnt, UART_TXRX_TIMEOUT);
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//
void test_uart(UART_HandleTypeDef *huart)
{
	uint8 val8;
	
	val8 = 0x2d;
	UART_Transmit_IT(huart, &val8, 1);
	while(!((huart->State == HAL_UART_STATE_READY) || (huart->State == HAL_UART_STATE_BUSY_RX)));
	val8 = 0x2b;
	UART_Transmit_IT(huart, &val8, 1);
	while(!((huart->State == HAL_UART_STATE_READY) || (huart->State == HAL_UART_STATE_BUSY_RX)));
	val8 = 0x2d;
	UART_Transmit_IT(huart, &val8, 1);
	while(!((huart->State == HAL_UART_STATE_READY) || (huart->State == HAL_UART_STATE_BUSY_RX)));
	
	while(1){
	
		val8 = 0x2d;
		UART_Transmit_IT(huart, &val8, 1);	
		while(!((huart->State == HAL_UART_STATE_READY) || (huart->State == HAL_UART_STATE_BUSY_RX)));
	
		val8 = 0xfe;
	
		HAL_Delay(10);
	
		if(__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) == SET){
			val8 = (uint8)(huart->Instance->DR & (uint8)0xFF);
			HAL_UART_Transmit_IT(huart, &val8, 1);
			//val8 = 0x2b;
			//UART_Transmit_IT(huart, &val8, 1);
			while(!((huart->State == HAL_UART_STATE_READY) || (huart->State == HAL_UART_STATE_BUSY_RX)));
		}
		else{
			val8 = 0x2a;
			UART_Transmit_IT(huart, &val8, 1);
			while(!((huart->State == HAL_UART_STATE_READY) || (huart->State == HAL_UART_STATE_BUSY_RX)));
		}

		LED_1_TOGGLE();
		HAL_Delay(2000);

	}	
}
//
void loopback_uart(UART_HandleTypeDef *huartmon, UART_HandleTypeDef *huarta, UART_HandleTypeDef *huartb)
{
	uint8 val8;
	
	
	while(1){
	
		val8 = 0x2b;
		UART_Transmit_IT(huartmon, &val8, 1);	
		while(!((huartmon->State == HAL_UART_STATE_READY) || (huartmon->State == HAL_UART_STATE_BUSY_RX)));

		val8 = 0x37;
		UART_Transmit_IT(huarta, &val8, 1);	
		while(!((huarta->State == HAL_UART_STATE_READY) || (huarta->State == HAL_UART_STATE_BUSY_RX)));

		val8 = 0xfe;
	
		HAL_Delay(10);
	
		if(__HAL_UART_GET_FLAG(huartb, UART_FLAG_RXNE) == SET){
			val8 = (uint8)(huartb->Instance->DR & (uint8)0xFF);
			HAL_UART_Transmit_IT(huartmon, &val8, 1);
			while(!((huartmon->State == HAL_UART_STATE_READY) || (huartmon->State == HAL_UART_STATE_BUSY_RX)));
		}
		else{
			val8 = 0x2a;
			UART_Transmit_IT(huartmon, &val8, 1);
			while(!((huartmon->State == HAL_UART_STATE_READY) || (huartmon->State == HAL_UART_STATE_BUSY_RX)));
		}

		LED_1_TOGGLE();
		HAL_Delay(2000);

	}	
}
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
