//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
fc_host_commands.h
4Cam host commands
file shared with host compiler
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef __FC_HOST_COMMANDS_H
#define __FC_HOST_COMMANDS_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

	 
	 
// ChipSight Feature list size, Region list size
#define FC_MAX_FEAT 1000
	 
	 
	 
	 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Host Commands
//
//----------------------------------
// system commands
#define FC_COM_NONE							0x00
// system reset
#define FC_COM_WR_STM32_RESET		0x01
// system control
#define FC_COM_WR_ENABLE				0x02
#define FC_COM_WR_DISABLE				0x03
// STM32 info
#define FC_COM_RD_CHIP_ID				0x05
#define FC_COM_RD_FLASH_SIZE		0x06
#define FC_COM_RD_UDID_1				0x07
#define FC_COM_RD_UDID_2				0x08
#define FC_COM_RD_UDID_3				0x09
// system test
#define FC_COM_WR_COMMS_TEST		0x0a
#define FC_COM_WR_CONTINUE			0x0b
#define FC_COM_RD_COMMS_TEST		0x0c
	
//----------------------------------
// data capture
#define FC_COM_RD_I_RAW_SIZE  	0x10
#define FC_COM_RD_I_RAW    			0x11

// select PDP channel
#define FC_COM_WR_CHANNEL_NUMBER		0x12

#define FC_COM_RD_CHANNEL_I_RAW_SIZE  	0x13
#define FC_COM_RD_CHANNEL_I_RAW    			0x14
#define FC_COM_RD_CHANNEL_I_FILTERED    			0x17



#define FC_COM_WR_COMMAND_STRUCT 	0x15
#define FC_COM_RD_STATUS_STRUCT		0x16



//----------------------------------
// Relay control

#define FC_COM_RD_RELAY_STATE  		0x20
#define FC_COM_WR_RELAY_ON  		0x21
#define FC_COM_WR_RELAY_OFF  		0x22







//----------------------------------
// image capture
#define FC_COM_RD_IMAGE    		0x100
//#define FC_COM_RD_IMAGE_NOW  		0x110
#define FC_COM_RD_BKG	  			0x110
#define FC_COM_RD_MOTBUF	  	0x120

#define FC_COM_RD_JPG_SIZE		0x130
#define FC_COM_RD_JPG					0x140





//----------------------------------
// camera commands
#define FC_COM_WR_RGB565_160x120			0x120
#define FC_COM_WR_RGB565_320x240			0x121
#define FC_COM_WR_RGB565_640x480			0x122
//#define FC_COM_WR_JPEG_320x240			0x123
#define FC_COM_WR_JPEG_640x480				0x124
#define FC_COM_WR_JPEG_800x600				0x125
// host sends color bar command to camera
// enable/disable is in byte after command
#define FC_COM_WR_CAMERA_TEST_PATTERN	0x126

#define FC_COM_WR_CAMERA_WRREG				0x127
#define FC_COM_WR_CAMERA_RDREG				0x128
#define FC_COM_RD_CAMERA_REG					0x129
#define FC_COM_WR_CAMERA_I2C_RESET		0x12a
#define FC_COM_WR_CAMERA_I2C_SPEED		0x12b

// camera status
#define FC_COM_RD_CAMERA_READY				0x12c
#define FC_COM_RD_FRAMERATE	  				0x12d

// camera format with zoom
#define FC_COM_WR_JPEG_ZOOM_640x480		0x12e
#define FC_COM_WR_JPEG_ZOOM_800x600		0x12f

// camera power
#define FC_COM_WR_CAMERA_POWER_DOWN		0x190
#define FC_COM_WR_CAMERA_POWER_UP			0x191

// camera exposure
#define FC_COM_WR_CAMERA_EXPOSURE			0x192


//----------------------------------
// I2C Master - Accelerometer commands



#define FC_COM_WR_ACCEL_WRREG 	0x194
#define FC_COM_WR_ACCEL_RDREG 	0x195 
#define FC_COM_RD_ACCEL_REG 		0x196



