//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: host.cpp
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
#include <QTimer>



#include "host.h"
#include "ui_mainwindow.h"
#include "mainwindow.h" // for Log() and file and display functions
#include <windows.h> // for Sleep()
#include <math.h> // for fmod(), sqrt(), abs(), log2(), pow()

#include "usb_vcp.h"


#include "pdp_command_c.h" // for data types and structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;


#include "fc_comport_c.h" // for Host to device serial port calls and TX_PACKET_SIZE_HOST


Host::Host(QWidget *ParentMainWin, USB_VCP *usbObj) :
    QWidget(ParentMainWin)
{

    // Rename obj pointers for convenience
    mainWin = (MainWindow*)ParentMainWin;
    ui = (Ui::MainWindow*)mainWin->ui;
    usb_vcp = (USB_VCP*)usbObj;



    // Comms
    connect(ui->pushButton_comms_open, SIGNAL(clicked()), this, SLOT(comms_open()));
    connect(ui->pushButton_comms_test, SIGNAL(clicked()), this, SLOT(comms_test()));
    //ui->listWidget_comms_list->hide();
    connect(ui->listWidget_comms_list, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(comms_open_selected(QListWidgetItem*)));



    // PDP
    connect(ui->pushButton_read_DMA, SIGNAL(clicked()), this, SLOT(read_DMA()));
    ui->pushButton_read_DMA->setVisible(false);


    connect(ui->pushButton_read_AC, SIGNAL(clicked()), this, SLOT(read_channelDMA()));
    ui->pushButton_read_AC->setVisible(false);
    connect(ui->pushButton_read_AC_filtered, SIGNAL(clicked()), this, SLOT(read_channelDMA_filtered()));

    connect(ui->pushButton_write_command, SIGNAL(clicked()), this, SLOT(write_command()));
    connect(ui->pushButton_read_status, SIGNAL(clicked()), this, SLOT(read_status()));


    connect(ui->pushButton_read_relay, SIGNAL(clicked()), this, SLOT(relay_read()));
    connect(ui->radioButton_relayOn, SIGNAL(clicked()), this, SLOT(relay_on()));
    connect(ui->radioButton_relayOff, SIGNAL(clicked()), this, SLOT(relay_off()));





    QGridLayout *gbox = new QGridLayout(ui->groupBox_relaySelect);
    int i = 0;
    for(int x = 0;x < 4; x++)
    {
        for(int y = 0;y < 4; y++)
        {
            relaySelectButton[i] = new QRadioButton(IntToStr(i));
            gbox->addWidget(relaySelectButton[i], y, x);
            connect(relaySelectButton[i], SIGNAL(clicked()), this, SLOT(relay_selected()));
            i++;
        }
    }
    ui->groupBox_relaySelect->setLayout(gbox);
    relayNumberSelected = 0;
    relaySelectButton[relayNumberSelected]->setChecked(true);



    // PDP graph init
    graph_init();



}
//
Host::~Host()
{

    //



}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Tabs
//
//
/*
void Host::newTabSelected(int tabIndex)
{
    static QString previousTab = ui->tabWidget_controls->tabText(0);
    //
    // Get the caption of the selected tab
    QString currentTab = ui->tabWidget_controls->tabText(tabIndex);
    //
    Log("Tab opened: " + currentTab);
    //
    //-----------------------------------
    //
    //-----------------------------------
    //
    previousTab = currentTab;
}
*/
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
//
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//




//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//

//
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
