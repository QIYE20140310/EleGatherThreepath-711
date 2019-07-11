#ifndef    __TEM_ADC_H
#define   __TEM_ADC_H


#ifdef __cplusplus
extern "C"{
#endif
#include "main.h"
void          Tem_AdcInit(void);
void          Tem_AdcLoop(void);
void          EleHandle_Loop(void);
extern  float Gather_temper[5];
extern  float gLeakCurrent;

#ifdef __cplusplus
}
#endif


#endif
