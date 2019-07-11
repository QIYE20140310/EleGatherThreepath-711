#include "e5lcd.h"


void show_voltage_line1(u16_t vol)
{
	u16_t rest_value = vol;
	u8_t dis_value = vol/100;
	show_number_in_line1(dis_value,0, 0);
	
	rest_value -= dis_value* 100;
	dis_value = rest_value/10;
	show_number_in_line1(dis_value,1, 0);
	
	rest_value -= dis_value* 10;
	show_number_in_line1(rest_value,2, 0);
	
	show_letter_v(1);
}

void show_voltage_line2(u32_t vol)
{
	u8_t dis_value;
	u8_t show_zero = 0;
	
	show_letter_in_line2('U', 0, 0);
	show_number_in_line2(0xff,1, 0);
	show_number_in_line2(0xff,2, 0);
	show_number_in_line2(0xff,3, 0);
	show_number_in_line2(0xff,4, 0);
	
	dis_value = vol>=100?vol/100:0;
	if(dis_value) {
		show_number_in_line2(dis_value,5, 0);
		show_zero = 1;
	}
	else
		show_number_in_line2(0xff,5, 0);
	
	vol -= dis_value* 100;
	dis_value = vol>=10?vol/10:0;
	if(dis_value ||show_zero) {
		show_number_in_line2(dis_value,6, 0);
		show_zero = 1;
	}
	else
		show_number_in_line2(0xff,6, 0);
	
	vol -= dis_value*10;	
	show_number_in_line2(vol,7, 0);
	
	show_units(LETTER_UNIT_OFF);
}

void show_letter_set_in_line1(void)
{
	show_letter_in_line1('S',0);
	show_letter_in_line1('E',1);
	show_letter_in_line1('T',2);
	show_letter_v(0);
	show_letter_addr(0);
}

void show_node_index_in_line2(u8_t idx)
{
	u8_t dis_value = idx/10;
	show_number_in_line2(dis_value,0, 0);
	dis_value = idx- dis_value*10;
	show_number_in_line2(dis_value,1, 0);	
}

