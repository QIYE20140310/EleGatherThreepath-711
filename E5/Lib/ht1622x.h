#ifndef _HT1622_H
#define _HT1622_H
#include "typedef.h"

/* HT1622x command list, read datasheet for details */
#define HT1622_CMD_SYS_DIS      0
#define HT1622_CMD_SYS_EN       1
#define HT1622_CMD_LCD_OFF      2
#define HT1622_CMD_LCD_ON       3
#define HT1622_CMD_TIMER_DIS    4
#define HT1622_CMD_WDT_DIS      5
#define HT1622_CMD_TIMER_EN     6
#define HT1622_CMD_WDT_EN       7
#define HT1622_CMD_TONE_OFF     8
#define HT1622_CMD_CLR_TIMER    13
#define HT1622_CMD_LLR_WD       15
#define HT1622_CMD_CLK_RC32K    0x18
#define HT1622_CMD_CLK_EXT32K   0x1c
#define HT1622_CMD_TONE_4K      0x40
#define HT1622_CMD_TONE_2K      0x60
#define HT1622_CMD_IRQ_DIS      0x80
#define HT1622_CMD_IRQ_EN       0x88
#define HT1622_CMD_CLK_OUT_1Hz  0xa0
#define HT1622_CMD_CLK_OUT_2Hz  0xa1
#define HT1622_CMD_CLK_OUT_4Hz  0xa2
#define HT1622_CMD_CLK_OUT_8Hz  0xa3
#define HT1622_CMD_CLK_OUT_16Hz 0xa4
#define HT1622_CMD_CLK_OUT_32Hz 0xa5
#define HT1622_CMD_CLK_OUT_64Hz 0xa6
#define HT1622_CMD_CLK_OUT_128Hz 0xa7
#define HT1622_CMD_TEST_MODE     0xe0
#define HT1622_CMD_NORMAL_MODE   0xE3


/**
  * @brief  send to data bits to ht1622x.
  * @param  data: data content
  * @param  cnt:  data content valid bits count
  * @retval None
  */
void ht1622x_send_bits(u8_t data,u8_t cnt);

/**
  * @brief  send to command to ht1622x.
  * @param  cmd: command content
  * @retval None
  */
void ht1622x_send_cmd(u8_t cmd);

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
void ht1622x_send_data(u8_t addr ,u8_t data);

/**
  * @brief  set all ht1622x internal RAM as value.
  * @param  value: 0~0xff , only higher 4 bits is valid
  * @retval None
  */
void  ht1622x_display_all(u8_t value);


#define BUZZ_OFF   0
#define BUZZ_2K_ON 1
#define BUZZ_4K_ON 2
/**
  * @brief  turn on/off ht1622x buzz.
  * @param  flag:BUZZ_OFF    --- off
  *              BUZZ_2K_ON  --- beep frequncy 2K Hz
  *              BUZZ_4K_ON  --- beep frequncy 4K Hz
  * @retval None
  */
void ht1622x_buzz(u8_t flag);

/**
  * @brief  init ht1622x chip.
  * @param  None
  * @retval None
  */
void ht1622x_init(void);


void show_voltage_line1(u16_t vol);


#endif
