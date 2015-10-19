//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_eeprom.c
user eeprom routines
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "i2c.h"
#include <stdio.h>

#include "uni_defines.h" // pins, ports, types, structs, commands, memory

#include "uni_eeprom.h" 


#include "uni_command.h"
extern IoTcam_Command_Struct ic; // IoTcam Host command
extern IoTcam_Status_Struct is; // IoTcam System status

//#include "..\..\uni_host\uni_host_structs.h" // PDP structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//

//pc.channel_number

//
void set_i2c_mux(uint8 channel)
{
	
	static uint8 controlval1;
	static uint8 controlval0;

	// clear both I2C mux
	EEPROM_I2C_PDPMUX_DISABLE();
	HAL_Delay(10);
	EEPROM_I2C_PDPMUX_ENABLE();
	
	//
	// select I2C channel
	//
	
	if((channel >= 0) && (channel <= 7)){	
		controlval1 = 0xf0;
		HAL_I2C_Master_Transmit(EEPROM_I2C, I2CMUX_ADDR_1, &controlval1, 1, EEPROM_I2C_TIMEOUT);
		controlval0 = 0xf8 + (channel & 0x07);
		HAL_I2C_Master_Transmit(EEPROM_I2C, I2CMUX_ADDR_0, &controlval0, 1, EEPROM_I2C_TIMEOUT);		
	}
	else if((channel >= 8) && (channel <= 15)){		
		controlval0 = 0xf0;
		HAL_I2C_Master_Transmit(EEPROM_I2C, I2CMUX_ADDR_0, &controlval0, 1, EEPROM_I2C_TIMEOUT);
		controlval1 = 0xf8 + (channel & 0x07);
		HAL_I2C_Master_Transmit(EEPROM_I2C, I2CMUX_ADDR_1, &controlval1, 1, EEPROM_I2C_TIMEOUT);		
	}
}
//
void eeprom_write(uint8 channel, uint8 addr, uint8* data, uint8 size)
{
	
	set_i2c_mux(channel);
	
	// ********* enable eeproms for writes ???
	EEPROM_0_WRITE_ENABLE();	
	
	HAL_Delay(1);
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, data, size, EEPROM_I2C_TIMEOUT);
	HAL_Delay(10); // 5 mS max write time
	
	EEPROM_0_WRITE_DISABLE();	

}
//
void eeprom_read(uint8 channel, uint8 addr, uint8* data, uint8 size)
{

	set_i2c_mux(channel);
	
	*data = 0xfe; // error detect
	HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, data, size, EEPROM_I2C_TIMEOUT);
	
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


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
