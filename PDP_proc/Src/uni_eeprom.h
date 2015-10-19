//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_eeprom.h
user eeprom declarations
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef _uni_eeprom_H
#define _uni_eeprom_H
#ifdef __cplusplus
 extern "C" {
#endif
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal_def.h"
#include "uni_defines.h"
	 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------




void eeprom_write(uint8 channel, uint8 addr, uint8* data, uint8 size);
	 
void eeprom_read(uint8 channel, uint8 addr, uint8* data, uint8 size);	 


	 


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
