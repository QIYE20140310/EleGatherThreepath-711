/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "..\user\includes.h"

#if 1
//#define ADDR_OF_ATT_CAL_INFO 0
//#define FMETERTYPE           DTSD
//#define FCALIBRATE           1
//#define ADDR_OF_ATT_CAL_DATA 0

UCHAR LineBuffer[256];
#endif


/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
UCHAR CaliEnableCnt = 0;
ULONG g_fastPulseCnt[EMFIBUT] = {0,0,0,0};
ULONG g_atmelCurrent[EMPPBUT] = {0,0,0};

const UCHAR MSRVoltageRegAddr[][3] = {
    {0x07,0x08,0x09},
    {0xD9,0xDA,0xDB},
    {0x0D,0x0E,0x0F}
};
const UCHAR MSRVoltageRegLen[] = {4,2,3}; 

const UCHAR MSRCurrentRegAddr[][4] = {
    {0x0B,0x0C,0x0D,0x0E},
    {0xDD,0xDE,0xDF,0xDC},
    {0x10,0x11,0x12,0x13},
};
const UCHAR MSRCurrentRegLen[] = {4,2,3};

const UCHAR MSRActPwrRegAddr[][4] = {
    {0x17,0x14,0x15,0x16},
    {0xB0,0xB1,0xB2,0xB3},
    {0x04,0x01,0x02,0x03},
};
const UCHAR MSRActPwrRegLen[] = {4,2,3};

const UCHAR MSRReActPwrRegAddr[][4] = {
    {0x1B,0x18,0x19,0x1A},
    {0xB4,0xB5,0xB6,0xB7},
    {0x08,0x05,0x06,0x07},
};
const UCHAR MSRReActPwrRegLen[] = {4,2,3};

const UCHAR MSRAprPwrRegAddr[][4] = {
    {0x1F,0x1C,0x1D,0x1E},
    {0xB8,0xB9,0xBA,0xBB},
    {0x0C,0x09,0x0A,0x0B},
};
const UCHAR MSRAprPwrRegLen[] = {4,2,3};

const UCHAR MSRPwrFactorRegAddr[][4] = {
    {0x23,0x20,0x21,0x22},
    {0xBC,0xBD,0xBE,0xBF},
    {0x17,0x14,0x15,0x16},
};
const UCHAR MSRPwrFactorRegLen[] = {3,2,3};

const UCHAR MSRVoltageAngleRegAddr[][3] = {
    {0x50,0x51,0x52},
    {0xFD,0xFE,0xFF},
    {0x26,0x27,0x28},
};    
const UCHAR MSRVoltageAngleRegLen[] = {3,2,3};

const UCHAR MSRCurrentAngleRegAddr[][3] = {
    {0x53,0x54,0x55},
    {0xF9,0xFA,0xFB},
    {0x18,0x19,0x1A},
};
const UCHAR MSRCurrentAngleRegLen[] = {3,2,3};

const UCHAR MSRPwrFreqRegAddr[] = {0x57,0xF8,0x1C};
const UCHAR MSRPwrFreqRegLen[] = {3,2,3};

const UINT MSRPwrDirRegAddr[] = {0x0166,0x0000,0x003D};
const UINT MSRPhaseSeqRegAddr[] = {0x0169,0x0071,0x002C};

const UINT MSRChecksumRegAddr1[] = {0x016A,0x02FA,0x005E};
const UINT MSRChecksumRegAddr2[] = {0x016A,0x007A,0x003E};
const UCHAR MSRChecksumRegLen[] = {3,2,3};

const UCHAR MSRPosActEngRegAddr[][4] = {
    {0x37,0x34,0x35,0x36},
    {0x80,0x81,0x82,0x83},
    {0x21,0x1E,0x1F,0x20},
};

const UCHAR MSRNegActEngRegAddr[][4] = {
    {0x3B,0x38,0x39,0x3A,},
    {0x84,0x85,0x86,0x87,},
    {0x21,0x1E,0x1F,0x20},
};

const UCHAR MSRPosReActEngRegAddr[][4] = {
    {0x43,0x40,0x41,0x42},
    {0x88,0x89,0x8A,0x8B},
    {0x25,0x22,0x23,0x24},
};

const UCHAR MSRNegReActEngRegAddr[][4] = {
    {0x47,0x44,0x45,0x46},
    {0x8C,0x8D,0x8E,0x8F},
    {0x25,0x22,0x23,0x24},
};

const UCHAR MSRFastCntRegAddr[4] = {0x3C,0x39,0x3A,0x3B};

const UINT MSRWREnbleRegAddr[] = {0x0180,0x007F,0x00C9};
const UCHAR MSRWREnbleRegLen[] = {1,2,3};
const UCHAR MSREnergyRegLen[] = {3,2,3};
const UINT MSREnergyRegMaxValue[] = {127,12700,127};
const UINT MSRSoftResetRegAddr[] = {0x0182,0x0070,0x00D3};
const ULONG MSRSoftResetRegValue[] = {0x000000FA,0x0000789A,0x00000000};
const UCHAR MSRSoftResetRegLen[] = {0x01,0x02,0x03};
const UINT VolPhaseSeqFlag[] = {BIT3,BIT9,BIT3};
const UINT CurPhaseSeqFlag[] = {BIT3,BIT8,BIT4};
const UCHAR MSRPhaseSeqRegLen[] = {2,2,3};
const UINT MSRDeviceRegAddr[] = {0x018f,0x02F0,0x0000};
const UCHAR MSRDeviceRegALen[] = {0x03,0x02,0x03};
const UCHAR MSRCalibrateRegLen[] = {2,2,3};
const UINT MSRHFConstRegAddr[][2] = {
    {0x0100,0x0100},
    {0x0031,0x0032},
    {0x001E,0x0033},
};

const UINT MSRIStartRegAddr[] = {0x0102,0x0035,0x001D};
const UINT MSRReserveRegAddr[][3] = {
    {0x0103,0x0103,0x0103},
    {0x001A,0x001B,0x001B},
    {0x0002,0x0002,0x0002},
};

const UINT MSRPrthIregionRegAddr[][4] = {
    {0x0106,0x0107,0x0108,0x0109},
    {0x0020,0x0021,0x0021,0x0021},
    {0x0037,0x0060,0x0060,0x0060},
};
const UCHAR MSRPrthIregionItem[] = {4,2,2};

const UINT MSRGainRegAddr[][5][3] = {//增益校正
    {
       {0x0113,0x0114,0x0115}, //电压增益
       {0x0116,0x0117,0x0118}, //电流增益
       {0x0128,0x0129,0x012A}, //有功功率增益
       {0x012B,0x012C,0x012D}, //无功功率增益
       {0x012E,0x012F,0x0130}, //视在功率增益
    },
    
    {
       {0x0061,0x0065,0x0069},
       {0x0062,0x0066,0x006A},
       {0x0047,0x0049,0x004B},
       {0x0047,0x0049,0x004B},
       {0x0047,0x0049,0x004B},       
    },
    
    {
       {0x0017,0x0018,0x0019},
       {0x001A,0x001B,0x001C},
       {0x0004,0x0005,0x0006},
       {0x0007,0x0008,0x0009},
       {0x000A,0x000B,0x000C},
    },
};

const UCHAR MSRPhsiRegLen[] = {3,2,3};
const UINT MSRPhsiRegAddr[][3][4] = {//相位校正
    {
       {0x010F,0x010F,0x010F,0x010F},
       {0x0110,0x0110,0x0110,0x0110},
       {0x0111,0x0111,0x0111,0x0111},              
    },
    
    {
        {0x0048,0x0024,0x0025,0x0025},
        {0x004A,0x0028,0x0029,0x0029},
        {0x004C,0x002C,0x002D,0x002D},        
    },
    
    {
        {0x000D,0x0010,0x0061,0x0061},
        {0x000E,0x0011,0x0062,0x0062},
        {0x000F,0x0012,0x0063,0x0063},
    },
};
const UCHAR MSRPhsiRegItem[] = {1,3,3};

const UINT MSRPOffsetRegAddr[][3] = {//有功功率Offset校正
    {0x0137,0x0138,0x0139},
    {0x0041,0x0043,0x0045},
    {0x0013,0x0014,0x0015},
};

const UINT MSRQOffsetRegAddr[][3] = {//无功功率Offset校正
    {0x013A,0x013B,0x013C},
    {0x0042,0x0044,0x0046},
    {0x0021,0x0022,0x0023},
};
const UCHAR MSRPOffsetRegLen[] = {2,3,3};

const UINT MSRIrmsOffsetRegAddr[][3] = {
    {0x0124,0x0125,0x0126},
    {0x0064,0x0068,0x006C},
    {0x0027,0x0028,0x0029},
};
const UCHAR MSRIrmsOffsetRegLen[] = {2,2,3};

const MsrchpCalData MSRCalibrateInitData[] = {
#if(FMSRTYPE == MSRRN8302)
    {
        0x68,
        0x0028,//脉冲常数
        0x08D2,//高频脉冲寄存器
        0x1000,//
        0x00,0x00,0x00,0x00,0x00,0x00,//电压增益
        0x00,0x00,0x00,0x00,0x00,0x00,//电流增益
        0x020E,//电压系数
        0x014F,//电流系数
        0x91,0x02,//启动电流
        0x20,0x00,//过零阈值
        0x2A,0x00,//adc增益
        0xAC,0x00,0xCD,0x00,0x3D,0x06,0x8F,0x06,
        0x00,0x00,0x00,0x00,0x00,0x00,//功率增益
        0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//相位分段补偿区域设置
        0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
        0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
        0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//通道功率OFFSET校正
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//电流OFFSET校正
        0x00000000,
        0x00000000,
        0x00,
        0x16
    },    
#elif(FMSRTYPE == MSR90E32AS)  
    {
        0x68,
        0x0028,
        0x2187,
        0x11A0,
        0xF2,0xC8,0x29,0xC9,0xA9,0xC9,
        0x26,0x97,0x49,0x97,0x3D,0x97,
        0x0001,
        0x0001,
        0x24,0x14,
        0x00,0x00,
        0x00,0x00,        
        0xC5,0x00,0x8F,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,
        0x10,0x00,0x00,0x0F,0x00,0x00,0x13,0x00,0x00,
        0xD7,0x00,0x00,0xDD,0x00,0x00,0xD2,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00000000,
        0x00000000,
        0x56,
        0x16
    },
#elif(FMSRTYPE == MSRATT7022E)
    {
        0x68,
        0x0028,
        0x015E,
        0x0000,
        0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,
        0x0001,
        0x0001,
        0x80,0x02,
        0x00,0x00,
        0x00,0x00,
        
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,        
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00000000,
        0x00000000,
        0x31,
        0x16
    }, 
#endif
};