void show_power_consumption( u32_t ps)
{
	u8_t dis_value ;
	u32_t rest_value = ps;
	if(ps<99999)
	{
		show_letter_in_line2('P', 0, 0);
		show_number_in_line2(0xff,1, 0);
		show_number_in_line2(0xff,2, 0);
		
		dis_value = rest_value/10000;
		if(dis_value) show_number_in_line2(dis_value,3, 0);
		else          show_number_in_line2(0xff,3, 0);
		
		rest_value -= dis_value* 10000;		
		dis_value = rest_value/1000;
		show_number_in_line2(dis_value,4, 1);
		
		rest_value -= dis_value* 1000;		
		dis_value = rest_value/100;
		show_number_in_line2(dis_value,5, 0);
		
		rest_value -= dis_value* 100;		
		dis_value = rest_value/10;
		show_number_in_line2(dis_value,6, 0);
		rest_value -= dis_value* 10;		
		dis_value = rest_value;
		show_number_in_line2(dis_value,7, 0);
		
	}
	else if(ps<999999)
	{
		show_letter_in_line2('P', 0, 0);
		show_number_in_line2(0xff,1, 0);
		show_number_in_line2(0xff,2, 0);
		
		dis_value = rest_value/100000;
		show_number_in_line2(dis_value,3, 0);
		
		rest_value -= dis_value* 100000;		
		dis_value = rest_value/10000;
		show_number_in_line2(dis_value,4, 0);
		
		rest_value -= dis_value* 10000;		
		dis_value = rest_value/1000;
		show_number_in_line2(dis_value,5, 1);
		
		rest_value -= dis_value* 1000;		
		dis_value = rest_value/100;
		show_number_in_line2(dis_value,6, 0);
		
		rest_value -= dis_value* 100;		
		dis_value = rest_value/10;
		show_number_in_line2(dis_value,7, 0);
	}
	else if(ps<9999999)
	{
		show_letter_in_line2('P', 0, 0);
		show_number_in_line2(0xff,1, 0);
		show_number_in_line2(0xff,2, 0);
		
		dis_value = rest_value/1000000;
		show_number_in_line2(dis_value,3, 0);
		
		rest_value -= dis_value* 1000000;		
		dis_value = rest_value/100000;
		show_number_in_line2(dis_value,4, 0);
		
		rest_value -= dis_value* 100000;		
		dis_value = rest_value/10000;
		show_number_in_line2(dis_value,5, 0);
		
		rest_value -= dis_value* 10000;		
		dis_value = rest_value/1000;
		show_number_in_line2(dis_value,6, 1);
		
		rest_value -= dis_value* 1000;		
		dis_value = rest_value/100;
		show_number_in_line2(dis_value,7, 0);
	}
	else if(ps<99999999)
	{
		
		dis_value = rest_value/10000000;
		show_number_in_line2(dis_value, 0, 0);
		
		rest_value -= dis_value* 10000000;		
		dis_value = rest_value/1000000;
		show_number_in_line2(dis_value, 1, 0);
		
		rest_value -= dis_value* 1000000;		
		dis_value = rest_value/100000;
		show_number_in_line2(dis_value, 2, 0);
		
		rest_value -= dis_value* 100000;		
		dis_value = rest_value/10000;
		show_number_in_line2(dis_value, 3, 0);
		
		rest_value -= dis_value* 10000;		
		dis_value = rest_value/1000;
		show_number_in_line2(dis_value, 4, 1);
		
		rest_value -= dis_value* 1000;		
		dis_value = rest_value/100;
		show_number_in_line2(dis_value, 5, 0);
		
		rest_value -= dis_value* 100;		
		dis_value = rest_value/10;
		show_number_in_line2(dis_value, 6, 0);
		
		rest_value -= dis_value* 10;		
		dis_value = rest_value;
		show_number_in_line2(dis_value, 7, 0);
	}
	else if(ps<999999999)
	{		
		dis_value = rest_value/100000000;
		show_number_in_line2(dis_value,0, 0);
		
		rest_value -= dis_value* 100000000;		
		dis_value = rest_value/10000000;
		show_number_in_line2(dis_value,1, 0);
		
		rest_value -= dis_value* 10000000;		
		dis_value = rest_value/1000000;
		show_number_in_line2(dis_value,2, 0);
		
		rest_value -= dis_value* 1000000;		
		dis_value = rest_value/100000;
		show_number_in_line2(dis_value,3, 0);
		
		rest_value -= dis_value* 100000;		
		dis_value = rest_value/10000;
		show_number_in_line2(dis_value,4, 0);
		
		rest_value -= dis_value* 10000;		
		dis_value = rest_value/1000;
		show_number_in_line2(dis_value,5, 1);
		
		rest_value -= dis_value* 1000;		
		dis_value = rest_value/100;
		show_number_in_line2(dis_value,6, 0);
		
		rest_value -= dis_value* 100;		
		dis_value = rest_value/10;
		show_number_in_line2(dis_value,7, 0);
	}
	else
	{		
		dis_value = rest_value/1000000000;
		show_number_in_line2(dis_value,0, 0);
		
		rest_value -= dis_value* 1000000000;		
		dis_value = rest_value/100000000;
		show_number_in_line2(dis_value,1, 0);
		
		rest_value -= dis_value* 10000000;		
		dis_value = rest_value/1000000;
		show_number_in_line2(dis_value,2, 0);
		
		rest_value -= dis_value* 1000000;		
		dis_value = rest_value/100000;
		show_number_in_line2(dis_value,3, 0);
		
		rest_value -= dis_value* 100000;		
		dis_value = rest_value/10000;
		show_number_in_line2(dis_value,4, 0);
		
		rest_value -= dis_value* 10000;		
		dis_value = rest_value/1000;
		show_number_in_line2(dis_value,5, 1);
		
		rest_value -= dis_value* 1000;		
		dis_value = rest_value/100;
		show_number_in_line2(dis_value,6, 0);
		
		rest_value -= dis_value* 100;		
		dis_value = rest_value/10;
		show_number_in_line2(dis_value,7, 0);
	}
	
	show_units(LETTER_UNIT_KWh);
}

void show_temperature(u32_t temp)
{
	u8_t dis_value;
	u8_t show_zero = 0;
	
	show_letter_in_line2('T', 0, 0);
	show_number_in_line2(0xff,1, 0);
	show_number_in_line2(0xff,2, 0);
	show_number_in_line2(0xff,3, 0);
		
	dis_value = temp>=1000? temp/1000:0;
	if(dis_value) {
		show_number_in_line2(dis_value,4, 0);
		show_zero = 1;
	}
	else
		show_number_in_line2(0xff,4, 0);
	
	temp -= dis_value* 1000;		
	dis_value = temp>=100? temp/100:0;
	if(dis_value ||show_zero) {
		show_number_in_line2(dis_value,5, 0);
		show_zero = 1;
	}
	else
		show_number_in_line2(0xff,5, 1);
	
	temp -= dis_value* 100;	
	dis_value = temp>=10? temp/10:0;
	show_number_in_line2(dis_value,6, 1);
	
	temp -= dis_value*10;	
	show_number_in_line2(temp,7, 0);
	
	show_units(LETTER_UNIT_DEGREE);
}


void show_power(u32_t pwr)
{
	u8_t dis_value;
	
	show_letter_in_line2('P', 0, 0);
	show_number_in_line2(0xff,1, 0);
	show_number_in_line2(0xff,2, 0);
	
	dis_value = pwr>=10000? pwr/10000:0;
	if(dis_value)
		show_number_in_line2(dis_value,3, 0);
	else
		show_number_in_line2(0xff,3, 0);
	
	pwr -= dis_value* 10000;	
	dis_value = pwr/1000;
	show_number_in_line2(dis_value,4, 1);
	
	pwr -= dis_value* 1000;		
	dis_value = pwr/100;
	show_number_in_line2(dis_value,5, 0);
	
	pwr -= dis_value* 100;	
	dis_value = pwr/10;
	show_number_in_line2(dis_value,6, 0);
	
	pwr -= dis_value*10;	
	show_number_in_line2(pwr,7, 0);
	
	show_units(LETTER_UNIT_kW);
}

