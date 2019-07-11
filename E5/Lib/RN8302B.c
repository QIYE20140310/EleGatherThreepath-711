
#include "RN8302B.h"
#include "main.h"




typedef unsigned char  		u8;
typedef signed char  		s8;
typedef unsigned short  	u16;
typedef signed short  		s16;
typedef unsigned long int  	u32;
typedef signed long int  	s32;
typedef unsigned short      WORD;
typedef	unsigned char	    BOOL;
#define DL645_MAX_FL   5U
#define MAX_COMPACK_SIZE 5U
#define NOP()            __NOP()
#if 1
#define PinWrite_ADSCLK(x)  	( (x) ? (HAL_GPIO_WritePin(GPIOA, SPI1_CLK_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(GPIOA, SPI1_CLK_PIN, GPIO_PIN_RESET)) )	//
#define PinRead_ADSDI()			  (HAL_GPIO_ReadPin(GPIOA,SPI1_MISO_PIN))
#define PinWrite_ADSDO(x)  		( (x) ? (HAL_GPIO_WritePin(GPIOA, SPI1_MOSI_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(GPIOA, SPI1_MOSI_PIN, GPIO_PIN_RESET)) )	//	
#define PinWrite_ADCS(x)  	( (x) ? (HAL_GPIO_WritePin(GPIOA, SPI1_CS_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(GPIOA, SPI1_CS_PIN, GPIO_PIN_RESET)) )
#else
#define PinWrite_ADSCLK(x)  	( (x) ? (HAL_GPIO_WritePin(GPIOB, SPI1_CLK_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(GPIOB, SPI1_CLK_PIN, GPIO_PIN_RESET)) )	//
#define PinRead_ADSDI()			  (HAL_GPIO_ReadPin(GPIOB,SPI1_MISO_PIN))
#define PinWrite_ADSDO(x)  		( (x) ? (HAL_GPIO_WritePin(GPIOB, SPI1_MOSI_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(GPIOB, SPI1_MOSI_PIN, GPIO_PIN_RESET)) )	//	
#define PinWrite_ADCS(x)  	( (x) ? (HAL_GPIO_WritePin(GPIOB, SPI1_CS_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(GPIOB, SPI1_CS_PIN, GPIO_PIN_RESET)) )

#endif

void Delay1us(void)
{
 for(int i=0;i<3;i++)
	__NOP();

}
void Delay_ms(unsigned int ms)
{
  for(unsigned int i=0;i<1000*ms;i++){
	  Delay1us();
  }
}
void Delay5ms(int ms)
{
  for(unsigned int i=0;i<1000*5*ms;i++){
	  Delay1us();
  }
}



typedef struct {
//	u32 Dat : 23;
//	u32 S : 1;
	u8 Dat0;
	u8 Dat1;
	u8 Dat2 : 7;
	u8 S : 1;			
} sDF09;
typedef struct {
	u32 Dat;
} sDF11;
typedef	sDF11 sFsEgSh_TypeDef;	

 u16	SysStamp;

u16 fnStamp_Through(u16 Ago)
{	
	if (SysStamp >= Ago)
		{return (SysStamp - Ago);}
	else
		{return ( (0xffff - Ago) + SysStamp);}
}

u16 fnHexToBcd_u16(u16 Dat)
{
	u16 Result = 0;

	Dat = Dat % 10000;
	Result += (Dat / 1000) * 0x1000;
	Dat = Dat % 1000;	
	Result += (Dat / 100) * 0x100;
	Dat = Dat % 100;	
	Result += (Dat / 10) * 0x10;
	Dat = Dat % 10;	
	Result += Dat;
		
	return(Result);	
}
u32	fnHexToBcd_u32(u32 Dat)
{	
	u32 result = 0;
	
	Dat = Dat % 100000000;
	result += (Dat / 10000000) * 0x10000000;
	Dat = Dat % 10000000;	
	result += (Dat / 1000000) * 0x1000000;
	Dat = Dat % 1000000;
	result += (Dat / 100000) * 0x100000;
	Dat = Dat % 100000;
	result += (Dat / 10000) * 0x10000;
	Dat = Dat % 10000;	
	result += (Dat / 1000) * 0x1000;
	Dat = Dat % 1000;
	result += (Dat / 100) * 0x100;
	Dat = Dat % 100;
	result += (Dat / 10) * 0x10;
	Dat = Dat % 10;	
	result += Dat;
	
	return(result);
}

u16 fnDFConver_Bcd16To16(s16 Dat)
{
	u16 Result;
	Result = abs(Dat) % 8000;			
	Result = fnHexToBcd_u16(Result);
        if(Dat < 0 ) Result |= 0x8000;
	else Result &= 0x7fff;	
	return(Result);
}
u32 fnDFConver_Bcd32To32(s32 Dat)
{
	u32 Result;
	Result = labs(Dat) % 80000000;			
	Result = fnHexToBcd_u32(Result);
        if(Dat < 0 ) Result |= 0x80000000;
	else Result &= 0x7fffffff;	
	return(Result);
}

sDF09 fnDFConver_Hex32ToDF09(s32 Dat)
{	
	sDF09	Result;	
	memset(&Result , 0 , sizeof(sDF09) );
	if(Dat < 0) Result.S = 1;
	else Result.S = 0;	        
	Dat = labs(Dat) % 800000;
	Dat = fnHexToBcd_u32(Dat);
	Result.Dat0 = Dat;
	Result.Dat1 = Dat >> 8;
	Result.Dat2 = Dat >> 16;
	return(Result);	
}