const MsrchpCalInfo MSRCalibrateInfo[] = {
    0x68,
    0x0000,
    0x00,0x00,0x18,0x12,0x12,0x13,
    0x0C,
    0x16
};

void MSRCVarInit(void)
{
    stuAttObj *p = (void *)&attObj;

    p->var.brkFlg.BYTE = 0;
    p->var.pseqFlg.BYTE = 0;
    p->var.vPseqCnt = 0;
    p->var.iPseqCnt = 0;
    p->var.dmndFlgCnt = 0;
    p->var.sumErrCnt = 0;
    p->var.msrChpFlg.BYTE = 0;
    p->var.commCalib = 0;
}

void MSRCRealValueInit(void)
{
    stuAttObj *p = (void *)&attObj;
    
    p->var.brkFlg.BYTE = 0;
    p->var.pseqFlg.BYTE = 0;
    p->var.varFlg[EMFPHT].BYTE = 0;
    p->var.varFlg[EMFPHA].BYTE = 0;
    p->var.varFlg[EMFPHB].BYTE = 0;
    p->var.varFlg[EMFPHC].BYTE = 0;
    MemSetZero((UCHAR *)&p->realVal.voltage[EMPPHA], sizeof(MsrchpRealValue));
    MemSetZero((UCHAR *)&p->instVal.voltage[EMPPHA], sizeof(MsrchpInstValue));
}

void MSRCSoftReset(void)
{
    ULONG tempulong;

    tempulong = MSRSoftResetRegValue[FMSRTYPE];
    MSRCWriteData[FMSRTYPE](MSRSoftResetRegAddr[FMSRTYPE], (UCHAR *)&tempulong, MSRSoftResetRegLen[FMSRTYPE]);
    Delay100ms();
}


#if (FMSRTYPE == MSR90E32AS)
void AT90E32InRegWRSelect(MSRCWRSelect item)
{
    UCHAR buf[3];

    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0x00;
    if(item == ENABLE)
    {
        buf[0] = 0xAA;
        buf[1] = 0x55;
    }
    else
    {
        buf[0] = 0x34;
        buf[1] = 0x12;
    }
    MSRCWriteData[FMSRTYPE](0x2FF, (UCHAR *)&buf[0], 2);
}
#endif

ULONG CheckMinus(UCHAR *p)
{
    ULONG temp = 0;

    #if(FMSRTYPE != MSRATT7022E)
    {
        memcpy((UCHAR *)&temp, p, 4);
        if(*(p+3) & 0x80)
        {
            memcpy((UCHAR *)&temp, p, 4);
            temp = (~temp+1);
        }
    }
    #else
    {
        memcpy((UCHAR *)&temp, p, 3);
        if(*(p+2) & 0x80)
        {
            temp = 16777216-temp;
        }
    }
	#endif

    if(temp < 5)
    {
        temp = 0;
    }
    return temp;
}

ULONG CalcFactor(UCHAR *p) //功率因数
{
    ULONG temp = 0;

    memcpy((UCHAR *)&temp, p, 3);
    if(temp & 0x00800000)
    {
        temp = 16777216-temp;
    }
    temp = temp>>8;
    temp = temp*10000/32768;    
    if(temp > 10000)
    {
        temp = 10000;
    }    
    return temp;
}

#if (FMSRTYPE == MSRATT7022E)
ULONG CalcATT7022SumPower(UCHAR *p)
{
    ULONG temp = 0;
    double temp1;

    temp = CheckMinus(p);
    temp1 = ((double)temp/(attObj.var.basicVal*attObj.calData.hfConst[0]))*61798;
    temp = (ULONG)temp1;
    if((temp1-temp) >= 0.5)
    {
        if(temp != 0)
        {
            temp += 1;
        }
    }
    
    if(temp < 5)
    {
        temp = 0;
    }
    
    return temp;
}

ULONG CalcATT7022PhasePower(UCHAR *p)
{
    ULONG temp = 0;
    double temp1;
    
    temp = CheckMinus(p);
    temp1 = ((double)temp/(attObj.var.basicVal*attObj.calData.hfConst[0]))*30899;
    if(temp1 < 5)
    {
        temp1 = 0;
    }
    
    return temp1;
}

ULONG CalcATT7022Voltage(UCHAR *p)
{
    UINT tempuint;
    ULONG tempulong = 0;
    ULONG tempulong1 = 0;
    UCHAR *p1;

    memcpy((UCHAR *)&tempuint, p+1, 2);
    if(tempuint < 32768)
    {
        tempulong = (ULONG)tempuint*0x5000;
        p1 = (UCHAR *)&tempulong;
        p1 = p1+2;
        memcpy((UCHAR *)&tempulong1, p1, 2);
        tempulong = tempulong1*1000;
    }
    
    return tempulong;
}

ULONG CalcATT7022Current(UCHAR *p)
{
    float f1, FLI0;
    ULONG tempulong = 0;

    memcpy((UCHAR *)&tempulong, p, 3);
    f1 = (float)tempulong/8192.0;
    f1 *= 10000;
    if(funcCfgObj.ratedIb ==50)
    {
        FLI0 = 6.0;
    }
    else if(funcCfgObj.ratedIb == 15)
    {
        FLI0 = 40.0;
    }
    f1 /= FLI0;
    tempulong = (ULONG)f1;
    
    return tempulong;
}
#endif

void MSRCReadEnergy(void)
{
    UCHAR i;
    ULONG pulseCnt;
    ULONG fastCnt;
    stuAttObj *p = (void *)&attObj;
    
    if(p->var.msrChpFlg.BIT.REINIT)
    {
        return;
    }
    
    #if(FMSRTYPE != MSR90E32AS)
    {
        p->realVal.pwrFlg.BYTE = 0; //读功率方向
        MSRCReadData[FMSRTYPE](MSRPwrDirRegAddr[FMSRTYPE], (UCHAR *)&p->instVal.pwrDir, 2);
        p->realVal.pwrFlg.BYTE = (UCHAR)(p->instVal.pwrDir & 0x77);
        p->realVal.pwrFlg.BYTE <<= 1;
        if(p->instVal.pwrDir & BIT3)
        {
            p->realVal.pwrFlg.BIT.WPHT = 1;
        }
        else
        {
            p->realVal.pwrFlg.BIT.WPHT = 0;
        }

        if(p->instVal.pwrDir & BIT7)
        {
            p->realVal.pwrFlg.BIT.VPHT = 1;
        }
        else
        {
            p->realVal.pwrFlg.BIT.VPHT = 0;
        }
    }
    #else
    {
        p->realVal.pwrFlg.BYTE = (UCHAR)p->instVal.pwrDir;
    }
	#endif
    p->realVal.pwrFlg.BIT.WPHA &= p->var.brkFlg.BIT.PHAV;
    p->realVal.pwrFlg.BIT.WPHA &= p->var.brkFlg.BIT.PHAI;
    if(FMETERTYPE == DTSD)
    {
        p->realVal.pwrFlg.BIT.WPHB &= p->var.brkFlg.BIT.PHBV;
        p->realVal.pwrFlg.BIT.WPHB &= p->var.brkFlg.BIT.PHBI;
    }
    else
    {
        p->realVal.pwrFlg.BIT.WPHB = 0;
    }
    p->realVal.pwrFlg.BIT.WPHC &= p->var.brkFlg.BIT.PHCV;
    p->realVal.pwrFlg.BIT.WPHC &= p->var.brkFlg.BIT.PHCI;
    p->realVal.pwrFlg.BIT.WPHT &= (p->realVal.pwrFlg.BIT.WPHA|p->realVal.pwrFlg.BIT.WPHB|p->realVal.pwrFlg.BIT.WPHC);

    p->realVal.pwrFlg.BIT.VPHA &= p->var.brkFlg.BIT.PHAV;
    p->realVal.pwrFlg.BIT.VPHA &= p->var.brkFlg.BIT.PHAI;
    if(FMETERTYPE == DTSD)
    {
        p->realVal.pwrFlg.BIT.VPHB &= p->var.brkFlg.BIT.PHBV;
        p->realVal.pwrFlg.BIT.VPHB &= p->var.brkFlg.BIT.PHBI;
    }
    else
    {
        p->realVal.pwrFlg.BIT.VPHB = 0;
    }
    p->realVal.pwrFlg.BIT.VPHC &= p->var.brkFlg.BIT.PHCV;
    p->realVal.pwrFlg.BIT.VPHC &= p->var.brkFlg.BIT.PHCI;
    p->realVal.pwrFlg.BIT.VPHT &= (p->realVal.pwrFlg.BIT.VPHA|p->realVal.pwrFlg.BIT.VPHB|p->realVal.pwrFlg.BIT.VPHC);
#ifdef DEMAND
	if(p->realVal.pwrFlg.BIT.WPHT != dmndObj.var.dir.BIT.wattDir)
    {
        if(p->var.dmndFlgCnt < 40)
        {
            p->var.dmndFlgCnt++;
        }
        else
        {
            dmndObj.var.dir.BIT.wattDir = p->realVal.pwrFlg.BIT.WPHT;
        }
    }
    else
    {
        p->var.dmndFlgCnt = 0;
    }
#endif
    
    MemSetZero((UCHAR *)&p->Pulse.posWatt[0], sizeof(MsrchpPulse));  
    for(i = 0; i < 4; i++)
    {
        WDTRestart();
        fastCnt = 0;
        pulseCnt = 0;
        #if(FMSRTYPE == MSRATT7022E)
        {
            if(MSRCReadData[FMSRTYPE](MSRPosActEngRegAddr[FMSRTYPE][i], (UCHAR *)&pulseCnt, MSREnergyRegLen[FMSRTYPE]) == OK)
            {
                if(pulseCnt > MSREnergyRegMaxValue[FMSRTYPE])
                {
                    pulseCnt = 0;
                }
            }

            if(pulseCnt >= 1)
            {
                if(g_fastPulseCnt[i] == 0)
                {
                    if(p->realVal.pwrFlg.BYTE & (BIT_WPWR_PHT << i))
                    {
                        p->Pulse.negWatt[i] = (UINT)pulseCnt;
                    }
                    else
                    {
                        p->Pulse.posWatt[i] = (UINT)pulseCnt;
                    }
                }
                else
                {
                    if(g_fastPulseCnt[i] & 0x800000)
                    {
                        p->Pulse.negWatt[i] = (UINT)pulseCnt;
                    }
                    else
                    {
                        p->Pulse.posWatt[i] = (UINT)pulseCnt;
                    }
                }
            }

            if(MSRCReadData[FMSRTYPE](MSRFastCntRegAddr[i], (UCHAR *)&fastCnt, 3) == OK)
            {
                g_fastPulseCnt[i] = fastCnt;
            }
        }
        #else
        {
            if(p->realVal.pwrFlg.BYTE & (BIT_WPWR_PHT << i))
            {
                if(MSRCReadData[FMSRTYPE](MSRNegActEngRegAddr[FMSRTYPE][i], (UCHAR *)&pulseCnt, MSREnergyRegLen[FMSRTYPE]) == OK)
                {
                    if(pulseCnt > MSREnergyRegMaxValue[FMSRTYPE])
                    {
                        pulseCnt = 0;
                    }
                    p->Pulse.negWatt[i] = (UINT)pulseCnt;
                }

                if(MSRCReadData[FMSRTYPE](MSRPosActEngRegAddr[FMSRTYPE][i], (UCHAR *)&pulseCnt, MSREnergyRegLen[FMSRTYPE]) == OK)
                {
                    if(pulseCnt > MSREnergyRegMaxValue[FMSRTYPE])
                    {
                        pulseCnt = 0;
                    }
                    p->Pulse.posWatt[i] = (UINT)pulseCnt;
                }

                if(p->Pulse.posWatt[i] >= p->Pulse.negWatt[i])
                {
                    p->Pulse.posWatt[i] += p->Pulse.negWatt[i];
                    p->Pulse.negWatt[i] = 0;
                }
            }
            else
            {
                if(MSRCReadData[FMSRTYPE](MSRPosActEngRegAddr[FMSRTYPE][i], (UCHAR *)&pulseCnt, MSREnergyRegLen[FMSRTYPE]) == OK)
                {
                    if(pulseCnt > MSREnergyRegMaxValue[FMSRTYPE])
                    {
                        pulseCnt = 0;
                    }
                    p->Pulse.posWatt[i] = (UINT)pulseCnt;
                }
            }
        }
		#endif
        
        if(p->realVal.pwrFlg.BYTE & (BIT_VPWR_PHT << i))
        {
            if(MSRCReadData[FMSRTYPE](MSRNegReActEngRegAddr[FMSRTYPE][i], (UCHAR *)&pulseCnt, MSREnergyRegLen[FMSRTYPE]) == OK)
            {
                if(pulseCnt > MSREnergyRegMaxValue[FMSRTYPE])
                {
                    pulseCnt = 0;
                }
                p->Pulse.var[i] = (UINT)pulseCnt;
            }
        }
        else
        {
            if(MSRCReadData[FMSRTYPE](MSRPosReActEngRegAddr[FMSRTYPE][i], (UCHAR *)&pulseCnt, MSREnergyRegLen[FMSRTYPE]) == OK)
            {
                if(pulseCnt > MSREnergyRegMaxValue[FMSRTYPE])
                {
                    pulseCnt = 0;
                }
                p->Pulse.var[i] = (UINT)pulseCnt;
            }
        }
    }           
    
    #if(FMSRTYPE == MSR90E32AS)
    {
        dmndObj.pulse.var += p->Pulse.var[EMPLSSUM];
        dmndObj.pulse.watt += (p->Pulse.posWatt[EMPLSSUM]+p->Pulse.negWatt[EMPLSSUM]);        
    }  
	#endif
}

