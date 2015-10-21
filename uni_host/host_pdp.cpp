//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: host_pdp.cpp
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


#include "usb_vcp.h"


#include "pdp_command_c.h" // for data types and structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;

#include "fc_comport_c.h" // for Host to device serial port calls and TX_PACKET_SIZE_HOST



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
// PDP
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
//
//
/*
typedef struct
{
        // add dummy values for 32 bit alignment
    uint16       i_dc[16];
    uint16       i_pk[16];
    uint16       i_rms[16];
    uint8       one;
    uint8       two;
    uint8       four;
    uint8      	dummy_2;
        uint8       three[8];
    uint16      five;
    uint16      six;
    uint16      seven;
    uint16      dummy_1;
    uint32      val32;
}PDP_Status_Struct;
*/

void Host::read_status_channel(uint8 channel)
{


    pdp_read_status_struct();


    Log("");
    Log("Status Struct");
    uint32 size = sizeof(PDP_Status_Struct);
    Log("size: " + IntToStr(size));
    Log("");

    int i = channel;


    float periodms = (float)ps.voltage_cycle_time/100.0f;
    float period = (float)ps.voltage_cycle_time/100000.0f;
    float freq = 1/period;


    Log("");
    Log("     pk[" + IntToStr(i) + "]: " + IntToStr(ps.pk[i]));
    Log("     rms[" + IntToStr(i) + "]: " + IntToStr(ps.rms[i]));
    Log("");
    Log("     real[" + IntToStr(i) + "]: " + IntToStr(ps.real[i]));
    Log("     apparent[" + IntToStr(i) + "]: " + IntToStr(ps.apparent[i]));
    Log("     power factor[" + IntToStr(i) + "]: " + IntToStr(ps.power_factor[i]));
    Log("");
    Log("     dc[" + IntToStr(i) + "]: " + IntToStr(ps.dc[i]));
    Log("");
    Log("     cycle time (mS): " + IntToStr(periodms));
    //Log("     cycle freq (Hz): " + IntToStr(freq));
    Log("     cycle freq (Hz): " + IntToStr(ps.voltage_freq)); // cycle freq from device
    Log("");
    Log("     samples per cycle: " + IntToStr(ps.voltage_cycle_samples));
    Log("");
    Log("     debug1: " + IntToStr(ps.debug1));
    Log("     debug2: " + IntToStr(ps.debug2));
    Log("");
    if(ps.voltage_cycle_length_error) Log("*** voltage_cycle_length_error");
    Log("");
    Log("---- Command check ----");
    Log("     trip_level[" + IntToStr(i) + "]: " + IntToStr(ps.trip_level[i]));
    Log("     ical[" + IntToStr(i) + "]: " + IntToStr(ps.ical[i]));
    Log("     vcal: " + IntToStr(ps.vcal));
    Log("     vphase: " + IntToStr(ps.vphase));
    Log("----");
    Log("");





    ui->label_ctdc->setText(IntToStr(ps.dc[i]));
    ui->label_peak->setText(IntToStr(ps.pk[i]));
    ui->label_rms->setText(IntToStr(ps.rms[i]));




    Log("");



}


void Host::read_status(void)
{


    pdp_read_status_struct();


    Log("");
    Log("Status Struct");
    uint32 size = sizeof(PDP_Status_Struct);
    Log("size: " + IntToStr(size));
    Log("");

    for(int i = 0;i < 16;i++){
        Log("     i_dc[]: " + IntToStr(i) + " - " + IntToStr(ps.dc[i]));

    }


    /*
    Log("one: " + IntToHex(ps.one));
    Log("two: " + IntToHex(ps.two));
    Log("three");
    Log("     " + IntToHex(ps.three[0]));
    Log("     " + IntToHex(ps.three[1]));
    Log("     " + IntToHex(ps.three[2]));
    Log("     " + IntToHex(ps.three[3]));
    Log("     " + IntToHex(ps.three[4]));
    Log("     " + IntToHex(ps.three[5]));
    Log("     " + IntToHex(ps.three[6]));
    Log("     " + IntToHex(ps.three[7]));
    Log("four: " + IntToHex(ps.four));
    Log("five: " + IntToHex(ps.five));
    Log("six: " + IntToHex(ps.six));
    Log("seven: " + IntToHex(ps.seven));
    Log("val32: " + IntToHex(ps.val32));
    */
    Log("");



}

void Host::write_command(void)
{

    /*
    pc.one = 0x98;
    pc.two = 0x76;
    pc.three[0] = 0x54;
    pc.three[1] = 0x55;
    pc.three[2] = 0x56;
    pc.three[3] = 0x57;
    pc.three[4] = 0x58;
    pc.three[5] = 0x59;
    pc.three[6] = 0x5a;
    pc.three[7] = 0x5b;
    pc.four = 0x32;
    pc.five = 0x1234;
    pc.six = 0x5678;
    pc.seven = 0x9876;
    pc.val32 = 0x12345678;
    */
    pdp_write_command_struct();


}