typedef union                                         //公共数据运算区8字节
{
	u8  ucTempBuf[8];
	u32 lTemp32;
	u16 wTemp16;
	u8  ucTemp8;
}sDl645StruDataComm_TypeDef;

  typedef struct
  {
	  u8	  ChkErrCnt;	   //读错误计数1
	  
	  s32	  Pw[12];		  //{Pa Pb Pc P Qa Qb Qc Q Sa Sb Sc S}	 48 
	  s32	  UI[7];		  //Ua Ub Uc Ia Ib Ic Inal	 28 
	  s32	  VectorU[9];	  // 正序、负序、零序电压
	  s32	  VectorI[9];	  // 正序、负序、零序电流
	  s32	  Pf[4];		  //Pf Pfa Pfb Pfc		16	   
	  u32	  Frequency;	  //电网频率，单位：		4  
	  s32	  YUI[3],YUU[2];  //20
								  
	  s32	  Pulse[15];	  //前台高频脉冲48
	  //---电能脉冲---	  
	  s32	  Pulse_EgTmp[20];	  //高频脉冲{P,Q,Ps},{Pa,Qa,Psa},{Pb,Qb,Psb},{Pc,Qc,Psc}{Fp,Fq}{Fpa,Fqa}{Fpb,Fqb}{Fpc,Fqc}
	  u32	  Pulse_Eg[20];  //低频脉冲数
	  //---需量脉冲---
	  s32	  Pulse_NeedTmp[12];  
	  u16	  Pulse_Need[12]; //{PNeed,QNeed,PsNeed},{PNeeda,QNeeda,PsNeeda},{PNeedb,QNeedb,PsNeedb},{PNeedc,QNeedc,PsNeedc}48
	  
	  u16	  Angle[9];
	  u16	  PDirect;	 //4
	  u32	  ChkSum1;	 //4
	  u32	  ChkSum2;	 //4
	  
	  u16	  Temperature;	  //温度4
	  u32	  ClockBat; 	  //时钟电池4
	  u32	  BackupBat;	  //后备电池4
	  
	  u16	CF1DelayStamp;
	  u16	CF2DelayStamp;
	  
	  u16	CfIn_P;
	  u16	CfIn_q;
	  
	  u16	CfTime_P;
	  u16	CfTime_q;
	  
  } sDl645FrontTmp_TypeDef;
  
  typedef struct
  {   
	  struct sFrontPubData_TypeDef	{
		  u16	  U[3]; 		  //---电压---NNN.N6
		  u16	  VectorU[9];	  // 正序电压 0--2	负序电压 3---5	零序电压 6---8
		  u32	  I[4]; 		  //---电流NNNN.NNNN(电流值要求3整3小，整定值要求2整4小，最高位表示方向)---16		  
		  u32	  VectorI[9];	  // 正序电流 0--2	负序电流 3---5	零序电流 6---8
		  sDF09   Pw[12];		  //---瞬时有功/无功/视在功率NN.NNNN---{P Pa Pb Pc Q Qa Qb Qc S Sa Sb Sc}36
		  u16	  Pf[4];		  //---功率因数N.NNN---   最高位表示方向{Pf Pfa Pfb Pfc}   8  sDF05
		  u16	  Angle[9]; 	  //---相角NNN.N---	  18
								  //PhUb,PhUc, 以A相电压为基准，B、C相角度				  //(hyg) BCD码
								  //PhIa,PhIb,PhIc,A相电流与A相电压间角度、B相电流与B相电压间角度、C相电流与C相电压间角度
								  //Angle A,Angle B,Angle C, A相电流与A相电压间角度、B相电流与A相电压间角度、C相电流与A相电压间角度
								  //Angle C-Angle A 
		  u32	  UnblU;		  //电压不平衡度NNNN.NN%4
		  u32	  UnblI;		  //电流不平衡度NNNN.NN%4
		  
		  u16	  FuzzyU[3];	  //---电压波形失真度NN.NN%--- 6   
		  u16	  FuzzyI[3];	  //---电流波形失真度NN.NN%--- 6   
		  u16	  WaveU[3][21];   //---相电压谐波含量NN.NN%--- 126 
		  u16	  WaveI[3][21];   //---相电流谐波含量NN.NN%---	 126
		  //---其他---
		  u16	  Frequency;	  //NN.NN 2
		  u32	  PPwave;		  //NN.NNNN 4
		  
		  u16	  Temperature;	  //NNN.N 2
		  u16	  ClockBat; 	  //NN.NN 2
		  u16	  BackupBat;	  //NN.NN 2
		  u8	  PDirect;		  //原功率方向，用于需量处理(0总/1A/2B/3C , 注意与FrontTmp不同) 1
	  } PubData;
	  
	  struct sFrontPriData_TypeDef	{	  
		  u8	  Flag; 		  //---工作异常标志---1
		  u8	  PhaseCalStep;
		  u16	  FrontStamp;
  //	  u8	  BatCalStep;
		  u16	  BatStamp;
  //	  u16	  BatDetStamp;
	  } PriData;  
	  
	  struct sFrontPriPara_TypeDef	{	  
		  u32  PConstE; 		  //有功常数
		  u32  QConstE; 		  //无功常数(Hex码)4
		  u16  Crc;//2
	  } PriPara;	  
  } sDl645Front_TypeDef;  //缓冲区转换后的有效数据
  
  typedef struct 
  { 	  
	  u8 FractionI;/*电流小数位数*/
	  u8 FractionU;/*电压小数位数*/
	  u8 FractionE;/*电能小数位数*/
	  u8 FractionP;/*功率需量小数位数*/
	  u16 ConstE;/*电表有功常数*/
	  u16 Pulse_deltaE;/*数*/
	  u8  HighPulseRate;
	  u8 PhaseCalStep;
	  u32 IRmsConst;		  //10
	  u16 Reserve[2];
	  u16 NOLOAD;
	  u16 ZEROSTAR; 				  //20
	  u32 ChkSum1;
	  u16 HFConst1;
	  u16 HFConst2;
	  u32 VRmsConst;		  //30
	  u16 Reserve1;
	  u32 PRmsConst;
	  u16 Reserve2;
	  u16 VGain[3]; 			  // 42
	  u16 IGain[4]; 			  //48
	  u8  PHSU[2];				  // 56
	  u32 PHSI[3];
	  u16 PRth[4];				  //70
	  u16 UI_Offset[7];
	  u16 P_PHS[3];/*有功相位校正寄存器*/				  // 92
	  u16 PGain[3];/*有功增益校正寄存器*/				  
  }sDl645FirmParaFile_TypeDef;
  
  typedef struct {
#pragma pack(1)
	  struct sDl645EgPubData_TypeDef
	  {
		  sFsEgSh_TypeDef AllPEgSh[DL645_MAX_FL + 1];	  
		  sFsEgSh_TypeDef PtPEgSh[DL645_MAX_FL + 1];	  
		  sFsEgSh_TypeDef NtPEgSh[DL645_MAX_FL + 1];	  
		  
		  sFsEgSh_TypeDef AllQEgSh[DL645_MAX_FL + 1];	  
		  sFsEgSh_TypeDef PtQEgSh[DL645_MAX_FL + 1];	  
		  sFsEgSh_TypeDef NtQEgSh[DL645_MAX_FL + 1];	  
		  
		  sFsEgSh_TypeDef Qd1QEgSh[DL645_MAX_FL + 1];	  
		  sFsEgSh_TypeDef Qd2QEgSh[DL645_MAX_FL + 1];	  
		  sFsEgSh_TypeDef Qd3QEgSh[DL645_MAX_FL + 1];	  
		  sFsEgSh_TypeDef Qd4QEgSh[DL645_MAX_FL + 1];	  
		  
		  sFsEgSh_TypeDef PtPsEgSh[DL645_MAX_FL + 1];	  
		  sFsEgSh_TypeDef NtPsEgSh[DL645_MAX_FL + 1];	  
		  
		  sFsEgSh_TypeDef AssEgSh;						  
		  sFsEgSh_TypeDef BasePtPEgSh;					  
		  sFsEgSh_TypeDef BaseNtPEgSh;					  
		  sFsEgSh_TypeDef WavePtPEgSh;					  
		  sFsEgSh_TypeDef WaveNtPEgSh;					  
		  sFsEgSh_TypeDef CopPEgSh; 					  
		  sFsEgSh_TypeDef IronPEgSh;						  
  //------------- 
		  sFsEgSh_TypeDef PtPEgSha; 					  
		  sFsEgSh_TypeDef NtPEgSha; 					  
		  
		  sFsEgSh_TypeDef PtQEgSha; 					  
		  sFsEgSh_TypeDef NtQEgSha; 					  
		  
		  sFsEgSh_TypeDef Qd1QEgSha;					  
		  sFsEgSh_TypeDef Qd2QEgSha;					  
		  sFsEgSh_TypeDef Qd3QEgSha;					  
		  sFsEgSh_TypeDef Qd4QEgSha;					  
		  
		  sFsEgSh_TypeDef PtPsEgSha;					  
		  sFsEgSh_TypeDef NtPsEgSha;					  
		  
		  sFsEgSh_TypeDef AssEgSha; 					  
		  sFsEgSh_TypeDef BasePtPEgSha; 				  
		  sFsEgSh_TypeDef BaseNtPEgSha; 				  
		  sFsEgSh_TypeDef WavePtPEgSha; 				  
		  sFsEgSh_TypeDef WaveNtPEgSha; 					  
		  sFsEgSh_TypeDef CopPEgSha;					  
		  sFsEgSh_TypeDef IronPEgSha;						  
  //------------- 
		  sFsEgSh_TypeDef PtPEgShb; 					  
		  sFsEgSh_TypeDef NtPEgShb; 					  
		  
		  sFsEgSh_TypeDef PtQEgShb; 					  
		  sFsEgSh_TypeDef NtQEgShb; 					  
		  
		  sFsEgSh_TypeDef Qd1QEgShb;					  
		  sFsEgSh_TypeDef Qd2QEgShb;					  
		  sFsEgSh_TypeDef Qd3QEgShb;					  
		  sFsEgSh_TypeDef Qd4QEgShb;					  
		  
		  sFsEgSh_TypeDef PtPsEgShb;					  
		  sFsEgSh_TypeDef NtPsEgShb;					  
		  
		  sFsEgSh_TypeDef AssEgShb; 					  
		  sFsEgSh_TypeDef BasePtPEgShb; 				  
		  sFsEgSh_TypeDef BaseNtPEgShb; 				  
		  sFsEgSh_TypeDef WavePtPEgShb; 				  
		  sFsEgSh_TypeDef WaveNtPEgShb; 					  
		  sFsEgSh_TypeDef CopPEgShb;					  
		  sFsEgSh_TypeDef IronPEgShb;						  
  //------------- 
		  sFsEgSh_TypeDef PtPEgShc; 					  
		  sFsEgSh_TypeDef NtPEgShc; 					  
		  
		  sFsEgSh_TypeDef PtQEgShc; 					  
		  sFsEgSh_TypeDef NtQEgShc; 					  
		  
		  sFsEgSh_TypeDef Qd1QEgShc;					  
		  sFsEgSh_TypeDef Qd2QEgShc;					  
		  sFsEgSh_TypeDef Qd3QEgShc;					  
		  sFsEgSh_TypeDef Qd4QEgShc;					  
		  
		  sFsEgSh_TypeDef PtPsEgShc;					  
		  sFsEgSh_TypeDef NtPsEgShc;					  
		  
		  sFsEgSh_TypeDef AssEgShc; 					  
		  sFsEgSh_TypeDef BasePtPEgShc; 				  
		  sFsEgSh_TypeDef BaseNtPEgShc; 				  
		  sFsEgSh_TypeDef WavePtPEgShc; 				  
		  sFsEgSh_TypeDef WaveNtPEgShc; 					  
		  sFsEgSh_TypeDef CopPEgShc;					  
		  sFsEgSh_TypeDef IronPEgShc; 
	  } PubData;					  
#pragma pack()
  
	  struct sDl645EgPubDataCrc_TypeDef
	  {
		  u16 AllPEgSh; 	  
		  u16 PtPEgSh;		
		  u16 NtPEgSh;		
		  
		  u16 AllQEgSh; 	
		  u16 PtQEgSh;		
		  u16 NtQEgSh;		
		  
		  u16 Qd1QEgSh; 	  
		  u16 Qd2QEgSh; 	  
		  u16 Qd3QEgSh; 	  
		  u16 Qd4QEgSh; 	  
		  
		  u16 PtPsEgSh; 	  
		  u16 NtPsEgSh; 	  
		  
		  u16 AssEgSh;						  
		  u16 BasePtPEgSh;					  
		  u16 BaseNtPEgSh;					  
		  u16 WavePtPEgSh;					  
		  u16 WaveNtPEgSh;					  
		  u16 CopPEgSh; 					  
		  u16 IronPEgSh;						  
  //------------- 
		  u16 PtPEgSha; 					  
		  u16 NtPEgSha; 					  
		  
		  u16 PtQEgSha; 					  
		  u16 NtQEgSha; 					  
		  
		  u16 Qd1QEgSha;					  
		  u16 Qd2QEgSha;					  
		  u16 Qd3QEgSha;					  
		  u16 Qd4QEgSha;					  
		  
		  u16 PtPsEgSha;					  
		  u16 NtPsEgSha;					  
		  
		  u16 AssEgSha; 					  
		  u16 BasePtPEgSha; 				  
		  u16 BaseNtPEgSha; 				  
		  u16 WavePtPEgSha; 				  
		  u16 WaveNtPEgSha; 					  
		  u16 CopPEgSha;					  
		  u16 IronPEgSha;						  
  //------------- 
		  u16 PtPEgShb; 					  
		  u16 NtPEgShb; 					  
		  
		  u16 PtQEgShb; 					  
		  u16 NtQEgShb; 					  
		  
		  u16 Qd1QEgShb;					  
		  u16 Qd2QEgShb;					  
		  u16 Qd3QEgShb;					  
		  u16 Qd4QEgShb;					  
		  
		  u16 PtPsEgShb;					  
		  u16 NtPsEgShb;					  
		  
		  u16 AssEgShb; 					  
		  u16 BasePtPEgShb; 				  
		  u16 BaseNtPEgShb; 				  
		  u16 WavePtPEgShb; 				  
		  u16 WaveNtPEgShb; 					  
		  u16 CopPEgShb;					  
		  u16 IronPEgShb;						  
  //------------- 
		  u16 PtPEgShc; 					  
		  u16 NtPEgShc; 					  
		  
		  u16 PtQEgShc; 					  
		  u16 NtQEgShc; 					  
		  
		  u16 Qd1QEgShc;					  
		  u16 Qd2QEgShc;					  
		  u16 Qd3QEgShc;					  
		  u16 Qd4QEgShc;					  
		  
		  u16 PtPsEgShc;					  
		  u16 NtPsEgShc;					  
		  
		  u16 AssEgShc; 					  
		  u16 BasePtPEgShc; 				  
		  u16 BaseNtPEgShc; 				  
		  u16 WavePtPEgShc; 				  
		  u16 WaveNtPEgShc; 					  
		  u16 CopPEgShc;					  
		  u16 IronPEgShc; 
	  } PubDataCrc; 	  
	  
	  struct sDl645EgPriData_TypeDef 
	  { 	   
		  u32 PieceEg[20];
		  u16 Crc;
	  } PriData;
	  
	  struct sDl645EgPubPara_TypeDef  
	  {
		  u8	  PExpression;		  
		  u8	  PtQExpression;	  
		  u8	  NtQExpression;	 
		  u8	  InvalidByte1;
		  u16	  PPlsDeltaE;		  
		  u16	  QPlsDeltaE;		  
		  u16	  PsPlsDeltaE;		  
		  
		  u8	  FractionE;		  
		  u8	  InvalidByte2;
		  u16	  Crc;
				  
		  u8	  fMinute;
		  u8	  InvalidByte3;   
	  } PubPara;
  }sDl645Eg_TypeDef;  //863BYTE   
  






	sDl645StruDataComm_TypeDef      Dl645RN8302DataComm;
	sDl645FrontTmp_TypeDef		  Dl645FrontTmp;
	sDl645Front_TypeDef			  Dl645Front;
	sDl645Eg_TypeDef				  Dl645Eg;
	sDl645FirmParaFile_TypeDef		  Dl645FirmPara;


	void fnDelay2us(void)
	{
	 for(int i=0;i<2;i++)
		__NOP();
	
	}
	void delay_ms(unsigned int ms)
	{
	  for(unsigned int i=0;i<500;i++){
		  fnDelay2us();
	  }
	}


