
#include "System_Config.h"
#include "lcd_rt625181.h"
#include "M2G.h"


unsigned int gEnternmode[4]={0,0,0,0};
unsigned int gEnternmodeenable[4]={0,0,0,0};


System_TimeInfo gSystem_TimeInfo;
System_Info  gSystem_Info;

#define ADDR_FLASH_PAGE_Start   ((uint32_t)0x08008000) /* Base @ of Page 34, 2 Kbytes */
#define ADDR_FLASH_PAGE_End     ((uint32_t)0x08008800) /* Base @ of Page 35, 2 Kbytes */
#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_Start   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_End   /* End @ of user Flash area */
static FLASH_EraseInitTypeDef EraseInitStruct;

void SysTem_SaveFileInit(void)
{
	HAL_FLASH_Unlock();
	uint32_t Address = 0, PageError = 0;
	/* Erase the user Flash area
	  (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
	
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase	= FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
	EraseInitStruct.NbPages 	= (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;
	
	if (HAL_FLASHEx_Erase((FLASH_EraseInitTypeDef*)&EraseInitStruct, &PageError) != HAL_OK)
	{

	}
	

}

void System_FileSave(unsigned int *pdata,unsigned int len)
{

	uint32_t startadd=FLASH_USER_START_ADDR;

	uint32_t i=0;

	if((pdata==(void*)NULL)||(len<=0)) return ;

	HAL_FLASH_Unlock();
    
    while(startadd<FLASH_USER_END_ADDR){

		 if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startadd, pdata[i]) == HAL_OK)
		 {

		   startadd = startadd + 4;

		   i=i+4;

		 }
		else
		 {
		
		 }

    }
	
	HAL_FLASH_Lock();

}


void System_SaveDb(void)
{
  gSystem_Info.volsave[1]=36;
	System_FileSave((unsigned int*)&gSystem_Info,sizeof(System_Info));

}

void System_FileRead(unsigned int *pdata,unsigned int len)
{

	uint32_t startadd=FLASH_USER_START_ADDR;

	uint32_t i=0;

	if((pdata==(void*)NULL)||(len<=0)) return ;

    while(startadd<FLASH_USER_END_ADDR){
         
          pdata[i]=*(__IO uint32_t *)(startadd+i);;

		  startadd=startadd+4;

		  i=i+4;

	}
}



void System_Loop(void)
{
	System_Info  _syscurinfo;

	System_FileRead((unsigned int *)&_syscurinfo,sizeof(System_Info));

	if(_syscurinfo.temper[0]!=gSystem_Info.temper[0]){


	}

	System_SaveDb();

}

/******************************************************************
*Function:
*Parame:
*Return:
*Author:hjw 
*****************************************************************/
unsigned int minutecnt=0;
unsigned int adcenable=0;
unsigned int Rn8302_enable=0;
unsigned int gsystem_run=0;

unsigned int gsystime5s=0;
void System_TimerUpdate(void)
{	
	gSystem_TimeInfo.ms++;
	if(gSystem_TimeInfo.ms%1000U==0U){
		gSystem_TimeInfo.secenable=1;
		if(++gSystem_TimeInfo.seccnt>59U){
					gSystem_TimeInfo.seccnt=0U;
					if(++gSystem_TimeInfo.minutecnt>59U){
		                 gSystem_TimeInfo.minutecnt=0;
						 gSystem_TimeInfo.hourenable=1;
					}
					gSystem_TimeInfo.minuteenable=1;
		}
	}
	if(gSystem_TimeInfo.powerenable){
       gSystem_TimeInfo.powerenablecnt++;
	}
    if(gQuer_M2GManeger.txsendenable)
	{
       gQuer_M2GManeger.txsendcnt++;  
	}
		//send timer's message Info by systemer
	if(gSystem_TimeInfo.ms%10000U==0){
		reportloopenable=1; //add by hjw in 4-30		
	}

	//send heart beat message
	if(gSystem_TimeInfo.minuteenable){
	   if(++minutecnt>=2){	
	   	    if(gHeart_Beatenable==0){
	          //reportloopenable=1; //cancel it 
	   	    }
			minutecnt=0;	
	   }
	  gHeart_Beatenable=1;
	  gSystem_TimeInfo.minuteenable=0;
	}

	if(gSystem_TimeInfo.ms%3000U==0U){
		gSystem_Info.lcdUpdateenable=1;
		gsystem_run=1;
	
	}
	// add by hjw in 5-3 
	if(gSystem_TimeInfo.ms%200U==0U){
       if(gSystem_Info.volsettigmode){
           gSystem_Info.volsetmodenable ^=1;
       }else{
            gSystem_Info.volsetmodenable =0;;
       }
	   if(gSystem_Info.cursettingmode){	   	
		   gSystem_Info.cursetmodenable ^=1;		   
	   }else{
		   gSystem_Info.cursetmodenable =0;  
	   }

	   if(gSystem_Info.leakcurmode){
          gSystem_Info.leakcurenable ^=1;
	   }else{
          gSystem_Info.leakcurenable =0;
	   }

	   if(gSystem_Info.tempermode){
		  gSystem_Info.temperenable ^=1;
	   }else{
		  gSystem_Info.temperenable =0;
	   }

       if(gEnternmode[0]){
		   gEnternmodeenable[0] ^=1;
       }else{
		   gEnternmodeenable[0]=0;
       }
	   if(gEnternmode[1]){
		   gEnternmodeenable[1] ^=1;
       }else{
		   gEnternmodeenable[1]=0;
       }
	   if(gEnternmode[2]){
		   gEnternmodeenable[2] ^=1;
       }else{
		   gEnternmodeenable[2]=0;
       }
	   if(gEnternmode[3]){
		   gEnternmodeenable[3] ^=1;
       }else{
		   gEnternmodeenable[3]=0;
       }
	   
	}
    if(gSystem_TimeInfo.ms%800U==0U){
		adcenable=1;
    }
	if(gSystem_TimeInfo.ms%50U==0U){
        Rn8302_enable=1;
	}
	
	if(gSystem_TimeInfo.ms%20U==0U){
	}

    if(gSystem_TimeInfo.ms%5000U==0U){
		gsystime5s=1;
    }
}




void System_Config(void)
{
	memset((char*)&gSystem_Info,0,sizeof(gSystem_Info));
	gSystem_Info.misscurable=0;
	gSystem_Info.missvolenable=0;
	gSystem_Info.overcurenable=0;
	gSystem_Info.overvolenable=0;
	gSystem_Info.protectvolup=263;
	gSystem_Info.protectvoldown=200;
	//gSystem_Info.protectculup=2000;
	//gSystem_Info.protectculdown=500;
	gSystem_Info.tempersetup=70;
	gSystem_Info.leakcursetup=30;	
	gSystem_Info.volsetmodenable=0;
	gSystem_Info.cursetmodenable=0;
	gSystem_Info.ployenable=0;
	
}

unsigned char ledstatus=0;

void System_TimerPeriod(void)
{
	if(adcenable)
	{
		adcenable=0;
		Tem_AdcLoop(); 			
	}
    if(Rn8302_enable){
        Rn8302_enable=0;
	    ReadAvol();	
    }
    if(gsystem_run)
    {
		gsystem_run=0;
		ledstatus ^=1;
		System_Led_Onoff(GREEN_LED,ledstatus);  
		
    }
	/****************Vol Adjust************************/
	for(int i=0;i<3;i++){
		if(gSystem_Info.vol[i]<=gSystem_Info.protectvoldown){
		gSystem_Info.missvolenable=1;
		}
		else if(gSystem_Info.vol[i]>=gSystem_Info.protectvolup)
		{		
		gSystem_Info.overvolenable=1;
		}
		else{                
		gSystem_Info.missvolenable=0;				
		gSystem_Info.overvolenable=0;
		}			
	}
	/****************Cur Adjust************************/
	for(int j=0;j<3;j++){
		if(gSystem_Info.cul[j]>=gSystem_Info.protectculup){
		gSystem_Info.overcurenable=1;
		}else if(gSystem_Info.cul[j]<=gSystem_Info.protectculdown){				
		gSystem_Info.misscurable=1;
		}else{            
		gSystem_Info.misscurable=0;			   
		gSystem_Info.overcurenable=0;
		}
	}
	if(gSystem_Info.powerenable){
	    M26_PowerOn();
	}

}



void Restory_Watch(void)
{
	if(gSystem_Info.enternwatchmode==1){
			gSystem_Info.enternwatchmode=0;
			Beep_Onoff(0);	   
			Switch_Alm(0); 
	}
}

unsigned int gbeepflag1=0;
unsigned int gbeepflag2=0;
unsigned int temperenable=0;
unsigned int leakenable=0;
unsigned int lockdowmenable=0;
void Watch_Loop(unsigned char i)
{
#if 0
	if(gSystem_Info.gathertemper[1]>=gSystem_Info.tempersetup){
		Disp_Sig(TOOHOT,1);
	if(gbeepflag1==0){
		Beep_Onoff(1);
	}
		Switch_Alm(1);	
		System_Led_Onoff(RED_LED,1);
	if(gSystem_Info.ployenable==0){
		lockdowmenable=1;
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
		gSystem_Info.ployenable=1;
	}
	temperenable=1;
	}
	if((gSystem_Info.gathertemper[1]<gSystem_Info.tempersetup)&&(temperenable==1))
	{
		Disp_Sig(TOOHOT,0);
	    Beep_Onoff(0);			
		Switch_Alm(0);	
	    System_Led_Onoff(RED_LED,0);		
	    gSystem_Info.ployenable=0;
	    gbeepflag1=0;
	    temperenable=0;
	}
	//////////////////////////////////////////////////////////////////
	if(gSystem_Info.leakcurent>=gSystem_Info.leakcursetup){
     	Disp_Sig(LEAKCURRENT,1);		
	   if(gbeepflag2==0){
			Beep_Onoff(1);
	   	}
		Switch_Alm(1);	
		System_Led_Onoff(RED_LED,1);
		if(gSystem_Info.ployenable==0){
		 lockdowmenable=1;
         //HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
         gSystem_Info.ployenable=1;
		 leakenable=1;
		}
	
	}
    if((gSystem_Info.leakcurent<gSystem_Info.leakcursetup)&&(leakenable==1))
	{
	    
	    Disp_Sig(LEAKCURRENT,0);
		Beep_Onoff(0);			
		Switch_Alm(0);	
		System_Led_Onoff(RED_LED,0);		
		gSystem_Info. ployenable=0;
		gbeepflag2=0;
		leakenable=0;
	}
#else
 if(gsystime5s==1){

    gsystime5s=0;

		if((gSystem_Info.gathertemper[1]>=gSystem_Info.tempersetup)&&(temperenable==0)){
				Disp_Sig(TOOHOT,1);
				if(gbeepflag1==0){
				Beep_Onoff(1);
				}
				Switch_Alm(1);	
				System_Led_Onoff(RED_LED,1);
				if(gSystem_Info.ployenable==0){
				lockdowmenable=1;
				gSystem_Info.ployenable=1;
				}
				temperenable=1;
		}

		//////////////////////////////////////////////////////////////////
		if((gSystem_Info.leakcurent>=gSystem_Info.leakcursetup)&&(leakenable==0)){
				Disp_Sig(LEAKCURRENT,1);		
				if(gbeepflag1==0){
				Beep_Onoff(1);
				}
				Switch_Alm(1);	
				System_Led_Onoff(RED_LED,1);
				if(gSystem_Info.ployenable==0){
				lockdowmenable=1;
				gSystem_Info.ployenable=1;
				}
				leakenable=1;
		}

		if((gSystem_Info.gathertemper[1]<gSystem_Info.tempersetup)&&(gSystem_Info.leakcurent<gSystem_Info.leakcursetup))
		{
			if(temperenable==1){
				Disp_Sig(LEAKCURRENT,0);	
				temperenable=0;	
				gbeepflag1=0;
				leakenable=0;
			}
			if(leakenable==1){			
				Disp_Sig(TOOHOT,0);
				leakenable=0;	
				gbeepflag2=0;
				leakenable=0;
			}
			Beep_Onoff(0);			
			Switch_Alm(0);	
			System_Led_Onoff(RED_LED,0);		
			gSystem_Info.ployenable=0;
		}

 }		
#endif
if(lockdowmenable==1){
	lockdowmenable=0;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_RESET);
	for(int i=0;i<3;i++){
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_RESET);
	    HAL_Delay(100);
    } 
}

}