//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
//
//
//
void Host::read_DMA(void)
{

        const int bufsize = 80000;
        uint8 buf[bufsize];
        int bufsize16 = bufsize/2; // 16 bit DMA

        uint16 *val16_ptr;
        uint16 val16;


        int size = pdp_readRawDMA(buf, bufsize);
        Log("FC_COM_RD_I_RAW_SIZE bytes: " + IntToStr(size));


        //for(int i = 0;i < 1000;i++){
        //    val32_ptr = (uint32*)&data_in[i * 64];
        //    *val32_ptr = i * 4;
        //}


        int channel = get_relay_selected();
        uint16 buf_16[bufsize16];

        if(size > 3){

            size = (size/2)/16; // halfword, one of 16 channels

            for(int i = 0;i < size;i++){
                val16_ptr = (uint16*)&buf[((i * 16) + channel) * 2]; // buf in bytes
                val16 = *val16_ptr;
                //Log("    " + IntToStr(val16));
                buf_16[i] = val16;
            }

            graph_ac_with_offset(channel, (uint16*)buf_16, size);
        }
        else Log("***USB-VCP fail pdp_readRawDMA()");


}
//
void Host::read_channelDMA_filtered(void)
{
    const int bufsize = 1000;
    const int bufbytes = 2 * bufsize;
    int16 buf[bufsize];

    int channel = get_relay_selected();

    //------------------------
    //------------------------
    // read raw DMA values for one channel

    int size = pdp_readChannelDMA_filtered(channel, (uint8*)buf, bufbytes); // bytes

    // FC_run(); is disabled for now

    Log("");
    Log("read_channelDMA_filtered()");
    Log("FC_COM_RD_CHANNEL_I_RAW_SIZE bytes: " + IntToStr(size));

    size = size/2; // halfword

    // plot directly
    graph_ac(channel, (int16*)buf, size);
    //graph_ac_with_offset(channel, (uint16*)buf, size);

    Log("    cycle length used by device: " + IntToStr(size));


    //------------------------
    //------------------------
    // read status


    //pdp_read_status_struct();


    read_status_channel(channel);



    //
    //
    //


    clear_metrics(channel);
    // or
    //show_metrics(channel);




    //
    //
    //
    FC_run(); // redundant




 }
//
void Host::read_channelDMA(void)
{
    const int bufsize = 1000;
    const int bufbytes = 2 * bufsize;
    uint16 buf[bufsize];

    int channel = get_relay_selected();

    //------------------------
    //------------------------
    // read raw DMA values for one channel

    int size = pdp_readChannelDMA(channel, (uint8*)buf, bufbytes); // bytes

    // FC_run(); is disabled for now

    Log("");
    Log("FC_COM_RD_CHANNEL_I_RAW_SIZE bytes: " + IntToStr(size));

    size = size/2; // halfword

    // plot directly
    graph_ac_with_offset(channel, (uint16*)buf, size);
    Log("    cycle length used by device: " + IntToStr(size));
    int cyclelength = size;

    /*
    //
    // or look for zero crossing
    //
    int lengthmin = 500;//972;
    int cyclelength = 973;
    int lengthmax = 974;

    int y1, y2, y3;

    if(size > lengthmin - 4){

        // find one complete cycle *** if no zero crossing signal

        y1 = buf[0];
        int i;
        for(i = lengthmin;i < lengthmax;i++){

            y2 = buf[i - 1];
            y3 = buf[i];

            if((y3 >= y2) && ((y1 >= y2) && (y1 <= y3))) {cyclelength = i - 1; break;}
            if((y3 <= y2) && ((y1 <= y2) && (y1 >= y3))) {cyclelength = i - 1; break;}

        }

        //if(i == lengthmax) Log("*** no cycle length");
        //else


        //cyclelength = 972; // set in PDP processor

        Log("    cycle length calculated by host: " + IntToStr(cyclelength));
        Log("    cycle length used by device: " + IntToStr(size));

        //graph_ac(channel, (uint16*)buf, cyclelength);//size);
        graph_ac_with_offset(channel, (uint16*)buf, size);

    }
    else Log("***USB-VCP fail pdp_readChannelDMA()");
    */

    //------------------------
    //------------------------
    // read status


    //pdp_read_status_struct();


    read_status_channel(channel);



    //FC_run(); // redundant




    //------------------------
    //------------------------
    // DC

    show_metrics(channel);


    size = cyclelength;
    uint16 buf_16[bufsize];
    uint32 sum = 0;
    uint16 dc = 0;
    uint16 val16;

    for(int i = 0;i < size;i++){
        val16 = buf[i];
        sum += val16;
        //Log("  " + IntToStr(i) + ": " + IntToStr(val16));
    }

    dc = sum/size;

    Log("");
    Log(" host calculated CT-DC " + IntToStr(dc));

    for(int i = 0;i < size;i++){
        buf_16[i] = dc;
    }

    //graph_dc(channel, (uint16*)buf_16, size);



    //------------------------
    //------------------------
    // avg dc
    /*
    sum = 0;
    uint16 avgdc = 0;
    for(int i = 0;i < size;i++){
        sum += buf_16[i];
    }
    avgdc = sum/size;
    Log(" avg CT-DC " + IntToStr(avgdc));
    */



}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
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
//




//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