void RN830B_SoftInit(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();		
	__HAL_RCC_GPIOC_CLK_ENABLE();	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	#if 1
	GPIO_InitStruct.Pin = SPI1_MOSI_PIN|SPI1_CLK_PIN;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = SPI1_MISO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull =  GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(SPI1_MOSI_PORT,SPI1_MOSI_PIN   ,GPIO_PIN_SET);        
	HAL_GPIO_WritePin(SPI1_MOSI_PORT,SPI1_CLK_PIN    ,GPIO_PIN_SET);	
 
    /////////////////////////////////////////////////////////////
	GPIO_InitStruct.Pin = SPI1_CS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull =  GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = SPI_INT;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull =  GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPI1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SPI11_RESET_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull =  GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull =  GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);

	
	HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 1);
   //AL_NVIC_EnableIRQ(EXTI4_IRQn);
      
	HAL_GPIO_WritePin(SPI1_MOSI_PORT,SPI1_CS_PIN     ,GPIO_PIN_SET);
#else

#endif

}
void RN8302B_Inout(unsigned int i,unsigned int out)
{

}

unsigned char _rxdata[5];



static ErrorStatus fnRN8302_Write(u16 wReg,u8 *pBuf,u8 DatLen);
static ErrorStatus fnRN8302_Read(u16 wReg,u8 *pBuf,u8 DatLen);
static void fnSpi2_WriteByte(u8 Dat);
static u8 fnSpi2_ReadByte(void);

unsigned int BufToU32(unsigned char *a)
{

	return (unsigned int)( ((a[3]&0xFF)<<24)| ((a[2]&0xFF)<<16)| ((a[1]&0xFF)<<8)| (a[0]&0xFF) );

}

ErrorStatus fnRN8302_Write(u16 wReg,u8 *pBuf,u8 DatLen)
{
    #if 1
	u8 i,temp,chksum,Repeat;
	ErrorStatus	err = SUCCESS;
	if( (DatLen == 0) || (DatLen > 4) ) return(ERROR);
	for( Repeat =3; Repeat != 0 ; Repeat--)		
	{
		PinWrite_ADCS(0);
		NOP();
		temp = (u8)(wReg & 0x00ff);
		chksum = temp;
		fnSpi2_WriteByte( temp); //first write hight Addr
		temp = (((u8)(wReg >> 4)) & 0xf0) + 0x80;
		chksum += temp;
		fnSpi2_WriteByte( temp);
		
		for(i = DatLen; i > 0;i-- )
		{
			fnSpi2_WriteByte(pBuf[i-1]);  // first write high data
			chksum +=pBuf[i-1];
		}
		chksum = ~chksum;
		fnSpi2_WriteByte(chksum);
		#if 1
		PinWrite_ADCS(1);
		NOP();
//¶ÁWData¼Ä´æÆ÷¼ì²é-----------------------
//		PinWrite_ADCS(0);	
//		NOP();
		#else
				PinWrite_ADCS(1);
				NOP();
		//¶ÁWData¼Ä´æÆ÷¼ì²é-----------------------
				PinWrite_ADCS(0);	
				NOP();

		#endif
		fnSpi2_WriteByte( 0x8D); 
		chksum = 0x8D;
		temp =  ( (u8)(0x018D >> 4) & 0xf0);			
		fnSpi2_WriteByte( temp );
		chksum += temp;
		for(i = 3 ; i > 0 ; i--)
		{
			temp = fnSpi2_ReadByte(); 
			if(DatLen >= i)
			{					
				if(temp != pBuf[i - 1]) 
				{
					err = ERROR;
					break;	
				}	
			}
			chksum += temp;
		}
		if(err == SUCCESS) 
        {
	        chksum = ~chksum;
	        temp = fnSpi2_ReadByte();   
			if(temp != chksum)  err = ERROR; 
       	}
		
		PinWrite_ADCS(1);
		if(err == SUCCESS) break;
		fnDelay2us();
		fnDelay2us();
	}
	PinWrite_ADSDO(1);
	PinWrite_ADSCLK(1);
	return(err);	
	#else

	#endif
}

//////////////////////////////////////////////////////////////////////////

unsigned char test[4];

