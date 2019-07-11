#include "ht1622x.h"
#include "lcd_rt623832.h"
#include "string.h"
#include "stdio.h"



typedef struct lcd_rt623832
{
	u8_t value[14];
	u8_t addr[14];
	u8_t node_idx;
	u8_t skip;
	u8_t fast_mode;
	u8_t show_logo;
} lcd_ram_t;

static lcd_ram_t lcd_ram;

/**
  * @brief  send the ram byte to ht1622.
  * @param  offset: 0~13
  * @retval None
  */
void display_ram_byte(u8_t offset)
{
	ht1622x_send_data(lcd_ram.addr[offset], lcd_ram.value[offset]);
	ht1622x_send_data(lcd_ram.addr[offset] + 2, lcd_ram.value[offset] << 4);
}

void show_letter_in_line1(u8_t value, u8_t offset)
{
	u8_t idx = offset;
	u8_t old_value = lcd_ram.value[idx];
	if(idx>2)
		return;
	
	switch(value)
	{
	case 'a':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xdf: 0xcf;
		else
			lcd_ram.value[idx]  = 0x6f;
		break;
	case 'b':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xdc: 0xcc;
		else
			lcd_ram.value[idx]  = 0x7c;
		break;
	case 'c':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xd8: 0xc8;
		else
			lcd_ram.value[idx]  = 0x68;
		break;
	case 'd':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xde: 0xce;
		else
			lcd_ram.value[idx]  = 0x6e;
		break;
	case 'e':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xfb: 0xeb;
		else
			lcd_ram.value[idx]  = 0x7b;
		break;
	case 'E':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xf9: 0xe9;
		else
			lcd_ram.value[idx]  = 0x79;
		break;
	case 'L':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xb8: 0xa8;
		else
			lcd_ram.value[idx]  = 0x58;
		break;
	case 'P':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xf3: 0xe3;
		else
			lcd_ram.value[idx]  = 0x73;
		break;
	case 'r':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xdf: 0xcf;
		else
			lcd_ram.value[idx]  = 0x6f;
		break;
	case 'S':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0x7d: 0x6d;
		else
			lcd_ram.value[idx]  = 0x3d;
		break;
	case 't':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xf8: 0xe8;
		else
			lcd_ram.value[idx]  = 0x78;
		break;
	case 'T':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0xb1: 0xa1;
		else
			lcd_ram.value[idx]  = 0x51;
		break;
	case 'u':
		if(idx==0)
			lcd_ram.value[idx]  = lcd_ram.show_logo? 0x9c: 0x8c;
		else
			lcd_ram.value[idx]  = 0x4c;
		break;
	}
	if(old_value != lcd_ram.value[idx])
		display_ram_byte(idx);
}

void show_number_in_line1(u8_t value, u8_t offset,u8_t dot)
{
	const u8_t segment_value_0_10[] = {0xaf, 0x06, 0xcb, 0x4f, 0x66, 0x6d, 0xed, 0x07, 0xef, 0x6f,0xcf,0xec,0xa9,0xce,0xe9,0xe1};
	const u8_t segment_value_1_2[]  = {0x5f, 0x06, 0x6b, 0x2f, 0x36, 0x3d, 0x7d, 0x07, 0x7f, 0x3f,0x6f,0x7c,0x59,0x6e,0x79,0x71};
	u8_t old_value =0;
	
	if (offset > 2)
		return;
	
	old_value = lcd_ram.value[offset];
	switch (offset)
	{
	case 0:
		lcd_ram.value[offset] &= 0x10;
		lcd_ram.value[offset] |= value > 15? 0: segment_value_0_10[value];
		break;
	case 1:
	case 2:
		lcd_ram.value[offset] &= 0x80;
		lcd_ram.value[offset] |= value > 15? 0: segment_value_1_2[value];
		break;
	default: break;
	}
	if(old_value != lcd_ram.value[offset])
		display_ram_byte(offset);
}

