#ifndef _ATT702X_H_
#define _ATT702X_H_


#define BIT_WPWR_PHT BIT0
#define BIT_WPWR_PHA BIT1
#define BIT_WPWR_PHB BIT2
#define BIT_WPWR_PHC BIT3
#define BIT_VPWR_PHT BIT4
#define BIT_VPWR_PHA BIT5
#define BIT_VPWR_PHB BIT6
#define BIT_VPWR_PHC BIT7

#define BIT_VAR_QM1 BIT0
#define BIT_VAR_QM2 BIT1
#define BIT_VAR_QM3 BIT2
#define BIT_VAR_QM4 BIT3

#define BIT_BRKV_PHA BIT0
#define BIT_BRKV_PHB BIT1
#define BIT_BRKV_PHC BIT2
#define BIT_BRKI_PHA BIT4
#define BIT_BRKI_PHB BIT5
#define BIT_BRKI_PHC BIT6

#define IDT_MULT 2
#define POWER_RATIO (float)(0.0032*2)

#define RN8302_MODE_GOSLP 0x18 //睡眠模式
#define RN8302_MODE_GONVM2 0x5C //NVM2模式
#define RN8302_MODE_GONVM1 0xE1 //NVM1模式
#define RN8302_MODE_GOEMM 0xA2 //工作模式
#define CALIBRATE_TIMER_COUNT 30




enum MSRCTypeSelect
{
	MSRRN8302 = 0,
//	MSR90E32AS,
//	MSRATT7022E,
	MSRBUT
};
enum PMTypeSelect{DSSD, DTSD};
enum GetVolSelect{pwrOn, pwrDwn};



#define MSR90E32AS    0

#define MSRATT7022E   0

#define FMSRTYPE      MSRRN8302

#define MSRRN8302      1



typedef unsigned char UCHAR; 
typedef unsigned long ULONG;
typedef unsigned int  UINT;

typedef UCHAR (*MSRC_PROC)(UINT wReg, UCHAR *data, UCHAR len);

#define EMPPBUT   3
#define EMFIBUT   4
#define EMPLSBUT  4


/*
*******************************************************************************
**  Variable define
*******************************************************************************
*/
#pragma pack(1)

typedef union
{
    UCHAR BYTE;

    struct
    {
        UCHAR WPHT :1; //合相有功功率方向    
        UCHAR WPHA :1; //A相有功功率方向
        UCHAR WPHB :1; //B相有功功率方向
        UCHAR WPHC :1; //C相有功功率方向

        UCHAR VPHT :1; //合相无功功率方向        
        UCHAR VPHA :1; //A相无功功率方向
        UCHAR VPHB :1; //B相无功功率方向
        UCHAR VPHC :1; //C相无功功率方向
    }BIT;
}UNIONPWRDIR;

typedef union
{
    UCHAR BYTE;

    struct
    {
        UCHAR VARI :1; //无功第1向限标志
        UCHAR VARII :1; //无功第2向限标志
        UCHAR VARIII :1; //无功第3向限标志 
        UCHAR VARIV :1; //无功第2向限标志
        UCHAR :1;
        UCHAR :1;
        UCHAR :1;
        UCHAR :1;
    }BIT;
}UNIONVARFLG;

typedef union
{
    UCHAR BYTE;

    struct
    {
        UCHAR PHAV :1; //A相断电
        UCHAR PHBV :1; //B相断电
        UCHAR PHCV :1; //C相断电
        UCHAR :1; 
        UCHAR PHAI :1; //A相断流
        UCHAR PHBI :1; //B相断流
        UCHAR PHCI :1; //C相断流
        UCHAR :1;
    }BIT;
}UNIONBRKVIFLG;

typedef union
{
    UCHAR BYTE;

    struct
    {
        UCHAR seqVErr :1; //电压逆相序
        UCHAR seqIErr :1; //电流逆相序
        UCHAR :1; 
        UCHAR :1;
        UCHAR :1; 
        UCHAR :1;
        UCHAR :1; 
        UCHAR :1;        
    }BIT;
}UNIONPHSSEQFLG;

typedef union
{
    UCHAR BYTE;

    struct
    {
        UCHAR REINIT :1; //ATT7022X需要重新初始化标志
        UCHAR PULSEDB :1; //设置脉冲常数加倍标志
        UCHAR ISVLD :1; //ATT7022X有效性标志
        UCHAR INITEND :1; //初始化完成标志
        UCHAR RDERR :1; //ATT7022x读数据出错标志
        UCHAR WRERR :1; //写数据错误
        UCHAR CDWR :1;
        UCHAR :1;
    }BIT;
}UNIONMSRCHPFLG;

typedef struct 
{
    UCHAR voltage[EMPPBUT][4]; //瞬时A,B,C相电压值
    UCHAR current[EMPPBUT+1][4]; //瞬时A,B,C相零线电流值
    ULONG volAngl[EMPPBUT]; //瞬时Uab,Uac,Ubc 电压夹角
    ULONG curAngl[EMPPBUT]; //瞬时A,B,C相电流与电压夹角    
    ULONG factor[EMFIBUT]; //瞬时总,A,B,C相功率因数        
    ULONG freq; //电压频率
    UINT pwrDir; //功率方向    

    UCHAR watPwr[EMFIBUT][4]; //瞬时总,A,B,C相有功功率值
    UCHAR varPwr[EMFIBUT][4]; //瞬时总,A,B,C相无功功率值
    UCHAR aprPwr[EMFIBUT][4]; //瞬时总,A,B,C相视在功率值    
}MsrchpInstValue;