ErrorStatus fnRN8302_Read(u16 wReg,u8 *pBuf,u8 DatLen)
{
    #if 1
	u8 i,temp,chksum,Repeat;
	ErrorStatus	err = SUCCESS;
	if(DatLen == 0) return(ERROR);
	for( Repeat=3; Repeat != 0 ; Repeat--)		
	{
	
	    RN8302B_Inout(1,1);
		PinWrite_ADCS(0);
		fnDelay2us();
		temp = (u8)(wReg & 0x00ff);
		chksum = temp;
		fnSpi2_WriteByte( temp); //first write hight Addr
		temp = ((u8)(wReg >> 4))& 0xf0;
		chksum += temp;
		fnSpi2_WriteByte( temp);
		
		for(i = DatLen; i > 0;i--)
		{
			pBuf[i-1] = fnSpi2_ReadByte(); 
			chksum += pBuf[i-1];
			test[i-1]=pBuf[i-1];
		}
		chksum = ~chksum;
			test[0]=chksum;
		
		if(fnSpi2_ReadByte()!=chksum)
			err = ERROR;
		#if 0
//		PinWrite_ADCS(1);
//---¶ÁRData¼Ä´æÆ÷¼ì²é------------------------------------
        #else
		PinWrite_ADCS(1);
		#endif
		if(err != SUCCESS) continue;
		
		fnSpi2_WriteByte( 0x8c); 
		chksum = 0x8c;
		temp =  ( (u8)(0x018C >> 4) & 0xf0);			
		fnSpi2_WriteByte( temp );
		chksum += temp;
		for(i = 4 ; i > 0 ; i--)
		{
			temp = fnSpi2_ReadByte(); 
			if(DatLen >= i)
			{					
				if(temp != pBuf[i - 1]) 
				{
					err = ERROR;
					break;	
				}	
			}
			chksum += temp;
		}
		if(err == SUCCESS) 
        {
			chksum = ~chksum;
			temp = fnSpi2_ReadByte(); 

			if(temp != chksum) 
			err = ERROR; 
       	}
		
		RN8302B_Inout(1,1);
		PinWrite_ADCS(1);
		if(err == SUCCESS) break;	
		fnDelay2us();
		fnDelay2us();
	}
	PinWrite_ADCS(1);
	PinWrite_ADSDO(1);
	PinWrite_ADSCLK(1);
	return(err);
	#else

	#endif
}


void fnSpi2_WriteByte(u8 Dat)
{
	u8 i;
	#if 1
	RN8302B_Inout(1,1);
	for(i=0;i<8;i++)
	{
		PinWrite_ADSCLK(1);
		PinWrite_ADSCLK(1);
		fnDelay2us();
		fnDelay2us();
		if(Dat & 0x80)
		{
			PinWrite_ADSDO(1);
			PinWrite_ADSDO(1);
			fnDelay2us();
			fnDelay2us();
			fnDelay2us();
		}
		else
		{
			PinWrite_ADSDO(0);
			PinWrite_ADSDO(0);
			fnDelay2us();
			fnDelay2us();
			fnDelay2us();
		}
		PinWrite_ADSCLK(0);
		PinWrite_ADSCLK(0);
		fnDelay2us();
		fnDelay2us();
		fnDelay2us();
		Dat <<= 1;
	}
    #else
		}
	#endif
	
	PinWrite_ADSDO(1);
	PinWrite_ADSDO(1);
	fnDelay2us();
	fnDelay2us();
	fnDelay2us();
}



u8 fnSpi2_ReadByte(void)
{
  #if 1
    u8 i,B=0;
  
	RN8302B_Inout(1,1);
    for(i=0;i<8;i++)
    {
    	B<<=1;
		
	    RN8302B_Inout(1,1);
      PinWrite_ADSCLK(1);
			PinWrite_ADSCLK(1);
			fnDelay2us();
			fnDelay2us();
			fnDelay2us();
			fnDelay2us();
			PinWrite_ADSCLK(0);
			PinWrite_ADSCLK(0);
			fnDelay2us();
			fnDelay2us();
			fnDelay2us();
			fnDelay2us();
		//if (PinRead_ADSDI())
		
		RN8302B_Inout(0,1);
        if(HAL_GPIO_ReadPin(GPIOA,SPI1_MISO_PIN)==GPIO_PIN_SET)
		{
		 B=B+0x1;
		}

		fnDelay2us();
		fnDelay2us();
		fnDelay2us();
    }
    return	B;
 #else
 #endif
}
		



unsigned char testxt[4]={0,0,0,0};
unsigned char testxt2[4]={0,0,0,0};
unsigned char testxt3[4]={0,0,0,0};

char RN_ReadDeviceID(void)
{

}

unsigned char testxt123[4]={0,0,0,0};

void TestDevice(void)
{

 fnRN8302_Read(0x018F,testxt123,4);
}
#define	WMSW		0x0181




typedef struct
{
	
unsigned char Cst_GSUx[3];
unsigned char Cst_GSIx[4];
unsigned char  Cst_GPx[3]; 
unsigned char Cst_Px_PHSL[3];
unsigned char Cst_Qx_PHSL[3];
unsigned char Cst_Ix_OS[3];

}StDef_RN8302Para_RegInfo;

StDef_RN8302Para_RegInfo StDef_RN8302Para_Reg;

