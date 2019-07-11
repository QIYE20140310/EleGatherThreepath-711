#ifndef    __M2G_H
#define    __M2G_H

#ifdef __cplusplus
  extern "C"{
#endif


#include "stdio.h"
#include "string.h"
#include "main.h"
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_uart.h"
#include "Server_Data.h"
#include "System_Config.h"


typedef unsigned char bool;
typedef unsigned int size_t;

#define configTOTAL_HEAP_SIZE   128U
#define AlignmentMask           0x03u
#define NULL                    (void*)0u
#define DMA_INTERENABLE  1

#define DEST_ADD_IP1          120U
#define DEST_ADD_IP2          77U
#define DEST_ADD_IP3          226U
#define DEST_ADD_IP4          82U


void M2G_DeviceID(void);

#define MAX_ModLend   256U


typedef enum{

Query_DeviceID=0,

Query_Softwarever,

QUery_IMEI,

Query_CheckSelf,

Query_CallPhone,

Query_NetStatus,// check your network every 10sec 



}M2GToServerMessageEnum;

typedef struct{
	size_t                  timeout;
	size_t                  curtime;
	M2GToServerMessageEnum  cmd;
	size_t                  rxenbale;
	char*                   sendptr;
	char*                   preceive;
    size_t                  suspend;
	char                    ref;//rever;
	size_t                  succnt;
	size_t                  errorcmdcnt;
	size_t                  txsendcnt;
	size_t                  txsendenable;
	size_t                  severlinkenable;
}Query_M2GManeger;


#define configM2G_UARST            USART3
extern  Query_M2GManeger gQuer_M2GManeger;
extern  unsigned int reportloopenable;
extern  unsigned int gHeart_Beatenable;
extern  unsigned int m26_Initable;

void BSP_UartReceive(size_t len);
void ReceiveData_Parse(void);
char m28_SendLoop(void);
void HAL_USART_User(void);
////////////////////////////////////////////
char m26_Status_ReportLoop(void);
char Heart_BeatLoop(void);

#ifdef __cplusplus
    }
#endif

#endif

