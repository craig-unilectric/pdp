//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: pdp_command_c.cpp
Date: 2015
User: Craig Sullender

Class name: 
Env. var.: 
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//

#include "pdp_command_c.h" // for data types and structs
#include "fc_comport_c.h" // for Host to device serial port calls

#include "mainwindow.h" // for Log()
//extern MainWindow *mainWin; // for Log()

//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Declare structs
//


// in fc_command_c.h
// Command struct
// Status struct
#include "uni_host_structs.h" // for Host-device structs
PDP_Command_Struct pc;
PDP_Status_Struct ps;



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// write command struct
//

void pdp_write_command_struct(void)
{
    uint32 size = sizeof(PDP_Command_Struct);

    FC_writeFile(FC_COM_WR_COMMAND_STRUCT, size, (uint8*)&pc);

}
//

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// read status struct
//

bool pdp_read_status_struct(void)
{
    uint32 size = sizeof(PDP_Status_Struct);

    FC_stop();

    if(!FC_read(FC_COM_RD_STATUS_STRUCT, size, (uint8*)&ps)) return 0;

    //FC_run();

    return 1;
}





//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// read raw adc dma buffer
//
int pdp_readRawDMA(uint8* buf, uint32 sizeMaxBytes)
{

    FC_stop();

    uint32 sizeBytes = FC_readWord32(FC_COM_RD_I_RAW_SIZE);
    if(sizeBytes > sizeMaxBytes) return 0;

    if(!FC_read(FC_COM_RD_I_RAW, sizeBytes, buf)) return 0;

    FC_run();

    return sizeBytes;

}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// read raw adc channel from dma buffer
//
int pdp_readChannelDMA(uint8 channel, uint8* buf, uint32 sizeMaxBytes)
{

    FC_writeByte(FC_COM_WR_CHANNEL_NUMBER, channel);

    Sleep(100); // give PDP time to capture new channel data

    FC_stop();

    uint32 sizeBytes = FC_readWord32(FC_COM_RD_CHANNEL_I_RAW_SIZE);
    if(sizeBytes > sizeMaxBytes) return 0;

    if(!FC_read(FC_COM_RD_CHANNEL_I_RAW, sizeBytes, buf)) return 0;

    //FC_run(); // ****************************** uncomment

    return sizeBytes;
}
//
int pdp_readChannelDMA_filtered(uint8 channel, uint8* buf, uint32 sizeMaxBytes)
{

    FC_writeByte(FC_COM_WR_CHANNEL_NUMBER, channel);

    Sleep(100); // give PDP time to capture new channel data

    FC_stop();

    uint32 sizeBytes = FC_readWord32(FC_COM_RD_CHANNEL_I_RAW_SIZE);
    if(sizeBytes > sizeMaxBytes) return 0;

    if(!FC_read(FC_COM_RD_CHANNEL_I_FILTERED, sizeBytes, buf)) return 0;

    //FC_run(); // ****************************** uncomment

    return sizeBytes;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
void FC_stop(void)
{
    FC_writeCommand_noAck(FC_COM_WR_DISABLE);
    FC_wait_for_slow_ack(5); // for slow camera framerate
}
//
void FC_run(void)
{
    FC_writeCommand(FC_COM_WR_ENABLE);
}
//
// Reset for BOARD_IOTCAM_REV10
//
void FC_stm32_reset(void)
{
    FC_stop();
    FC_writeCommand_noAck(FC_COM_WR_STM32_RESET);
    // FC_wait_for_slow_ack(1); for other boards without reset
    // FC_run(); for other boards without reset
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Read the STM32F chip ID.
//
uint32 FC_read_chip_ID(void)
{
    uint32 val = 0;
    //
    FC_stop();
    //
    val = FC_readWord32(FC_COM_RD_CHIP_ID);
    val = (val & 0xfff);
    //Log("ID: 0x" + IntToHex(val));
    //if(val == 0x413) Log("STM32F40x or STM32F41x");
    //if(val == 0x419) Log("STM32F42x or STM32F43x");
    //
    FC_run();
    //
    return val;
}
//
// Read the chip 96 bit unique ID.
//
void FC_read_chip_UDID(uint32* val1, uint32* val2, uint32* val3)
{
    //
    FC_stop();
    //
    *val1 = FC_readWord32(FC_COM_RD_UDID_1);
    //Log("UDID_1: 0x" + IntToHex(*val1));
    *val2 = FC_readWord32(FC_COM_RD_UDID_2);
    //Log("UDID_2: 0x" + IntToHex(*val2));
    *val3 = FC_readWord32(FC_COM_RD_UDID_3);
    //Log("UDID_3: 0x" + IntToHex(*val3));
    //
    FC_run();
    //
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Test tx/rx data.
// Returns number of errors.
//
uint32 FC_txrx_test(void)
{
    static uint8 counter = 0; // change data each time test is run
    uint32 i;
    uint8 j;
    uint32 errors = 0;
    //
    FC_stop();
    //
    //

    uint32 testsize = 59; // 59 is max data for tx packet, TX_PACKET_SIZE_HOST - 2
    //
    // init buffers
    uint8 data_out[testsize];
    uint8 data_in[testsize];
    j = counter;
    for(i = 0;i < testsize;i++){
        data_out[i] = j++;
        data_in[i] = 0xff;
    }
    //
    // write data
    FC_write(FC_COM_WR_COMMS_TEST, testsize, data_out);
    // read data
    FC_read(FC_COM_RD_COMMS_TEST, testsize, data_in);
    // compare
    for(i = 0;i < testsize;i++){
        if(data_out[i] != data_in[i]){
            errors++;
            //Log("error " + IntToStr(i) + "  out " + IntToHex(data_out[i]) + "  in " + IntToHex(data_in[i]));
            //Log("*** error ***");
        }
        //Log(IntToStr(i) + "  out " + IntToHex(data_out[i]) + "  in " + IntToHex(data_in[i]));
    }
    //
    FC_run();
    //
    counter++;
    //

    return errors;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//









//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
/*
//
// Camera commands
//
//
uint8 FC_read_frameRate(void)
{
    int framecnt = FC_readByte(FC_COM_RD_FRAMERATE);
    return framecnt;
}
//
void FC_camera_wrReg(uint16 camRegAddr, uint8 data)
{
    uint8 data_out[4];
    // explicit packing for unknown processor
    data_out[0] = (uint8)(camRegAddr & 0xff);
    data_out[1] = (uint8)((camRegAddr >> 8) & 0xff);
    data_out[2] = data;
    FC_writeWord24(FC_COM_WR_CAMERA_WRREG, data_out);
}
//
uint8 FC_camera_rdReg(uint16 camRegAddr)
{
    FC_writeWord16(FC_COM_WR_CAMERA_RDREG, camRegAddr);
    uint8 val = FC_readByte(FC_COM_RD_CAMERA_REG);
    return val;
}
//
bool FC_camera_wrReg_array(uint8* regArray)
{
    uint8 address;
    uint8 value;
    uint8 *pdata = regArray;
    if (!pdata) return 0;
    while(1){
        address = *pdata++;
        value = *pdata++;
        if((address == 0xff) && (value == 0xff)) return 1;
        FC_camera_wrReg(address, value);
        //Log("0x" + IntToHex(address) + "  0x" + IntToHex(value));
    }
    return 0;
}
//
bool FC_camera_I2C_reset(void)
{
    FC_stop();
    FC_writeCommand(FC_COM_WR_CAMERA_I2C_RESET);
    FC_run();
    return 1;
}
//
// Camera I2C speed, 10k - 400k (10000 - 400000)
//
bool FC_camera_I2C_speed(uint32 speed)
{
    FC_stop();
    FC_writeWord32(FC_COM_WR_CAMERA_I2C_SPEED, speed);
    FC_run();
    return 1;
}
//
bool FC_camera_check(void)
{
    uint8 val1, val2;
    bool success = true;
    //
    FC_stop();
    //
    FC_camera_wrReg(0xff, 0x01);
    val1 = FC_camera_rdReg(0x0A); // OV2640 = 0x26, camera product ID msb
    if(val1 != 0x26) success = false;
    val2 = FC_camera_rdReg(0x0B); // OV2640 = 0x42, camera product ID lsb
    if(val2 != 0x42) success = false;
    val1 = FC_camera_rdReg(0x1C); // OV2640 = 0x7f, camera manufacturer ID msb
    if(val1 != 0x7f) success = false;
    val2 = FC_camera_rdReg(0x1D); // OV2640 = 0xa2, camera manufacturer ID lsb
    if(val2 != 0xa2) success = false;
    //
    FC_run();
    //
    return success;
}
//
// enable/disable gamma
// 0xC3 CTRL1 Bit[5] RAW_GMA
void FC_camera_gammaEn(bool enable)
{
    uint8 val8;
    FC_stop();//_and_wait_for_capture();
    //
    FC_camera_wrReg(0xff, 0x00);
    val8 = FC_camera_rdReg(0xC3);
    if(enable) val8 = val8 | 0x20;
    else val8 = val8 & ~0x20;
    FC_camera_wrReg(0xC3, val8);
    //
    FC_run();
}
//
//
bool FC_camera_rgb565(void)
{
    bool success = true;
    //
    FC_stop();
    //
    FC_writeCommand_noAck(FC_COM_WR_RGB565_160x120);
    if(!FC_wait_for_slow_ack(20)) success = false;
    if(!FC_readByte(FC_COM_RD_CAMERA_READY)) success = false;
    //
    FC_run();
    //
    return success;
}
//
bool FC_camera_JPG(void)
{
    bool success = true;
    //
    FC_stop();
    //
    FC_writeCommand_noAck(FC_COM_WR_JPEG_640x480);
    if(!FC_wait_for_slow_ack(20)) success = false;
    if(!FC_readByte(FC_COM_RD_CAMERA_READY)) success = false;
    //
    FC_run();
    //
    return success;
}
//
bool FC_camera_JPG_zoom(void)
{
    bool success = true;
    //
    FC_stop();
    //
    FC_writeCommand_noAck(FC_COM_WR_JPEG_ZOOM_640x480);
    if(!FC_wait_for_slow_ack(20)) success = false;
    if(!FC_readByte(FC_COM_RD_CAMERA_READY)) success = false;
    //
    FC_run();
    //
    return success;
}
//
bool FC_camera_colorbar(bool enable)
{
    bool success = true;
    //
    FC_stop();
    //
    FC_writeByte_noAck(FC_COM_WR_CAMERA_TEST_PATTERN, (uint8)enable);
    if(!FC_wait_for_slow_ack(20)) success = false;
    if(!FC_readByte(FC_COM_RD_CAMERA_READY)) success = false;
    //
    FC_run();
    //
    return success;
}
//




//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Device master I2C accel
//
//



void FC_accel_wrReg(uint8 regAddr, uint8 data)
{
    uint8 data_out[4];
    // explicit packing for unknown processor
    data_out[0] = regAddr;
    data_out[1] = data;
    //FC_writeWord16(FC_COM_WR_ACCEL_WRREG, data_out);
    FC_write(FC_COM_WR_ACCEL_WRREG, 2, data_out);
}
//
uint64 FC_accel_rdReg(uint8 regAddr)
{
    FC_writeByte(FC_COM_WR_ACCEL_RDREG, regAddr);
    uint64 val = FC_readWord64(FC_COM_RD_ACCEL_REG);
    return val;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// I2C Address
//
uint8 FC_read_i2c_addr(void)
{
    uint8 addr;
    //
    FC_stop();
    //
    addr = FC_readByte(FC_COM_RD_I2CADDR);
    //
    FC_run();
    //
    return addr;
}
//
bool FC_set_i2c_addr(uint8 addr)
{
    uint8 checkaddr;
    bool success = true;
    //
    FC_stop();
    //
    // program I2C addr variable in flash
    if(!FC_writeByte_noAck(FC_COM_WR_I2CADDR, addr)) success = false;
    // wait for slow flash programming
    if(!FC_wait_for_slow_ack(5)) success = false;
    // confirm that flash programming was finished
    if(!FC_readByte(FC_COM_RD_PROG_FLASH_DONE)) success = false;
    // test for flash prog errors
    if(!FC_readByte(FC_COM_RD_PROG_FLASH_GOOD)) success = false;
    // Test if the correct I2C addr is in device.
    checkaddr = FC_readByte(FC_COM_RD_I2CADDR);
    if(checkaddr != addr) success = false;
    //
    FC_run();
    //
    return success;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Read RGB565_BMP from device
// convert to RGB32_BMP
//
#define RED(a)      ((((a) & 0xf800) >> 11) << 3)
#define GREEN(a)    ((((a) & 0x07e0) >> 5) << 2)
#define BLUE(a)     (((a) & 0x001f) << 3)
//
uint32 rgb565_to_rgb32(uint16 rgb565)
{
    uint8 r, g, b;
    uint32 rgb;
    r = (uint8)RED(rgb565);
    g = (uint8)GREEN(rgb565);
    b = (uint8)BLUE(rgb565);
    rgb = (((r << 8) + g) << 8) + b;
    return rgb;
}
//
// The device buffer is sized for the 160x120_BMP.
//
uint32 FC_capture_RGB565_image(uint16* buf_16, uint32* buf_32, uint32 pixelcount)
{
    uint32 i;
    uint16 rgb565;
    uint32 rgb;
    bool success = false;
    uint32 bmpsize = 0;
    //
    FC_stop();
    //
    // rx image
    success = FC_read(FC_COM_RD_IMAGE, (uint32)(pixelcount * 2), (uint8*)buf_16);
    //
    FC_run();
    //
    // convert RGB565 to RGB32
    for(i = 0;i < pixelcount;i++){
        rgb565 = buf_16[i];
        rgb = rgb565_to_rgb32(rgb565);
        buf_32[i] = rgb;
    }
    if(success) bmpsize = pixelcount * 4;
    return bmpsize;
}
//
//
// Call with a buffer to hold the rx data.
// Device image buffer will hold the 640x480_JPG.
// The device buffer is sized for the 160x120_BMP.
// const int size = 160 * 120 * 2;
// uint8 image_buf[size];
// Or buffer size can be taked from:
// FC_BMP_WIDTH;
// FC_BMP_HEIGHT;
//
// FC_capture_JPG_image() returns actual jpg size.
//
// bytes 0xFF, 0xD8 indicate start of jpg image
// bytes 0xFF, 0xD9 indicate end of jpg image
//
uint32 FC_capture_JPG_image(uint8* buf, uint32 bufsize)
{
    uint32 i;
    uint8 prev = 0;
    uint8 val = 0;
    bool success = false;
    bool good_jpg = false;
    uint32 jpgsize = 0;
    //
    FC_stop();
    //
    // rx image
    success = FC_read(FC_COM_RD_JPG, bufsize, buf);
    //
    FC_run();
    //
    if(!success) return 0;
    //
    //--------------------------
    // Determine end of jpg image (EOI).
    // Find EOI code for jpg image within larger buffer.
    for(i = 0;i < bufsize;i++){
        val = buf[i];
        good_jpg = ((prev == 0xff) && (val == 0xd9));
        if(good_jpg) break;
        prev = val;
    }
    if(good_jpg){
        jpgsize = i + 1;
    }

    return jpgsize;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Motion
//
//
void FC_update_motion_settings(uint8 motEn, uint8 thresh, uint8 bkgUpdEn,
                                uint8 bkgUpdRate, uint8 bkgUpdFrameDiv, uint8 newBkgEn)
{
    //
    FC_stop();
    //
    FC_writeByte(FC_COM_WR_MOTION_ENABLE, motEn);
    FC_writeByte(FC_COM_WR_MOTION_THRESH, thresh);
    FC_writeByte(FC_COM_WR_MOTION_BKGUPDATE_ENABLE, bkgUpdEn);
    FC_writeByte(FC_COM_WR_MOTION_BKGUPDATE_RATE, bkgUpdRate);
    FC_writeByte(FC_COM_WR_MOTION_BKGUPDATE_FRAME, bkgUpdFrameDiv);
    FC_writeByte(FC_COM_WR_MOTION_NEW_BKG, newBkgEn);
    //
    FC_run();
    //
}
//
void FC_new_background(void)
{
    //
    FC_stop();
    //
    FC_writeByte(FC_COM_WR_MOTION_ENABLE, (uint8)1);
    FC_writeByte(FC_COM_WR_MOTION_NEW_BKG, (uint8)1);
    //
    FC_run();
    //
}
//

*/


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Color
//
//
/*
The color window LUT will be sent to the device as a byte array.
Make sure the host compiler interprets the byte pointer
uint8 *ptr8 = (uint8*)clut;
so that ptr8[0] = clut[0].c2Hi
in order to transmit the CLUT in the order the device expects.
*/

/*

void FC_update_color_settings(uint8 clutEn, ColorWindowLUT* clut)
{
    uint32 clutSize = sizeof(ColorWindowLUT) * 256; // 1024
    //
    FC_stop();
    //
    FC_writeByte(FC_COM_WR_CLUT_ENABLE, clutEn);
    //
    FC_writeFile(FC_COM_WR_CLUT, clutSize, (uint8*)clut);
    //
    FC_run();
    //
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// ChipSight
//
//
void FC_update_chipsight_settings(uint8 segFilterEn, uint8 segFilterPopMinSize, uint8 segFilterGapMinSize,
                                    uint16 precEnable, uint16 prec, uint16 maxRes, uint16 minRes,
                                    uint8 regionFilterEn, uint8 regionFilterMinSize)
{
    //
    FC_stop();
    //
    FC_writeByte(FC_COM_WR_SEG_FILTER_ENABLE, segFilterEn);
    FC_writeByte(FC_COM_WR_SEG_FILTER_POPMINSIZE, segFilterPopMinSize);
    FC_writeByte(FC_COM_WR_SEG_FILTER_GAPMINSIZE, segFilterGapMinSize);
    //
    FC_writeWord16(FC_COM_WR_PREC_ENABLE, precEnable);
    FC_writeWord16(FC_COM_WR_PREC, prec);
    FC_writeWord16(FC_COM_WR_MAXRES, maxRes);
    FC_writeWord16(FC_COM_WR_MINRES, minRes);
    //
    FC_writeByte(FC_COM_WR_REGION_FILTER_ENABLE, regionFilterEn);
    FC_writeByte(FC_COM_WR_REGION_FILTER_MINSIZE, regionFilterMinSize);
    //
    FC_run();
    //
}
//
//-------------------------------------------------------------------------
//
// See the note above on host compiler struct byte order that the device expects.
//
bool read_featureList(FeatureList *f)
{
    uint32 fsize;
    bool success = true;

    //FC_stop();

    f->feat_cnt = FC_readWord16(FC_COM_RD_FE_FCNT);
    f->filtered_cnt = FC_readWord16(FC_COM_RD_FE_FILTCNT);
    fsize = FC_readByte(FC_COM_RD_FE_FSIZE);
    fsize = f->feat_cnt * fsize;

    if(fsize > 0) success = FC_read(FC_COM_RD_FE_FLIST, fsize, (uint8*)f->feat);

    //FC_run();

    return success;
}
//
bool read_regionList(RegionList *r)
{
    uint32 rsize;
    bool success = true;

    //FC_stop();

    r->region_cnt = FC_readWord16(FC_COM_RD_FE_RCNT);
    rsize = FC_readByte(FC_COM_RD_FE_RSIZE);
    rsize = r->region_cnt * rsize;


    //Log("    RCNT: " + IntToStr(r->region_cnt));
    //Log("    Rsize: " + IntToStr(rsize));


    if(rsize > 0) success = FC_read(FC_COM_RD_FE_RLIST, rsize, (uint8*)r->region);

    //FC_run();

    return success;
}
//
bool read_bmp(uint16* buf_16, uint32* buf_32, uint32 pixelcount)
{
    uint32 i;
    uint16 rgb565;
    uint32 rgb;
    bool success;
    //
    //FC_stop();
    //
    // rx image
    success = FC_read(FC_COM_RD_IMAGE, (uint32)(pixelcount * 2), (uint8*)buf_16);
    //
    //FC_run();
    //
    // convert RGB565 to RGB32
    for(i = 0;i < pixelcount;i++){
        rgb565 = buf_16[i];
        rgb = rgb565_to_rgb32(rgb565);
        buf_32[i] = rgb;
    }
    return success;
}
//
bool FC_capture_chipsight(RegionList *r, FeatureList *f)
{
    bool success = true;
    //
    FC_stop();
    //
    // rx feature lists

    if(!read_regionList(r)) success = false;

    else if(!read_featureList(f)) success = false;

    //
    FC_run();
    //
    return success;
}
//
bool FC_capture_chipsight_overlay(RegionList *r, FeatureList *f, uint16* buf_16, uint32* buf_32, uint32 pixelcount)
{
    bool success = true;
    //
    FC_stop();
    //
    // rx image

    if(!read_bmp(buf_16, buf_32, pixelcount)) success = false;

    else if(!read_regionList(r)) success = false;

    else if(!read_featureList(f)) success = false;

    //
    FC_run();
    //
    return success;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// SD Card
//

bool FC_SDcard_info(uint8* ready, uint64* capacity, uint64* blockSize, uint64* blockCount, uint32* imageMax, uint32* imageCounter, uint8* goodWrite)
{
    uint64 val64 = 0;
    *ready = 0;
    *capacity = 0;
    *blockSize = 0;
    *blockCount = 0;
    *imageMax = 0;
    *imageCounter = 0;
    *goodWrite = 0;
    //
    FC_stop();
    //
    val64 = FC_readByte(FC_COM_RD_SD_DETECTED);
    if(val64 == 0){
        return 0;
    }
    else{
        val64 = FC_readByte(FC_COM_RD_SD_READY);
        *ready = val64;
        val64 = FC_readWord64(FC_COM_RD_SD_CAPACITY);
        *capacity = val64;
        val64 = FC_readWord64(FC_COM_RD_SD_BLOCKSIZE);
        *blockSize = val64;
        val64 = FC_readWord64(FC_COM_RD_SD_BLOCKCOUNT);
        *blockCount = val64;
        val64 = FC_readWord32(FC_COM_RD_SD_IMAGEMAX);
        *imageMax = val64;
        val64 = FC_readWord32(FC_COM_RD_SD_IMAGECOUNTER);
        *imageCounter = val64;
        val64 = FC_readByte(FC_COM_RD_SD_GOODWRITE);
        *goodWrite = val64;
    }
    //
    FC_run();
    //
    return 1;
}
//
void FC_update_SDcard_settings(uint8 SDcardEn)
{
    //
    FC_stop();
    //
    FC_writeByte(FC_COM_WR_SD_ENABLE, SDcardEn);
    //
    FC_run();
    //
}
//
uint32 FC_capture_SDcard_image(uint32 imageNumber, uint8* buf, uint32 bufsize, uint8* goodread)
{
    uint32 jpgsize;
    //
    FC_stop();
    //
    // read the SD card
    FC_writeWord32(FC_COM_WR_SD_IMAGENUMBER, imageNumber);
    // check for a good SD card read
    *goodread = FC_readByte(FC_COM_RD_SD_GOODREAD);
    // read device image buffer
    jpgsize = FC_capture_JPG_image(buf, bufsize);
    //
    //FC_run(); // in FC_capture_JPG_image() call above
    //
    return jpgsize;
}

*/

//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------





//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------





//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