char fnRN8302_Init(void)
{
	uint8_t regbuf[6];

    if(RN_ReadDeviceID()){
    }

	        regbuf[0] = 0xE5;
			fnRN8302_Write(WREN,regbuf,1);
			//切换到EMM模式
			regbuf[0] = 0xA2;
			fnRN8302_Write(WMSW,regbuf,1);
			//复位
			regbuf[0] = 0xFA;
			fnRN8302_Write(SOFTRST,regbuf,1);
			Delay5ms(4);
			//写使能
			regbuf[0] = 0xE5;
			fnRN8302_Write(WREN,regbuf,1);
			//切换到EMM模式
			regbuf[0] = 0xA2;
			fnRN8302_Write(WMSW,regbuf,1);
			//写HFCONST1
			//regbuf[0] = 0x2E;
			//regbuf[1] = 0xA1;
			regbuf[0] = 0;//0x73;
			regbuf[1] = 0;//0x06;
			fnRN8302_Write(HFCONST1,regbuf,2);
			//写电压增益
			regbuf[0]=0;			
			regbuf[1]=0;
			//regbuf[0] = StDef_RN8302Para_Reg.Cst_GSUx[0]/256;
			//regbuf[1] = StDef_RN8302Para_Reg.Cst_GSUx[0]%256;
			fnRN8302_Write(GSUA,regbuf,2);
			//regbuf[0] = StDef_RN8302Para_Reg.Cst_GSUx[1]/256;
			//regbuf[1] = StDef_RN8302Para_Reg.Cst_GSUx[1]%256;
			fnRN8302_Write(GSUB,regbuf,2);
			//regbuf[1] = StDef_RN8302Para_Reg.Cst_GSUx[2]%256;
			//regbuf[0] = StDef_RN8302Para_Reg.Cst_GSUx[2]/256;
			fnRN8302_Write(GSUC,regbuf,2);
			//写电流增益
			regbuf[1] = StDef_RN8302Para_Reg.Cst_GSIx[0]/256;
			regbuf[0] = StDef_RN8302Para_Reg.Cst_GSIx[0]%256;
			//fnRN8302_Write(GSIA,regbuf,2);
			regbuf[1] = StDef_RN8302Para_Reg.Cst_GSIx[1]/256;
			regbuf[0] = StDef_RN8302Para_Reg.Cst_GSIx[1]%256;
			//fnRN8302_Write(GSIB,regbuf,2);
			regbuf[1] = StDef_RN8302Para_Reg.Cst_GSIx[2]/256;
			regbuf[0] = StDef_RN8302Para_Reg.Cst_GSIx[2]%256;
			//fnRN8302_Write(GSIC,regbuf,2);
			regbuf[1] = StDef_RN8302Para_Reg.Cst_GSIx[3]/256;
			regbuf[0] = StDef_RN8302Para_Reg.Cst_GSIx[3]%256;
			//fnRN8302_Write(GSIN,regbuf,2);
			//采样通道IA分段相位校正
	//		  RN8302_WriteReg(PHSIA,(uint8_t*)&StDef_RN8302Para_Reg.PHSI[0],2);
	//		  RN8302_WriteReg(PHSIB,(uint8_t*)&StDef_RN8302Para_Reg.PHSI[1],2);
	//		  RN8302_WriteReg(PHSIC,(uint8_t*)&StDef_RN8302Para_Reg.PHSI[2],2);
			//通道相位分段校正，电流阈值				 
	//		  RN8302_WriteReg(PRTH1L,(uint8_t*)&Dl645FirmPara.PRth[0],2);
	//		  RN8302_WriteReg(PRTH1H,(uint8_t*)&Dl645FirmPara.PRth[1],2);
	//		  RN8302_WriteReg(PRTH2L,(uint8_t*)&Dl645FirmPara.PRth[2],2);
	//		  RN8302_WriteReg(PRTH2H,(uint8_t*)&Dl645FirmPara.PRth[3],2);
			//x相有功功率增益
			regbuf[0] = StDef_RN8302Para_Reg.Cst_GPx[0]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_GPx[0]%256;
			fnRN8302_Write(GPA,regbuf,2);
			regbuf[0] = StDef_RN8302Para_Reg.Cst_GPx[1]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_GPx[1]%256;
			fnRN8302_Write(GPB,regbuf,2);
			regbuf[0] = StDef_RN8302Para_Reg.Cst_GPx[2]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_GPx[2]%256;
			fnRN8302_Write(GPC,regbuf,2);
			//x相有功分段相位校正
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Px_PHSL[0]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Px_PHSL[0]%256;
			fnRN8302_Write(PA_PHS,regbuf,2);
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Px_PHSL[1]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Px_PHSL[1]%256;
			fnRN8302_Write(PB_PHS,regbuf,2);
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Px_PHSL[2]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Px_PHSL[2]%256;
			fnRN8302_Write(PC_PHS,regbuf,2);
			//x相无功分段相位校正
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Qx_PHSL[0]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Qx_PHSL[0]%256;
			fnRN8302_Write(QA_PHS,regbuf,2);
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Qx_PHSL[1]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Qx_PHSL[1]%256;
			fnRN8302_Write(QB_PHS,regbuf,2);
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Qx_PHSL[2]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Qx_PHSL[2]%256;
			fnRN8302_Write(QC_PHS,regbuf,2);
	
			//x相无功功率增益
	//		  RN8302_WriteReg(GQA,(uint8_t*)&Dl645FirmPara.PGain[0],2);
	//		  RN8302_WriteReg(GQB,(uint8_t*)&Dl645FirmPara.PGain[1],2);
	//		  RN8302_WriteReg(GQC,(uint8_t*)&Dl645FirmPara.PGain[2],2);
			//x相视在功率增益
	//		  RN8302_WriteReg(GSA,(uint8_t*)&Dl645FirmPara.PGain[0],2);
	//		  RN8302_WriteReg(GSB,(uint8_t*)&Dl645FirmPara.PGain[1],2);
	//		  RN8302_WriteReg(GSC,(uint8_t*)&Dl645FirmPara.PGain[2],2);
			//x相有功功率offset
	//		  RN8302_WriteReg(PA_OS,(uint8_t*)&Dl645FirmPara.UI_Offset[0],2);
	//		  RN8302_WriteReg(PB_OS,(uint8_t*)&Dl645FirmPara.UI_Offset[1],2);
	//		  RN8302_WriteReg(PC_OS,(uint8_t*)&Dl645FirmPara.UI_Offset[2],2);
					//电流offset
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Ix_OS[0]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Ix_OS[0]%256;
			fnRN8302_Write(IA_OS,regbuf,2);
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Ix_OS[1]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Ix_OS[1]%256;
			fnRN8302_Write(IB_OS,regbuf,2);
			regbuf[0] = StDef_RN8302Para_Reg.Cst_Ix_OS[2]/256;
			regbuf[1] = StDef_RN8302Para_Reg.Cst_Ix_OS[2]%256;
			fnRN8302_Write(IC_OS,regbuf,2);
					
			//有功视在启动电流阈值
			regbuf[0] = 0x02;
			regbuf[1] = 0x36;
			fnRN8302_Write(IStart_PS,regbuf,2);
			//无功视在启动电流阈值
			regbuf[0] = 0x02;
			regbuf[1] = 0x36;
			fnRN8302_Write(IStart_Q,regbuf,2);
			//无功视在启动电流阈值
			regbuf[0] = 0x04;
			regbuf[1] = 0x00;
			fnRN8302_Write(LostVoltT,regbuf,2);
			//无功视在启动电流阈值
			regbuf[0] = 0x00;
			regbuf[1] = 0x2C;
			fnRN8302_Write(ZXOT,regbuf,2);
	
			//CF管脚配置
			regbuf[0] = 0x04;
			regbuf[1] = 0x77;
			regbuf[2] = 0x10;
			fnRN8302_Write(CFCFG,regbuf,3);
			//计量单元配置寄存器
			regbuf[0] = 0x40;
			regbuf[1] = 0x00;
			regbuf[2] = 0x00;
			fnRN8302_Write(EMUCFG,regbuf,3);
			//写缓存
			regbuf[0] = 0x10;
			fnRN8302_Write(WSAVECON,regbuf,1);
			//三相四线
			regbuf[0] = 0x00;
			fnRN8302_Write(MODESEL,regbuf,1);
			//计量控制位
			regbuf[0] = 0x77;regbuf[1] = 0x77;regbuf[2] = 0x77;
			fnRN8302_Write(EMUCON,regbuf,3);
			//写保护
			regbuf[0] = 0xDC;
			fnRN8302_Write(WREN,regbuf,1);	
}
void fnDl645Front_Exec(void)
{
	//读电压电流
	int i=0;
	int j=0;
	s32 Tempw,Tempq,Tempws,TempIa,TempIb,TempIc;
	
  for(i=0;i<4;i++)
	{
	   	Dl645FrontTmp.UI[i] = 0 ;
	    fnRN8302_Read( 0x000b+i , (u8 *)&Dl645FrontTmp.UI[i] , 4 ) ;
	    
	}
	for(i=0;i<3;i++)
	{
		Dl645FrontTmp.UI[i+4] = 0 ;
		fnRN8302_Read( 0x0007+i , (u8 *)&Dl645FrontTmp.UI[i+4] , 4 ) ;
	}
    
    //读功率
	for(i=0;i<12;i++)
	{
		Dl645FrontTmp.Pw[i] = 0 ;
		fnRN8302_Read(0x0014+i,(u8 *)&Dl645FrontTmp.Pw[i],4);
		if((Dl645FrontTmp.Pw[i]<30)&&(Dl645FrontTmp.Pw[i]>-30))
		Dl645FrontTmp.Pw[i]=0;
	}
    //读功率方向
  fnRN8302_Read(PQSign,(u8 *)&Dl645FrontTmp.PDirect,2);
    
    //读电压电流角度值
  for(i=0;i<5;i++)
	{
		Dl645FrontTmp.Angle[i] = 0 ;
		fnRN8302_Read(YUB+i,(u8 *)&Dl645FrontTmp.Angle[i],3);
		Dl645Front.PubData.Angle[i] = fnHexToBcd_u16((u16)(((float)Dl645FrontTmp.Angle[i]/16777216)*3600));
	}
    //功率计算
  for(j=0;j<3;j++)
	{
		for(i=0;i<3;i++)
		{
			Dl645Front.PubData.Pw[j*4+i+1]=fnDFConver_Hex32ToDF09((s32)((Dl645FrontTmp.Pw[j*4+i])/((Dl645FirmPara.PRmsConst))));//计算｛Pa Pb Pc｝、｛Qa Qb Qc｝、｛Sa Sb Sc｝
		}
	}
	Tempw = (s32)((Dl645FrontTmp.Pw[3])/((Dl645FirmPara.PRmsConst))) ;
	Dl645Front.PubData.Pw[0]=fnDFConver_Hex32ToDF09(Tempw);
	Tempq = (s32)((Dl645FrontTmp.Pw[7])/((Dl645FirmPara.PRmsConst))) ;
	Dl645Front.PubData.Pw[4]=fnDFConver_Hex32ToDF09(Tempq);
	// 计算总视在功率
	Tempws = (s32)sqrt((float)Tempw*(float)Tempw+(float)Tempq*(float)Tempq);
	Dl645Front.PubData.Pw[8] = fnDFConver_Hex32ToDF09(Tempws);//计算P、Q
	
	//功率因数计算
	//总功率因数
	Dl645Front.PubData.Pf[0]=fnDFConver_Bcd16To16((s16)((1000*(double)(Tempw))/(Tempws)));
	
	//电压电流计算
	for(i=0;i<3;i++)
	{
		Dl645Front.PubData.U[i]=0x7fff&(fnDFConver_Bcd16To16((s16)(Dl645FrontTmp.UI[i+4]/(10*(Dl645FirmPara.VRmsConst))))); //电压
	}
    
    
    TempIa = (s32)(Dl645FrontTmp.UI[0]/(Dl645FirmPara.IRmsConst));
    TempIb = (s32)(Dl645FrontTmp.UI[1]/(Dl645FirmPara.IRmsConst));
    TempIc = (s32)(Dl645FrontTmp.UI[2]/(Dl645FirmPara.IRmsConst));
    Dl645Front.PubData.I[0] = fnDFConver_Bcd32To32(TempIa);
    Dl645Front.PubData.I[1] = fnDFConver_Bcd32To32(TempIb);
    Dl645Front.PubData.I[2] = fnDFConver_Bcd32To32(TempIc);
    Dl645Front.PubData.I[3] = fnDFConver_Bcd32To32((s32)(Dl645FrontTmp.UI[3]/(Dl645FirmPara.IRmsConst)));

	if(Dl645FrontTmp.PDirect&0x0001) Dl645Front.PubData.I[0] |= 0x80000000;
	if(Dl645FrontTmp.PDirect&0x0002) Dl645Front.PubData.I[1] |= 0x80000000;
	if(Dl645FrontTmp.PDirect&0x0004) Dl645Front.PubData.I[2] |= 0x80000000;    

    
	//频率计算
	if((Dl645FrontTmp.Frequency>1638400)||(Dl645FrontTmp.Frequency<1092266))  //when 50HZ is 5120 ;+- 20% frequence is 6144 and 4096
		Dl645FrontTmp.Frequency=1310720;
	Dl645Front.PubData.Frequency=fnHexToBcd_u16((u16)(65536000/(Dl645FrontTmp.Frequency/100)));
	
	//转换功率方向
	Dl645Front.PubData.PDirect = ( (Dl645FrontTmp.PDirect << 1) & 0xee) | ( (Dl645FrontTmp.PDirect >> 3) & 0x11);

	
	//if(Dl645Bkgrd.PriPara.WireMode==0x01)   //三相三线
	{
		Dl645Front.PubData.Angle[3]=0;
		Dl645Front.PubData.Angle[6]=0;
	}
	
	
	//从RN8302读出的即为脉冲数//{P,Q,Ps},{Pa,Qa,Psa},{Pb,Qb,Psb},{Pc,Qc,Psc}{Fp,Fq}{Fpa,Fqa}{Fpb,Fqb}{Fpc,Fqc}
    /*读电能寄存器*/
	for(i=0;i<3;i++)
	{
		Dl645FrontTmp.Pulse[(i+1)*3] = 0;
		fnRN8302_Read(EPA+i,(u8 *)&Dl645FrontTmp.Pulse[(i+1)*3],3);   //有功
		Dl645FrontTmp.Pulse[(i+1)*3+1] = 0;
		fnRN8302_Read(EQA+i,(u8 *)&Dl645FrontTmp.Pulse[(i+1)*3+1],3);  //无功
		Dl645FrontTmp.Pulse[(i+1)*3+2] = 0;
		fnRN8302_Read(ESA+i,(u8 *)&Dl645FrontTmp.Pulse[(i+1)*3+2],3);   //视在有功
		Dl645FrontTmp.Pulse[14+i*2] =0;
		fnRN8302_Read(FEPA+i,(u8 *)&Dl645FrontTmp.Pulse[14+i*2],3);      //基波有功
		Dl645FrontTmp.Pulse[15+i*2] = 0;
		fnRN8302_Read(FEQA+i,(u8 *)&Dl645FrontTmp.Pulse[15+i*2],3);      //基波无功
	}
	Dl645FrontTmp.Pulse[0] = 0;
	fnRN8302_Read(EPT,(u8 *)&Dl645FrontTmp.Pulse[0],3);   		//总有功
	Dl645FrontTmp.Pulse[1] = 0;
	fnRN8302_Read(EQT,(u8 *)&Dl645FrontTmp.Pulse[1],3);   		//总无功
	Dl645FrontTmp.Pulse[2] = 0;
	fnRN8302_Read(EST,(u8 *)&Dl645FrontTmp.Pulse[2],3);   		//总视在
	Dl645FrontTmp.Pulse[12] =0;
	fnRN8302_Read(FEPT,(u8 *)&Dl645FrontTmp.Pulse[12],3);      //总基波有功
	Dl645FrontTmp.Pulse[13] =0;
	fnRN8302_Read(FEQT,(u8 *)&Dl645FrontTmp.Pulse[13],3);      //总基波无功
	

    for(i=0;i<20;i++)//电能高频脉冲转换成低频脉冲
	{
		Dl645FrontTmp.Pulse_Eg[i]+=Dl645FrontTmp.Pulse[i];
    }
}