void show_letter_in_line2(u8_t value, u8_t offset, u8_t dot)
{
	u8_t idx = offset+3;
	u8_t old_value = lcd_ram.value[idx];
	if(idx>10)
		return;
	
	switch(value)
	{
	case 'a':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xdf: 0xd7;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xfd: 0x7d;
		else
			lcd_ram.value[idx]  = 0xcf;
		break;
	case 'b':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xee: 0xe6;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xee: 0x6e;
		else
			lcd_ram.value[idx]  = 0xfc;
		break;
	case 'c':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xca: 0xc2;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xac: 0x2c;
		else
			lcd_ram.value[idx]  = 0xc8;
		break;
	case 'C':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xf8: 0xf0;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0x8f: 0x0f;
		else
			lcd_ram.value[idx]  = 0xa9;
		break;
	case 'd':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xcf: 0xc7;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xfc: 0x7c;
		else
			lcd_ram.value[idx]  = 0xce;
		break;
	case 'e':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xfb: 0xf3;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xbf: 0x3f;
		else
			lcd_ram.value[idx]  = 0xeb;
		break;
	case 'H':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0x6f: 0x67;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xf6: 0x76;
		else
			lcd_ram.value[idx]  = 0xe6;
		break;
	case 'L':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xe8: 0xe0;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0x8e: 0x0e;
		else
			lcd_ram.value[idx]  = 0xa8;
		break;
	case 'P':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0x7b: 0x73;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xb7: 0x37;
		else
			lcd_ram.value[idx]  = 0xe3;
		break;
	case 'R':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0x7f: 0x77;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xf7: 0x77;
		else
			lcd_ram.value[idx]  = 0xe7;
		break;
	case 'r':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0x4a: 0x42;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xa4: 0x24;
		else
			lcd_ram.value[idx]  = 0xc0;
		break;
	case 'S':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xbe: 0xb6;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xeb: 0x6b;
		else
			lcd_ram.value[idx]  = 0x3d;
		break;
	case 't':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xea: 0xe2;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xae: 0x2e;
		else
			lcd_ram.value[idx]  = 0xe8;
		break;
	case 'T':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0x78: 0x70;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0x87: 0x07;
		else
			lcd_ram.value[idx]  = 0xa1;
		break;
	case 'u':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xcc: 0xc4;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xcc: 0x4c;
		else
			lcd_ram.value[idx]  = 0x8c;
		break;
	case 'U':
		if(idx<8)
			lcd_ram.value[idx]  = dot? 0xed: 0xe5;
		else if(idx<10)
			lcd_ram.value[idx]  = dot? 0xde: 0x5e;
		else
			lcd_ram.value[idx]  = 0x67;
		break;
	}
	if(old_value != lcd_ram.value[idx])
		display_ram_byte(idx);
}

void show_number_in_line2(u8_t value, u8_t offset,u8_t dot)
{
	const u8_t segment_value_0_10[] = {0xaf, 0x06, 0xcb, 0x4f, 0x66, 0x6d, 0xed, 0x07, 0xef, 0x6f,0xcf,0xec,0xa9,0xce,0xe9,0xe1};
	const u8_t segment_value_4to8[] = {0xf5, 0x05, 0xd3, 0x97, 0x27, 0xb6, 0xf6, 0x15, 0xf7, 0xb7,0xd7,0xe6,0xf0,0xc7,0xf2,0x72};
	const u8_t segment_value_9_10[] = {0x5f, 0x50, 0x3d, 0x79, 0x72, 0x6b, 0x6f, 0x51, 0x7f, 0x7b,0x7d,0x6e,0x0f,0x7c,0x2f,0x27};
	u8_t idx = offset+3;
	u8_t old_value = lcd_ram.value[idx];
	
	if (offset > 7)
		return;
	
	switch (idx)
	{
	case 10:
		
		lcd_ram.value[idx]  = dot? 0x10: 0;
		lcd_ram.value[idx] |= value > 15? 0: segment_value_0_10[value];
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		lcd_ram.value[idx]  = dot? 0x08: 0;
		lcd_ram.value[idx] |= value > 15? 0: segment_value_4to8[value];
		break;
	case 8:
	case 9:
		lcd_ram.value[idx]  = dot? 0x80: 0;
		lcd_ram.value[idx] |= value > 15? 0: segment_value_9_10[value];
		break;
	}
	if(old_value != lcd_ram.value[idx])
		display_ram_byte(idx);
}

