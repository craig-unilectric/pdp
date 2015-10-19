//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: fc_comport_c.cpp
Date: 6/5/2013
User: Craig Sullender

Class name: 
Env. var.: 
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


// Host Commands
#include "pdp_command_c.h" // for data types and FC_COM_WR_CONTINUE

#include "fc_comport_c.h"

//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Drivers and Windows interface
//
// Replaced by user for the target embedded processor
// or for Windows demo only.
//

// Replaced by user for the target embedded processor.
// access to usb-vcp drivers
#include "usb_vcp.h" // for usb-vcp driver
extern USB_VCP *usbVCP; // for usb-vcp driver





// Windows demo only.
//#include "mainwindow.h" // for Log()
//extern MainWindow *mainWin; // for Log(), IntToStr(), IntToHex()
//#include <math.h> // for sqrt(), abs(), log2(), pow()
//#include <QApplication> // for qApp->processEvents()
//#define PROCESS_WIN_EVENTS()  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents)

//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Communications interface
//
// For multiple serial ports used by the Host demo on a PC.
//
//
bool Host_DeviceRead(uint32 numBytesToRead, uint8* buf)
{
    if(usbVCP->enabled)
        return usbVCP->VCP_DeviceRead((int)numBytesToRead, buf);
    return 0;
}
//
bool Host_DeviceWrite(uint8 numBytesToWrite, uint8* buf)
{
    if(usbVCP->enabled)
        return usbVCP->VCP_DeviceWrite((int)numBytesToWrite, buf);
    return 0;
}
//
bool Host_WaitForDeviceAck(uint8 timeoutSeconds)
{
    if(usbVCP->enabled)
        return usbVCP->VCP_WaitForDeviceAck(timeoutSeconds);
    return 0;
}
//
//
// Wait for I2C/SPI ready.
//
// Wait until device I2C/SPI signals ready before host master read.
// Used in FC_readBuf().
//
bool Host_WaitForDeviceReady(uint8 timeoutSeconds)
{
    if(usbVCP->enabled) // USB/UART
        return 1;
    return 0;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
/*

Moving the Host demo code to an embedded system:

1. Files required

fc_comport_c.c
fc_comport_c.h
fc_command_c.c
fc_command_c.h
fc_host_commands.h"

2. Commport calls

For embedded host drivers, replace
Host_DeviceRead(uint32 size, uint8* buf)
Host_DeviceWrite(uint32 size, uint8* buf)
Host_WaitForDeviceAck(uint8 timeoutSeconds)
Host_WaitForDeviceReady(uint8 timeoutSeconds)
with appropriate drivers.

For example, if using USB:
#define Host_DeviceRead             usb->DeviceRead
#define Host_DeviceWrite            usb->DeviceWrite
#define Host_WaitForDeviceAck       usb->WaitForAck
#define Host_WaitForDeviceReady(x)  1

3. Use a 1 second timeout in DeviceRead() and DeviceWrite().

4. WaitForDeviceAck(timeoutSeconds) should call DeviceRead(1, &ack)
until (ack == 1) or until timeoutSeconds.

Example:
bool WaitForDeviceAck(uint8 timeoutSeconds)
{
    uint8 ack;
    uint32 i;
    for(i = 0;i < timeoutSeconds;i++){
        DeviceRead(1, &ack);
        if(ack == 1) break;
    }
    if(ack == 1) return 1; // ack
    return 0; // no ack
}

*/
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
// Catch a late ack from device.
//
// If device frame rate is slow, FC_stop() can be slow to ack.
//
// Device sends a late ack after slow camera register updates
// and slow flash programming routines.
bool FC_wait_for_slow_ack(uint8 timeoutSeconds)
{
    return Host_WaitForDeviceAck(timeoutSeconds);
}
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*

Communications routines are designed to support USB, UART, I2C etc. between the
host (master) and device (slave).

Tx - this host sends data to device
Rx - this host reads data from device

The host sends commands and data to the device. The tx data is usually just a few bytes.

The host reads data from the device. The rx data size can be as large as an image
captured from the camera.

txbuf[] is used to insert the coammnd at the beginning (txbuf[0]] of the packet.

Tx:

The txbuf[] contains:
1. 1 byte: size - size of tx packet, including command byte, not including size byte
2. 1 byte: command - IoTcam command that the data will be written to
3. n bytes: data

Tx packets will be <= 64 bytes to accommodate USB max packet size.


Rx:

The rxbuf[] contains:
1. n bytes - data

For rx of more than a few bytes,
rx data is received directly to the destination buffer.

*/
//
// txbuf[] is for the command packet (txsize + command + data)
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//
// Send command first
bool FC_read(uint8 command, uint32 size, uint8* buf)
{
    if(!FC_writeCommand_noAck(command)) return 0;
    // Wait for device I2C ready before host master read
    if(!Host_WaitForDeviceReady(1)) return 0;
    if(!Host_DeviceRead(size, buf)) return 0;
    return 1;
}
//
//-------------------------
uint64 FC_readWord64(uint8 command)
{
    uint8 rxbuf[8];
    FC_read(command, 8, rxbuf);
    return *((uint64*)rxbuf);
}
//-------------------------
uint32 FC_readWord32(uint8 command)
{
    uint8 rxbuf[4];
    FC_read(command, 4, rxbuf);
    //return *((uint32*)rxbuf);
    // explicit conversion for unknown processor
    uint32 val32;
    val32 = (rxbuf[0] << 0); // lsb
    val32 += (rxbuf[1] << 8);
    val32 += (rxbuf[2] << 16);
    val32 += (rxbuf[3] << 24); // msb
    return val32;
}
//-------------------------
uint16 FC_readWord16(uint8 command)
{
    uint8 rxbuf[4];
    FC_read(command, 2, rxbuf);
    //return *((uint16*)rxbuf);
    // explicit conversion for unknown processor
    uint16 val16;
    val16 = (rxbuf[0] << 0); // lsb
    val16 += (rxbuf[1] << 8); // msb
    return val16;
}
//-------------------------
uint8 FC_readByte(uint8 command)
{
    uint8 rxbuf[4];
    FC_read(command, 1, rxbuf);
    return rxbuf[0];
}
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//
// Repack data into txbuff packet with size and command.
//
// max tx datasize = TX_PACKET_SIZE_HOST - 2
bool FC_write_noAck(uint8 command, uint8 datasize, uint8* buf)
{
    static uint8 txbuf[TX_PACKET_SIZE_HOST];
    uint8 txsize = datasize + 2; // tx size
    if(txsize > TX_PACKET_SIZE_HOST) return 0; // limit tx size
    // txsize in txbuf[0]
    txbuf[0] = txsize; // tx size
    // command in txbuf[1]
    txbuf[1] = command;
    // Copy buf to txbuf data section.
    for(uint8 i = 2;i < txsize; i++){
        txbuf[i] = buf[i - 2];
    }
    if(!Host_DeviceWrite(txsize, txbuf)) return 0;
    return 1;
}
//
// max tx datasize = TX_PACKET_SIZE_HOST - 2
bool FC_write(uint8 command, uint8 datasize, uint8* buf)
{
    if(!FC_write_noAck(command, datasize, buf)) return 0;
    if(!Host_WaitForDeviceAck(1)) return 0;
    return 1;
}
//
bool FC_writeFile(uint8 command, uint32 size, uint8* buf)
{
    if(!FC_writeCommand(command)) return 0; // write buffer destination address
    uint8* bufptr = buf;
    uint32 page_size = TX_PACKET_SIZE_HOST - 2; // - size - header
    uint32 pages = size/page_size;
    uint32 bytes = size - (pages * page_size);
    for(uint32 i = 0;i < pages;i++){
        // log a message
        //if(fmod((float)(i + 1), 200.0) == 0.0) Log("--- writing to device"); // debug
        if(!FC_write(FC_COM_WR_CONTINUE, page_size, bufptr)) return 0;
        bufptr += page_size;
    }
    if(bytes){
        if(!FC_write(FC_COM_WR_CONTINUE, bytes, bufptr)) return 0;
    }
    return 1;
}
//
//-------------------------
bool FC_writeWord32(uint8 command, uint32 data32)
{
    // write 32 bit value
    //FC_writeBuf(command, 4, (uint8*)&data32);
    // explicit conversion for unknown processor
    static uint8 buf[4];
    buf[0] = (data32 & 0xff) >> 0; // lsb
    buf[1] = (data32 & 0xff00) >> 8;
    buf[2] = (data32 & 0xff0000) >> 16;
    buf[3] = (data32 & 0xff000000) >> 24; // msb
    if(!FC_write(command, (uint8)4, buf)) return 0;
    return 1;
}
//-------------------------
// FC_writeWord24() called by FC_camera_wrReg()
bool FC_writeWord24(uint8 command, uint8* data24)
{
    // no conversion because data24 is a uint8 buf[]
    if(!FC_write(command, (uint8)3, data24)) return 0;
    return 1;
}
//-------------------------
bool FC_writeWord16(uint8 command, uint16 data16)
{
    // write 16 bit value
    //FC_writeBuf(command, 2, (uint8*)&data16);
    // explicit conversion for unknown processor
    static uint8 buf[2];
    buf[0] = (data16 & 0xff) >> 0; // lsb
    buf[1] = (data16 & 0xff00) >> 8; // msb
    if(!FC_write(command, (uint8)2, buf)) return 0;
    return 1;
}
//-------------------------
bool FC_writeByte(uint8 command, uint8 data8)
{
    // send pointer to data8
    if(!FC_write(command, (uint8)1, (uint8*)&data8)) return 0;
    return 1;
}
bool FC_writeByte_noAck(uint8 command, uint8 data8)
{
    // send pointer to data8
    if(!FC_write_noAck(command, (uint8)1, (uint8*)&data8)) return 0;
    return 1;
}
//-------------------------
bool FC_writeCommand(uint8 command)
{
    static uint8 txbuf[4];
    if(!FC_write(command, (uint8)0, (uint8*)txbuf)) return 0;
    return 1;
}
bool FC_writeCommand_noAck(uint8 command)
{
    static uint8 txbuf[4];
    if(!FC_write_noAck(command, (uint8)0, (uint8*)txbuf)) return 0;
    return 1;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Reset the IoTcam UART
// in case it gets out of sync with host packets.
// Host tx 64 bytes of 0x00
//
// 64 bytes max for USB
// 61 for I2C, USB-I2C bridge driver, SiLab part CP2112
//
bool FC_resetUART(void)
{
    static uint8 txbuf[TX_PACKET_SIZE_HOST];
    uint8 txsize = TX_PACKET_SIZE_HOST; // size
    for(uint8 i = 0;i < txsize; i++){
        txbuf[i] = 0;
    }
    txbuf[0] = txsize;
    if(!Host_DeviceWrite(txsize, txbuf)) return 0;
    if(!Host_DeviceWrite(txsize, txbuf)) return 0;
    return 1;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