//----------------------------------
// Feature Encoder Output commands
#define FC_COM_RD_FE_FCNT				0x130
#define FC_COM_RD_FE_FSIZE			0x131
#define FC_COM_RD_FE_FLIST			0x132
#define FC_COM_RD_FE_RCNT				0x133
#define FC_COM_RD_FE_RSIZE			0x134
#define FC_COM_RD_FE_RLIST			0x135
#define FC_COM_RD_FE_FILTCNT		0x136
//#define FC_COM_WR_FE_PRECEN		0x138
//#define FC_COM_WR_FE_MINRES		0x139
//#define FC_COM_WR_FE_MAXRES		0x13a
//----------------------------------
// CS_Settings commands
//
// Read/write the whole struct of settings
#define FC_COM_RD_CHIPSIGHT_SETTINGS_SIZE	0x40
#define FC_COM_WR_CHIPSIGHT_SETTINGS			0x41
#define FC_COM_RD_CHIPSIGHT_SETTINGS			0x42
//
// Filters
//#define FC_COM_WR_PIXEL_FILTER_ENABLE		0x43
//#define FC_COM_WR_PIXEL_FILTER_MINSIZE		0x44
#define FC_COM_WR_SEG_FILTER_ENABLE				0x45
#define FC_COM_WR_SEG_FILTER_POPMINSIZE		0x46
#define FC_COM_WR_SEG_FILTER_GAPMINSIZE		0x47
#define FC_COM_WR_REGION_FILTER_ENABLE		0x48
#define FC_COM_WR_REGION_FILTER_MINSIZE		0x49 
// 
// Motion
#define FC_COM_WR_MOTION_ENABLE			0x4a
#define FC_COM_WR_MOTION_THRESH			0x4b
#define FC_COM_WR_MOTION_BKGUPDATE_ENABLE	0x4c
#define FC_COM_WR_MOTION_BKGUPDATE_RATE	0x4d
#define FC_COM_WR_MOTION_BKGUPDATE_FRAME	0x4e
#define FC_COM_WR_MOTION_NEW_BKG		0x4f
//
// Feature Precision
#define FC_COM_WR_PREC_ENABLE	0x50
#define FC_COM_WR_PREC			0x51
#define FC_COM_WR_MAXRES		0x52
#define FC_COM_WR_MINRES		0x53
//
// Color
#define FC_COM_WR_CLUT_ENABLE	0x54
#define FC_COM_WR_CLUT			0x55
#define FC_COM_RD_CLUT			0x56




//
//----------------------------------
// 





//----------------------------------
// Luminance
#define FC_COM_RD_PWM				0x70
#define FC_COM_RD_OCCFRAMECNT		0x71
#define FC_COM_WR_OCCLUMINIT		0x72
#define FC_COM_RD_EXP				0x73
#define FC_COM_RD_GAIN				0x74
#define FC_COM_RD_EXPGAIN			0x75
#define FC_COM_RD_EXPGAIN_STABLE	0x76
#define FC_COM_RD_AUTOEXP_READY		0x77
#define FC_COM_RD_BRIGHTNESS		0x78
#define FC_COM_RD_LUMINANCE			0x79
#define FC_COM_WR_CLKDIV			0x7a
#define FC_COM_RD_CLKDIV			0x7b
#define FC_COM_WR_BRTSCALE			0x7c


//
//----------------------------------
// User application commands

#define FC_COM_RD_APP_REGION_TOTAL_COUNT	0x80
#define FC_COM_RD_APP_REGION_TOTAL_SIZE		0x81
#define FC_COM_WR_APP_REGION_FILTER_HIGH	0x82
#define FC_COM_WR_APP_REGION_FILTER_LOW		0x83
#define FC_COM_RD_APP_REGION_FILTERED_COUNT	0x84
#define FC_COM_RD_APP_REGION_FILTERED_SIZE	0x85
#define FC_COM_RD_APP_REGION_MIDDLE_COUNT	0x86


//----------------------------------
//----------------------------------
//
// SD card commands
//

#define FC_COM_WR_SD_ENABLE		0xa0
#define FC_COM_RD_SD_DETECTED	0xa1
#define FC_COM_RD_SD_READY		0xa2
#define FC_COM_RD_SD_CAPACITY	0xa3
#define FC_COM_RD_SD_BLOCKSIZE	0xa4
#define FC_COM_RD_SD_BLOCKCOUNT	0xa5
#define FC_COM_RD_SD_IMAGECOUNTER	0xa6
#define FC_COM_RD_SD_IMAGEMAX	0xa7
#define FC_COM_RD_SD_GOODWRITE	0xa8
// image capture
#define FC_COM_WR_SD_IMAGENUMBER		0xa9
//#define FC_COM_RD_SD_IMAGE  	0xaa
#define FC_COM_RD_SD_GOODREAD	0xab





//----------------------------------
//----------------------------------






