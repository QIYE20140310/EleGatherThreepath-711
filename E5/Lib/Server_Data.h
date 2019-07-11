#ifndef __SERVER_DATA_H
#define __SERVER_DATA_H

#ifdef __cplusplus
  extern "C"{
#endif

#include "CRC32.h"
#include "Server_Parse.h"

#define IP_MAC0   0x12U
#define IP_MAC1   0x34U
#define IP_MAC2   0x45U
#define IP_MAC3   0x56U
#define IP_MAC4   0x67U
#define IP_MAC5   0x87U

typedef struct
{

unsigned char PVER;

unsigned char CMD;// thi;s file is the function

unsigned char CMD_VER; // the user must be define this file by the correct data length

unsigned char CMD_VER2;

unsigned short Len;

unsigned short CMD_No;

unsigned char MAC[6];

unsigned char TIMEZONE;

unsigned char HOSTID;

unsigned int  _crc;

}ModbusDataToServer; 

void DataToServerLoop(void);
void CRC_Hradware_Init(void);

void Push_Net_Config(void);
void NetConfig_Parse(void);

void Push_Sw_Config(void);
void SW_Netconfig(void);

void Status_ReportMessage(void);
void DataToServerHeartBeat(void);

extern char _tempcrc[200];

#ifdef __cplusplus
}
#endif
#endif

