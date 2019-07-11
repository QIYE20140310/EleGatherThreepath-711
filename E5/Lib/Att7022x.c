/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "Att7022x.h"
#include  "RN8302B.h"


#define MSRC_CS_OUT1       	HAL_GPIO_WritePin(GPIOB, SPI1_CS_PIN, GPIO_PIN_RESET);
#define MSRC_CS_OUT0        HAL_GPIO_WritePin(GPIOB, SPI1_CS_PIN, GPIO_PIN_SET);

#define MSRC_SCLK_OUT0      HAL_GPIO_WritePin(GPIOB, SPI1_CLK_PIN, GPIO_PIN_SET);
#define MSRC_SCLK_OUT1      HAL_GPIO_WritePin(GPIOB, SPI1_CLK_PIN, GPIO_PIN_RESET);


#define MSRC_DIN_OUT0       HAL_GPIO_WritePin(GPIOB, SPI1_MOSI_PIN, GPIO_PIN_SET);
#define MSRC_DIN_OUT1       HAL_GPIO_WritePin(GPIOB, SPI1_MOSI_PIN, GPIO_PIN_RESET);

#define MSRC_DOUT           HAL_GPIO_ReadPin(GPIOB,SPI1_MISO_PIN);

#define MSRC_RESET_OUT0
#define MSRC_RESET_OUT1

void   _NOP(void)
{
	__NOP();

}
void delay1us(void)
{
	for(int i=0;i<8;i++)
	   __NOP();

}
void Delay(int ms)
{
for(int i=0;i<ms;i++)
	delay1us();
}
void Delay5ms(void)
{
   for(int i=0;i<5;i++){
       for(int j=0;j<1000;j++)
	   	delay1us();
   }

}

#if (FMSRTYPE == MSRRN8302)
void RN8302WriteByte(UCHAR wdata)
{
    UCHAR i;

    for(i = 0; i < 8; i++)
    {
        MSRC_SCLK_OUT0;
        if(wdata & 0x80)
        {
            MSRC_DIN_OUT0;
        }
        else
        {
            MSRC_DIN_OUT1;
        }
        wdata <<= 1;
        delay1us();delay1us();delay1us();delay1us();delay1us();//Delay(5);
        MSRC_SCLK_OUT1;
        delay1us();delay1us();//Delay(2);
    }   
}
#endif



#if (FMSRTYPE == MSRRN8302)
UCHAR RN8302ReadByte(void)
{
    UCHAR rdData = 0;
    UCHAR bitCnt = 8;

    do
    {
        MSRC_SCLK_OUT0;
        delay1us();delay1us();delay1us();delay1us();delay1us();//Delay(5);Delay(10);
//        delay1us();delay1us();delay1us();delay1us();delay1us();//Delay(5);Delay(10);
        rdData <<= 1;
        //if(MSRC_DOUT)
        if(HAL_GPIO_ReadPin(GPIOB,SPI1_MISO_PIN))
		{
            rdData |= 0x01;
        }
        MSRC_SCLK_OUT1;
		delay1us();delay1us();//Delay(2);
    }while(--bitCnt);

    return(rdData);
}
#endif


#if (FMSRTYPE == MSRRN8302)
UCHAR RN8302ReadData(UINT wReg, UCHAR *data, UCHAR len)
{
    UCHAR i,temp,cs;
    UCHAR buffer[4];

    if((data == NULL) || (len == 0))
    {
        return 1;
    }
    cs = 0;
    memset((UCHAR *)&buffer[0], 4);
    MSRC_CS_OUT1;
    _NOP();_NOP();_NOP();_NOP();
    temp = (UCHAR)(wReg & 0x00FF);
    RN8302WriteByte(temp);
    cs += temp;
    temp = (((UCHAR)(wReg >> 4)) & 0xf0);
    RN8302WriteByte(temp);
    cs += temp;
    for(i = len; i > 0; i--)
    {
        buffer[i-1] = RN8302ReadByte();
        cs += buffer[i-1];
    }
    temp = RN8302ReadByte();
    cs += temp;
    _NOP();
    _NOP();
    _NOP();
    _NOP();
    MSRC_CS_OUT0;
    MSRC_DIN_OUT0;
    MSRC_SCLK_OUT0;
    if(cs == 0xFF)
    {
        memcpy(data, (UCHAR *)&buffer[0], len);
         return 0;
    }

    return 1;
}
#endif



#if (FMSRTYPE == MSRRN8302)
UCHAR RN8302WriteData(UINT wReg, UCHAR *data, UCHAR len)
{
    UCHAR i, temp, cs = 0;
    ULONG tempulong = 0;

    if((data == NULL) || (len == 0) || (len > 4))
    {
        return 1;
    }
    
    MSRC_CS_OUT1;
    _NOP();_NOP();_NOP();_NOP();
    temp = (UCHAR)(wReg & 0x00FF);
    cs += temp;
    RN8302WriteByte(temp);
    temp = (((UCHAR)(wReg >> 4)) & 0xf0)+0x80;
    cs += temp;
    RN8302WriteByte(temp);
    for(i = len; i > 0;)
    {
        --i;
        RN8302WriteByte(data[i]);
        cs += data[i];
    }
    cs = cs ^ 0xFF;
    RN8302WriteByte(cs);
    _NOP();
    _NOP();
    _NOP();
    _NOP();
    MSRC_CS_OUT0;
    MSRC_DIN_OUT0;
    MSRC_SCLK_OUT0;
    tempulong = 0;
    RN8302ReadData(0x018D, (UCHAR *)&tempulong, 3); //WData
    //if(CmpTwoArry((UCHAR *)&tempulong, data, len) != EQUAL)
    {
        
        return 1;
    }

	return 0;
}

#endif







unsigned char testnet[3];
void PDMSRCSPIInit(void)
{
//    CLOSE_MSRCHP_VCC;
    MSRC_RESET_OUT0;
    MSRC_DIN_OUT0;
    MSRC_CS_OUT0;
    MSRC_SCLK_OUT0;
	RN8302ReadData(0x008F,testnet,3);
//    MSRC_DOUT_OUT1;
}

void MSRCHardReset(void)
{
    MSRC_RESET_OUT0;
    Delay5ms();
    MSRC_RESET_OUT1;
    Delay5ms();
    MSRC_RESET_OUT0;
    Delay5ms();    
}

