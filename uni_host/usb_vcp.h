//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: usb_vcp.h
Date: 6/5/2013
User: Craig Sullender

Class name: USB_VCP
Env. var.: 
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#ifndef USB_VCP_H
#define USB_VCP_H
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*






*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#include <QMainWindow>
#include "mainwindow.h"


#include "pdp_command_c.h" // for data types


#include <QtSerialPort/QSerialPort.h>
#include <QtSerialPort/QSerialPortInfo.h>

QT_USE_NAMESPACE


#include <QListWidget>


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

class USB_VCP : public QMainWindow
{
    Q_OBJECT

public:

    explicit USB_VCP(QWidget *parentMainWin);
    ~USB_VCP();

    bool VCP_DeviceWrite(int numBytesToWrite, uint8* buf);
    bool VCP_DeviceRead(int numBytesToRead, uint8* buf);
    bool VCP_WaitForDeviceAck(int timeoutSeconds);

    QSerialPort* serial; // public for dev

    bool vcp_open(bool uart);

    bool show_ports(void);

    bool open_port(QListWidgetItem * item);


    bool enabled;
    bool using_uart;

    int uart_baud;


private slots:





public slots:


    //bool vcp_open();
    bool vcp_close();
    bool vcp_reset();
    //void vcp_test();




protected:




private:

    MainWindow* mainWin;


    Ui::MainWindow *ui;


    QList<QSerialPortInfo> comList;
    QList<QSerialPortInfo> deviceList;



    bool readWait(uint16 timeout_mS);

    bool portRead(int maxThisRead, uint8* bufptr, int numuint8sToRead, int* uint8sRx, int* uint8sTotal);

    bool DeviceWrite(int numuint8sToWrite, uint8* buf);
    bool DeviceRead(int numuint8sToRead, uint8* buf);

    bool clear_buffers(void);


    void show_info(QSerialPortInfo info);


    //bool open_port(void);


};











//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#endif // USB_VCP_H
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