void RN8302IPSEQCheck(void)
{
    UINT tempuint;
    stuAttObj *p = (void *)&attObj;
    
    if(p->realVal.curAngl[EMPPHA] > p->realVal.curAngl[EMPPHC])
    {
        tempuint = 3600-(p->realVal.curAngl[EMPPHA]-p->realVal.curAngl[EMPPHC]);
    }
    else
    {
        tempuint = p->realVal.curAngl[EMPPHC] - p->realVal.curAngl[EMPPHA];
    }
    if((tempuint > 1000) && (tempuint < 1400))
    {
        p->var.pseqFlg.BIT.seqIErr = 1;
        p->var.pseqFlg.BIT.seqIErr &= p->var.brkFlg.BIT.PHAI;
        if(FMETERTYPE == DTSD)
        {
            p->var.pseqFlg.BIT.seqIErr &= p->var.brkFlg.BIT.PHBI;
        }
        p->var.pseqFlg.BIT.seqIErr &= p->var.brkFlg.BIT.PHCI;
    }
    else
    {
        p->var.pseqFlg.BIT.seqIErr = 0;
    }     
}

void CalcRealTimeValue(void)
{
    UCHAR i;
    int tempint;
    float f1,f2,f3;
    ULONG startPwr, minVol;
    stuAttObj *p = (void *)&attObj;
    
    //计算启动功率
    startPwr = (((ULONG)funcCfgObj.ratedUb*(ULONG)funcCfgObj.ratedIb)/10000);    
    for(i = EMPPHA; i < EMPPBUT;)
    {
        WDTRestart();
        apdApp.u32Buffer[0] = (*(ULONG *)&p->instVal.voltage[i][0]);
        apdApp.u32Buffer[1] = (*(ULONG *)&p->instVal.current[i][0]);
        #if(FMSRTYPE == MSRRN8302)
        {
            p->realVal.voltage[i] = (apdApp.u32Buffer[0]*100)/p->calData.volRatio;
            p->realVal.current[i] = (apdApp.u32Buffer[1]*10)/p->calData.curRatio;
            f1 = 8388608.0/((ULONG)p->calData.volRatio*p->calData.curRatio);
            p->realVal.watPwr[i+EMFPHA] = (ULONG)(((float)CheckMinus((UCHAR *)&p->instVal.watPwr[i+EMFPHA][0])/10000.0)*f1);
            p->realVal.varPwr[i+EMFPHA] = (ULONG)(((float)CheckMinus((UCHAR *)&p->instVal.varPwr[i+EMFPHA][0])/10000.0)*f1);
            p->realVal.aprPwr[i+EMFPHA] = (ULONG)(((float)CheckMinus((UCHAR *)&p->instVal.aprPwr[i+EMFPHA][0])/10000.0)*f1);
            p->realVal.factor[EMFPHA+i] = CalcFactor((UCHAR *)&p->instVal.factor[EMFPHA+i]);
            p->realVal.volAngl[i] = (UINT)(((float)p->instVal.volAngl[i]/16777216)*3600);
            p->realVal.curAngl[i] = (UINT)(((float)p->instVal.curAngl[i]/16777216)*3600);
            RN8302IPSEQCheck();
            if(p->realVal.curAngl[i] < p->realVal.volAngl[i])
            {
                p->realVal.curAngl[i] = 3600+p->realVal.curAngl[i]-p->realVal.volAngl[i];
            }
            else
            {
                p->realVal.curAngl[i] -= p->realVal.volAngl[i];
            }
            p->realVal.curAngl[i] = p->realVal.curAngl[i]%3600;
        }
        #elif(FMSRTYPE == MSR90E32AS)
        {
            p->realVal.voltage[i] = (apdApp.u32Buffer[0]>>16)*100;
            apdApp.u32Buffer[3] = (apdApp.u32Buffer[1]>>16)*10*IDT_MULT;
            p->realVal.current[i] = (g_atmelCurrent[i] < apdApp.u32Buffer[3])?g_atmelCurrent[i]:apdApp.u32Buffer[3];
            g_atmelCurrent[i] = apdApp.u32Buffer[3];
            p->realVal.watPwr[i+EMFPHA] = (ULONG)((float)CheckMinus((UCHAR *)&p->instVal.watPwr[i+EMFPHA][0])*POWER_RATIO+0.5);
            p->realVal.varPwr[i+EMFPHA] = (ULONG)((float)CheckMinus((UCHAR *)&p->instVal.varPwr[i+EMFPHA][0])*POWER_RATIO+0.5);
            p->realVal.aprPwr[i+EMFPHA] = (ULONG)((float)CheckMinus((UCHAR *)&p->instVal.aprPwr[i+EMFPHA][0])*POWER_RATIO+0.5);
            p->realVal.factor[i+EMFPHA] = p->instVal.factor[i+EMFPHA]*10;
            tempint = *((int *)&p->instVal.volAngl[i]);
            if(tempint < 0)
            {
                tempint += 3600;
            }
            p->realVal.volAngl[i] = tempint;
            tempint = *((int *)&p->instVal.curAngl[i]);
            if(tempint < 0)
            {
                tempint += 3600;
            }
            p->realVal.curAngl[i] = tempint;
        }
        #else
        {
            p->realVal.voltage[i] = CalcATT7022Voltage((UCHAR *)&p->instVal.voltage[i][0]);
            p->realVal.current[i] = CalcATT7022Current((UCHAR *)&p->instVal.current[i][0]);
            p->realVal.watPwr[EMFPHA+i] = CalcATT7022PhasePower((UCHAR *)&p->instVal.watPwr[EMFPHA+i][0]);
            p->realVal.varPwr[EMFPHA+i] = CalcATT7022PhasePower((UCHAR *)&p->instVal.varPwr[EMFPHA+i][0]);
            p->realVal.aprPwr[EMFPHA+i] = CalcATT7022PhasePower((UCHAR *)&p->instVal.aprPwr[EMFPHA+i][0]);
            p->realVal.factor[EMFPHA+i] = CalcFactor((UCHAR *)&p->instVal.factor[EMFPHA+i]);
            if(p->instVal.curAngl[i] >= 1048576)
            {
                p->instVal.curAngl[i] = 16777216-p->instVal.curAngl[i];
            }
            f1 = (float)p->instVal.curAngl[i]/1048576;
            p->realVal.curAngl[i] = (UINT)(f1*1800);
            if(p->realVal.pwrFlg.BYTE & (BIT_VPWR_PHA << i))
            {
                p->realVal.curAngl[i] = 3600-p->realVal.curAngl[i];
            }
        }
		#endif

        minVol = (ULONG)((float)funcCfgObj.ratedUb*45.5);
        if((p->realVal.voltage[i] >= 8388607) || (p->realVal.voltage[i] < minVol))
        {
            p->realVal.voltage[i] = 0;
            p->var.brkFlg.BYTE &= (UCHAR)~(BIT_BRKV_PHA << i);
        }
        else
        {
            p->var.brkFlg.BYTE |= (UCHAR)(BIT_BRKV_PHA << i);
        }

        if((p->realVal.current[i] < funcCfgObj.startI) || (p->realVal.current[i] >= 8388607))
        {
            p->realVal.current[i] = 0;
            p->var.brkFlg.BYTE &=(UCHAR)~(BIT_BRKI_PHA << i);
        }
        else
        {
            p->var.brkFlg.BYTE |= (UCHAR)(BIT_BRKI_PHA << i);
        }

        if(p->realVal.aprPwr[i+EMFPHA] < startPwr)
        {
            p->realVal.watPwr[i+EMFPHA] = 0;
            p->realVal.varPwr[i+EMFPHA] = 0;
            p->realVal.aprPwr[i+EMFPHA] = 0;
        }

        if(!(p->var.brkFlg.BYTE & (BIT_BRKI_PHA << i)))
        {
            p->realVal.factor[EMFPHA+i] = 10000;
        }

        if(!(p->var.brkFlg.BYTE & (BIT_BRKV_PHA << i)))
        {
            p->realVal.factor[EMFPHA+i] = 10000;
        }

        if(p->realVal.current[i] < funcCfgObj.startI)
        {
            p->realVal.curAngl[i] = 0;
        }
        ADD_I(i);
    }

    #if(FMSRTYPE == MSRRN8302)
    {
        p->realVal.watPwr[EMFPHT] = (ULONG)(((float)CheckMinus((UCHAR *)&p->instVal.watPwr[EMFPHT][0])/10000.0)*f1);
        p->realVal.varPwr[EMFPHT] = (ULONG)(((float)CheckMinus((UCHAR *)&p->instVal.varPwr[EMFPHT][0])/10000.0)*f1);
        p->realVal.aprPwr[EMFPHT] = (ULONG)(((float)CheckMinus((UCHAR *)&p->instVal.aprPwr[EMFPHT][0])/10000.0)*f1);    
        f1 = (float)p->realVal.watPwr[EMFPHT]/100.0;
        f2 = (float)p->realVal.varPwr[EMFPHT]/100.0;
        f3 = sqrt(f1*f1+f2*f2);
        if((f3*100) < startPwr)
        {
            p->realVal.factor[EMFPHT] = 10000;
        }
        else
        {
            p->realVal.factor[EMFPHT] = (ULONG)(((float)p->realVal.watPwr[EMFPHT]*100.0)/f3);
        }
        if(p->var.brkFlg.BYTE & 0x07)
        {
            if((p->instVal.freq > 1638400) || (p->instVal.freq < 1092266))
            {
                p->instVal.freq = 1310720;
            }
            p->realVal.freq = (UINT)(65536000/(p->instVal.freq/10));
        }
    }
    #elif(FMSRTYPE == MSR90E32AS)
    {
        p->realVal.watPwr[EMFPHT] = (ULONG)((float)CheckMinus((UCHAR *)&p->instVal.watPwr[EMFPHT][0])*POWER_RATIO+0.5);
        p->realVal.varPwr[EMFPHT] = (ULONG)((float)CheckMinus((UCHAR *)&p->instVal.varPwr[EMFPHT][0])*POWER_RATIO+0.5);
        f1 = (float)p->realVal.watPwr[EMFPHT]/100.0;
        f2 = (float)p->realVal.varPwr[EMFPHT]/100.0;
        f3 = sqrt(f1*f1+f2*f2);
        p->realVal.aprPwr[EMFPHT] = (ULONG)(f3*100);
        if((f3*100) < startPwr)
        {
            p->realVal.factor[EMFPHT] = 10000;
        }
        else
        {
            p->realVal.factor[EMFPHT] = (ULONG)(((float)p->realVal.watPwr[EMFPHT]*100.0)/f3);
        }
        
        if(p->var.brkFlg.BYTE & 0x07)
        {
            p->realVal.freq = p->instVal.freq/10;
        }
    }
    #else
    {
        p->realVal.watPwr[EMFPHT] = CalcATT7022SumPower((UCHAR *)&p->instVal.watPwr[EMFPHT][0]);
        p->realVal.varPwr[EMFPHT] = CalcATT7022SumPower((UCHAR *)&p->instVal.varPwr[EMFPHT][0]);
        p->realVal.aprPwr[EMFPHT] = CalcATT7022SumPower((UCHAR *)&p->instVal.aprPwr[EMFPHT][0]);
        p->realVal.factor[EMFPHT] = CalcFactor((UCHAR *)&p->instVal.factor[EMFPHT]);
        if(p->var.brkFlg.BYTE & 0x07)
        {
            p->realVal.freq = ((p->instVal.freq*10)>>13);
        }
    }
	#endif

    if(p->realVal.factor[EMFPHT] > 10000)
    {
        p->realVal.factor[EMFPHT] = 10000;
    }
    
    if((p->realVal.current[EMPPHA] < funcCfgObj.startI) &&
        (p->realVal.current[EMPPHB] < funcCfgObj.startI) &&
        (p->realVal.current[EMPPHC] < funcCfgObj.startI))
    {
        p->realVal.factor[EMFPHT] = 10000;
        p->realVal.factor[EMFPHA] = 10000;
        p->realVal.factor[EMFPHB] = 10000;
        p->realVal.factor[EMFPHC] = 10000;
    }
    
    if((p->realVal.factor[EMFPHT] < 15) && (p->realVal.factor[EMFPHA] >= 9700) && 
        (p->realVal.factor[EMFPHB] >= 9700) && (p->realVal.factor[EMFPHC] >= 9700))
    {
        p->realVal.factor[EMFPHT] = 10000;
    }

    if(FMETERTYPE)
    {
        f1 = startPwr*3;
    }
    else
    {
        f1 = startPwr*2;
    }
    
    if(p->realVal.aprPwr[EMFPHT] < f1)
    {
        p->realVal.aprPwr[EMFPHT] = 0;
        p->realVal.watPwr[EMFPHT] = 0;
        p->realVal.varPwr[EMFPHT] = 0;
    }
}