/**
  * @brief  show mantunsci logo
  * @param  ena: 0 - empty/ non zero - show logo
  * @retval None
  */
void show_logo(u8_t ena)
{
	u8_t old_value = lcd_ram.value[0];

	if (ena)
		lcd_ram.value[0] |= 0x10;
	else
		lcd_ram.value[0] &= 0xef;
	if(old_value != lcd_ram.value[0])
		display_ram_byte(0);

}


void show_AudoMode(u8_t enable)
{
	u8_t old_value = lcd_ram.value[1];
	if (enable)
		lcd_ram.value[1] |= 0xff;
	else
		lcd_ram.value[1] &= 0x7f;	
		display_ram_byte(1);
}

/**
  * @brief  show letter A
  * @param  ena: 0 - empty/ non zero - show letter A
  * @retval None
  */
void show_letter_a(u8_t ena)
{
	u8_t old_value = lcd_ram.value[1];
	if (ena)
		lcd_ram.value[1] |= 0x80;
	else
		lcd_ram.value[1] &= 0x7f;
	if(old_value != lcd_ram.value[1])
		display_ram_byte(1);
}

/**
  * @brief  show letter B
  * @param  ena: 0 - empty/ non zero - show letter B
  * @retval None
  */
void show_letter_b(u8_t ena)
{
	u8_t old_value = lcd_ram.value[2];
	if (ena)
		lcd_ram.value[2] |= 0x80;
	else
		lcd_ram.value[2] &= 0x7f;
	if(old_value != lcd_ram.value[2])
		display_ram_byte(2);
}

/**
  * @brief  show letter C
  * @param  ena: 0 - empty/ non zero - show letter C
  * @retval None
  */
void show_letter_c(u8_t ena)
{
	u8_t old_value = lcd_ram.value[11];
	if (ena)
		lcd_ram.value[11] |= 0x80;
	else
		lcd_ram.value[11] &= 0x7f;
	if(old_value != lcd_ram.value[11])
		display_ram_byte(11);
}

/**
  * @brief  show letter ABC
  * @param  ena: 0 - empty/ non zero - show letter ABC
  * @retval None
  */
void show_letter_abc(u8_t ena)
{
	u8_t old_value = lcd_ram.value[11];
	if (ena)
		lcd_ram.value[11] |= 0x08;
	else
		lcd_ram.value[11] &= 0xf7;
	if(old_value != lcd_ram.value[11])
		display_ram_byte(11);
}

/**
  * @brief  show letter ACB
  * @param  ena: 0 - empty/ non zero - show letter ACB
  * @retval None
  */
void show_letter_acb(u8_t ena)
{
	u8_t old_value = lcd_ram.value[12];
	if (ena)
		lcd_ram.value[12] |= 0x08;
	else
		lcd_ram.value[12] &= 0xf7;
	if(old_value != lcd_ram.value[12])
		display_ram_byte(12);
}

/**
  * @brief  show letter V
  * @param  ena: 0 - empty/ non zero - show letter V
  * @retval None
  */
void show_letter_v(u8_t ena)
{
	u8_t old_value = lcd_ram.value[11];
	if (ena)
		lcd_ram.value[11] |= 0x20;
	else
		lcd_ram.value[11] &= 0xdf;
	if(old_value != lcd_ram.value[11])
		display_ram_byte(11);
}

/**
  * @brief  show letter addr
  * @param  ena: 0 - empty/ non zero - show letter addr
  * @retval None
  */