void fnDl645Energy_Exec(void)
{
	u8 Ep[20];
	if( !(Dl645FrontTmp.Pulse_Eg[0] | Dl645FrontTmp.Pulse_Eg[1] | Dl645FrontTmp.Pulse_Eg[2] | Dl645FrontTmp.Pulse_Eg[3] |
		Dl645FrontTmp.Pulse_Eg[4] | Dl645FrontTmp.Pulse_Eg[5] | Dl645FrontTmp.Pulse_Eg[6] | Dl645FrontTmp.Pulse_Eg[7] |
		Dl645FrontTmp.Pulse_Eg[8] | Dl645FrontTmp.Pulse_Eg[9] | Dl645FrontTmp.Pulse_Eg[10] | Dl645FrontTmp.Pulse_Eg[11] |
		Dl645Eg.PubPara.fMinute) ) 
	{
		return;
	}
	//Dl645Eg.PubPara.PPlsDeltaE为脉冲数，如脉冲数为800，因电能小数为2位，故此值为8
	for(int i = 0 ; i < 12 ; i++)
	{
		Ep[i] = 0;
		
		if(Dl645FrontTmp.Pulse_Eg[i])
		{
			Dl645Eg.PriData.PieceEg[i] += Dl645FrontTmp.Pulse_Eg[i];
			Dl645FrontTmp.Pulse_Eg[i] = 0;
			
			switch(i % 3)
			{
			case 0:
				if(Dl645Eg.PubPara.PPlsDeltaE == 0) break;				
				while (Dl645Eg.PriData.PieceEg[i] >= Dl645Eg.PubPara.PPlsDeltaE)//需按电能小数位数进行计算
		  	    {
		  	    	Ep[i]++;
		  	    	Dl645Eg.PriData.PieceEg[i] -= Dl645Eg.PubPara.PPlsDeltaE;
		  	    }
				break;
			case 1:
				if(Dl645Eg.PubPara.QPlsDeltaE == 0) break;
				while (Dl645Eg.PriData.PieceEg[i] >= Dl645Eg.PubPara.QPlsDeltaE)//需按电能小数位数进行计算
		  	    {
		  	    	Ep[i]++;
		  	    	Dl645Eg.PriData.PieceEg[i] -= Dl645Eg.PubPara.QPlsDeltaE;
		  	    }
				break;
			default:
				if(Dl645Eg.PubPara.PsPlsDeltaE == 0) break;
				while (Dl645Eg.PriData.PieceEg[i] >= Dl645Eg.PubPara.PsPlsDeltaE)//需按电能小数位数进行计算
		  	    {
		  	    	Ep[i]++;
		  	    	Dl645Eg.PriData.PieceEg[i] -= Dl645Eg.PubPara.PsPlsDeltaE;
		  	    }
				break;	
			}
		}	
	}
	
	for(int i = 0 ; i < 8 ; i++)
	{
		Ep[i+12] = 0;
		
		if(Dl645FrontTmp.Pulse_Eg[i+12])
		{
			Dl645Eg.PriData.PieceEg[i+12] += Dl645FrontTmp.Pulse_Eg[i+12];
			Dl645FrontTmp.Pulse_Eg[i+12] = 0;
			
			switch((i+12) % 2)
			{
			case 0:
				if(Dl645Eg.PubPara.PPlsDeltaE == 0) break;				
				while (Dl645Eg.PriData.PieceEg[i+12] >= Dl645Eg.PubPara.PPlsDeltaE)//需按电能小数位数进行计算
		  	    {
		  	    	Ep[i+12]++;
		  	    	Dl645Eg.PriData.PieceEg[i+12] -= Dl645Eg.PubPara.PPlsDeltaE;
		  	    }
				break;
			default :
				if(Dl645Eg.PubPara.QPlsDeltaE == 0) break;
				while (Dl645Eg.PriData.PieceEg[i+12] >= Dl645Eg.PubPara.QPlsDeltaE)//需按电能小数位数进行计算
		  	    {
		  	    	Ep[i+12]++;
		  	    	Dl645Eg.PriData.PieceEg[i+12] -= Dl645Eg.PubPara.QPlsDeltaE;
		  	    }
				break;
			}
		}	
	}
}




RN8302_Info gRN_Info;
unsigned int Kv=1;

unsigned int testAwave[3];
unsigned int testIwave[3];
unsigned int testPower[3];