void MSRCReadValue(void)
{
    UCHAR i;
    long templong;
    ULONG tempulong;
    stuAttObj *p = (void *)&attObj;

    if(p->var.msrChpFlg.BIT.REINIT)
    {
        return;
    }
    
    WDTRestart();
    MSRCCalDataVerify();
    for(i = EMPPHA; i < EMPPBUT;)
    {
        //读电压电流
        #if(FMSRTYPE == MSR90E32AS)
        {
            MSRCReadData[FMSRTYPE](MSRVoltageRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.voltage[i][2], MSRVoltageRegLen[FMSRTYPE]);
            MSRCReadData[FMSRTYPE](MSRVoltageRegAddr[FMSRTYPE][i]+0x10, (UCHAR *)&p->instVal.voltage[i][0], MSRVoltageRegLen[FMSRTYPE]);            
            MSRCReadData[FMSRTYPE](MSRCurrentRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.current[i][2], MSRCurrentRegLen[FMSRTYPE]);
            MSRCReadData[FMSRTYPE](MSRCurrentRegAddr[FMSRTYPE][i]+0x10, (UCHAR *)&p->instVal.current[i][0], MSRCurrentRegLen[FMSRTYPE]);
        }
        #else
        {
            MSRCReadData[FMSRTYPE](MSRVoltageRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.voltage[i], MSRVoltageRegLen[FMSRTYPE]);
            MSRCReadData[FMSRTYPE](MSRCurrentRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.current[i], MSRCurrentRegLen[FMSRTYPE]);
        }   
		#endif
        ADD_I(i);
    }

    p->instVal.pwrDir = 0;
    for(i = 0; i < EMFIBUT; i++)
    {
        //功率因数
        MSRCReadData[FMSRTYPE](MSRPwrFactorRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.factor[i], MSRPwrFactorRegLen[FMSRTYPE]); 
        //读有功,无功,视在功率
        #if(FMSRTYPE == MSR90E32AS)
        {
            MSRCReadData[FMSRTYPE](MSRActPwrRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.watPwr[i][2], 2); //有功功率高字节        
            MSRCReadData[FMSRTYPE](MSRActPwrRegAddr[FMSRTYPE][i]+0x10, (UCHAR *)&p->instVal.watPwr[i][0], 2); //有功功率低字节
            templong = *((long *)&p->instVal.watPwr[i][0]);
            if(templong < 0)
            {
                p->instVal.pwrDir |= (UCHAR)(BIT_WPWR_PHT << i);
            }
            else
            {
                p->instVal.pwrDir &= (UCHAR)~(BIT_WPWR_PHT << i);
            }
            MSRCReadData[FMSRTYPE](MSRReActPwrRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.varPwr[i][2], 2); //无功功率高字节
            MSRCReadData[FMSRTYPE](MSRReActPwrRegAddr[FMSRTYPE][i]+0x10, (UCHAR *)&p->instVal.varPwr[i][0], 2); //无功功率低字节
            templong = *((long *)&p->instVal.varPwr[i][0]);
            if(templong < 0)
            {
                p->instVal.pwrDir |= (UCHAR)(BIT_VPWR_PHT << i);
            }
            else
            {
                p->instVal.pwrDir &= (UCHAR)~(BIT_VPWR_PHT << i);
            }
            MSRCReadData[FMSRTYPE](MSRAprPwrRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.aprPwr[i][2], 2); //视在功率高字节                       
            MSRCReadData[FMSRTYPE](MSRAprPwrRegAddr[FMSRTYPE][i]+0x10, (UCHAR *)&p->instVal.aprPwr[i][0], 2); //视在功率低字节
            if(p->instVal.factor[i] >= 0x8000)
            {
                p->instVal.factor[i] = 0x10000-p->instVal.factor[i];
            }
        }
        #else
        {
            MSRCReadData[FMSRTYPE](MSRActPwrRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.watPwr[i][0], MSRActPwrRegLen[FMSRTYPE]); //有功功率 
            MSRCReadData[FMSRTYPE](MSRReActPwrRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.varPwr[i][0], MSRReActPwrRegLen[FMSRTYPE]); //无功功率
            MSRCReadData[FMSRTYPE](MSRAprPwrRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.aprPwr[i][0], MSRAprPwrRegLen[FMSRTYPE]); //视在功率
        }
		#endif
    }

    for(i = 0; i < EMPPBUT; i++)
    {
        //读电压,电流相角
        p->instVal.volAngl[i] = 0;
        MSRCReadData[FMSRTYPE](MSRVoltageAngleRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.volAngl[i], MSRVoltageAngleRegLen[FMSRTYPE]);
        p->instVal.curAngl[i] = 0;
        MSRCReadData[FMSRTYPE](MSRCurrentAngleRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.curAngl[i], MSRCurrentAngleRegLen[FMSRTYPE]);
    }
    
    //读电压线频率
    if(FMETERTYPE == DTSD)
    {
        i = 0x07;
    }
    else
    {
        i = 0x05;
    }
    if(p->var.brkFlg.BYTE & i)
    {
        MSRCReadData[FMSRTYPE](MSRPwrFreqRegAddr[FMSRTYPE], (UCHAR *)&p->instVal.freq, MSRPwrFreqRegLen[FMSRTYPE]);
    }

    //读相序寄存器
    tempulong = 0;
    MSRCReadData[FMSRTYPE](MSRPhaseSeqRegAddr[FMSRTYPE], (UCHAR *)&tempulong, MSRPhaseSeqRegLen[FMSRTYPE]);  
    if(tempulong & VolPhaseSeqFlag[FMSRTYPE])
    {
        p->var.pseqFlg.BIT.seqVErr = 1;
        p->var.pseqFlg.BIT.seqVErr &= p->var.brkFlg.BIT.PHAV;
        if(FMETERTYPE == DTSD)
        {
            p->var.pseqFlg.BIT.seqVErr &= p->var.brkFlg.BIT.PHBV;
        }
        p->var.pseqFlg.BIT.seqVErr &= p->var.brkFlg.BIT.PHCV;
        if(FMETERTYPE == DSSD)
        {
            if((p->realVal.voltage[EMPPHA] <= 550000) && (p->realVal.voltage[EMPPHC] <= 550000))
            {
                p->var.pseqFlg.BIT.seqVErr = 0;
            }
        }
    }
    else
    {
        p->var.pseqFlg.BIT.seqVErr = 0;
    }

    #if(FMSRTYPE != MSRRN8302)
    {
        if(tempulong & CurPhaseSeqFlag[FMSRTYPE])
        {
            p->var.pseqFlg.BIT.seqIErr = 1;
            p->var.pseqFlg.BIT.seqIErr &= p->var.brkFlg.BIT.PHAI;
            if(FMETERTYPE == DTSD)
            {
                p->var.pseqFlg.BIT.seqIErr &= p->var.brkFlg.BIT.PHBI;
            }
            p->var.pseqFlg.BIT.seqIErr &= p->var.brkFlg.BIT.PHCI;
        }
        else
        {
            p->var.pseqFlg.BIT.seqIErr = 0;
        }
    }  
	#endif
}