void System_Led_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_SET);
	System_Led_Onoff(YELLOW_LED,0);	
	System_Led_Onoff(GREEN_LED,1);	
	System_Led_Onoff(RED_LED,0);
}

void System_Led_Onoff(char ledtype,char onoff)
{
  if(ledtype==RED_LED){
     if(onoff==0){
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10, GPIO_PIN_SET);
     }else{
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10, GPIO_PIN_RESET);
     }
  }
  if(ledtype==YELLOW_LED){
     if(onoff==0){
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_SET);
     }else{
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_RESET);
     }
  }
  if(ledtype==GREEN_LED){
     if(onoff==0){
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, GPIO_PIN_SET);
     }else{
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, GPIO_PIN_RESET);
     }
  }
}


/********************************************************/
void Beep_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_RESET);
}
void Beep_Onoff(char onoff)
{
	if(onoff==0){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_SET);
	}
}

/**********************************************************/

void Switch_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin   =GPIO_PIN_1|GPIO_PIN_0;
	GPIO_InitStruct.Mode  =GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  =GPIO_PULLUP;
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, GPIO_PIN_RESET);
}

void Switch_Ploy(char i)
{    	
    #if 0
    if(i==1){
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);	
   	}else{	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_RESET);
   	}
	#endif
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
	//HAL_Delay(50);
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_RESET);
	//HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
	
}

