#ifndef __E5LCD_H
#define __E5LCD_H

typedef unsigned char    u8_t;
typedef unsigned int     u32_t;
typedef unsigned short   u16_t;
typedef signed short     s16_t;


#define LETTER_UNIT_OFF     0
#define LETTER_UNIT_A       1
#define LETTER_UNIT_mA      2
#define LETTER_UNIT_kW      3
#define LETTER_UNIT_KWh     4
#define LETTER_UNIT_PECENT  5
#define LETTER_UNIT_DEGREE  6


#define DIR_AHEAD_NO_SKIP 0
#define DIR_AHEAD_SKIP_2  1
#define DIR_BACK_SKIP_2   2

#define BUZZ_OFF   0
#define BUZZ_2K_ON 1
#define BUZZ_4K_ON 2


typedef struct
{
   u16_t vol;
   u16_t lki;
   u16_t pwr;
   s16_t tmp;
   u16_t cur;
   u16_t alm;
   u16_t psl;
   u16_t psh;
   u16_t vol_a;
   u16_t vol_b;
   u16_t vol_c;
   u16_t cur_a;
   u16_t cur_b;
   u16_t cur_c;
   u16_t cur_n;
   u16_t pwr_a;
   u16_t pwr_b;
   u16_t pwr_c;
   u16_t alm_a;
   u16_t alm_b;
   u16_t alm_c;
   u8_t  alm1;
   u8_t  sws;
}  sw_status_t;

#endif