void show_letter_addr(u8_t ena)
{
	u8_t old_value = lcd_ram.value[11];
	if (ena)
		lcd_ram.value[11] |= 0x40;
	else
		lcd_ram.value[11] &= 0xbf;
	if(old_value != lcd_ram.value[11])
		display_ram_byte(11);
}

/**
  * @brief  show letter %
  * @param  ena: 0 - empty/ non zero - show letter %
  * @retval None
  */
void show_units(u8_t ena)
{
	u8_t old_value = lcd_ram.value[13];
	switch (ena)
	{
	case LETTER_UNIT_A:
		lcd_ram.value[13] &= 0x11;
		lcd_ram.value[13] |= 0x02;
		break;
	case LETTER_UNIT_mA:
		lcd_ram.value[13] &= 0x11;
		lcd_ram.value[13] |= 0x22;
		break;
	case LETTER_UNIT_kW:
		lcd_ram.value[13] &= 0x11;
		lcd_ram.value[13] |= 0x40;
		break;
	case LETTER_UNIT_KWh:
		lcd_ram.value[13] &= 0x11;
		lcd_ram.value[13] |= 0x44;
		break;
	case LETTER_UNIT_PECENT:
		lcd_ram.value[13] &= 0x11;
		lcd_ram.value[13] |= 0x80;
		break;
	case LETTER_UNIT_DEGREE:
		lcd_ram.value[13] &= 0x11;
		lcd_ram.value[13] |= 0x08;
		break;
	default:
	case LETTER_UNIT_OFF:
		lcd_ram.value[13] &= 0x11;
		break;
	}
	if(old_value != lcd_ram.value[13])
		display_ram_byte(13);
}


/**
  * @brief  show alarms
  * @param  alrm: each bit has its own description
  * @retval None
  */
void show_alarms(u8_t type, u32_t alrm)
{
	u8_t ov0 = lcd_ram.value[10];
	u8_t ov1 = lcd_ram.value[11];
	u8_t ov2 = lcd_ram.value[12];
	u8_t ov3 = lcd_ram.value[13];
	
#ifdef E9
		lcd_ram.value[12] &= 0xfe; 
		lcd_ram.value[11] &= 0xef;

#else
	if (alrm & 0x10000){
		lcd_ram.value[11] |= 0x10; //manual off
		lcd_ram.value[12] &= 0xfe; 
	}
	else{
		lcd_ram.value[11] &= 0xef;
		lcd_ram.value[12] |= 0x01; //auto mode
	}
#endif	
	
	if (alrm & 0x01)
		lcd_ram.value[11] |= 0x01; //short current
	else
		lcd_ram.value[11] &= 0xfe;

	if (alrm & 0x02)
		lcd_ram.value[12] |= 0x02; //surge
	else
		lcd_ram.value[12] &= 0xfd;

	if (alrm & 0x04)
		lcd_ram.value[10] |= 0x10; //overload
	else
		lcd_ram.value[10] &= 0xef;

	if (alrm & 0x08)
		lcd_ram.value[12] |= 0x40; //over temperature
	else
		lcd_ram.value[12] &= 0xbf;

	if (alrm & 0x4010)
		lcd_ram.value[13] |= 0x10; //leakage
	else
		lcd_ram.value[13] &= 0xef;

	if (alrm & 0x8020)
		lcd_ram.value[12] |= 0x10; //over current
	else
		lcd_ram.value[12] &= 0xef;

	if (alrm & 0x1040)
		lcd_ram.value[11] |= 0x02; //over voltage
	else
		lcd_ram.value[11] &= 0xfd;

	if (alrm & 0x0400)
		lcd_ram.value[11] |= 0x04; //spark
	else
		lcd_ram.value[11] &= 0xfb;

	if (alrm & 0x2800)
		lcd_ram.value[12] |= 0x20; //under voltage
	else
		lcd_ram.value[12] &= 0xdf;
	
	if (alrm & 0xf000)
		lcd_ram.value[13] |= 0x01; //under voltage
	else
		lcd_ram.value[13] &= 0xfe;
	/*
	if((type == FUNC_TYPE_SSW) || (type == FUNC_TYPE_TSW))
	{
		if (alrm & 0x0200)
			lcd_ram.value[12] |= 0x04; //lose phase
		else
			lcd_ram.value[12] &= 0xfb;
		
		if (alrm & 0x400000)
			lcd_ram.value[12] |= 0x80; //unbalance
		else
			lcd_ram.value[12] &= 0x7f;
		
		if (alrm & 0x800000)
		{
			lcd_ram.value[12] |= 0x08; // phase acb
			lcd_ram.value[11] &= 0xf7; 
		}
		else{
			lcd_ram.value[11] |= 0x08; // phase abc
			lcd_ram.value[12] &= 0xf7;
		}
	}
	else
	{
		lcd_ram.value[11] &= 0xf7; 
		lcd_ram.value[12] &= 0x73;
	}
	*/
	if(ov0 != lcd_ram.value[10])
		display_ram_byte(10);
	if(ov1 != lcd_ram.value[11])
		display_ram_byte(11);
	if(ov2 != lcd_ram.value[12])
		display_ram_byte(12);
	if(ov3 != lcd_ram.value[13])
		display_ram_byte(13);
}

