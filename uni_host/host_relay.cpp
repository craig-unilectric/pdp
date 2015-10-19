//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: host_relay.cpp
Date: 2015
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


#include "pdp_command_c.h" // for data types and structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;




//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
// Relay
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------






//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// RadioButtons
//
//
//
void Host::relay_selected(void)
{
    for(int i = 0; i < RELAY_COUNT;i++){
        if(relaySelectButton[i]->isChecked()) relayNumberSelected = i;
    }
    Log("Relay selected: " + IntToStr(relayNumberSelected));

    ui->label_channel->setText("Channel " + IntToStr(relayNumberSelected));


    // write to device

    FC_writeByte(FC_COM_WR_CHANNEL_NUMBER, relayNumberSelected);






}
//
// ********************* need to add get_relay_selected() - read real value from device
//
int Host::get_relay_selected(void)
{
    //for(int i = 0; i < RELAY_COUNT;i++){
    //    if(relaySelectButton[i]->isChecked()) relayNumberSelected = i;
    //}
    return relayNumberSelected;
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
//
//
void Host::relay_read(void)
{

    Log("");
    Log("relay_read()");


    uint8 val = FC_readByte(FC_COM_RD_RELAY_STATE);
    Log("relay state: " + IntToStr(val));


    if(val) ui->radioButton_relayOn->setChecked(true);
    else ui->radioButton_relayOff->setChecked(true);

}
//
void Host::relay_on(void)
{

    Log("");
    Log("relay_on()");

    FC_writeCommand(FC_COM_WR_RELAY_ON);


}
//
void Host::relay_off(void)
{

    Log("");
    Log("relay_off()");


    FC_writeCommand(FC_COM_WR_RELAY_OFF);


}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
