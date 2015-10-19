//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: fc_command_c.h
Date: 3/31/2013
User: Craig Sullender

Class name: 
Env. var.: 
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#ifndef FC_COMMAND_C_H
#define FC_COMMAND_C_H

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Data Types
//


#include <QtGlobal> // for Qt data types


#define int8    qint8
#define int16   qint16
#define int32   qint32
#define int64   qint64
#define uint8   quint8
#define uint16  quint16
#define uint32  quint32
#define uint64  quint64

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Host Commands
//
//

#include "fc_host_commands.h" // for Host-device commands

//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Color Window LUT
//
typedef struct
{                       // device byte order for serial port buffer
    uint8   c2Hi;       // device (uint8*)cwlut[0]
    uint8   c2Lo;       // device (uint8*)cwlut[1]
    uint8   vHi;        // device (uint8*)cwlut[2]
    uint8   vLo;        // device (uint8*)cwlut[3]
    uint32  colorAvg;   // device (uint8*)cwlut[7-4]
}ColorWindowLUT;
//
//extern ColorWindowLUT cwlut[]; // in fc_command_c.cpp
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Feature List
//
//
/*
first_x1 and first_x2
are the initial endpoints of subextents
and are not output in the feature list.
In hardware these two locations are
reused for rptr and dptr during
region-sort and output.
*/
#define first_x1	frptr
#define first_x2	dptr
//
typedef struct
{
    // add dummy values for 32 bit alignment
    uint32      color;
    uint32      size;
    uint16      x1;
    uint16      x2;
    uint16      y1;
    uint16      y2;
    uint16      subext; // bool flag
    uint16      uptr;
    uint16      frptr;
    uint16      dptr;
    uint16      regionptr;
    uint16      dummy_1;
}Feature;
//
typedef struct
{
    uint16      feat_cnt;
    uint16      filtered_cnt;
    Feature     feat[FC_MAX_FEAT];
}FeatureList;
//
// allocate the feature list
//
//extern FeatureList fl;
//
//
// Region List
//
// save one location as a flag
#define MAX_REGION (FC_MAX_FEAT - 1)
#define FLAG_NO_REGION 0xffff
//#define FLAG_NO_REGION MAX_REGION
//
typedef struct
{
    // add dummy values for 32 bit alignment
    uint32      color;
    uint32      size;
    uint16      x1;
    uint16      x2;
    uint16      y1;
    uint16      y2;
    uint16      frptr; // feature root ptr
    uint16      dummy_1;
}Region;
typedef struct
{
    uint16     	region_cnt;
    Region      region[MAX_REGION];
}RegionList;
//
// allocate the feature list
//
//extern RegionList rl;
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//

// Device controls
void FC_stop(void);
void FC_run(void);
void FC_stm32_reset(void);

// Device ID
uint32 FC_read_chip_ID(void);
void FC_read_chip_UDID(uint32* val1, uint32* val2, uint32* val3);


// Device comms test
uint32 FC_txrx_test(void);


// Camera
uint8 FC_read_frameRate(void);
bool FC_camera_check(void);
bool FC_camera_I2C_reset(void);
bool FC_camera_I2C_speed(uint32 speed);
void FC_camera_gammaEn(bool enable);
void FC_camera_wrReg(uint16 camRegAddr, uint8 data);
uint8 FC_camera_rdReg(uint16 camRegAddr);
bool FC_camera_wrReg_array(uint8* regArray);
bool FC_camera_rgb565(void);
bool FC_camera_JPG(void);
bool FC_camera_JPG_zoom(void);
bool FC_camera_colorbar(bool enable);



// Accel
void FC_accel_wrReg(uint8 regAddr, uint8 data);
uint64 FC_accel_rdReg(uint8 regAddr);



// I2C address
uint8 FC_read_i2c_addr(void);
bool FC_set_i2c_addr(uint8 addr);

// Capture
uint32 FC_capture_JPG_image(uint8* buf, uint32 bufsize);
uint32 FC_capture_RGB565_image(uint16* buf_16, uint32* buf_32, uint32 pixelcount);

// Motion
void FC_update_motion_settings(uint8 motEn, uint8 thresh, uint8 bkgUpdEn,
                                uint8 bkgUpdRate, uint8 bkgUpdFrameDiv, uint8 newBkgEn);
void FC_new_background(void);


// Color
void FC_update_color_settings(uint8 clutEn, ColorWindowLUT* clut);



// ChipSight
void FC_update_chipsight_settings(uint8 segFilterEn, uint8 segFilterPopMinSize, uint8 segFilterGapMinSize,
                                    uint16 precEnable, uint16 prec, uint16 maxRes, uint16 minRes,
                                    uint8 regionFilterEn, uint8 regionFilterMinSize);
bool FC_capture_chipsight(RegionList *r, FeatureList *f);
bool FC_capture_chipsight_overlay(RegionList *r, FeatureList *f, uint16* buf_16, uint32* buf_32, uint32 pixelcount);



// SD Card
bool FC_SDcard_info(uint8* ready, uint64* capacity, uint64* blockSize, uint64* blockCount,
                uint32* imageMax, uint32* imageCounter, uint8* goodWrite);
void FC_update_SDcard_settings(uint8 SDcardEn);
uint32 FC_capture_SDcard_image(uint32 imageNumber, uint8* buf, uint32 bufsize, uint8* goodread);



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------







//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#endif // FC_COMMAND_C_H
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

