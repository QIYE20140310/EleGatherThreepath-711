
#include "ht1622x.h"
#include "RN8302B.h"
#include "main.h"


#define  HT_LCD_CS_PORT         GPIOA
#define  HT_LCD_CS_PIN          GPIO_PIN_15

#define  HT_LCD_WR_POERT        GPIOB
#define  HT_LCD_WR_PIN          GPIO_PIN_4

#define  HT_LCD_RD_POER         GPIOB
#define  HT_LCD_RD_PIN          GPIO_PIN_5

#define  HT_LCD_DATA_PORT       GPIOB
#define  HT_LCD_Data_PIN        GPIO_PIN_3


void delay_us(unsigned int us)
{
	 for(int i=0;i<4;i++)
		__NOP();
}

static void ht1622_gpio_config(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = HT_LCD_WR_PIN|HT_LCD_RD_PIN|HT_LCD_Data_PIN;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = HT_LCD_CS_PIN;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(HT_LCD_CS_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(HT_LCD_CS_PORT,HT_LCD_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,HT_LCD_WR_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,HT_LCD_RD_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,HT_LCD_Data_PIN, GPIO_PIN_SET);

    #if 1
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, GPIO_PIN_RESET);
	#endif

}

static void ht1622_data_out(u8_t out)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	GPIO_InitStruct.Pin=HT_LCD_Data_PIN;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    out?HAL_GPIO_WritePin(HT_LCD_DATA_PORT,HT_LCD_Data_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(HT_LCD_DATA_PORT,HT_LCD_Data_PIN, GPIO_PIN_RESET);
}


u8_t ht1622_data_in(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	GPIO_InitStruct.Pin=HT_LCD_Data_PIN;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(HT_LCD_DATA_PORT, &GPIO_InitStruct);
	return HAL_GPIO_ReadPin(HT_LCD_DATA_PORT,HT_LCD_Data_PIN);
}

static void ht1622_rd_out(u8_t out)
{
	out?HAL_GPIO_WritePin(HT_LCD_RD_POER,HT_LCD_RD_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(HT_LCD_RD_POER,HT_LCD_RD_PIN, GPIO_PIN_RESET);
}

static void ht1622_wr_out(u8_t out)
{
	out?HAL_GPIO_WritePin(HT_LCD_WR_POERT,HT_LCD_WR_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(HT_LCD_WR_POERT,HT_LCD_WR_PIN, GPIO_PIN_RESET);
}

static void ht1622_cs_out(u8_t out)
{	
 	out?HAL_GPIO_WritePin(HT_LCD_CS_PORT,HT_LCD_CS_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(HT_LCD_CS_PORT,HT_LCD_CS_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  send to data bits to ht1622x.
  * @param  data: data content
  * @param  cnt:  data content valid bits count
  * @retval None
  */


 
void ht1622x_send_bits(u8_t data, u8_t cnt)
{ /*if cnt>8, it will send 0 for rest bits*/
  u8_t i;
  u8_t out_data = data;
  for (i = 0; i < cnt; i++)
  {
    ht1622_wr_out(0);
    ht1622_data_out(out_data & 0x80);
    delay_us(2);
    ht1622_wr_out(1);
    delay_us(2);
    out_data <<= 1;
  }
}

/**
  * @brief  send to command to ht1622x.
  * @param  cmd: command content
  * @retval None
  */

void ht1622XX_send_1high(void)
{
    delay_us(2);
	ht1622_data_out(1);
    delay_us(2);
	ht1622_wr_out(0);
    delay_us(2);
	ht1622_data_out(1);
    delay_us(2);
	ht1622_wr_out(1);


}

void ht1622x_send_cmd(u8_t cmd)
{
  ht1622_cs_out(0);
  delay_us(2);
  ht1622x_send_bits(0x80, 3);
  ht1622x_send_bits(cmd, 9); // send 1 bit 0 more, read ht1622 datasheet for details

  delay_us(2);
  ht1622_cs_out(1);  
  delay_us(2);
}




/**
  * @brief  send to data to ht1622x to control SEG and COM pins.
  * @param  addr: 0~63
  * @param  data: for COM7~7 while addr is odd, for COM0~3 while addr is even
  *               only lower 4 bits is valid,
  *               bit3 --- COM0/COM4
  *               bit2 --- COM1/COM5
  *               bit1 --- COM2/COM6
  *               bit0 --- COM3/COM7
  * @retval None
  */
void ht1622x_send_data(u8_t addr, u8_t data)
{
  ht1622_cs_out(0);
  delay_us(1);
  ht1622x_send_bits(0xA0, 3);
  ht1622x_send_bits(addr << 2, 6); //A5 A4 A3 A2 A1 A0��д���ַ����
  ht1622x_send_bits(data, 4);      //D0 D1 D2 D3��д������
  ht1622_cs_out(1);
}

/**
  * @brief  set all ht1622x internal RAM as value.
  * @param  value: 0~0xff , only higher 4 bits is valid
  * @retval None
  */
void ht1622x_display_all(u8_t value)
{
  u8_t i;
  ht1622_cs_out(0);
  ht1622x_send_bits(0xa0, 3);
  ht1622x_send_bits(0, 6);
  for (i = 0; i < 32; i++)
  {
    ht1622x_send_bits(value, 4);
  }
  ht1622_cs_out(1);
}

/**
  * @brief  turn on/off ht1622x buzz.
  * @param  flag:BUZZ_OFF    --- off
  *              BUZZ_2K_ON  --- beep frequncy 2K Hz
  *              BUZZ_4K_ON  --- beep frequncy 4K Hz
  * @retval None
  */
void ht1622x_buzz(u8_t flag)
{
  switch (flag)
  {
  case BUZZ_OFF:
    ht1622x_send_cmd(HT1622_CMD_TONE_OFF);
    break;
  case BUZZ_2K_ON:
    ht1622x_send_cmd(HT1622_CMD_TONE_2K);
    break;
  case BUZZ_4K_ON:
    ht1622x_send_cmd(HT1622_CMD_TONE_4K);
    break;
  default:
    break;
  }
}

/**
  * @brief  init ht1622x chip.
  * @param  None
  * @retval None
  */
void ht1622x_init(void)
{
  
  ht1622_gpio_config();
  ht1622x_send_cmd(HT1622_CMD_NORMAL_MODE); //working in normal mode
  ht1622x_send_cmd(HT1622_CMD_CLK_RC32K);   //eanble interna RC32k crystal
  ht1622x_send_cmd(HT1622_CMD_SYS_EN);      //enable system
  ht1622x_send_cmd(HT1622_CMD_LCD_ON);      //enable LCD module
}




