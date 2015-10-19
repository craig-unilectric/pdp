//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: usb_vcp.cpp
Date: 6/5/2013
User: Craig Sullender

Class name: USB_VCP
Env. var.: 
*/

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Device Disconnect
//
//

/*

notes from somewhere online:

QextSerialPort has similar signals into QextSerialEnumerator class:

    deviceDiscovered()
    deviceRemoved()

But I am not sure that they are implemented for all platforms.

Though, you can use other module QtSerialPort [qt-project.org]
which has QSerialPort::error() [qt-project.org] signal with
ResourceError [qt-project.org] code. This signal will be
emitted if to pull out already open device from system
(e.g. pull USB cable).

You can check it in “Simply Terminal” example (which is
delivered with QtSerialPport).
Just try to open device and pull an USB cable, then
Terminal should display the messagebox with “ResourceError” error.

QtSerialPort works, it provides signals for all serial port errors
so i can able to detect the USB port disconnect event.
but for this i have ported my code to QT5 then it worked

*/

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#include "usb_vcp.h"
#include <windows.h> // for Sleep()
#include "mainwindow.h" // for Log() and file and display functions
#include <math.h> // for sqrt(), abs(), log2(), pow(), fmod()
#include <QTime>

#include "ui_mainwindow.h"



#include <QApplication> // for qApp->processEvents()
#define PROCESS_WIN_EVENTS()  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents)

//#include <QSerialPort>
//#include <QSerialPortInfo>


#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QT_USE_NAMESPACE

// required in this file for QSerialPort to work
//QT_USE_NAMESPACE


//#include "fc_comport_c.h" // for FC_readByte() used for debug


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// IoTcam Virtual COM Port driver
//
// Install the STMicroelectronics Virtual COM Port driver
// V1.3.1 - 07/23/2010
// bad link -
// http://www.st.com/internet/com/SOFTWARE_RESOURCES/SW_COMPONENT/SW_DRIVER/stm32_vcp.zip
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

