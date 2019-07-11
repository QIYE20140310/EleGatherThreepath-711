
#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H

#ifdef __cplusplus
  extern "C"{
#endif

#include "Touchkey.h"


typedef struct{

	unsigned int ms;

	unsigned int secenable;

	unsigned int minuteenable;

	unsigned int minute2enable;

	unsigned int hourenable;

	unsigned int setmodecount;

	unsigned int powerenable;

	unsigned int powerenablecnt;

	unsigned int seccnt;
	unsigned int minutecnt;
	unsigned int hourcnt;

}System_TimeInfo;


typedef struct{

	float vol[3];

	float cul[3];

	float power[3];

	float temper[4];

	float totalpower;

	unsigned int volsave[3];

	unsigned int volsettigmode;

	unsigned int cursettingmode;

	unsigned int volsetmodenable;

	unsigned int tempermode;

	unsigned int temperenable;

					 int tempersetup;

	unsigned int leakcurmode;

	unsigned int leakcurenable;

	unsigned int leakcursetup;

	unsigned int cursetmodenable;

	unsigned int protectvolup;

	unsigned int protectvoldown;

	unsigned int protectculup;

	unsigned int protectculdown;

	unsigned int overvolenable;

	unsigned int missvolenable;

	unsigned int overcurenable;

	unsigned int misscurable;

	unsigned int discorrectplus;

	unsigned int powerenable;

	unsigned int lcdUpdateenable;

    unsigned int  enternwatchmode;  
		
	float        leakcurent;

	float        gathertemper[5];  

	unsigned  int ployenable;

}System_Info;

#define GREEN_LED   1U
#define YELLOW_LED  2U
#define RED_LED     3U

void SysTem_SaveFileInit(void);
void System_Config(void);
void System_TimerUpdate(void);
void System_TimerPeriod(void);
void System_Loop(void);
void System_Led_Init(void);
void System_Led_Onoff(char ledtype,char onoff);
void Beep_Init(void);
void Beep_Onoff(char onoff);
void Switch_Init(void);
void Switch_Alm(char onoff);
void Switch_Test(void);
void                                                Key_HandleByUser(void);
void Restory_Watch(void);


extern System_Info     gSystem_Info;
extern System_TimeInfo gSystem_TimeInfo;
extern unsigned int    gEnternmodeenable[4];
extern __IO char   password[4];
extern unsigned int    msemode;
extern __IO unsigned char  enternfactormode;

void Switch_Ploy(char i);
void System_EnterMode(void);
char Mode_adjust(void);
void Watch_Loop(unsigned char i);

#ifdef __cplusplus
}
#endif

#endif
