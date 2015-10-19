//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: pdp_command_c.h
Date: 2015
User: Craig Sullender

Class name: 
Env. var.: 
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#ifndef PDP_COMMAND_C_H
#define PDP_COMMAND_C_H

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


// Command struct
// Status struct
#include "uni_host_structs.h" // for Host-device structs



//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//



void pdp_write_command_struct(void);
bool pdp_read_status_struct(void);


int pdp_readRawDMA(uint8* buf, uint32 sizeMaxBytes);
int pdp_readChannelDMA(uint8 channel, uint8* buf, uint32 sizeMaxBytes);
int pdp_readChannelDMA_filtered(uint8 channel, uint8* buf, uint32 sizeMaxBytes);



// Device controls
void FC_stop(void);
void FC_run(void);
void FC_stm32_reset(void);

// Device ID
uint32 FC_read_chip_ID(void);
void FC_read_chip_UDID(uint32* val1, uint32* val2, uint32* val3);


// Device comms test
uint32 FC_txrx_test(void);







//-------------------------------------------------------------------------
//-------------------------------------------------------------------------







//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