typedef struct
{
    ULONG voltage[EMPPBUT]; //A,B,C相实时电压值
    ULONG current[EMPPBUT+1]; // A,B,C相及零线电流实时值
    int volAngl[EMPPBUT]; //瞬时A,B,C相电压相角
    int curAngl[EMPPBUT]; //瞬时A,B,C相电流相角

    ULONG factor[EMFIBUT]; //总,A,B,C相功率因数值                  
    ULONG freq; //电网频率    
    UNIONPWRDIR pwrFlg; //功率方向标志
    UINT temperature;
    
    long watPwr[EMFIBUT]; //瞬时总，A,B,C相有功功率；
    long varPwr[EMFIBUT]; //瞬时总，A,B,C相无功功率；
    long aprPwr[EMFIBUT]; //瞬时总，A,B,C相视在功率；    
}MsrchpRealValue;

typedef struct
{
    UCHAR vPseqCnt; //电压逆相序判次数
    UCHAR iPseqCnt; //电流逆相序判次数
    UCHAR sumErrCnt; //校验和出错次数
    UCHAR dmndFlgCnt; //需量方向变化记数
    UNIONMSRCHPFLG msrChpFlg; //计量芯片操作标志    
    ULONG basicVal; //功率常数
    ULONG regCsVal; //寄存器校验和值
    UINT pulseConst; //脉冲常数
    UCHAR buffer[3];
    UCHAR pulseDb[6]; //脉冲常数加倍设置    
    UNIONVARFLG varFlg[EMFIBUT]; //无功方向标志
    UNIONBRKVIFLG brkFlg; //断压断流标志
    UNIONPHSSEQFLG pseqFlg; //相序错误标志
    UINT commCalib; //通信校表
}MsrchpVariable;

#pragma pack(1)
typedef struct
{
    UCHAR head; //帧头0x68
    
    UINT pulseConst; //脉冲常数
    UINT hfConst[2]; //高频脉冲输出寄存器
    UCHAR VGain[3][2]; //电压增益
    UCHAR IGain[3][2]; //电流增益
    UINT volRatio; //电压系数
    UINT curRatio; //电流系数    
    UCHAR curStart[2]; //启动电流设置
    UCHAR reserve1[2]; //备用校表数据1（过零阈值）
    UCHAR reserve2[2]; //备用校表数据2（电压增益）
    UCHAR phsIreg[4][2]; //相位分段补偿区域设置
    UCHAR PGain[3][2]; //功率增益
    UCHAR PHSI[4][3][3]; //相位分段校正0,1,2,3
    UCHAR PHOS[3][3]; // 通道功率OFFSET校正    
    UCHAR IrmsOffset[3][3]; //电流OFFSET校正
    ULONG InRegCs; //内部寄存器校验和
    ULONG ExtRegCs; //外部寄存器校验和
    
    UCHAR chkSum; //校表数据区校验和
    UCHAR endflg; //校验数据结束符
}MsrchpCalData;

typedef struct
{
    UCHAR head;
    UINT cnt;
    UCHAR calTime[6];
    UCHAR chkSum;
    UCHAR endflg;    
}MsrchpCalInfo;

typedef struct 
{
    int posWatt[EMPLSBUT]; //A，B，C及合相有功脉冲数
    int negWatt[EMPLSBUT];
    int var[EMPLSBUT]; //A，B，C及合相无功脉冲数
}MsrchpPulse;

typedef struct
{
    MsrchpCalData calData;
    MsrchpInstValue instVal;
    MsrchpRealValue realVal;
    MsrchpVariable var;
    MsrchpPulse Pulse;
}stuAttObj;




 const MSRC_PROC MSRCWriteData[MSRBUT];
 const MSRC_PROC MSRCReadData[MSRBUT];

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
 void MSRCSPIInit(void);
 void MSRCHardReset(void);
 void MSRCSoftReset(void);
 void MSRCAPIFunc(void);
 void MSRCVarInit(void);
 void MSRCRealValueInit(void);
 void MSRCReadEnergy(void);
 void MSRCDeviceInit(void);
 void MSRCCalParaInit(void);
 void MSRCCalInfoInit(void);
 UCHAR MSRCCalInfoVerify(void);
 void PDMSRCSPIInit(void);
 void GetFullLowvCurrent(void);
 void CalcPulseConst(void);
 void MSRCChksumCompare(void);
 void MSRCStatusCheck(void);
 void MSRCValidCheck(void);
 void ATT7022ReadCalibrateData(UINT wReg, UCHAR *data, UCHAR len);
 UCHAR MSRCCalDataVerify(void);
 UCHAR GetVoltagePwrOnDwn(enum GetVolSelect item);
 UCHAR SlvsWriteCalData(void);
 UCHAR SlvsReadCalData(void);

#endif