UCHAR MSRCCalDataCheck(void)
{
    stuAttObj *p = (void *)&attObj;

    if((p->calData.head != 0x68) || (p->calData.endflg != 0x16) || 
        (p->calData.chkSum != CalcChksumValue((UCHAR *)&p->calData.head, sizeof(MsrchpCalData)-2, ADD55)))
    {
        return ERROR;
    }

    return OK;
}

void MSRCSaveCalData(void)
{
    stuAttObj *p = (void *)&attObj;

    p->calData.head = 0x68;
    p->calData.endflg = 0x16;
    p->calData.chkSum = CalcChksumValue((UCHAR *)&p->calData.head, sizeof(MsrchpCalData)-2, ADD55);
    E2promWrite(ADDR_OF_ATT_CAL_DATA, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
    E2promWrite(ADDR_OF_SYSPARA_BACK_BASE+ADDR_OF_ATT_CAL_DATA, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
//    DFWriteFlash(ADDR_OF_DF_CALIBRATION, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
//    DFReadFlash(ADDR_OF_DF_CALIBRATION, (UCHAR *)&apdApp.u8Buffer[0], sizeof(MsrchpCalData));
//    if(CmpTwoArry((UCHAR *)&p->calData.head, (UCHAR *)&apdApp.u8Buffer[0], sizeof(MsrchpCalData)) != EQUAL)
//    {
//        DFWriteFlash(ADDR_OF_DF_CALIBRATION, (UCHAR *)&attObj.calData.head, sizeof(MsrchpCalData));
//    }
}

void MSRCSaveCalInfo(void)
{
    MsrchpCalInfo *recinfo = (MsrchpCalInfo *)&LineBuffer[0];   

    recinfo->head = 0x68;
    recinfo->cnt = BcdAddShort(recinfo->cnt, 1);
//    CopySystemTime((UCHAR *)&recinfo->calTime[0], 6);    
    recinfo->chkSum = CalcChksumValue((UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo)-2, ADD55);
    recinfo->endflg = 0x16;
    E2promWrite(ADDR_OF_ATT_CAL_INFO, (UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo));
    E2promWrite(ADDR_OF_SYSPARA_BACK_BASE+ADDR_OF_ATT_CAL_INFO, (UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo));        
}

UCHAR MSRCCalDataVerify(void)
{
    UCHAR ret = 0;
    stuAttObj *p = (void *)&attObj;
    
    ret = MSRCCalDataCheck();
    if(ret == OK)
    {
        return OK;
    }
    
    E2promRead(ADDR_OF_ATT_CAL_DATA, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
    ret = MSRCCalDataCheck();
    if(ret == OK)
    {
        return OK;
    }
    Delay100ms();
    E2promRead(ADDR_OF_SYSPARA_BACK_BASE+ADDR_OF_ATT_CAL_DATA, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
    ret = MSRCCalDataCheck();
    if(ret == OK)
    {
        E2promWrite(ADDR_OF_ATT_CAL_DATA, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
        return OK;
    }

#ifdef EXFLASH
    DFReadFlash(ADDR_OF_DF_CALIBRATION, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
    ret = MSRCCalDataCheck();
    if(ret == OK)
    {
        E2promWrite(ADDR_OF_ATT_CAL_DATA, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
        E2promWrite(ADDR_OF_SYSPARA_BACK_BASE+ADDR_OF_ATT_CAL_DATA, (UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
        return OK;        
    }
#endif

    memcpy((UCHAR *)&p->calData.head, (UCHAR *)&MSRCalibrateInitData[FMSRTYPE], sizeof(MsrchpCalData));
    MSRCSaveCalData();
    return ERROR;
}

void MSRCCalParaInit(void)
{
    MSRCCalDataVerify();
}

UCHAR MSRCCalInfoVerify(void)
{
    MsrchpCalInfo *recinfo = (MsrchpCalInfo *)&LineBuffer[0];
    
    E2promRead(ADDR_OF_ATT_CAL_INFO, (UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo));
    if((recinfo->head == 0x68) && (recinfo->endflg == 0x16) && 
        (recinfo->chkSum == CalcChksumValue((UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo)-2, ADD55)))
    {
        return OK;
    }
    
    E2promRead(ADDR_OF_ATT_CAL_INFO+ADDR_OF_SYSPARA_BACK_BASE, (UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo));
    if((recinfo->head == 0x68) && (recinfo->endflg == 0x16) && 
        (recinfo->chkSum == CalcChksumValue((UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo)-2, ADD55)))
    {
        E2promWrite(ADDR_OF_ATT_CAL_INFO, (UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo));
        return OK;
    }
    
    memcpy((UCHAR *)&recinfo->head, (UCHAR *)&MSRCalibrateInfo[0], sizeof(MsrchpCalInfo));
    E2promWrite(ADDR_OF_ATT_CAL_INFO, (UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo));
    E2promWrite(ADDR_OF_ATT_CAL_INFO+ADDR_OF_SYSPARA_BACK_BASE, (UCHAR *)&recinfo->head, sizeof(MsrchpCalInfo));
    return ERROR;
}

void MSRCCalInfoInit(void)
{    
    if(MSRCCalInfoVerify() != OK)
    {
        MSRCDeviceInit();
        MSRCRealValueInit();
        MSRCChksumCompare();
    }
}

#if (FMSRTYPE == MSR90E32AS)
void AT90E32PulseDecInit(void)
{
    UCHAR i;
    stuEngObj *p = (void *)&engObj;

    for(i = 0; i < EMPLSBUT; i++)
    {
        p->instInt.posWatt[i] = (p->instInt.posWatt[i]/100)*100;
        p->instInt.negWatt[i] = (p->instInt.negWatt[i]/100)*100;
        p->instInt.var[i][EMQM1] = (p->instInt.var[i][EMQM1]/100)*100;
        p->instInt.var[i][EMQM2] = (p->instInt.var[i][EMQM2]/100)*100;
        p->instInt.var[i][EMQM3] = (p->instInt.var[i][EMQM3]/100)*100;
        p->instInt.var[i][EMQM4] = (p->instInt.var[i][EMQM4]/100)*100;
    }
    sysDataAPIFunc(SDEINSTINT, EMDOSCALC);
}
#endif

void MSRCModeProcess(UCHAR mode)
{
    #if(FMSRTYPE == MSRRN8302)
    {
        if((mode != RN8302_MODE_GOSLP) && (mode != RN8302_MODE_GOEMM) &&
           (mode != RN8302_MODE_GONVM1) && (mode != RN8302_MODE_GONVM2))
        {
            return;
        }

        attObj.var.buffer[0] = mode;
        MSRCWriteData[FMSRTYPE](0x0181, &attObj.var.buffer[0], 1);
    }  
	#endif
}

void MSRCDeviceInit(void)
{
    UCHAR i,j,flg;
    ULONG tempulong = 0;
    stuAttObj *p = (void *)&attObj;
    
    MSRCSPIInit();    
    Delay5ms();
    Delay5ms();
    Delay5ms();
    for(i = 0; i < 10; i++)
    {
        WDTRestart();
        if((i & 0x01) == 0)
        {
            MSRCHardReset();
        }
        else
        {
            MSRCSoftReset();
        }
        
        tempulong = 0;
        MSRCReadData[FMSRTYPE](MSRDeviceRegAddr[FMSRTYPE], (UCHAR *)&tempulong, MSRDeviceRegALen[FMSRTYPE]);
        #if(FMSRTYPE == MSR90E32AS)
        {
            tempulong = (tempulong & 0xFF00);
        }
		#endif
        if(tempulong == MSRDeviceIDValue[FMSRTYPE])
        {
            #if(FMSRTYPE == MSRRN8302)
            {
                MSRCWRRegSelect(ENABLE);
                MSRCModeProcess(RN8302_MODE_GOEMM);
                tempulong = 0;
                MSRCReadData[FMSRTYPE](0x018A, (UCHAR *)&tempulong, 2);
                if((tempulong & 0xC000) == 0x4000)
                {
                    i = 100;
                    break;
                }
            }
            #else
            {
                i = 100;
                break;
            }
			#endif
        }
    }    

    if(i < 100)
    {
        p->var.msrChpFlg.BIT.REINIT = 1;
        deviceFlag.BIT.MSR = 1;
        return;
    }

#if 0
	MSRCWRRegSelect(ENABLE);
    #if(FMSRTYPE == MSRRN8302)
    {
        p->var.buffer[0] = 0x77;
        p->var.buffer[1] = 0x77;
        p->var.buffer[2] = 0x77;
        MSRCWriteData[FMSRTYPE](0x0162, &p->var.buffer[0], 3); //EMUCON
    }
	#endif
    MSRCWRRegSelect(DISABLE);
    Delay100ms();
#endif

#if 1
    //校表数据有效性判定
    MSRCCalDataVerify();
    p->var.msrChpFlg.BIT.WRERR = 0;
    p->var.sumErrCnt = 0;
    deviceFlag.BIT.MSR = 0;
    
    // 校表数据写使能
    #if(FMSRTYPE == MSR90E32AS)
    {
        AT90E32InRegWRSelect(ENABLE);
    }
	#endif
    MSRCWRRegSelect(ENABLE);
    #if(FMSRTYPE == MSRRN8302)
    {
        p->var.buffer[0] = 0x77;
        p->var.buffer[1] = 0x77;
        p->var.buffer[2] = 0x77;
        MSRCWriteData[FMSRTYPE](0x0162, &p->var.buffer[0], 3); //EMUCON
        
        //禁止CF3输出脉冲
        if(IsPowerOn())
        {
            p->var.buffer[0] = 0x10;
            p->var.buffer[1] = 0x37;
            p->var.buffer[2] = 0x04;
        }
        else
        {
            p->var.buffer[0] = 0x77;
            p->var.buffer[1] = 0x37;
            p->var.buffer[2] = 0x04;       
        }
        MSRCWriteData[FMSRTYPE](0x0160, &p->var.buffer[0], 3); //CFCFG
        
        //选择三相三线模式
        if(FMETERTYPE == DSSD)
        {
            p->var.buffer[0] = 0x33;
            MSRCWriteData[FMSRTYPE](0x0186, &p->var.buffer[0], 1); //MODSEL
        }

        //过零阀值初始化
        p->var.buffer[0] = p->calData.reserve1[0];
        p->var.buffer[1] = p->calData.reserve1[1];
        MSRCWriteData[FMSRTYPE](0x0105, &p->var.buffer[0], 2); //ZXOT

        //ADC配置寄存器初始化
        p->var.buffer[0] = p->calData.reserve2[0];
        p->var.buffer[1] = p->calData.reserve2[1];
        MSRCWriteData[FMSRTYPE](0x0183, &p->var.buffer[0], 2); //ADCCFG
		
        // 清空采样数据缓存区
        p->var.buffer[0] = 0x10;
        MSRCWriteData[FMSRTYPE](0x0163, &p->var.buffer[0], 1); //WSAVECON
    }
    #elif(FMSRTYPE == MSR90E32AS)
    {
        if(IsPowerOn())
        {
            p->var.buffer[0] = 0xFF;
            p->var.buffer[1] = 0x00;
        }
        else
        {
            p->var.buffer[0] = 0x00;
            p->var.buffer[1] = 0x00;        
        }
        MSRCWriteData[FMSRTYPE](0x00, (UCHAR *)&p->var.buffer[0], 2); //MeterEn
        
        p->var.buffer[0] = 0x00;
        p->var.buffer[1] = 0x00;
        MSRCWriteData[FMSRTYPE](0x10, (UCHAR *)&p->var.buffer[0], 2); //DetectCtrl

        if(FMETERTYPE == DTSD)
        {
            p->var.buffer[0] = 0x87;
            p->var.buffer[1] = 0x00;
        }
        else
        {
            p->var.buffer[0] = 0x85;
            p->var.buffer[1] = 0x01;
        }
        MSRCWriteData[FMSRTYPE](0x33, (UCHAR *)&p->var.buffer[0], 2); //MMode0          
        
        p->var.buffer[0] = 0x00;
        p->var.buffer[1] = 0x00;
        MSRCWriteData[FMSRTYPE](0x34, (UCHAR *)&p->var.buffer[0], 2); //MMode1

        p->var.buffer[0] = 0x88;
        p->var.buffer[1] = 0x13;
        MSRCWriteData[FMSRTYPE](0x22, (UCHAR *)&p->var.buffer[0], 2); //f0

        if(FTEMPCMPST)
        {
            p->var.buffer[0] = p->calData.reserve1[0];
            p->var.buffer[1] = p->calData.reserve1[1];
        }
        else
        {
            p->var.buffer[0] = 0x00;
            p->var.buffer[1] = 0x00;
        }
        MSRCWriteData[FMSRTYPE](0x1A, (UCHAR *)&p->var.buffer[0], 2); //UGainTAB
        
        if(FTEMPCMPST)
        {
            p->var.buffer[0] = p->calData.reserve2[0];
            p->var.buffer[1] = p->calData.reserve2[1];
        }
        else
        {
            p->var.buffer[0] = 0x00;
            p->var.buffer[1] = 0x00;
        }
        MSRCWriteData[FMSRTYPE](0x1B, (UCHAR *)&p->var.buffer[0], 2); //UGainTC
    }
    #else
    {
        //写模式配置寄存器
        p->var.buffer[0] = 0x7E;
        p->var.buffer[1] = 0xB9;
        p->var.buffer[2] = 0x00;
        MSRCWriteData[FMSRTYPE](0x01, (UCHAR *)&p->var.buffer[0], 3); //wModeCfg
        
        //写EMU单元配置寄存器
        if(FMETERTYPE == DTSD)
        {
            p->var.buffer[0] = 0b11000100;
        }
        else
        {
            p->var.buffer[0] = 0b10000100;
        }

        if(IsPowerOn())
        {
            p->var.buffer[1] = 0b11111000;
        }
        else
        {
            p->var.buffer[1] = 0b00001000;
        }
        p->var.buffer[2] = 0x00;
        MSRCWriteData[FMSRTYPE](0x03, (UCHAR *)&p->var.buffer[0], 3); //wEMUCfg

        //写新算法控制寄存器
        if(FMETERTYPE == DTSD)
        {
            p->var.buffer[0] = 0b00000000;
            if(FTEMPCMPST)
            {
                p->var.buffer[0] = 0b00000010;
            }
        }
        else
        {
            p->var.buffer[0] = 0b00000001; //wEMCfg
            if(FTEMPCMPST)
            {
                p->var.buffer[0] = 0b00000011;
            }
        }
        p->var.buffer[1] = 0x00;
        p->var.buffer[2] = 0x00;
        MSRCWriteData[FMSRTYPE](0x70, (UCHAR *)&p->var.buffer[0], 3); //wEMUCfg
        
        //写模拟模块使能寄存器
        p->var.buffer[0] = 0x37;
        p->var.buffer[1] = 0x34;
        p->var.buffer[2] = 0x00;
        MSRCWriteData[FMSRTYPE](0x31, (UCHAR *)&p->var.buffer[0], 3); //wModuleCFG
    
        //写ADC增益寄存器
        p->var.buffer[0] = p->calData.reserve1[0];
        p->var.buffer[1] = p->calData.reserve1[1];
        p->var.buffer[2] = 0x00;
        MSRCWriteData[FMSRTYPE](0x02, (UCHAR *)&p->var.buffer[0], 3); //wPGACtrl

        if(FTEMPCMPST)
        {
            p->var.buffer[0] = 0x11;
            p->var.buffer[1] = 0xFF;
            p->var.buffer[2] = 0x00;
            MSRCWriteData[FMSRTYPE](0x6D, (UCHAR *)&p->var.buffer[0], 3);
        
            p->var.buffer[0] = 0x53;
            p->var.buffer[1] = 0x2B;
            p->var.buffer[2] = 0x00;
            MSRCWriteData[FMSRTYPE](0x6E, (UCHAR *)&p->var.buffer[0], 3);

            p->var.buffer[0] = 0x83;
            p->var.buffer[1] = 0xD4;
            p->var.buffer[2] = 0x00;
            MSRCWriteData[FMSRTYPE](0x6F, (UCHAR *)&p->var.buffer[0], 3);
        }
    }
	#endif

    //初始化高频脉冲常数
    p->var.buffer[0] = (UCHAR)p->calData.hfConst[0];
    p->var.buffer[1] = (UCHAR)(p->calData.hfConst[0]>>8);
    p->var.buffer[2] = 0;
    MSRCWriteData[FMSRTYPE](MSRHFConstRegAddr[FMSRTYPE][0], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
    #if(FMSRTYPE == MSR90E32AS)
    {
        p->var.buffer[0] = (UCHAR)p->calData.hfConst[1];
        p->var.buffer[1] = (UCHAR)(p->calData.hfConst[1]>>8);
        p->var.buffer[2] = 0;
        MSRCWriteData[FMSRTYPE](MSRHFConstRegAddr[FMSRTYPE][1], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);        
    }
	#endif
    
    //启动电流初始化
    p->var.buffer[0] = p->calData.curStart[0];
    p->var.buffer[1] = p->calData.curStart[1];
    p->var.buffer[2] = 0;
    MSRCWriteData[FMSRTYPE](MSRIStartRegAddr[FMSRTYPE], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
    #if(FMSRTYPE == MSR90E32AS)
    {
        MSRCWriteData[FMSRTYPE](0x0036, &p->var.buffer[0], 2); //无功启动功率寄存器
        MSRCWriteData[FMSRTYPE](0x0037, &p->var.buffer[0], 2); //视在启动功率寄存器
    }
	#endif

    for(i = EMPPHA; i < EMPPBUT; i++)
    {
        // 电压增益初始化
        p->var.buffer[0] = p->calData.VGain[i][0];
        p->var.buffer[1] = p->calData.VGain[i][1];
        p->var.buffer[2] = 0;
        MSRCWriteData[FMSRTYPE](MSRGainRegAddr[FMSRTYPE][0][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
        
        // 电流增益初始化
        p->var.buffer[0] = p->calData.IGain[i][0];
        p->var.buffer[1] = p->calData.IGain[i][1];
        p->var.buffer[2] = 0;
        MSRCWriteData[FMSRTYPE](MSRGainRegAddr[FMSRTYPE][1][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
        
        //有功功率增益初始化
        p->var.buffer[0] = p->calData.PGain[i][0];
        p->var.buffer[1] = p->calData.PGain[i][1];
        p->var.buffer[2] = 0;
        MSRCWriteData[FMSRTYPE](MSRGainRegAddr[FMSRTYPE][2][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
        
        //无功功率增益初始化
        p->var.buffer[0] = p->calData.PGain[i][0];
        p->var.buffer[1] = p->calData.PGain[i][1];
        p->var.buffer[2] = 0;
        MSRCWriteData[FMSRTYPE](MSRGainRegAddr[FMSRTYPE][3][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
        
        //视在功率增益初始化
        p->var.buffer[0] = p->calData.PGain[i][0];
        p->var.buffer[1] = p->calData.PGain[i][1];
        p->var.buffer[2] = 0;
        MSRCWriteData[FMSRTYPE](MSRGainRegAddr[FMSRTYPE][4][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);        
    }
    
    // 通道相位分段参数
    for(i = 0; i < MSRPrthIregionItem[FMSRTYPE]; i++)
    {
        p->var.buffer[0] = p->calData.phsIreg[i][0];
        p->var.buffer[1] = p->calData.phsIreg[i][1];
        p->var.buffer[2] = 0;
        MSRCWriteData[FMSRTYPE](MSRPrthIregionRegAddr[FMSRTYPE][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
    }
    
    // 采样通道电流相位校正
    for(i = 0; i < MSRPhsiRegItem[FMSRTYPE]; i++)
    {
        for(j = EMPPHA; j < EMPPBUT; j++)
        {    
            p->var.buffer[0] = p->calData.PHSI[i][j][0];
            p->var.buffer[1] = p->calData.PHSI[i][j][1];
            p->var.buffer[2] = p->calData.PHSI[i][j][2];
            MSRCWriteData[FMSRTYPE](MSRPhsiRegAddr[FMSRTYPE][j][i], &p->var.buffer[0], MSRPhsiRegLen[FMSRTYPE]);
        }
    }
    
    // 通道功率OFFSET校正
    for(i = EMPPHA; i < EMPPBUT; i++)
    {
        #if(FMSRTYPE == MSRATT7022E)
        {
            p->var.buffer[0] = p->calData.PHOS[i][1];
            p->var.buffer[1] = p->calData.PHOS[i][2];
            p->var.buffer[2] = 0;
            MSRCWriteData[FMSRTYPE](MSRPOffsetRegAddr[FMSRTYPE][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
            MSRCWriteData[FMSRTYPE](MSRQOffsetRegAddr[FMSRTYPE][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);

            p->var.buffer[0] = p->calData.PHOS[i][0];
            p->var.buffer[1] = 0;
            p->var.buffer[2] = 0;
            MSRCWriteData[FMSRTYPE](0x64+i, &p->var.buffer[0], 3);
            MSRCWriteData[FMSRTYPE](0x67+i, &p->var.buffer[0], 3);
        }
        #else
        {
            p->var.buffer[0] = p->calData.PHOS[i][0];
            p->var.buffer[1] = p->calData.PHOS[i][1];
            p->var.buffer[2] = 0;
            MSRCWriteData[FMSRTYPE](MSRPOffsetRegAddr[FMSRTYPE][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);
            MSRCWriteData[FMSRTYPE](MSRQOffsetRegAddr[FMSRTYPE][i], &p->var.buffer[0], MSRCalibrateRegLen[FMSRTYPE]);            
        }
		#endif
    }

    //电流Offset校正
    for(i = EMPPHA; i < EMPPBUT; i++)
    {
        p->var.buffer[0] = p->calData.IrmsOffset[i][0];
        p->var.buffer[1] = p->calData.IrmsOffset[i][1];
        p->var.buffer[2] = p->calData.IrmsOffset[i][2];
        MSRCWriteData[FMSRTYPE](MSRIrmsOffsetRegAddr[FMSRTYPE][i], &p->var.buffer[0], MSRIrmsOffsetRegLen[FMSRTYPE]);
    }

    //脉冲常数加倍选择    
    if(FCALIBRATE)
    {
        if(p->var.msrChpFlg.BIT.PULSEDB)
        {
            if((p->var.pulseDb[4] == 0x53) && (p->var.pulseDb[5] == 0x55))
            {
                #if(FMSRTYPE == MSRRN8302)
                {
                    p->var.buffer[0] = p->var.pulseDb[0];
                    p->var.buffer[1] = p->var.pulseDb[1];
                    MSRCWriteData[FMSRTYPE](MSRHFConstRegAddr[FMSRTYPE][0], &p->var.buffer[0], 2);
                }
                #elif(FMSRTYPE == MSR90E32AS)
                {
                    p->var.buffer[0] = p->var.pulseDb[0];
                    p->var.buffer[1] = p->var.pulseDb[1];
                    MSRCWriteData[FMSRTYPE](MSRHFConstRegAddr[FMSRTYPE][0], &p->var.buffer[0], 2);
                    p->var.buffer[0] = p->var.pulseDb[2];
                    p->var.buffer[1] = p->var.pulseDb[3];
                    MSRCWriteData[FMSRTYPE](MSRHFConstRegAddr[FMSRTYPE][1], &p->var.buffer[0], 2);
                }
                #elif(FMSRTYPE == MSRATT7022E)
                {
                    p->var.buffer[0] = p->var.pulseDb[0];
                    p->var.buffer[1] = p->var.pulseDb[1];
                    p->var.buffer[2] = 0x00;
                    MSRCWriteData[FMSRTYPE](MSRHFConstRegAddr[FMSRTYPE][1], &p->var.buffer[0], 3);
                }
				#endif
            }                
        }
    }

    if(p->var.msrChpFlg.BIT.WRERR == 0)
    {
        OPEN_PULSE_OUT();
        #if(FMSRTYPE == MSR90E32AS)
        {
            AT90E32PulseDecInit();
        }
		#endif
    }
    
    //关闭校表设置    
    flg = 0;
    MSRCWRRegSelect(DISABLE);
    #if(FMSRTYPE == MSR90E32AS)
    {
        AT90E32InRegWRSelect(DISABLE);
    }
	#endif
    Delay100ms();

    if(p->var.msrChpFlg.BIT.WRERR)
    {
        p->var.msrChpFlg.BIT.REINIT = 1;
    }
    else
    {
        p->var.msrChpFlg.BIT.REINIT = 0;
    }
#endif
    
    MemSetZero((UCHAR *)&p->Pulse.posWatt[0], sizeof(MsrchpPulse));
    MemSetZero((UCHAR *)&g_fastPulseCnt[0], sizeof(g_fastPulseCnt));
    MemSetZero((UCHAR *)&g_atmelCurrent[0], sizeof(g_atmelCurrent));
}

void CalcPulseConst(void)
{
    stuAttObj *p = (void *)&attObj;

    #if(FMSRTYPE == MSR90E32AS)
    {
        engObj.var.intPulseConst = p->calData.pulseConst*10;
    }
    #else
    {
        engObj.var.intPulseConst = p->calData.pulseConst/10;
    }
	#endif
    attObj.var.basicVal = p->calData.pulseConst*10;
}

void GetFullLowvCurrent(void)
{
#if 0
    UCHAR i;
    ULONG IRMS;
    stuAttObj *p = (void *)&attObj;

    for(i = EMPPHA; i < EMPPBUT;)
    {
        MSRCReadData[FMSRTYPE](MSRCurrentRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.current[i][0], MSRCurrentRegLen[FMSRTYPE]);      
        IRMS = (*(ULONG *)&p->instVal.current[i][0]);        
        if(FMSRTYPE == MSRRN8302)
        {
            p->realVal.current[i] = (IRMS*10)/p->calData.curRatio;
        }
        else if(FMSRTYPE == MSR90E32AS)
        {
            p->realVal.current[i] = IRMS*10*IDT_MULT;
        }
        else
        {
            p->realVal.current[i] = CalcATT7022Current((UCHAR *)&p->instVal.current[i][0]);
        }
        
        if((p->realVal.current[i] < funcCfgObj.startI) || (p->realVal.current[i] >= 8388607))
        {
            p->realVal.current[i] = 0;
        }
        ADD_I(i);
    }
#endif
}

#if 0
UCHAR GetVoltagePwrOnDwn(enum GetVolSelect item)
{
    UCHAR i;
    ULONG URMS;
    stuAttObj *p = (void *)&attObj;

    for(i = EMPPHA; i < EMPPBUT;)
    {
        MSRCReadData[FMSRTYPE](MSRVoltageRegAddr[FMSRTYPE][i], (UCHAR *)&p->instVal.voltage[i], MSRVoltageRegLen[FMSRTYPE]);
        URMS = (*(ULONG *)&p->instVal.voltage[i][0]);        
        if(FMSRTYPE == MSRRN8302)
        {
            p->realVal.voltage[i] = (URMS*100)/p->calData.volRatio;
        }
        else if(FMSRTYPE == MSR90E32AS)
        {
            p->realVal.voltage[i] = URMS*100;
        }
        else
        {
            p->realVal.voltage[i] = CalcATT7022Voltage((UCHAR *)&p->instVal.voltage[i][0]);
        }
        
        if(p->realVal.voltage[i] >= 8388607 || p->realVal.voltage[i] < 100000)
        {
            p->realVal.voltage[i] = 0;
        }
        ADD_I(i);
    }

    if(item == pwrOn)
    {
        if((p->realVal.voltage[EMPPHA] <= evtObj.para.lcdVol) &&
          (p->realVal.voltage[EMPPHB] <= evtObj.para.lcdVol) &&
          (p->realVal.voltage[EMPPHC] <= evtObj.para.lcdVol))
        {
            FCRITICALVOL = 1;
        }

        if((p->realVal.voltage[EMPPHA] <= FPULSE_VOLTAGE) &&
          (p->realVal.voltage[EMPPHB] <= FPULSE_VOLTAGE) &&
          (p->realVal.voltage[EMPPHC] <= FPULSE_VOLTAGE))
        {
            FPULSEOUT = 1;
        }
        return 0;
    }
    else
    {
        if(((p->realVal.voltage[EMPPHA] >= PWR_ABNM_VOLTAGE_UP) && (p->realVal.voltage[EMPPHA] < PWR_ABNM_VOLTAGE_DOWN)) ||
            ((p->realVal.voltage[EMPPHB] >= PWR_ABNM_VOLTAGE_UP) && (p->realVal.voltage[EMPPHB] < PWR_ABNM_VOLTAGE_DOWN)) ||
            ((p->realVal.voltage[EMPPHC] >= PWR_ABNM_VOLTAGE_UP) && (p->realVal.voltage[EMPPHC] < PWR_ABNM_VOLTAGE_DOWN)))
        {
            p->realVal.voltage[EMPPHA] = 0;
            p->realVal.voltage[EMPPHB] = 0;
            p->realVal.voltage[EMPPHC] = 0;
            return 1;
        }
        
        p->realVal.voltage[EMPPHA] = 0;
        p->realVal.voltage[EMPPHB] = 0;
        p->realVal.voltage[EMPPHC] = 0;    
        return 0;
    }        
}
#endif

void MSRCValidCheck(void)
{
    ULONG tempulong1 = 0;
    ULONG tempulong2 = 0;
    stuAttObj *p = (void *)&attObj;

    if(p->var.msrChpFlg.BIT.REINIT)
    {
        MSRCDeviceInit();
        MSRCRealValueInit();
    }
    
    if((p->var.msrChpFlg.BIT.REINIT == 1) ||
       (p->var.msrChpFlg.BIT.PULSEDB == 1))
    {
        return;
    }

    MSRCReadData[FMSRTYPE](MSRChecksumRegAddr1[FMSRTYPE], (UCHAR *)&tempulong1, MSRChecksumRegLen[FMSRTYPE]);
    MSRCReadData[FMSRTYPE](MSRChecksumRegAddr2[FMSRTYPE], (UCHAR *)&tempulong2, MSRChecksumRegLen[FMSRTYPE]);
    if((p->calData.InRegCs != tempulong1) || (p->calData.ExtRegCs != tempulong2))
    {
        p->var.sumErrCnt++;
    }
    else
    {
        p->var.sumErrCnt = 0;
    }
    
    if(p->var.sumErrCnt >= 3)
    {
        p->var.sumErrCnt = 0;
        p->var.msrChpFlg.BIT.REINIT = 1;
    }    
    MemSetZero((UCHAR *)&p->var.pulseDb[0], sizeof(p->var.pulseDb));
}

void MSRCChksumCompare(void)
{
    UCHAR flg;
    ULONG tempulong;
    stuAttObj *p = (void *)&attObj;

    tempulong = 0;
    MSRCReadData[FMSRTYPE](MSRChecksumRegAddr1[FMSRTYPE], (UCHAR *)&tempulong, MSRChecksumRegLen[FMSRTYPE]);
    if(p->calData.InRegCs != tempulong)
    {
        flg = 1;
        p->calData.InRegCs = tempulong;
    }

    tempulong = 0;
    MSRCReadData[FMSRTYPE](MSRChecksumRegAddr2[FMSRTYPE], (UCHAR *)&tempulong, MSRChecksumRegLen[FMSRTYPE]);
    if(p->calData.ExtRegCs != tempulong)
    {
        flg = 1;
        p->calData.ExtRegCs = tempulong;
    }
    
    if(flg)
    {
        MSRCSaveCalData();
        MSRCCalInfoVerify();
        MSRCSaveCalInfo();
    }        
}

void MSRCStatusCheck(void)
{
#if 1
    stuAttObj *p = (void *)&attObj;

    if((p->var.commCalib == 0xA5A5) && (FCALIBRATE))
    {
        p->var.commCalib = 0;
        if(p->var.msrChpFlg.BIT.PULSEDB == 0)
        {
            MSRCDeviceInit();
            MSRCRealValueInit();
            MSRCChksumCompare();
        }
        else
        {
            p->var.msrChpFlg.BIT.PULSEDB = 0;
            MSRCDeviceInit();
            MSRCChksumCompare();
            p->var.msrChpFlg.BIT.PULSEDB = 1;
            MSRCDeviceInit();
        }
    }
#endif

	MSRCReadData[FMSRTYPE](0x0162, (UCHAR *)&apdApp.u8Buffer[0], 3);
}

UCHAR SlvsWriteCalData(void)
{
    UCHAR i;
    stuAttObj *p = (void *)&attObj;

#if 1
    if(!FCALIBRATE)
    {
        commErrByte.BIT.password = 1;
        return RET_SEND_ERROR_RESPONSION;
    }
#endif

    if((DI1 == 0x10) && (DI0 == 0x05))
    {
        if(dl645Buff[FRM_DATA+12] == 0xA5)
        {
//            MemSetZero((UCHAR *)&p->calData.head, sizeof(MsrchpCalData));
			memcpy((UCHAR *)&p->calData.head, (UCHAR *)&MSRCalibrateInitData[FMSRTYPE], sizeof(MsrchpCalData));
		#if 0
            p->calData.head = 0X68;
            p->calData.pulseConst = 0x0028;
            #if(FMSRTYPE == MSRRN8302)
            {
                p->calData.hfConst[0] = 0x213D;
                p->calData.hfConst[1] = 0x0100;
                p->calData.volRatio = 0x03E8;
                p->calData.curRatio = 0x029F;
                for(i = 0; i < 3; i++)
                {
                    p->calData.PHSI[i][0][0] = 0x80;
                    p->calData.PHSI[i][0][1] = 0x80;
                    p->calData.PHSI[i][0][2] = 0x80;

                    p->calData.PHSI[i][1][0] = 0x80;
                    p->calData.PHSI[i][1][1] = 0x80;
                    p->calData.PHSI[i][1][2] = 0x80;

                    p->calData.PHSI[i][2][0] = 0x80;
                    p->calData.PHSI[i][2][1] = 0x80;
                    p->calData.PHSI[i][2][2] = 0x80;

                    p->calData.PHSI[i][3][0] = 0x80;
                    p->calData.PHSI[i][3][1] = 0x80;
                    p->calData.PHSI[i][3][2] = 0x80;                    
                }
            }
            #elif(FMSRTYPE == MSR90E32AS)
            {
                p->calData.hfConst[0] = 0x0861;
                p->calData.hfConst[1] = 0xC468;
                for(i = 0; i < 3; i++)
                {
                    p->calData.VGain[i][0] = 0x00;
                    p->calData.VGain[i][1] = 0x80;
                    p->calData.IGain[i][0] = 0x00;
                    p->calData.IGain[i][1] = 0x80;
                }                
            }
            #else
            {
                p->calData.hfConst[0] = 0x015E;
                p->calData.hfConst[1] = 0x0000;            
            }
			#endif
		#endif
            p->calData.chkSum = CalcChksumValue((UCHAR *)&p->calData.head, sizeof(MsrchpCalData)-2, ADD55);
//            p->calData.endflg = 0x16;
            p->var.commCalib = 0xA5A5;
            return RET_SEND_RIGHT_RESPONSION;
        }
        else if(dl645Buff[FRM_DATA+12] == 0xAA)
        {
            if((dl645Buff[FRM_DATA+13] == 0x06) && (dl645Buff[FRM_DATA+14] == 0x53))
            {
                memcpy((UCHAR *)&p->var.pulseDb[0], &dl645Buff[FRM_DATA+15], 4);
                p->var.pulseDb[4] = dl645Buff[FRM_DATA+14];
                p->var.pulseDb[5] = dl645Buff[FRM_DATA+19];
                p->var.msrChpFlg.BIT.REINIT = 1;
                p->var.msrChpFlg.BIT.PULSEDB = 1;
                return RET_SEND_RIGHT_RESPONSION;
            }
        }
        else
        {
            UCHAR *p1 = (UCHAR *)&p->calData.head;
            memcpy(p1+dl645Buff[FRM_DATA+12], &dl645Buff[FRM_DATA+14], dl645Buff[FRM_DATA+13]);
            p->calData.head = 0x68;
            p->calData.chkSum = CalcChksumValue((UCHAR *)&p->calData.head, sizeof(MsrchpCalData)-2, ADD55);
            p->calData.endflg = 0x16;
            p->var.commCalib = 0xA5A5;
            return RET_SEND_RIGHT_RESPONSION;
        }
    }
    
    commErrByte.BIT.others = 1;
    return RET_SEND_ERROR_RESPONSION;
}

UCHAR SlvsReadCalData(void)
{
    UCHAR ofst;
    UINT tempuint;
    stuAttObj *p = (void *)&attObj;

    if(DI1 == 0x10)
    {
        if(DI0 == 0x00)
        {
            apdApp.u8Buffer[0] = (UCHAR)FMSRTYPE; //计量芯片型号
            apdApp.u8Buffer[1] = funcCfgObj.msrVer; //计量芯片版本号
            apdApp.u8Buffer[2] = (UCHAR)FMETERTYPE; //表计型号
            tempuint = Hex2BcdShort(funcCfgObj.ratedUb); //额定电压            
            memcpy(&apdApp.u8Buffer[3], (UCHAR *)&tempuint, 2);
            tempuint = Hex2BcdShort(funcCfgObj.ratedIb); //额定电流
            memcpy(&apdApp.u8Buffer[5], (UCHAR *)&tempuint, 2);
            tempuint = Hex2BcdShort(funcCfgObj.maxIb); //最大电流
            memcpy(&apdApp.u8Buffer[7], (UCHAR *)&tempuint, 2);
            tempuint = Hex2BcdShort(funcCfgObj.startI); //启动电流
            memcpy(&apdApp.u8Buffer[9], (UCHAR *)&tempuint, 2);
            commLen = 11;
            return RET_SEND_RIGHT_RESPONSION;
        }
        else if(DI0 == 0x01)
        {
            ofst = 0;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.voltage[EMPPHA], 4); //A相电压4字节
            ofst += 4;            
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.voltage[EMPPHB], 4); //B相电压4字节
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.voltage[EMPPHC], 4); //C相电压4字节
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.current[EMPPHA], 4); //A相电流4字节
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.current[EMPPHB], 4); //B相电流4字节
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.current[EMPPHC], 4); //C相电流4字节
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.watPwr[EMFPHA], 4); //A相有功功率4字节
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.watPwr[EMFPHB], 4); //B相有功功率4字节
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.watPwr[EMFPHC], 4); //C相有功功率4字节
            ofst += 4;
            commLen = ofst;
            return RET_SEND_RIGHT_RESPONSION;
        }
        else if(DI0 == 0x02)
        {
            ofst = 0;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.watPwr[EMFPHT], 4);
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.watPwr[EMFPHA], 4);
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.watPwr[EMFPHB], 4);
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.watPwr[EMFPHC], 4);
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.aprPwr[EMFPHT], 4);
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.aprPwr[EMFPHA], 4);
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.aprPwr[EMFPHB], 4);
            ofst += 4;
            memcpy(&apdApp.u8Buffer[ofst], (UCHAR *)&p->instVal.aprPwr[EMFPHC], 4);
            ofst += 4;
            commLen = ofst;
            return RET_SEND_RIGHT_RESPONSION;
        }
        else if(DI0 == 0x03)
        {
            memcpy(&apdApp.u8Buffer[0], &p->calData.head, sizeof(MsrchpCalData));
            commLen = sizeof(MsrchpCalData);
            return RET_SEND_RIGHT_RESPONSION;
        }
        else if(DI0 == 0x04)
        {
            commLen = dl645Buff[FRM_DATA+6];
            if((commLen == 0x00) || (commLen > 0x04))
            {
                return RET_SEND_ERROR_RESPONSION;
            }
            tempuint = (*(UINT *)&dl645Buff[FRM_DATA+4]);            
            #if(FMSRTYPE == MSRATT7022E)
            {
                p->var.buffer[0] = 0x5A;
                p->var.buffer[1] = 0x00;
                p->var.buffer[2] = 0x00;
                MSRCWriteData[FMSRTYPE](0xC6, (UCHAR *)&p->var.buffer[0], 3);
                ATT7022ReadCalibrateData(tempuint, &apdApp.u8Buffer[0], 3);
                p->var.buffer[0] = 0x55;
                p->var.buffer[1] = 0x00;
                p->var.buffer[2] = 0x00;
                MSRCWriteData[FMSRTYPE](0xC6, (UCHAR *)&p->var.buffer[0], 3);        
            }
            #else
            {
                MSRCReadData[FMSRTYPE](tempuint, &apdApp.u8Buffer[0], commLen);
            }
			#endif
            return RET_SEND_RIGHT_RESPONSION;
        }
    }

    commErrByte.BIT.noAskData = 1;
    return RET_SEND_ERROR_RESPONSION;
}

void CheckCalibrateEnable(void)
{
#if 0
    if(!FSELFTEST)
    {
        return;
    }
    
    if(!KEY_UP && !KEY_PROG)
    {
        if(CaliEnableCnt < 5)
        {
            CaliEnableCnt++;
        }
        else
        {
            CaliEnableCnt = 0;
            if(!FCALIBRATE)
            {
                FCALIBRATE = 1;
                veryVar.progTmCnt = PROG_TIMER_COUNT;
            }
            else
            {
                FCALIBRATE = 0;
            }
            sysDataAPIFunc(SDSYS, EMDOSCALC);
        }
    }
    else
    {
        CaliEnableCnt = 0;
    }
#endif
}

void MSRCAPIFunc(void)
{
    MSRCReadValue();
    CalcRealTimeValue();
    CheckCalibrateEnable();
}

