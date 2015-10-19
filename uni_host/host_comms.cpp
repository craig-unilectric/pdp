//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: host_comms.cpp
Date: 3/9/2014
User: Craig Sullender

Class name: host
Env. var.:
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


#include <QPainter>
#include <QtCore/qmath.h>
#include <QLineF>
#include <QTime>


#include "host.h"
#include "ui_mainwindow.h"
#include "mainwindow.h" // for Log() and file and display functions
extern MainWindow *mainWin; // for Log(), ProgressBar(), StatusBar()
#include <windows.h> // for Sleep()
#include <math.h> // for fmod(), sqrt(), abs(), log2(), pow()

//#include "usb_i2c.h"
#include "usb_vcp.h"


#include "pdp_command_c.h" // for data types and structs
//#include "fc_command_c.h" // for data types and structs
//extern FeatureList fl;
//extern RegionList rl;
//extern ColorWindowLUT cwlut[]; // in fc_command_c.h

#include "fc_comport_c.h" // for Host to device serial port calls and TX_PACKET_SIZE_HOST



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
// Comms
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
//
//

void Host::comms_open_selected(QListWidgetItem * item)
{
    bool success = false;

    //int index = ui->listWidget_comms_list->row(item);
    //Log("row selected " + IntToStr(index));

    success = usb_vcp->open_port(item);
    if(success) usb_vcp->enabled = true;

    // disable comms select buttons
    if(success){
        ui->pushButton_comms_open->setEnabled(false);
    }
}

bool Host::comms_open(void)
{
    bool success = false;

    usb_vcp->enabled = false;

    Log("Open USB-UART channel");
    success = usb_vcp->vcp_open(1);
    if(success) usb_vcp->enabled = true;


    // disable comms select buttons
    if(success){
        ui->pushButton_comms_open->setEnabled(false);
    }
    return success;
}
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Comms Test
//
//

/*
 * bool FC_write_noAck(uint8 command, uint8 datasize, uint8* buf)
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
*/
void Host::comms_test(void)
{
    Log("");
    Log("comms_test()");
    //
    //
    //FC_stop();
/*
     FC_writeCommand_noAck(FC_COM_WR_DISABLE);

    Sleep(1000);
    Sleep(1000);

    Log("comms_test()");
    FC_writeCommand_noAck(FC_COM_WR_DISABLE);
*/
/*
    uint8 buf[100];
    for(int i = 0;i < 100;i++) buf[i] = 0;
    buf[0] = 0x05;
    buf[1] = 'r';
    buf[2] = 'n';
    buf[3] = 0x00;
    buf[0] = 0x00;
    usb_vcp->VCP_DeviceWrite(1, buf);
    //usb_vcp->VCP_DeviceWrite(16, buf);
*/


    /*
    FC_stop();
    Sleep(1000);
    FC_run();
    Sleep(1000);
    FC_stop();
    Sleep(1000);
    FC_run();
    Sleep(1000);
    FC_stop();
    */

    //FC_stop();




    FC_resetUART();


    //
    bool commsgood = true;
    int errors = FC_txrx_test(); // packet txrx test
    if(errors){
        Log("*** " + IntToStr(errors) + " packet txrx errors");
        Log("*** Comms test error");
        commsgood = false;
    }
    else{
        //
        read_chipID();
        //
        commsgood = txrx_test(); // verbose file txrx test
        Log("");
        //
        if(commsgood){
            //
            // Test results
            //
            Log("");
            if(commsgood) Log("--- good comms test");
            else Log("*** comms test error");
        }
    }

    //
    Log("");
    //
}


