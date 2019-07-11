
#ifndef  __RN8302B_H
#define  __RN8302B_H
  
#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"


#if 1
#define  SPI1_MOSI_PORT        GPIOA
#define  SPI1_CLK_PIN          GPIO_PIN_5
#define  SPI1_MOSI_PIN         GPIO_PIN_7
#define  SPI1_MISO_PIN         GPIO_PIN_6
#define  SPI1_CLK_PORT         GPIOA

#define  SPI1_CS_PIN           GPIO_PIN_4
#define  SPI1_CS_PORT          GPIOA

#define  SPI11_RESET_PIN       GPIO_PIN_5
#define  SPI_INT               GPIO_PIN_4
#define  SPI1_PORT             GPIOC
#else
#define  SPI1_MOSI_PORT        GPIOB
#define  SPI1_CLK_PIN          GPIO_PIN_13
#define  SPI1_MOSI_PIN         GPIO_PIN_15
#define  SPI1_MISO_PIN         GPIO_PIN_14
#define  SPI1_CLK_PORT         GPIOB

#define  SPI1_CS_PIN           GPIO_PIN_12
#define  SPI1_CS_PORT          GPIOB

#define  SPI11_RESET_PIN       GPIO_PIN_12
#define  SPI_INT               GPIO_PIN_4
#define  SPI1_PORT             GPIOC

#endif
//parame

#define	EPA			0x0030
#define	EPB			0x0031
#define	EPC			0x0032
#define	EPT			0x0033
#define	PosEPA		0x0034
#define	PosEPB		0x0035
#define	PosEPC		0x0036
#define	PosEPT		0x0037
#define	NegEPA		0x0038
#define	NegEPB		0x0039
#define	NegEPC		0x003A
#define	NegEPT		0x003B

#define	EQA			0x003C
#define	EQB			0x003D
#define	EQC			0x003E
#define	EQT			0x003F
#define	PosEQA		0x0040
#define	PosEQB		0x0041
#define	PosEQC		0x0042
#define	PosEQT		0x0043
#define	NegEQA		0x0044
#define	NegEQB		0x0045
#define	NegEQC		0x0046
#define	NegEQT		0x0047

#define	ESA			0x0048
#define	ESB			0x0049
#define	ESC			0x004A
#define	EST			0x004B

#define YUA			0x0050
#define YUB			0x0051
#define YUC			0x0052
#define YIA			0x0053
#define YIB			0x0054
#define YIC			0x0055
#define YIN			0x0056

#define	FEPA		0x007A
#define	FEPB		0x007B
#define	FEPC		0x007C
#define	FEPT		0x007D
#define	PosFEPA		0x007E
#define	PosFEPB		0x007F
#define	PosFEPC		0x0080
#define	PosFEPT		0x0081
#define	NegFEPA		0x0082
#define	NegFEPB		0x0083
#define	NegFEPC		0x0084
#define	NegFEPT		0x0085

#define	FEQA		0x0086
#define	FEQB		0x0087
#define	FEQC		0x0088
#define	FEQT		0x0089
#define	PosFEQA		0x008A
#define	PosFEQB		0x008B
#define	PosFEQC		0x008C
#define	PosFEQT		0x008D
#define	NegFEQA		0x008E
#define	NegFEQB		0x008F
#define	NegFEQC		0x0090
#define	NegFEQT		0x0091

#define	FESA		0x0092
#define	FESB		0x0093
#define	FESC		0x0094
#define	FEST		0x0095

#define	HFCONST1	0x0100
#define	HFCONST2	0x0101
#define	IStart_PS	0x0102
#define	ZXOT		0x0105
#define	PRTH1L		0x0106
#define	PRTH1H		0x0107
#define	PRTH2L		0x0108
#define	PRTH2H		0x0109
#define	PHSUA		0x010C
#define	PHSUB		0x010D
#define	PHSUC		0x010E
#define	PHSIA		0x010F
#define	PHSIB		0x0110
#define	PHSIC		0x0111
#define	GSUA		0x0113
#define	GSUB		0x0114
#define	GSUC		0x0115
#define	GSIA		0x0116
#define	GSIB		0x0117
#define	GSIC		0x0118
#define	GSIN		0x0119
#define	DCOS_UA		0x011A
#define	DCOS_UB		0x011B
#define	DCOS_UC		0x011C
#define	GPA			0x0128
#define	GPB			0x0129
#define GPC			0x012A
#define	GQA			0x012B
#define	GQB			0x012C
#define	GQC			0x012D
#define	GSA			0x012E
#define	GSB			0x012F
#define	GSC			0x0130
#define	PA_PHS		0x0131
#define	PB_PHS		0x0132
#define	PC_PHS		0x0133
#define	PA_OS		0x0137
#define	PB_OS		0x0138
#define	PC_OS		0x0139

#define GFPA		0X0143
#define GFPB		0X0144
#define	GFPC		0X0145

#define	GFSA		0x0149
#define	GFSB		0x014A
#define	GFSC		0x014B

#define	CFCFG		      0x0160
#define EMUCON            0x0162
#define	PQSign		      0x0166
#define	CheckSum1	      0x016A
#define	WMSW		      0x0181
#define	DeviceID	      0x018f
#define WREN              0x0180
#define SOFTRST           0x0182
#define QA_PHS            0x0134
#define QB_PHS            0x0135
#define QC_PHS            0x0136
#define IA_OS             0x0124
#define IB_OS             0x0125
#define IC_OS             0x0126
#define IStart_Q          0x0103
#define LostVoltT         0x0104
#define EMUCFG            0x0161
#define WSAVECON          0x0163
#define MODESEL           0x0186

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


//private
typedef unsigned char uint8_t;


typedef struct
{

float  vol[3];

float  cur[3];

unsigned char overvol[3];
unsigned char missvol[3];
unsigned char misscur;

float power[3];

unsigned int temper;
unsigned int alm;
unsigned int powercnt;
}RN8302_Info;
extern RN8302_Info gRN_Info;

char RN_ReadDeviceID(void);
void Test_delay(void);
void RN_8302B_HardwareInit(void);

//public
void RN830B_SoftInit(void);
char fnRN8302_Init(void);
void fnDl645Energy_Exec(void);
void fnDl645Front_Exec(void);
void RN8302B_DCOffset(void);
void ReadAvol(void);
void RN8302_Loop(void);
void RN8302_ReadID(void);
uint8_t RN8302_CalibrateInit(void);
uint16_t RN8302_CalibrateUGain(uint16_t vol,uint8_t phase);
void RN8302_Systick(void);
void TestDevice(void);

#ifdef __cplusplus
	}
#endif

#endif
