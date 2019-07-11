#ifndef _LCDdisplay_H
#define _LCDdisplay_H

#include "ht1622x.h"

void display_ram_byte(u8_t offset);
void show_letter_in_line1(u8_t value, u8_t offset);
void show_letter_in_line2(u8_t value, u8_t offset, u8_t dot);
void show_number_in_line1(u8_t value, u8_t offset, u8_t dot);
void show_number_in_line2(u8_t value, u8_t offset, u8_t dot);

void show_logo(u8_t ena);

void show_letter_a(u8_t ena);
void show_letter_b(u8_t ena);
void show_letter_c(u8_t ena);

void show_letter_abc(u8_t ena);
void show_letter_acb(u8_t ena);

void show_letter_v(u8_t ena);
void show_letter_addr(u8_t ena);
void show_AudoMode(u8_t enable);

#define LETTER_UNIT_OFF     0
#define LETTER_UNIT_A       1
#define LETTER_UNIT_mA      2
#define LETTER_UNIT_kW      3
#define LETTER_UNIT_KWh     4
#define LETTER_UNIT_PECENT  5
#define LETTER_UNIT_DEGREE  6
void show_units(u8_t ena);

void show_alarms(u8_t type, u32_t alrm);

#define DIR_AHEAD_NO_SKIP 0
#define DIR_AHEAD_SKIP_2  1
#define DIR_BACK_SKIP_2   2
u8_t switch_node_index(char dir);
u8_t set_index_of_screen_node(u8_t idx);

u8_t get_lcd_ram_byte(u8_t idx);

u8_t is_skip_enable(void);
u8_t is_lcd_fast_mode(void);
void set_lcd_show_mode(u8_t fast);

void rt623832_init(u8_t showlogo);

#endif