//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Read STM32 Chip Info
//
/*
MCU_IDCODE
Bits 31:16 REV_ID(15:0) Revision identifier
This field indicates the revision of the device:
0x1000 = Revision A
0x1001 = Revision Z
Bits 15:12 Reserved, must be kept at reset value.
Bits 11:0 DEV_ID(11:0): Device identifier (STM32F40x and STM32F41x)
The device ID is 0x413.
Bits 11:0 DEV_ID(11:0): Device identifier (STM32F42x and STM32F43x)
The device ID is 0x419.
*/
/*
STM32 ROP & user option bytes (RDP & USER)
FC_COM_RD_RDP
Bit 15:8
0xAA: Level 0, no protection
0xCC: Level 2, chip protection (debug and boot from RAM features disabled)
Others: Level 1, read protection of memories (debug features limited)


STM32 Write protection nWRP bits for sectors 0 to 11
FC_COM_RD_WRP1
Bits 11:0
nWRPi:
0: Write protection active on selected sector
1: Write protection not active on selected sector

STM32 Write protection nWRP bits for sectors 12 to 23
STM32F42x and STM32F43x
FC_COM_RD_WRP2
warning do not access unless STM32F42x or STM32F43x


USER: User option byte
This byte is used to configure the following features:
– Select the watchdog event: Hardware or software
– Reset event when entering the Stop mode
– Reset event when entering the Standby mode
Bit 7
nRST_STDBY
0: Reset generated when entering the Standby mode
1: No reset generated
Bit 6
nRST_STOP
0: Reset generated when entering the Stop mode
1: No reset generated
Bit 5
WDG_SW
0: Hardware independent watchdog
1: Software independent watchdog
Bit 4 0x1: Not used
Bits 3:2
BOR_LEV: BOR reset Level
These bits contain the supply level threshold that activates/releases the reset.
They can be written to program a new BOR level value into Flash memory.
00: BOR Level 3 (VBOR3). reset threshold level from 2.70 to 3.60 V
01: BOR Level 2 (VBOR2). reset threshold level from 2.40 to 2.70 V
10: BOR Level 1 (VBOR1). reset threshold level from 2.10 to 2.40 V
11: BOR off (VBOR0), reset threshold level from 1.8 to 2.10 V
Bits 1:0 0x1: Not used


Before setting RDP to level 1:
Set User option byte: 0x3f
Clear nRST_STOP
Clear nRST_STDBY
Set WDG_SW

*/
void Host::read_chipID(void)
{
    bool big_flash = false;
    //
    FC_stop();
    //
    Log("");
    Log("FC_read_chipID");
    uint32 val = FC_readWord32(FC_COM_RD_CHIP_ID);
    int id = (val & 0xfff);
    Log("ID: 0x" + IntToHex(id));
    if(id == 0x431){
        Log("STM32F411");
    }
    else if(id == 0x413){
        Log("STM32F40x or STM32F41x");
    }
    else if(id == 0x419){
        Log("STM32F42x or STM32F43x"); // 2MByte flash
        big_flash = true;
    }
    else Log("Chip ID - error ***");
    int rev = (val >> 16);
    Log("Rev: 0x" + IntToHex(rev));
    if(rev == 0x1000){
        Log("Revision A");
    }
    else if(rev == 0x1001){
        Log("Revision Z");
    }
    else{
        Log("Revision - error ***");
    }
    //
    val = FC_readWord16(FC_COM_RD_FLASH_SIZE);
    Log("Flash size: " + IntToStr(val) + "k Bytes");
    val = FC_readWord32(FC_COM_RD_UDID_1);
    Log("UDID_1: 0x" + IntToHex(val));
    val = FC_readWord32(FC_COM_RD_UDID_2);
    Log("UDID_2: 0x" + IntToHex(val));
    val = FC_readWord32(FC_COM_RD_UDID_3);
    Log("UDID_3: 0x" + IntToHex(val));
    //
    Log("");
    Log("STM32 option bytes");
    Log("");
    val = FC_readWord16(FC_COM_RD_RDP);
    int rdp = (val >> 8) & 0xff;
    Log("RDP: 0x" + IntToHex(rdp));
    if(rdp == 0xaa){
        Log("RDP Level 0, no read protection");
    }
    else if(rdp == 0xcc){
        Log("RDP Level 2, chip is protected");
    }
    else{
        Log("RDP Level 1, RDP on");
    }
    Log("");
    Log("User options: 0x" + IntToHex(val & 0xff));
    if((val & 0xff) == 0xff) Log("User options set to default");
    else Log("--User options programmed");
    Log("");
    val = FC_readWord16(FC_COM_RD_WRP1);
    int wrp = val & 0xfff;
    Log("WRP sect 11-0: 0x" + IntToHex(wrp));
    Log("      0 = protected");
    Log("      1 = unprotected");
    if(wrp == 0xfe0) Log("--Boot area is write protected.");
    //
    // warning do not access unless STM32F42x or STM32F43x
    if(big_flash){
        //val = FC_readWord16_addr(FC_COM_RD_WRP2);
        //Log("WRP sect 23-12: 0x" + IntToHex(val & 0xfff));
    }
    //
    Log("");
    //
    /*
    // test if flash is protected from reads
    val = FC_readWord32_addr(FC_COM_RD_FLASH_BOOT);
    Log("Flash boot val: 0x" + IntToHex(val));
    val = FC_readWord32_addr(FC_COM_RD_FLASH_IP);
    Log("Flash IP val: 0x" + IntToHex(val));
    val = FC_readWord32_addr(FC_COM_RD_FLASH_USER);
    Log("Flash user val: 0x" + IntToHex(val));
    */
    //
    FC_run();
    //
    Log("Done");

}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// write_file_to_device() calls FC_writeBuf();
// 64 bytes max for USB
// 61 for I2C, USB-I2C bridge driver, SiLab part CP2112
//
// To write more than one packet of data to a device buffer:
// 1. write command (or address) to device
// 2. write repeated "continue" command with data
//
bool Host::write_file_to_device(uint8 command, uint32 size, uint8* buf)
{
    if(!FC_writeCommand(command)) return 0; // write buffer destination address
    uint8* bufptr = buf;
    uint32 page_size = TX_PACKET_SIZE_HOST - 2; // - size - header
    uint32 pages = size/page_size;
    uint32 bytes = size - (pages * page_size);
    for(uint32 i = 0;i < pages;i++){
        // log a message
        if(fmod((float)i, 200.0) == 0.0) Log("--- writing file to device");
        if(!FC_write(FC_COM_WR_CONTINUE, page_size, bufptr)) return 0;
        bufptr += page_size;
    }
    if(bytes){
        if(!FC_write(FC_COM_WR_CONTINUE, bytes, bufptr)) return 0;
    }
    return 1;
}
//
bool Host::txrx_test(void)
{
    uint32 i, j;
    static uint8 counter = 0; // change data each time test is run

    Log("");
    Log("FC_comms_test");
    Log("-The host OS (eg Win) causes long tx and rx times.");

    FC_stop();//_and_wait_for_features();

    const int bufsize = 1000;
    uint8 data_out[bufsize];
    uint8 data_in[bufsize];
    j = counter++;
    for(i = 0;i < bufsize;i++){
        data_out[i] = j++;
        data_in[i] = 0xff;
    }

    uint32 size = 1000; // test size

    QTime myTimer;
    // The first byte of each write is a command
    // so the multiple tx bursts (> 16 bytes)
    // need a "continue" command
    // to keep incrementing the rx address on the IoTcam
    myTimer.start();
    if(!write_file_to_device(FC_COM_WR_COMMS_TEST, size, data_out)){
        Log("***USB-VCP fail FC_comms_test write");
    }
    int nMillisecondsUp = myTimer.elapsed();
    Log("To device: " + IntToStr(nMillisecondsUp) + " mSec");
    int speedUp = (size * 8 * 1000)/nMillisecondsUp;
    Log(IntToStr(speedUp) + " bits per second");

    // The read address is always incremented
    // for multiple reads after the command FC_COM_WR_COMMS_TEST
    myTimer.start();
    if(!FC_read(FC_COM_RD_COMMS_TEST, size, data_in)){
        Log("***USB-VCP fail FC_comms_test read");
    }
    int nMillisecondsDown = myTimer.elapsed();
    Log("From device: " + IntToStr(nMillisecondsDown) + " mSec");
    if(nMillisecondsDown > 0){
        int speedDown = (size * 8 * 1000)/nMillisecondsDown;
        Log(IntToStr(speedDown) + " bits per second");
    }
    else Log("Over 8 megabits per second");

    int error = 0;
    for(i = 0;i < size;i++){
        if(data_out[i] != data_in[i]){
            error++;
            //Log("error " + IntToStr(i) + "  out " + IntToHex(data_out[i]) + "  in " + IntToHex(data_in[i]));
            //Log("*** error ***");
        }
        //Log(IntToStr(i) + "  out " + IntToHex(data_out[i]) + "  in " + IntToHex(data_in[i]));
    }

    //for(i = 0;i < size;i++){
    //    //Log(IntToStr(i) + ": " + IntToStr(data_in[i]));
    //    Log(IntToStr(i) + ": " + IntToHex(data_in[i]));
    //}
    Log(IntToStr(size) + " bytes");
    if(error)
        Log("*** " + IntToStr(error) + " errors");
    else
        Log(" - no errors");

    FC_run();

    if(error) return 0; // error
    return 1; // success
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