void ReadAvol(void)
{
	uint32_t regtemp=0;
	unsigned char avol[4];
	unsigned char bvol[4];
	unsigned char cvol[4];
	unsigned char acol[4];
	unsigned char bcol[4];
	unsigned char ccol[4];
	unsigned char ncol[4];
	unsigned char apower[4];
	unsigned char bpower[4];
	unsigned char cpower[4];
	
	float k_a,k_b,k_c;
	uint8_t regArry[]={0x0007,0x0008,0x0009};	
	fnRN8302_Read(0x0007,(u8 *)avol,4);	
	fnRN8302_Read(0x0008,(u8 *)bvol,4);
	fnRN8302_Read(0x0009,(u8 *)cvol,4);
	testAwave[0]=BufToU32(avol);
	testAwave[1]=BufToU32(bvol);
	testAwave[2]=BufToU32(cvol);
	//9200       x
    //44000000   220
    for(int i=0;i<3;i++){
			gRN_Info.vol[i]=(float)(testAwave[i]*220/4223646.0);
			if(gRN_Info.vol[i]>500){
               gRN_Info.vol[i]=0.0;
			}
		    gSystem_Info.vol[i]=gRN_Info.vol[i];
      //gRN_Info.vol[i]=(float)(testAwave[i]/19197.3);
	}

	fnRN8302_Read(0x000B,(u8 *)acol,4);	
	fnRN8302_Read(0x000C,(u8 *)bcol,4);
	fnRN8302_Read(0x000D,(u8 *)ccol,4);
	fnRN8302_Read(0x000E,(u8 *)ncol,4);
	testIwave[0]=BufToU32(acol);
	testIwave[1]=BufToU32(bcol);
	testIwave[2]=BufToU32(ccol);
		
	for(int i=0;i<3;i++){
		if(testIwave[i]>1904100){
			gRN_Info.cur[i]=(float)(testIwave[i]*1/400000.0/*748918.03*/);
		}else{
			gRN_Info.cur[i]=(float)(testIwave[i]*1/400507.0/*748918.03*/);
		}
		if(gRN_Info.cur[i]>4000){
			gRN_Info.cur[i]=0.0;
		}
		if(gSystem_Info.cul[i]<0.1){
			gSystem_Info.cul[i]=0;

		}else{
		gSystem_Info.cul[i]=gRN_Info.cur[i];
		}
	}
				
	fnRN8302_Read(0x0014,(u8 *)apower,4);	
	fnRN8302_Read(0x0015,(u8 *)bpower,4);
	fnRN8302_Read(0x0016,(u8 *)cpower,4);
	//fnRN8302_Read(0x000E,(u8 *)ncol,4);
	testPower[0]=BufToU32(apower);
	testPower[1]=BufToU32(bpower);
	testPower[2]=BufToU32(cpower);

	gSystem_Info.totalpower=0.0;
	
	for(int i=0;i<3;i++){
			gRN_Info.power[i]=(float)(testPower[i]/1759.9);
			if(gRN_Info.power[i]>1000){
               gRN_Info.power[i]=0.0;
			}
			gSystem_Info.power[i]=gRN_Info.power[i];
			gSystem_Info.totalpower+=gSystem_Info.power[i];
	}
	gSystem_Info.totalpower=gSystem_Info.totalpower/1000.0;
	
}

/********************************************************
功能描述：   RN8302校表初始化
参数说明：
返回说明：
调用方式：
全局变量：
读写时间：
注意事项：
日期    ：
********************************************************/
unsigned char regstatus[2];
uint8_t RN8302_CalibrateInit(void)
{
    uint8_t regbuf[4];
    uint8_t status = 1;

    fnRN8302_Read(0x018F,regbuf,3);
    if((regbuf[2]==0x83)&&(regbuf[1]==0x02)&&(regbuf[0]==0x00)){
            status = 0;
    }
     else{
            status = 1;
     }
    
    
    //deviceID读取成功
    if(status == 0){
        //写使能
        regbuf[0] = 0xE5;
        fnRN8302_Write(WREN,regbuf,1);
        //切换到EMM模式
        regbuf[0] = 0xA2;
        fnRN8302_Write(WMSW,regbuf,1);
        //复位
        regbuf[0] = 0xFA;
        fnRN8302_Write(SOFTRST,regbuf,1);
        Delay_ms(20);
        //写使能
        regbuf[0] = 0xE5;
        fnRN8302_Write(WREN,regbuf,1);
        //切换到EMM模式
        regbuf[0] = 0xA2;
        fnRN8302_Write(WMSW,regbuf,1);
        //写ADC 0x83 00
        regbuf[1] = 0;
        regbuf[0] = 0;
		
        fnRN8302_Read(0x183,regstatus,2);
        fnRN8302_Write(0x183,regbuf,2); //1倍增益
	
        regbuf[1] = 0x2E;
        regbuf[0] = 0xA1;

		
        //写HFCONST1
        regbuf[1] = 0x3E;
		regbuf[0] = 0x73;
        fnRN8302_Write(HFCONST1,regbuf,2);
        fnRN8302_Read(HFCONST1,regstatus,2);
        //写电压增益
        StDef_RN8302Para_Reg.Cst_GSUx[0] = 0;
        StDef_RN8302Para_Reg.Cst_GSUx[1] = 0;
        StDef_RN8302Para_Reg.Cst_GSUx[2] = 0;
        fnRN8302_Write(GSUA,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GSUx[0],2);
        fnRN8302_Write(GSUB,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GSUx[1],2);
        fnRN8302_Write(GSUC,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GSUx[2],2);
        //写电流增益
        StDef_RN8302Para_Reg.Cst_GSIx[0] = 100;
        StDef_RN8302Para_Reg.Cst_GSIx[1] = 0;

        fnRN8302_Write(GSIA,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GSIx[0],2);
        fnRN8302_Write(GSIB,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GSIx[0],2);
        fnRN8302_Write(GSIC,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GSIx[0],2);
        fnRN8302_Write(GSIN,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GSIx[0],2);
        //x相有功功率增益
        StDef_RN8302Para_Reg.Cst_GPx[0] = 0x0;
        StDef_RN8302Para_Reg.Cst_GPx[1] = 0;
        StDef_RN8302Para_Reg.Cst_GPx[2] = 0;
        fnRN8302_Write(GPA,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GPx[0],2);
        fnRN8302_Write(GPB,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GPx[0],2);
        fnRN8302_Write(GPC,(uint8_t*)&StDef_RN8302Para_Reg.Cst_GPx[0],2);
        //x相有功分段相位校正
        StDef_RN8302Para_Reg.Cst_Px_PHSL[0] = 0;
        StDef_RN8302Para_Reg.Cst_Px_PHSL[1] = 0;
        StDef_RN8302Para_Reg.Cst_Px_PHSL[2] = 0;
        fnRN8302_Write(PA_PHS,(uint8_t*)&StDef_RN8302Para_Reg.Cst_Px_PHSL[0],2);
        fnRN8302_Write(PB_PHS,(uint8_t*)&StDef_RN8302Para_Reg.Cst_Px_PHSL[1],2);
        fnRN8302_Write(PC_PHS,(uint8_t*)&StDef_RN8302Para_Reg.Cst_Px_PHSL[2],2);
        //x相无功分段相位校正
        StDef_RN8302Para_Reg.Cst_Qx_PHSL[0] = 0;
        StDef_RN8302Para_Reg.Cst_Qx_PHSL[1] = 0;
        StDef_RN8302Para_Reg.Cst_Qx_PHSL[2] = 0;
        fnRN8302_Write(QA_PHS,(uint8_t*)&StDef_RN8302Para_Reg.Cst_Qx_PHSL[0],2);
        fnRN8302_Write(QB_PHS,(uint8_t*)&StDef_RN8302Para_Reg.Cst_Qx_PHSL[1],2);
        fnRN8302_Write(QC_PHS,(uint8_t*)&StDef_RN8302Para_Reg.Cst_Qx_PHSL[2],2);
                //电流offset
        regbuf[0] = 0;
        regbuf[1] = 0;
        fnRN8302_Write(IA_OS,regbuf,2);
        regbuf[0] = 0;
        regbuf[1] = 0;
        fnRN8302_Write(IB_OS,regbuf,2);
        regbuf[0] = 0;
        regbuf[1] = 0;
        fnRN8302_Write(IC_OS,regbuf,2);
        //有功视在启动电流阈值
        regbuf[0] = 0x36;//0x02
        regbuf[1] = 0x02;//0x36
        fnRN8302_Write(IStart_PS,regbuf,2);
        //无功视在启动电流阈值
        regbuf[0] = 0x36;//0x02
        regbuf[1] = 0x02;//0x36
        fnRN8302_Write(IStart_Q,regbuf,2);
        //无功视在启动电流阈值
        regbuf[0] = 0x00;//0x04
        regbuf[1] = 0x04;//0x00
        fnRN8302_Write(LostVoltT,regbuf,2);
        //无功视在启动电流阈值
        regbuf[0] = 0x2C;//0x00
        regbuf[1] = 0x00;//0x2C
        fnRN8302_Write(ZXOT,regbuf,2);
        //CF管脚配置
        regbuf[0] = 0x10;//0x04
        regbuf[1] = 0x32;//0x32
        regbuf[2] = 0x04;//0x10
        fnRN8302_Write(CFCFG,regbuf,3);
        //计量单元配置寄存器
        regbuf[0] = 0x00;//0x40 //open the high pass
        regbuf[1] = 0x00;//0x00
        regbuf[2] = 0x40;//0x00
        fnRN8302_Write(EMUCFG,regbuf,3);
        memset(regbuf,0,3);
        fnRN8302_Read(EMUCFG,regbuf,3);
        //计量控制位
        regbuf[0] = 0x77;//0x00
				regbuf[1] = 0x00;//0x00
				regbuf[2] = 0x00;//0x77
				
        fnRN8302_Write(EMUCON,regbuf,3);
        //校表时不要写保护
        //regbuf[0] = 0xDC;
        //RN8302_WriteReg(WREN,regbuf,1);
    }

    return status;
}