//----------------------------------
//----------------------------------
//
// Flash commands - 0xc0-0xdf
//
#define FC_COM_WR_JUMPTO_BOOT	0xc0
#define FC_COM_WR_JUMPTO_USER	0xc1
#define FC_COM_RD_IN_BOOT		0xc2
#define FC_COM_WR_BINFILE_SIZE	0xc3
#define FC_COM_RD_BINFILE_SIZE	0xc4
#define FC_COM_WR_BINFILE_ADDR	0xc5
#define FC_COM_RD_BINFILE_ADDR	0xc6
#define FC_COM_WR_BINFILE		0xc7
#define FC_COM_RD_BINFILE		0xc8
#define FC_COM_WR_FLASH_READY	0xc9
#define FC_COM_RD_FLASH_READY	0xca
#define FC_COM_WR_PROG_FLASH	0xcb
#define FC_COM_RD_PROG_FLASH_DONE	0xcc
#define FC_COM_RD_PROG_FLASH_GOOD	0xcd

#define FC_COM_RD_FLASH_BOOT	0xce				
#define FC_COM_RD_FLASH_IP		0xcf				
#define FC_COM_RD_FLASH_USER	0xe0				

#define FC_COM_RD_USER_ADDR		0xe1				

// STM32 option bytes
#define FC_COM_RD_RDP			0xe2
#define FC_COM_RD_WRP1			0xe3 				
// warning do not access WRP2 unless STM32F42x or STM32F43x
//#define FC_COM_RD_WRP2			0xe4				


// I2C address in flash
#define FC_COM_WR_I2CADDR		0xe5
#define FC_COM_RD_I2CADDR		0xe6


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Flash related defines
//
//
// See flash_if.h for sector addresses.
//
// STM32 flash locations
//
/*
Sector 0 0x0800 0000 - 0x0800 3FFF 16 Kbytes
Sector 1 0x0800 4000 - 0x0800 7FFF 16 Kbytes
Sector 2 0x0800 8000 - 0x0800 BFFF 16 Kbytes
Sector 3 0x0800 C000 - 0x0800 FFFF 16 Kbytes
Sector 4 0x0801 0000 - 0x0801 FFFF 64 Kbytes
Sector 5 0x0802 0000 - 0x0803 FFFF 128 Kbytes
Sector 6 0x0804 0000 - 0x0805 FFFF 128 Kbytes
Sector 7 0x0806 0000 - 0x0807 FFFF 128 Kbytes // 512kB flash
.
Sector 11 0x080E 0000 - 0x080F FFFF 128 Kbytes // 1MB flash
*/

//
// ISP for user code
//
#define FLASH_BOOT_ADDRESS		((uint32)0x08000000)
#define FLASH_IP_ADDRESS   		((uint32)0x08010000)
#define FLASH_USER_ADDRESS   	((uint32)0x08020000)
//

// 512kB flash with I2C address or other user storage in top 128k sector
// Program space is 512k - 128k
#define FLASH_PROGRAM_START_ADDRESS		((uint32)0x08000000)
#define FLASH_PROGRAM_END_ADDRESS  		((uint32)0x0805FFFF)
#define FLASH_512k_END_ADDRESS  		((uint32)0x0807ffff)
//

//
// Store Comms settings
//
// Using top sector of 512kB flash part 
#define FLASH_I2CADDR_ADDRESS  			((uint32)0x08060000)
#define FLASH_I2CADDR  					(*(uint32*)FLASH_I2CADDR_ADDRESS)
//


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// STM32 registers
// STM32 MCU_IDCODE - chip ID
#define STM32REG_IDCODE_ADDR 				0xE0042000
// STM32 flash size
#define STM32REG_FLASHSIZE_ADDR			0x1FFF7A22
// STM32 unique ID
#define STM32REG_UDID1_ADDR 				0x1FFF7A10
#define STM32REG_UDID2_ADDR 				0x1FFF7A14
#define STM32REG_UDID3_ADDR 				0x1FFF7A18
//
// Flash control register addresses
//
// STM32 ROP & user option bytes (RDP & USER)
#define STM32REG_RDP_ADDR 				0x1FFFC000  
// STM32 Write protection nWRP bits for sectors 0 to 11
#define STM32REG_WRP1_ADDR 				0x1FFFC008 
// STM32 Write protection nWRP bits for sectors 12 to 23
// STM32F42x and STM32F43x
// warning do not access unless STM32F42x or STM32F43x
//#define STM32REG_WRP2_ADDR			0x1FFEC008 
//
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------



// 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif /* __FC_HOST_COMMANDS_H */
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------


