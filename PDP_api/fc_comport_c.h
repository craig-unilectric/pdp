//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: fc_comport_c.h
Date: 6/5/2013
User: Craig Sullender

Class name:
Env. var.:
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#ifndef FC_COMPORT_C_H
#define FC_COMPORT_C_H
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


#define TX_PACKET_SIZE_HOST 61  // max 61 bytes for I2C, 64 bytes for USB



// Comms commands

bool FC_wait_for_slow_ack(uint8 timeoutSeconds);

bool FC_read(uint8 command, uint32 size, uint8* buf);
uint64 FC_readWord64(uint8 command);
uint32 FC_readWord32(uint8 command);
uint16 FC_readWord16(uint8 command);
uint8 FC_readByte(uint8 command);

bool FC_write(uint8 command, uint8 size, uint8* buf);
bool FC_write_noAck(uint8 command, uint8 size, uint8* buf);
bool FC_writeFile(uint8 command, uint32 size, uint8* buf);
bool FC_writeWord32(uint8 command, uint32 data);
bool FC_writeWord24(uint8 command, uint8* data);
bool FC_writeWord16(uint8 command, uint16 data);
bool FC_writeByte(uint8 command, uint8 data);
bool FC_writeByte_noAck(uint8 command, uint8 data);
bool FC_writeCommand(uint8 command);
bool FC_writeCommand_noAck(uint8 command);
bool FC_resetUART(void);


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#endif // FC_COMPORT_C_H
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