void show_current(u32_t temp)
{
	u8_t dis_value;
	u8_t show_zero = 0;
	
	show_letter_in_line2('C', 0, 0);
	show_number_in_line2(0xff,1, 0);
	show_number_in_line2(0xff,2, 0);
	
	dis_value = temp>=10000? temp/10000:0;
	if(dis_value) {
		show_number_in_line2(dis_value,3, 0);
		show_zero = 1;
	}
	else
		show_number_in_line2(0xff,3, 0);
	
	temp -= dis_value* 10000;	
	dis_value = temp>=1000? temp/1000:0;
	if(dis_value ||show_zero) {
		show_number_in_line2(dis_value,4, 0);
		show_zero = 1;
	}
	else
		show_number_in_line2(0xff,4, 0);
	
	temp -= dis_value* 1000;		
	dis_value = temp>=100? temp/100:0;
	show_number_in_line2(dis_value,5, 1);
	
	temp -= dis_value* 100;	
	dis_value = temp>=10? temp/10:0;
	show_number_in_line2(dis_value,6, 0);
	
	temp -= dis_value*10;	
	show_number_in_line2(temp,7, 0);
	
	show_units(LETTER_UNIT_A);
}

void show_leakage_current(u32_t lki)
{
	u8_t dis_value;
	u8_t show_zero = 0;
	
	show_letter_in_line2('L', 0, 0);
	show_number_in_line2(0xff,1, 0);
	show_number_in_line2(0xff,2, 0);
	
	dis_value = lki>=10000?lki/10000:0;
	if(dis_value) {
		show_number_in_line2(dis_value,3, 0);
		show_zero = 1;
	}
	else
		show_number_in_line2(0xff,3, 0);
	
	lki -= dis_value* 10000;
	dis_value = lki>=1000?lki/1000:0;
	if(dis_value ||show_zero) {
		show_number_in_line2(dis_value,4, 0);
		show_zero = 1;
	}
	else
		show_number_in_line2(0xff,4, 0);
	
	lki -= dis_value* 1000;		
	dis_value = lki>=100?lki/100:0;	
	if(dis_value ||show_zero) {
		show_number_in_line2(dis_value,5, 0);
		show_zero = 1;
	}
	else 
		show_number_in_line2(0xff,5, 0);
	
	lki -= dis_value* 100;
	dis_value = lki>=10?lki/10:0;	
	show_number_in_line2(dis_value,6, 1);
	
	lki -= dis_value*10;	
	show_number_in_line2(lki,7, 0);
	
	show_units(LETTER_UNIT_mA);
}

void show_current_rate(u8_t rate)
{
	u8_t dis_value = rate;
	
	show_letter_in_line2('R', 0, 0);
	show_number_in_line2(0xff,1, 0);
	show_number_in_line2(0xff,2, 0);
	show_number_in_line2(0xff,3, 0);
	show_number_in_line2(0xff,4, 0);
		
	dis_value = rate>=100? rate/100 :0;	
	show_number_in_line2(dis_value,5, 0);
	
	rate -= dis_value* 100;
	dis_value = rate>=10?rate/10:0;	
	show_number_in_line2(dis_value,6, 0);
	
	rate -= dis_value*10;	
	show_number_in_line2(rate,7, 0);
	
	show_units(LETTER_UNIT_OFF);
}

void show_letter_lh(u8_t h)
{
	if(h)
		show_letter_in_line2('H', 1, 0);
	else
		show_letter_in_line2('L', 1, 0);
}

void show_addrss_number(u8_t idx)
{
	show_node_index_in_line2(idx);	
	show_number_in_line2(0xff,2, 0);
	show_number_in_line2(0xff,3, 0);
	show_number_in_line2(0xff,4, 0);
	show_number_in_line2(0xff,5, 0);
	show_number_in_line2(0xff,6, 0);
	show_number_in_line2(0xff,7, 0);
	show_units(LETTER_UNIT_OFF);
}

void e9lcd_init(void)
{
	rt623832_init(1);
}


extern unsigned int systicks;

void e9lcd_show_status(u32_t time)
{
	sw_status_t nsta;
	if((time&0x7) == 0)
	{
		if(is_skip_enable())
			__nop();
		else{
			switch_node_index(DIR_AHEAD_NO_SKIP);
		}
	}
  nsta.cur=12;
	nsta.vol=102;
	nsta.lki=18;
	nsta.tmp=43;
	show_voltage_line1(nsta.vol);
	switch(systicks/200)
	{
		case 0:
		case 1:
			show_current( nsta.cur);
			break;
		case 2:
		case 3:
			show_leakage_current(nsta.lki);
			break;
		case 4:
		case 5:
			show_temperature(nsta.tmp);
			break;
		case 6:
		case 7:
			systicks=0;
			show_power_consumption(nsta.psh<<16|nsta.psl);
			//show_power(nsta.pwr);
			break;
	}
	show_alarms(0, nsta.alm);
}



