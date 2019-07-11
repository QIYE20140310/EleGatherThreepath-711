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

#define RN8302_MODE_GOSLP 0x18 //˯��ģʽ
#define RN8302_MODE_GONVM2 0x5C //NVM2ģʽ
#define RN8302_MODE_GONVM1 0xE1 //NVM1ģʽ
#define RN8302_MODE_GOEMM 0xA2 //����ģʽ
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
        UCHAR WPHT :1; //�����й����ʷ���    
        UCHAR WPHA :1; //A���й����ʷ���
        UCHAR WPHB :1; //B���й����ʷ���
        UCHAR WPHC :1; //C���й����ʷ���

        UCHAR VPHT :1; //�����޹����ʷ���        
        UCHAR VPHA :1; //A���޹����ʷ���
        UCHAR VPHB :1; //B���޹����ʷ���
        UCHAR VPHC :1; //C���޹����ʷ���
    }BIT;
}UNIONPWRDIR;

typedef union
{
    UCHAR BYTE;

    struct
    {
        UCHAR VARI :1; //�޹���1���ޱ�־
        UCHAR VARII :1; //�޹���2���ޱ�־
        UCHAR VARIII :1; //�޹���3���ޱ�־ 
        UCHAR VARIV :1; //�޹���2���ޱ�־
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
        UCHAR PHAV :1; //A��ϵ�
        UCHAR PHBV :1; //B��ϵ�
        UCHAR PHCV :1; //C��ϵ�
        UCHAR :1; 
        UCHAR PHAI :1; //A�����
        UCHAR PHBI :1; //B�����
        UCHAR PHCI :1; //C�����
        UCHAR :1;
    }BIT;
}UNIONBRKVIFLG;

typedef union
{
    UCHAR BYTE;

    struct
    {
        UCHAR seqVErr :1; //��ѹ������
        UCHAR seqIErr :1; //����������
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
        UCHAR REINIT :1; //ATT7022X��Ҫ���³�ʼ����־
        UCHAR PULSEDB :1; //�������峣���ӱ���־
        UCHAR ISVLD :1; //ATT7022X��Ч�Ա�־
        UCHAR INITEND :1; //��ʼ����ɱ�־
        UCHAR RDERR :1; //ATT7022x�����ݳ����־
        UCHAR WRERR :1; //д���ݴ���
        UCHAR CDWR :1;
        UCHAR :1;
    }BIT;
}UNIONMSRCHPFLG;

typedef struct 
{
    UCHAR voltage[EMPPBUT][4]; //˲ʱA,B,C���ѹֵ
    UCHAR current[EMPPBUT+1][4]; //˲ʱA,B,C�����ߵ���ֵ
    ULONG volAngl[EMPPBUT]; //˲ʱUab,Uac,Ubc ��ѹ�н�
    ULONG curAngl[EMPPBUT]; //˲ʱA,B,C��������ѹ�н�    
    ULONG factor[EMFIBUT]; //˲ʱ��,A,B,C�๦������        
    ULONG freq; //��ѹƵ��
    UINT pwrDir; //���ʷ���    

    UCHAR watPwr[EMFIBUT][4]; //˲ʱ��,A,B,C���й�����ֵ
    UCHAR varPwr[EMFIBUT][4]; //˲ʱ��,A,B,C���޹�����ֵ
    UCHAR aprPwr[EMFIBUT][4]; //˲ʱ��,A,B,C�����ڹ���ֵ    
}MsrchpInstValue;

typedef struct
{
    ULONG voltage[EMPPBUT]; //A,B,C��ʵʱ��ѹֵ
    ULONG current[EMPPBUT+1]; // A,B,C�༰���ߵ���ʵʱֵ
    int volAngl[EMPPBUT]; //˲ʱA,B,C���ѹ���
    int curAngl[EMPPBUT]; //˲ʱA,B,C��������

    ULONG factor[EMFIBUT]; //��,A,B,C�๦������ֵ                  
    ULONG freq; //����Ƶ��    
    UNIONPWRDIR pwrFlg; //���ʷ����־
    UINT temperature;
    
    long watPwr[EMFIBUT]; //˲ʱ�ܣ�A,B,C���й����ʣ�
    long varPwr[EMFIBUT]; //˲ʱ�ܣ�A,B,C���޹����ʣ�
    long aprPwr[EMFIBUT]; //˲ʱ�ܣ�A,B,C�����ڹ��ʣ�    
}MsrchpRealValue;

typedef struct
{
    UCHAR vPseqCnt; //��ѹ�������д���
    UCHAR iPseqCnt; //�����������д���
    UCHAR sumErrCnt; //У��ͳ������
    UCHAR dmndFlgCnt; //��������仯����
    UNIONMSRCHPFLG msrChpFlg; //����оƬ������־    
    ULONG basicVal; //���ʳ���
    ULONG regCsVal; //�Ĵ���У���ֵ
    UINT pulseConst; //���峣��
    UCHAR buffer[3];
    UCHAR pulseDb[6]; //���峣���ӱ�����    
    UNIONVARFLG varFlg[EMFIBUT]; //�޹������־
    UNIONBRKVIFLG brkFlg; //��ѹ������־
    UNIONPHSSEQFLG pseqFlg; //��������־
    UINT commCalib; //ͨ��У��
}MsrchpVariable;

#pragma pack(1)
typedef struct
{
    UCHAR head; //֡ͷ0x68
    
    UINT pulseConst; //���峣��
    UINT hfConst[2]; //��Ƶ��������Ĵ���
    UCHAR VGain[3][2]; //��ѹ����
    UCHAR IGain[3][2]; //��������
    UINT volRatio; //��ѹϵ��
    UINT curRatio; //����ϵ��    
    UCHAR curStart[2]; //������������
    UCHAR reserve1[2]; //����У������1��������ֵ��
    UCHAR reserve2[2]; //����У������2����ѹ���棩
    UCHAR phsIreg[4][2]; //��λ�ֶβ�����������
    UCHAR PGain[3][2]; //��������
    UCHAR PHSI[4][3][3]; //��λ�ֶ�У��0,1,2,3
    UCHAR PHOS[3][3]; // ͨ������OFFSETУ��    
    UCHAR IrmsOffset[3][3]; //����OFFSETУ��
    ULONG InRegCs; //�ڲ��Ĵ���У���
    ULONG ExtRegCs; //�ⲿ�Ĵ���У���
    
    UCHAR chkSum; //У��������У���
    UCHAR endflg; //У�����ݽ�����
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
    int posWatt[EMPLSBUT]; //A��B��C�������й�������
    int negWatt[EMPLSBUT];
    int var[EMPLSBUT]; //A��B��C�������޹�������
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


