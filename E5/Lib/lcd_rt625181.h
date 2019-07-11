#ifndef __RT_625181_H
#define __RT_625181_H

#ifdef __cplusplus
	extern "c"{
#endif

#include "RN8302B.h"

#define LCD_RT_NUMBERS   50

#define TOOHOT        1
#define MAKEFIRE      2
#define ENVIRONMENT   
#define LOSELEC       3
#define OVERCURRENT   4
#define OVERVOL       5
#define MissVols      6
#define SHORTCIR      7 
#define LEAKCURRENT   8
typedef struct{

unsigned char data[LCD_RT_NUMBERS];

}LCD_Ram_Info;

void disp_init(void);

//public
void dispLCD_Loop(void);

void dispSystick(void);
void LCD_PowerOn(void);


#ifdef __cplusplus
	}
#endif

#endif
