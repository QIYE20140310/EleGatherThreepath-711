#ifndef _TOUCHKEY_H
#define _TOUCHKEY_H

//#include "FreeRTOS.h"
//#include "task.h"

#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"
#include "RN8302B.h"

typedef enum{

	  KEY_ENUM_STATUS_INIT=1u,

	  KEY_ENUM_STATUS_SHORT=2u,
	  
	  KEY_ENUM_STATUS_LONG=3u,

  }KEY_STATUSENUM;
	  
#define J7_Borad 0

#if J7_Borad   
#define  KEY_A_PORT                                  GPIOC
#define  KEY_A_PIN                                   GPIO_PIN_3
#define  KEY_B_PORT                                  GPIOC
#define  KEY_B_PIN                                   GPIO_PIN_13
#define  KEY_C_PORT                                  GPIOD
#define  KEY_C_PIN                                   GPIO_PIN_4
#define  KET_MENU_OK_PORT                            GPIOI
#define  KEY_MENU_OK_PIN                             GPIO_PIN_8
#define  KET_TOP1_PORT                               GPIOG
#define  KEY_TOP1_PIN                                GPIO_PIN_2
#define  KET_TOP2_PORT                               GPIOG
#define  KEY_TOP2_PIN                                GPIO_PIN_3
/***************OUT*************/
#define  KET_MENU_OKLED_PORT                         GPIOE
#define  KEY_MENU_OKLED_PIN                          GPIO_PIN_3
#define  KET_MENU_ReturnLED_PORT                     GPIOF
#define  KEY_MENU_ReturnLED_PIN                      GPIO_PIN_7
#else 
#define  KET_LEFT_PORT                               GPIOC
#define  KEY_LEFT_PIN                                GPIO_PIN_10
#define  KET_RIGHT_PORT                              GPIOC
#define  KEY_RIGHT_PIN                               GPIO_PIN_11
#endif
                                    
#define KEYEVNET(x,y)								     ((unsigned short)( (unsigned short)(y<<8)|(x)))
#define Is_KeyDown(x)								     (keyEventValue==KEYEVNET(x,KEY_ENUM_STATUS_SHORT))
#define Is_KeyLongDown(x)							   (keyEventValue==KEYEVNET(x,KEY_ENUM_STATUS_LONG))
#define Is_KeyNone(x)								     (keyEventValue==KEYEVNET(0,KEY_ENUM_STATUS_INIT))

#define KeyFactorLeftValue		                             0x01u
#define KeyFactorRightValue 	                             0x02u
#define KeyNodeValue			                                 0x03u

extern unsigned int                                        gKey_LoopEnable;
extern unsigned int                                        gkey_timer_1msenable;

void	                                            touchBSP(void);
unsigned char                                       Key_ReadFactory(void);
void                                                I2C_TouchGetKey(void);
void                                                Key_VirtualInit(void);
unsigned char                                       Key_Loop(void);
unsigned short                                      Key_GetEvent(void);
void                                                Key_PeriodHandle(void);

extern unsigned short								 keyEventValue;


#ifdef __cplusplus
		}
#endif

#endif

