//（3）RN8302校表，电压、电流增益校准（校表台设置3相电压设置成220V,3相电流设置成5A）
/********************************************************
功能描述：   功率法校正电压增益
参数说明：   vol：扩大100倍
返回说明：
调用方式：
全局变量：-0.9207739        0.0792261   15708.2
读写时间：
注意事项：Ux=3485946 Ix=796773
日期    ：
********************************************************/
uint16_t RN8302_CalibrateUGain(uint16_t vol,uint8_t phase)
{
    uint8_t regbuf[5];
    const uint16_t regURMS[]={0x0007,0x0008,0x0009};
    const uint16_t regGUSx[]={GSUA,GSUB,GSUC};
    float err=0;
    uint16_t uGain;
    int32_t data;

    for(int i=0;i<3;i++){

		fnRN8302_Read(regURMS[phase],regbuf,4);
                data = BufToU32(regbuf);
        err = (data-44000000)*1.0/44000000;
        err = -err/(1+err);
        if(err > 0){
            uGain = (uint16_t)(err*32768);
        }
        else{
            uGain = (uint16_t)(err*32768+65536);
        }
                //写使能
        regbuf[0] = 0xE5;
        fnRN8302_Write(WREN,regbuf,1);
        regbuf[0] =0x26;
		    regbuf[1] =0x22;
        fnRN8302_Write(regGUSx[i],regbuf,2);

         //Lose vol set mode 
         regbuf[0]=0x00;//0x5B;
		 regbuf[1]=0x03;
		 fnRN8302_Read(0x104,regstatus,2);
		 fnRN8302_Write(0x104,regbuf,2);
		 fnRN8302_Read(0x104,regstatus,2);
         memset(regbuf,0x00,2);
         fnRN8302_Read(regGUSx[i],regbuf,2);
         StDef_RN8302Para_Reg.Cst_GSUx[phase] = uGain;		
    }

    return uGain;
}

/********************************************************
功能描述：   功率法校准电流
参数说明：
返回说明：
调用方式：
全局变量：
读写时间：
注意事项：
日期    ：
********************************************************/
uint16_t RN8302_CalibrateIGain(uint16_t cur,uint8_t phase)
{
    uint8_t regbuf[5];
    const uint16_t regIRMS[]={0x000B,0x000C,0x000D};
    const uint16_t regGISx[]={GSIA,GSIB,GSIC};
    float err=0;
    uint16_t iGain;
    int32_t data;
    //804571    1A
	for(int i=0;i<3;i++){
        fnRN8302_Read(regIRMS[phase],regbuf,4);
        data = BufToU32(regbuf);
        err = (data-3500000)*1.0/3500000;
        err = -err/(1+err);
        if(err > 0){
            iGain = (uint16_t)(err*32768);
        }
        else{
            iGain = (uint16_t)(err*32768+65536);
        }
        //写使能
        regbuf[0] = 0xE5;
        fnRN8302_Write(WREN,regbuf,1);
	      regbuf[0]=0 ;
        regbuf[1]=0 ;
        fnRN8302_Write(regGISx[i],regbuf,2);
        memset(regbuf,0x00,2);
        fnRN8302_Read(regGISx[phase],regbuf,2);
        StDef_RN8302Para_Reg.Cst_GSIx[phase] = iGain;
	}
    return iGain;
}

/*

（4）RN8302校表，功率因素在1.0情况下可以不校准，只需通过校表台查看一下误差，误差较大的，再进行校准；正常情况下，电压、电流校准后，1.0无需校准
（5）RN8302校表，0.5L,A相校准,（校表台设置A相电压设置成220V,A相电流设置成5A,功率因素0.5L）
（6）RN8302校表，0.5L,B相校准,（校表台设置B相电压设置成220V,B相电流设置成5A,功率因素0.5L）
（7）RN8302校表，0.5L,C相校准,（校表台设置C相电压设置成220V,C相电流设置成5A,功率因素0.5L）
*/
/********************************************************
功能描述：   功率法校准 Un Ib 0.5增益
参数说明：   err扩大1000倍 xx.xxx
返回说明：
调用方式：
全局变量：
读写时间：
注意事项：
日期    ：
********************************************************/
uint16_t RN8302_CalibratePx_PHSL(uint8_t phase)
{
		uint8_t regbuf[5],i;
		uint32_t regtemp[5],regtotal=0;
		float err = 0;
		const uint16_t regGPx[]={PA_PHS,PB_PHS,PC_PHS};
		const uint16_t regGQx[]={QA_PHS,QB_PHS,QC_PHS};
		const uint16_t regArry[]={0x0014,0x0015,0x0016};
		uint16_t pGain;
		uint32_t data;
    for(i=0;i<5;i++){
        if(fnRN8302_Read(regArry[phase],regbuf,4)==0){
            regtemp[i] = BufToU32(regbuf);
            regtotal += regtemp[i];
        }
        Delay_ms(400);
    }
    data = regtotal / 5;
    err = (data-9179115)*1.0/9179115;
    err = -err/(sqrt(3));
    if(err > 0){
        pGain = (uint16_t)(err*32768);
    }
    else{
        pGain = (uint16_t)(err*32768+65536);
    }
    //写使能
    regbuf[0] = 0xE5;
    fnRN8302_Write(WREN,regbuf,1);
    regbuf[0] = pGain/256;regbuf[1] = pGain%256;
    fnRN8302_Write(regGPx[phase],regbuf,2);
    fnRN8302_Write(regGQx[phase],regbuf,2);
    StDef_RN8302Para_Reg.Cst_Px_PHSL[phase] = pGain;
    StDef_RN8302Para_Reg.Cst_Qx_PHSL[phase] = pGain;
    return pGain;
}

uint8_t RN8302_CalibrateCurrentOffset(uint8_t phase)
{
    uint8_t regbuf[5];
    uint32_t regtemp[10],regtotal=0;
    const uint16_t regArry[]={0x000B,0x000C,0x000D};
    const uint16_t regIx_OS[]={IA_OS,IB_OS,IC_OS};
    uint8_t i = 0;
    uint16_t temp;
    for(i=0;i<12;i++)
    {
        if(fnRN8302_Read(regArry[phase],regbuf,4)==0){
            regtemp[i] = (regbuf[0]<<24)+(regbuf[1]<<16)+\
                    (regbuf[2]<<8)+(regbuf[3]);
        }
        regtotal += regtemp[i];
        delay_ms(300);
    }
    regtotal /= 12;
    regtotal = regtotal * regtotal;
 
    regtotal = ~regtotal;
    temp = (regtotal / 0x4000)&0xFFFF;

    if(regtotal & 0x80000000)
        temp |= 0x8000;

    regbuf[0] = 0xE5;
    fnRN8302_Write(WREN,regbuf,1);
    regbuf[0] = temp/256;regbuf[1] = temp%256;
    fnRN8302_Write(regIx_OS[phase],regbuf,2);
    StDef_RN8302Para_Reg.Cst_Ix_OS[phase] = temp;
    return 0;
}



void RN8302B_CalibConfig(void)
{
	RN8302_CalibrateInit();
	RN8302B_DCOffset();
	RN8302_CalibrateUGain(220,0);
	RN8302_CalibrateIGain(0,0);
	for(int i=0;i<3;i++){
    RN8302_CalibrateCurrentOffset(i);
	}

}

 uint8_t dc_offser_regbuf[12];
void RN8302B_DCOffset(void)
{
	dc_offser_regbuf[0]=0x02;
	fnRN8302_Write(0x170,dc_offser_regbuf,1);
	dc_offser_regbuf[0]=0x03;
	fnRN8302_Write(0x191,dc_offser_regbuf,1);
	delay_ms(20000);
	for(int i=0;i<6;i++){
	fnRN8302_Read(0x01A+i,&dc_offser_regbuf[i],2);
	}
}