void Switch_Alm(char onoff)
{
	if(onoff){
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, GPIO_PIN_RESET);	
	}
}




/*************************************************************
*Desigen:Huajinwen in 2019-5-5
*Function:The user can modify it by your need
*Parame:None
*Return:None
*************************************************************/
    __IO char password[4]={0U,0U,0U,0U};
    unsigned int  msemode=0;
	unsigned int  passwordindex=0;
	__IO unsigned char  enternfactormode=0U;	
	__IO unsigned char  enternfactormode2=0U;
	void Key_HandleByUser(void)
	{
		if(Is_KeyDown(KeyFactorLeftValue)){
			if((gSystem_Info.tempermode)&&(enternfactormode==2)){			  
				if(gSystem_Info.tempersetup++>120){
					gSystem_Info.tempersetup=120;
				}
		    }
		    if((gSystem_Info.leakcurmode)&&(enternfactormode==2)){				  
			    if(gSystem_Info.leakcursetup++>15000){
				   gSystem_Info.leakcursetup=15000;
				}
			}
			if(enternfactormode==1){
				if(++password[passwordindex]>9){
	                 password[passwordindex]=9;
				}
			}
			Beep_Onoff(0);			
			gbeepflag1=1;			
			gbeepflag2=1;
		}		
		if(Is_KeyDown(KeyFactorRightValue)){			  
			if((gSystem_Info.tempermode)&&(enternfactormode==2)){			  
				if((gSystem_Info.tempersetup--)<=(-20)){
					gSystem_Info.tempersetup=-20;
				}
		    }
			if((gSystem_Info.leakcurmode)&&(enternfactormode==2)){				  
				if(gSystem_Info.leakcursetup--<=0){
						gSystem_Info.leakcursetup=0;
				}
			}
			if(enternfactormode==1){
				if(password[passwordindex]--<=0){
					password[passwordindex]=0;
				}
			}		
			Beep_Onoff(0);			
			gbeepflag1=1;			
			gbeepflag2=1;
		}
		if(Is_KeyLongDown(KeyFactorLeftValue)){ 	
		}
		if(Is_KeyLongDown(KeyFactorRightValue)){
			if(enternfactormode==0U){
				enternfactormode=1U;
				password[0]=1U;
				password[1]=2U;
				password[2]=3U;
				password[3]=4U;
				msemode=0;
			}
			if(enternfactormode==1){
				if(++msemode>5){
					 msemode=0;
				}
			}
			if((gSystem_Info.leakcurmode==1)&&(enternfactormode==2)){
					enternfactormode=0;
					msemode=0;
					gEnternmode[0] =0;				
					gEnternmode[1] =0;
					gEnternmode[2] =0;
					gEnternmode[3] =0;
					gSystem_Info.leakcurmode =0;
					gSystem_Info.temperenable=0;
					gSystem_Info.tempermode=0;
					gSystem_Info.leakcurenable=0;
			}
			if((gSystem_Info.tempermode==1)&&(enternfactormode==2)){
					gEnternmode[0] =0;				
					gEnternmode[1] =0;
					gEnternmode[2] =0;
					gEnternmode[3] =0;
					gSystem_Info.leakcurmode =1;
					gSystem_Info.tempermode=0;
					gSystem_Info.temperenable=0;
			}
		}
		Key_ClearEvent();
	}