USB_VCP::USB_VCP(QWidget *parentMainWin) :
    QMainWindow(parentMainWin)
{

    // rename obj pointers
    mainWin = (MainWindow*)parentMainWin;



    ui = (Ui::MainWindow*)mainWin->ui;





    serial = new QSerialPort(this);


    enabled = false;

    using_uart = false;

    uart_baud = 115200;
    /*
    Log("---------------------------------");
    Log("USB comms");
    Log("Host USB-VCP Driver for USB Virtual COM Port");
    Log("Requires STMicroelectronics VCP drivers installed on host PC.");
    */
    Log("---------------------------------");
    Log("UART comms");
    Log(IntToStr(uart_baud) + "-8-N-1");
    Log("Drives USB to UART bridges such as the SiLabs CP2102.");
    Log("No driver installion required, or check SiLabs site.");
    Log("---------------------------------");



}
//
USB_VCP::~USB_VCP()
{
    vcp_close();
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Channel Write
//
//
bool USB_VCP::DeviceWrite(int numBytesToWrite, uint8* buf)
{
    //
    int16 bytesTx = serial->write((char*)buf, numBytesToWrite);
    if(bytesTx == -1){
        Log("*** VCP_DeviceWrite() tx error");
        return 0;
    }
    /*
    // Tx timeout
    // waitForBytesWritten() doesn't work in Qt5.
    // Returns false for an unknown error.
    if(!serial->waitForBytesWritten(100)) // timeout
    {
        Log("*** VCP_DeviceWrite() timeout");
        return 0;
    }
    */
    // Tx timeout replacement for Qt5.
    // Fully blocking, waits for all data to tx.
    int timeout_mS = numBytesToWrite/10;
    if(timeout_mS < 10) timeout_mS = 10;
    if(timeout_mS > 10000) timeout_mS = 10000;
    int i = 0;
    while(serial->bytesToWrite() && (i++ < timeout_mS)){ // mS timeout
        // For large files, log a message after each 1 sec.
        if(fmod((float)i, 1000.0) == 0.0) Log(IntToStr(serial->bytesToWrite()) + " bytes remain to write");
        Sleep(1); // 1mS delay
        // allow Windows to process events
        PROCESS_WIN_EVENTS(); // and to make Sleep() work in the loop
    }
    if(serial->bytesToWrite()){
        Log("*** VCP_DeviceWrite() timeout");
        Log(IntToStr(serial->bytesToWrite()) + " bytes remain to write");
        PROCESS_WIN_EVENTS(); // Log messages
        return 0;
    }
    return 1;
}
//
// optional routine to break large writes into fifo sized chunks
// for compatibility with SiLabs USB-I2C driver
bool USB_VCP::VCP_DeviceWrite(int numBytesToWrite, uint8* buf)
{
    // allow Windows to process events
    PROCESS_WIN_EVENTS();
    //
    // Clear com port rx buffer
    // to erase unread acks or data before next command
    //
    // Clear com port tx and rx buffers
    // causes error with CP2102 USB-UART when repeatedly sending slow data
    //clear_buffers(); // clears tx and rx buffers
    // Clear com port rx buffer
    serial->clear(QSerialPort::Input); // doesn't work
    int bytesavail = serial->bytesAvailable();
    uint8 val = 0;
    for(int i = 0; i < bytesavail;i++){
        serial->read((char*)&val, 1); // read one byte from port
    }
    /*
    // debug extra bytes in the rx buffer
    int bytesavail = serial->bytesAvailable();
    Log("*** Bytes in rx buffer: " + IntToStr(bytesavail));
    if(bytesavail){
        int val = FC_readByte(); // read one byte
        Log("*** val: 0x" + IntToHex(val));
    }
    */
    //
    if(!serial->isOpen()){
        Log("*** Port not open");
        return 0;
    }
    //
    if(!DeviceWrite(numBytesToWrite, buf)) return 0;
    //
    /*
    //
    // wait for write to finish to make write fully blocking
    // long timeout for large writes
    // for example 10k bytes at 115200 baud takes < 1 second
    // not an error state
    // comment out this section if not waiting for data to be sent
    //
    int i = 0;
    while(serial->bytesToWrite() && (i++ < 1000)){ // 10 second timeout
        // log a message after each 1 sec
        if(fmod((float)i, 100.0) == 0.0) Log(IntToStr(serial->bytesToWrite()) + " remain to write");
        // allow Windows to process events
        PROCESS_WIN_EVENTS();
        // 10mS delay
        Sleep(10); // mS
    }
    if(serial->bytesToWrite()){
        Log("*** VCP_DeviceWrite still writing");
    }
    */
    //
    return 1;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Channel Read
//
//
//
// Check if IoTcam responded before read was called.
//
bool USB_VCP::readWait(uint16 timeout_mS)
{
    if(serial->bytesAvailable()) return 1; // checks for old data
    if(serial->waitForReadyRead(timeout_mS)) return 1; // waits for new data to arrive
    /*
    //if(serial->waitForReadyRead(10000)) return 1; // waits for new data to arrive
    //else Log("+++ error waitForReadyRead()");
    int i = 0;
    while(!serial->bytesAvailable() && (i++ < timeout_mS)){ // mS timeout
        Sleep(1); // 1mS delay
        // allow Windows to process events
        PROCESS_WIN_EVENTS(); // and to make Sleep() work in the loop
    }
    if(serial->bytesAvailable()) return 1;
    Log("+++ error waitForReadyRead()");
    */
    return 0;
}
//
/*
QByteArray out = serial.readAll();
for(int i=0; i< out.length(); i++) {
qDebug() << (int) out[i];

    QString command;
    if(value != "") command = code + "=" + value + "\r";
    else command = code + "\r";
    usb_vcp->serial->write(command.toUtf8().constData(), command.length());

*/
//
bool USB_VCP::portRead(int maxThisRead, uint8* bufptr, int numBytesToRead, int* bytesRx, int* bytesTotal)
{
    *bytesRx = serial->read((char*)bufptr, maxThisRead);
    if(*bytesRx == -1){
        Log("*** VCP_DeviceRead() rx error");
        return 0;
    }
    *bytesTotal = *bytesTotal + *bytesRx;
    if(*bytesTotal > numBytesToRead){
        Log("*** VCP_DeviceRead() too many bytes rx");
        Log(IntToStr(numBytesToRead) + " bytes requested");
        Log("*** " + IntToStr(*bytesTotal) + " bytes rx");
        return 0;
    }
    if(numBytesToRead > 100) mainWin->setProgress((100 * *bytesTotal)/numBytesToRead);
    return 1;
}
//
bool USB_VCP::DeviceRead(int numBytesToRead, uint8* buf)
{
    uint8* bufptr = buf;
    int bytesTotal = 0;
    int bytesRx;
    //------------------------------------
    // long timeout loop
    //for(int i = 0;i < timeoutSeconds;i++){
        // Read all bytes requested
        if(readWait(1000)){ // timeout mS  //if(serial->waitForReadyRead(1000)){
            // read bytes from port
            if(!portRead(numBytesToRead, bufptr, numBytesToRead, &bytesRx, &bytesTotal)){
                Log("*** VCP portRead() error");
                return 0;
            }
            bufptr += bytesRx;
            if(bytesTotal == numBytesToRead) return 1; // good read of all bytes requested
            // read port until all bytes received
            while((bytesTotal < numBytesToRead) && readWait(100)){
            //while((bytesTotal < numBytesToRead) && serial->waitForReadyRead(100)){
            //while(serial->waitForReadyRead(100)){
                int remaining = (numBytesToRead - bytesTotal);
                if(!portRead(remaining, bufptr, numBytesToRead, &bytesRx, &bytesTotal)){
                    Log("*** VCP portRead() error");
                    return 0;
                }
                bufptr += bytesRx;
                PROCESS_WIN_EVENTS(); // allow Windows to process events
            }
            if(bytesTotal != numBytesToRead){
                Log("*** VCP_DeviceRead() too few bytes rx");
                Log(IntToStr(numBytesToRead) + " bytes requested");
                Log("*** " + IntToStr(bytesTotal) + " bytes rx");
                return 0;
            }
            return 1; // good read of all bytes requested
        }
        // progress message
        //Log("    * Waiting for ack or rx data");
        //mainWin->setProgress((100 * i)/timeoutSeconds);
        //PROCESS_WIN_EVENTS(); // allow Windows to process events
    //}
    //------------------------------------
    // timeout error
    //else{ // timeout
        Log("*** VCP_DeviceRead() timeout");
        Log(IntToStr(numBytesToRead) + " bytes requested");
        Log("*** " + IntToStr(bytesTotal) + " bytes rx");
        return 0;
    //}
    //------------------------------------
    return 1;
}
//
// For IoTcam USB
// last USB packet must be < 64 bytes
// so IoTcam adds one byte to the tx buffer when sending
// multiples of 64 bytes
//
// IoTcam UART does not add a byte.
//
// Instead of having the host rx one extra byte for USM comms,
// the VCP_DeviceWrite() function clears the rx buffer.
//
// Since the host is master, we expect to tx a command
// and then rx all data from that command.
// So clearing the rx buffer before each tx (VCP_DeviceWrite()) should do no harm.
//
bool USB_VCP::VCP_DeviceRead(int numBytesToRead, uint8* buf)
{
    if(!serial->isOpen()){
        Log("*** VCP_DeviceRead() port not open");
        return 0;
    }

    // allow Windows to process events
    PROCESS_WIN_EVENTS();

    bool success = true;
    if(numBytesToRead > 0) success = DeviceRead(numBytesToRead, buf);

    //int bytesavail = serial->bytesAvailable();
    //Log("*** Bytes remaining to read: " + IntToStr(bytesavail));

    // allow Windows to process events
    PROCESS_WIN_EVENTS();
    //if(numBytesToRead > 100)
    mainWin->idleProgress();
    if(!success) return 0;
    return 1;
}
//
// timeoutSeconds > 0
bool USB_VCP::VCP_WaitForDeviceAck(int timeoutSeconds)
{
    if(!serial->isOpen()){
        Log("*** VCP_DeviceRead() port not open");
        return 0;
    }
    // wait loop
    uint8 ack = 0;
    bool success = false;
    // check for fast ack
    if(readWait(1000)){
        success = true;
        serial->read((char*)&ack, 1); // read one byte from port
    }
    // wait for slow ack
    int i = 1;
    while(!success && (i++ < timeoutSeconds)){
        Log("--- waiting for ack");
        // allow Windows to process events
        mainWin->setProgress((100 * i)/timeoutSeconds);
        PROCESS_WIN_EVENTS();
        // wait for rx data
        if(readWait(1000)){
            success = true;
            serial->read((char*)&ack, 1); // read one byte from port
        }
    }
    //
    //Log("ack value: " + IntToStr(ack));
    //
    if(!success) Log("*** no ack");
    else if(ack != 1){
        Log("*** wrong ack value: " + IntToStr(ack));
        success = false;
    }
    //
    // allow Windows to process events
    mainWin->idleProgress();
    PROCESS_WIN_EVENTS();
    if(success) return 1;
    return 0;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Open Channel
//
//



void USB_VCP::show_info(QSerialPortInfo info)
{
    Log("");
    Log("Name: " + info.portName());
    Log("Desc: " + info.description());
    Log("Manu: " + info.manufacturer());
    Log("VID : 0x" + IntToHex(info.vendorIdentifier()));
    Log("PID : 0x" + IntToHex(info.productIdentifier()));
}
//
//
// unused after ListWidget was added
bool USB_VCP::show_ports(void)
{
    //
    Log("Select lowest Commport number of " + IntToStr(deviceList.count()) + " ports");
    //
    if(deviceList.count() > 0){
        QSerialPortInfo info = deviceList[0]; // select first device

        QString lowstr;
        QString newstr;
        QString lowstrnum;
        QString newstrnum;
        int lownum;
        int newnum;
        bool ok;

        //lowstr = info.portName();
        //lowstrnum = lowstr.right(lowstr.size() - 3); // comxx, xx number only
        //lownum = lowstrnum.toInt(&ok, 10);
        //Log("*** test port: " + lowstr);
        //Log("*** test port num: " + lowstrnum);
        //Log("*** test num: " + IntToStr(lownum));

        for(int i = 0;i < deviceList.count();i++){
            lowstr = info.portName();
            newstr = deviceList[i].portName();

            lowstrnum = lowstr.right(lowstr.size() - 3); // comxx, xx number only
            newstrnum = newstr.right(newstr.size() - 3); // comxx, xx number only

            lownum = lowstrnum.toInt(&ok, 10);
            if(!ok) Log("*** Bad port name to number conversion ***: " + info.portName());
            newnum = newstrnum.toInt(&ok, 10);
            if(!ok) Log("*** Bad port name to number conversion ***: " + deviceList[i].portName());

            if(newnum < lownum) info = deviceList[i];
        }
    }
    else Log("*** No IoTcam devices found");
    Log("");
    //open_port(int index)
}

bool USB_VCP::open_port(QListWidgetItem* item)
{
        int index = ui->listWidget_comms_list->row(item);
        //Log("row " + IntToStr(index));
        QSerialPortInfo info = deviceList[index];
        show_info(info);
        Log("");

        Log("Open Port...");

        // close port if already open
        serial->close();
        serial->setPort(info);
        if(serial->open(QIODevice::ReadWrite)){
            Log("Opened: " + info.portName());
            int baud = uart_baud;
            //int baud = 115200;
            //int baud = 460800;  // Peeple demo
            if(using_uart) Log("Settings: " + IntToStr(baud) + "-8-n-1");
            if(
                serial->setBaudRate(baud) &&
                //serial->setDataBits((QSerialPort::DataBits)8) &&
                serial->setDataBits(QSerialPort::Data8) &&
                serial->setParity(QSerialPort::NoParity) &&
                serial->setStopBits(QSerialPort::OneStop) &&
                serial->setFlowControl(QSerialPort::NoFlowControl))
            {
                //serial->setReadBufferSize(2048); // set 0 for unlimited
                //int size = serial->readBufferSize(); // returns 0 for unlimited
                //Log(IntToStr(size) + " rx buffer size");

                ui->listWidget_comms_list->setEnabled(false);
                //ui->listWidget_comms_list->hide();

                Log("");
                Log("-------------------");
                Log("");

                return 1;
            }
            else{
                Log("*** Could not set: " + IntToStr(baud) + "-8-n-1");;
                Log("");
                return 0;
            }
        }
        else Log("*** Could not open: " + info.portName());
    return 0;
}


/*
Name: COM4
Desc: STMicroelectronics Virtual COM Port
Manu: STMicroelectronics.
VID : 0x483
PID : 0x5740
*/
/*
Name: COM9
Desc: Silicon Labs CP210x USB to UART Bridge
Manu: Silicon Laboratories
VID : 0x10c4
PID : 0xea60
*/
bool USB_VCP::vcp_open(bool uart)
{
    bool success = false;
    using_uart = uart;
    comList.clear();
    deviceList.clear();
    // read all of the com ports
    comList = QSerialPortInfo::availablePorts();
    Log("");
    Log("-------------------");
    Log("Com ports found: " + IntToStr(comList.count()));
    // detect IoTcam devices
    foreach(const QSerialPortInfo &info, comList){
        show_info(info);
        //
        // Filter to select the IoTcam board:
        //
        /*
        bool found = false;
        if(info.description() == "STMicroelectronics Virtual COM Port") found = true;
        if(info.description() == "Silicon Labs CP210x USB to UART Bridge") found = true;
        */
        bool found = true;
        //
        // USB to IoTcam
        if(!using_uart){
            if(!(info.description() == "STMicroelectronics Virtual COM Port")) found = false;
            if(!(info.manufacturer() == "STMicroelectronics.")) found = false;
            if(!(info.vendorIdentifier() == 0x483)) found = false;
            if(!(info.productIdentifier() == 0x5740)) found = false;


            // nucleo board stlink vcp port is too flaky for Qt serial port ?
            // IoTcam
            if(!found){
                found = true;
                if(!(info.description() == "STMicroelectronics STLink Virtual COM Port")) found = false;
                if(!(info.manufacturer() == "STMicroelectronics")) found = false;
                if(!(info.vendorIdentifier() == 0x483)) found = false;
                if(!(info.productIdentifier() == 0x374b)) found = false;
            }


        }
        // USB to UART to IoTcam
        else if(using_uart){

            // SiLabs CP2102 USB-UART bridge
            if(!(info.description() == "Silicon Labs CP210x USB to UART Bridge")) found = false;
            if(!(info.manufacturer() == "Silicon Laboratories")) found = false;





            // Broadcom wifi module
            if(!found){
                found = true;
                if(!(info.description() == "WICED USB Serial Port")) found = false;
                if(!(info.manufacturer() == "FTDI")) found = false;
            }

        }
        //
        // add to device list
        if(found){
            deviceList.append(info);
        }
    }
    // list the comports with IoTcam attached
    Log("");
    Log("IoTcam devices found: " + IntToStr(deviceList.count()));
    //-----------------------
    // select a port
    // using QDeviceList and QListWidget
    if(deviceList.count() > 0){

        mainWin->ui->listWidget_comms_list->clear();
        for(int i = 0;i < deviceList.count();i++){
            QString portinfo = deviceList[i].portName();
            ui->listWidget_comms_list->addItem(portinfo);
        }
        //ui->listWidget_comms_list->item(0)->setSelected(true);

        // show list of ports
        // or if only one port then select it automatically
        //if(deviceList.count() > 1) ui->listWidget_comms_list->show();
        //else open_port(ui->listWidget_comms_list->item(0));

        //if(deviceList.count() == 1) success = open_port(ui->listWidget_comms_list->item(0));
        //else{
            success = false; // keep comms buttons enabled until port is selected and opened
            ui->listWidget_comms_list->show();
            Log("");
            Log("-------------------");
        //}
    }
    else Log("*** No IoTcam devices found");
    Log("");
    if(success) return 1;
    return 0;
}
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//
// Close Channel
//
//
bool USB_VCP::vcp_close(void)
{
    serial->close();

    //Log("Closing the CP2112 channel");
    // Close the device if it is opened
    /*
    if((HidSmbus_IsOpened(m_hidSmbus, &opened) == HID_SMBUS_SUCCESS) && opened)
    {
        HidSmbus_Close(m_hidSmbus);
        m_hidSmbus = NULL;
        return 1;
    }
    */
    //Log("*** USB_close_channel - already closed");
    return 0;
    // Notes:
    // Do not throw an exception here as this routine might be part of Win program close.
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Tests
//
//

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Reset Channel
//
//
bool USB_VCP::vcp_reset(void)
{
    Log("---- Reset USB channel");
    serial->close();
    Sleep(1000);
    int i = 0;
    while(!vcp_open(using_uart) && (i++ < 10)){
        Log("---- Port not ready");
        Sleep(1000);
    }
    if(serial->isOpen()){
        Log("Port open ----");
        return 1;
    }
    Log("*** Port not open");
    return 0;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Clear Buffers
//
//
bool USB_VCP::clear_buffers(void)
{
    //
    // Clear com port tx and rx buffers
    // serial->clear(QSerialPort::AllDirections);
    // serial->clear(QSerialPort::Input);
    // serial->clear(QSerialPort::Output);
    // QSerialPort bug:
    // -does not clear VCP buffers
    // -does clear USB buffers
    // Instead use: close() + open() to clear the buffers
    serial->close();
    serial->open(QIODevice::ReadWrite);
    //
    if(!serial->isOpen()){
        Log("*** Port not open");
        return 0;
    }
    //
    /*
    // Test for empty buffers
    // QSerialPort bug - prevents next read()
    int avail = serial->bytesAvailable();
    if(avail){
        Log("*** VCP read buf not clear");
        Log("*** Remaining bytes: " + IntToStr(avail));
        return 0;
    }
    */
    return 1;
}
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/*
uint8* bufptr = buf;
int avail;
int readcnt = 0;
int total = 0;
//int numBytesToRead = 1000;

    //if(serial->readWait(1000)){
    if(serial->waitForReadyRead(1000)){

        avail = serial->bytesAvailable();
        readcnt = serial->read((char*)bufptr, numBytesToRead);
        //Log(IntToStr(avail) + " avail");
        //Log(IntToStr(readcnt) + " readcnt");
        total += readcnt;
        bufptr += readcnt;
        //Log(IntToStr(total) + " total");


        while(serial->waitForReadyRead(100) && (total < numBytesToRead)){

            avail = serial->bytesAvailable();
            readcnt = serial->read((char*)bufptr, numBytesToRead);
            //Log(IntToStr(avail) + " avail");
            //Log(IntToStr(readcnt) + " readcnt");
            total += readcnt;
            bufptr += readcnt;
            //Log(IntToStr(total) + " total");



        }
    }






    uint8* bufptr = buf;
    const uint8 page_width = 8; // shift 8 = 256 max
    const uint16 page_size = pow(2, page_width);
    uint16 pages = numBytesToRead >> page_width; // divide by page_size
    uint16 bytes = numBytesToRead - (pages << page_width);
    for(unsigned int i = 0;i < pages;i++){
        if(!DeviceRead(page_size, bufptr)) return 0;
        bufptr += page_size;
    }
    if(bytes){
        if(!DeviceRead(bytes, bufptr)) return 0;
    }





    uint8* bufptr = buf;
    const uint8 page_width = 8; // shift 8 = 256 max
    const uint16 page_size = pow(2, page_width);
    uint16 pages = numBytesToWrite >> page_width; // divide by page_size
    uint16 bytes = numBytesToWrite - (pages << page_width);
    for(unsigned int i = 0;i < pages;i++){
        if(!DeviceWrite(page_size, bufptr)) return 0;
        bufptr += page_size;
    }
    if(bytes){
        if(!DeviceWrite(bytes, bufptr)) return 0;
    }



    if(!usb_vcp->serial->isOpen()){
        Log("*** VCP_DeviceRead() port not open");

    }

uint8* bufptr = buf;
int avail;
int readcnt = 0;
int total = 0;
int numBytesToRead = 1000;

    if(usb_vcp->serial->waitForReadyRead(1000)){

        avail = usb_vcp->serial->bytesAvailable();
        readcnt = usb_vcp->serial->read((char*)bufptr, numBytesToRead);
        //Log(IntToStr(avail) + " avail");
        //Log(IntToStr(readcnt) + " readcnt");
        total += readcnt;
        bufptr += readcnt;
        //Log(IntToStr(total) + " total");
        while(usb_vcp->serial->waitForReadyRead(100) && (total < numBytesToRead)){

            avail = usb_vcp->serial->bytesAvailable();
            readcnt = usb_vcp->serial->read((char*)bufptr, numBytesToRead);
            //Log(IntToStr(avail) + " avail");
            //Log(IntToStr(readcnt) + " readcnt");
            total += readcnt;
            bufptr += readcnt;
            //Log(IntToStr(total) + " total");


        }


    }

*/




//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