void assign_value_address(void)
{
	lcd_ram.addr[0] = 1;	 //0;
	lcd_ram.addr[1] = 5;	 //1;
	lcd_ram.addr[2] = 9;	 //4;
	lcd_ram.addr[3] = 0;	 //5;
	lcd_ram.addr[4] = 4;	 //8;
	lcd_ram.addr[5] = 8;	 //9;
	lcd_ram.addr[6] = 12;	//12;
	lcd_ram.addr[7] = 16;	//13;
	lcd_ram.addr[8] = 24;	//17;
	lcd_ram.addr[9] = 20;	//16;
	lcd_ram.addr[10] = 21; //20;
	lcd_ram.addr[11] = 13; //21;
	lcd_ram.addr[12] = 17; //24;
	lcd_ram.addr[13] = 25; //25
}

u8_t switch_node_index(char dir)
{
	#if 0
	u8_t cnt = 0;
	if(dir == DIR_BACK_SKIP_2 || dir == DIR_AHEAD_SKIP_2)
		lcd_ram.skip = 3;
	else
		lcd_ram.skip = 0;
	do{
		lcd_ram.node_idx = dir==DIR_BACK_SKIP_2?
		(lcd_ram.node_idx==0? NUMBER_OF_NODES-1 : lcd_ram.node_idx-1):
		(lcd_ram.node_idx>NUMBER_OF_NODES? 0 : lcd_ram.node_idx+1);
		if(cnt++ >NUMBER_OF_NODES)
			return 0;
	}while(!is_node_online(lcd_ram.node_idx));
	return lcd_ram.node_idx;
 #endif
}

u8_t set_index_of_screen_node(u8_t idx)
{
	if(idx <NUMBER_OF_SLOTS)
	{
		lcd_ram.node_idx = idx;
		lcd_ram.skip = 3;
	}
	return 0;
}

u8_t get_lcd_ram_byte(u8_t idx)
{
	return lcd_ram.value[idx];
}

void set_lcd_show_mode(u8_t fast)
{
	lcd_ram.fast_mode = fast;
}

u8_t is_lcd_fast_mode(void){return lcd_ram.fast_mode? 1: 0;}
u8_t is_skip_enable(void)
{
	if(lcd_ram.skip) lcd_ram.skip--;
	return lcd_ram.skip;
}

void rt623832_init(u8_t showlogo)
{
	memset(&lcd_ram, 0, sizeof(lcd_ram_t));
	assign_value_address();
	ht1622x_init();
	ht1622x_display_all(0);
	lcd_ram.show_logo = showlogo;
	show_logo(showlogo);
	show_alarms(0,0x8020);
}