__IO unsigned char gSystemmsemodenable=0;	
void System_EnterMode(void)
{  
	if(enternfactormode==1){
		if(msemode==0){
				passwordindex=0;
				gEnternmode[0] =0;
				gEnternmode[1] =0;
				gEnternmode[2] =0;
				gEnternmode[3] =0;		
				gSystemmsemodenable=0;
		}   
		else if(msemode==1){
				gEnternmode[0] =1;				
				gEnternmode[1] =0;
				gEnternmode[2] =0;
				gEnternmode[3] =0;
				passwordindex=0;
		}
		else if(msemode==2){
				gEnternmode[1] =1;
				gEnternmode[0] =0;
				gEnternmode[2] =0;
				gEnternmode[3] =0;
				passwordindex=1;
		}
		else if(msemode==3){
				gEnternmode[2] =1;
				gEnternmode[0] =0;
				gEnternmode[1] =0;
				gEnternmode[3] =0;
				passwordindex=2;
		}
		else if(msemode==4){
				gEnternmode[3] =1;
				gEnternmode[0] =0;
				gEnternmode[1] =0;
				gEnternmode[2] =0;
				passwordindex=3;	
		}
	else if(msemode==5){
			if(Mode_adjust()==1){
				enternfactormode=2;
				gEnternmode[0] =0;
				gEnternmode[1] =0;
				gEnternmode[2] =0;		
				gEnternmode[3] =0;
				gEnternmodeenable[0]=0;
				gEnternmodeenable[1]=0;
				gEnternmodeenable[2]=0;
				gEnternmodeenable[3]=0;
				gSystem_Info.tempermode=1;
				gSystem_Info.leakcurmode=0;
			}else{
				enternfactormode=0;
				gEnternmode[0] =0;				
				gEnternmode[1] =0;
				gEnternmode[2] =0;
				gEnternmode[3] =0;
				gSystem_Info.leakcurmode =0;
				gSystem_Info.temperenable=0;
				gSystem_Info.tempermode=0;
				gSystem_Info.leakcurenable=0;
				}
			}
		}
}


char Mode_adjust(void)
{
	for(int i=0;i<4;i++){
		if(password[i]!=0x06){
			return 0;
		}
	}
	return 1;    
}
