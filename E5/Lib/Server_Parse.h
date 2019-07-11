#ifndef __SERVER_PARSE_H
#define __SERVER_PARSE_H

#ifdef __cplusplus
  extern "C"{
#endif

#include "CRC32.h"
#include "main.h"
#include "M2G.h"


typedef struct{

unsigned char PVER;

unsigned char CMD;// this file is the function

unsigned char CMD_VER; // the user must be define this file by the correct data length

unsigned char CMD_VER2;

unsigned short Len;

unsigned short CMD_No;

unsigned int   TIME;

unsigned int   UID;

unsigned int  _crc;

}ServerToMessageInfo; 


void ServerParseLoop(void);


#ifdef __cplusplus
  extern "C"{
#endif

#endif

