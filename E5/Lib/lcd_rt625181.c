#include "lcd_rt625181.h"
#include "System_Config.h"
#include "Tem_ADC.h"
LCD_Ram_Info glcd_raminfo;
//private 
void Disp_Parame(void)
{
	memset((char*)&glcd_raminfo,0,sizeof(LCD_Ram_Info));
}

void Disp_Ram(unsigned char add_index,unsigned char data)
{
	ht1622x_send_data(add_index,data);
	ht1622x_send_data(add_index+1,data<<4);
}
/*
void Disp_Ram1(unsigned char add_index,unsigned char data)
{
	ht1622x_send_data(add_index,data);
	ht1622x_send_data(add_index+2,data<<4);
}
 */  
/*****************************************************
*
*
*
*****************************************************/

#define DISP_CHANNEL_A    1u
#define DISP_CHANNEL_B    2u
#define DISP_CHANNEL_C    3u

void Disp_ABC(unsigned    char channel,char enable)
{
   #if 1
	
	if(enable){

	            //A channel
	            if(channel==DISP_CHANNEL_A){
								glcd_raminfo.data[0]  &=0x80;
								glcd_raminfo.data[24] &=0xee;
								glcd_raminfo.data[0]  |=0x81;
								glcd_raminfo.data[24] |=0x11;
								Disp_Ram(0,glcd_raminfo.data[0]);	
								Disp_Ram(24,glcd_raminfo.data[24]);
	            }
		        //B channel		
		        else if(channel==DISP_CHANNEL_B){
								glcd_raminfo.data[0] &=0x80;
								glcd_raminfo.data[24]&=0xdd;
								glcd_raminfo.data[0] |=0x22;
								glcd_raminfo.data[24]|=0x22;
								Disp_Ram(0,glcd_raminfo.data[0]);	
								Disp_Ram(24,glcd_raminfo.data[24]);
		        }else if(channel==DISP_CHANNEL_C){
								glcd_raminfo.data[0] &=0x8b;
								glcd_raminfo.data[24]&=0xbb;
								glcd_raminfo.data[0] |=0x44;
								glcd_raminfo.data[24]|=0x44;
								Disp_Ram(0,glcd_raminfo.data[0]);	
								Disp_Ram(24,glcd_raminfo.data[24]);
		        }
	}else{
		        //A channel
		        if(channel==DISP_CHANNEL_A){
					glcd_raminfo.data[0]  &=0x8e;
					glcd_raminfo.data[24] &=0xee;
					Disp_Ram(0,glcd_raminfo.data[0]); 
					Disp_Ram(24,glcd_raminfo.data[24]);
		        }
                //B channel
                else if(channel==DISP_CHANNEL_B){
					glcd_raminfo.data[0]  &=0xdd;
					glcd_raminfo.data[24] &=0xdd;
					Disp_Ram(0,glcd_raminfo.data[0]);	
					Disp_Ram(24,glcd_raminfo.data[24]);			
                }else if(channel==DISP_CHANNEL_C){
					glcd_raminfo.data[0] &=0xbb;
					glcd_raminfo.data[24]&=0xbb;
					Disp_Ram(0,glcd_raminfo.data[0]);	
					Disp_Ram(24,glcd_raminfo.data[24]);
                }
                
	}
	#endif
}



void Disp_Sig(unsigned char cmd,unsigned char enable)
{
    //too hot
	glcd_raminfo.data[18]  &=0xf0;	
	if(cmd==LOSELEC){
		glcd_raminfo.data[18]  |=0x02;	
	    Disp_Ram(18,glcd_raminfo.data[18]); 
	}else if(cmd==OVERCURRENT){
	    glcd_raminfo.data[18]  |=0x04;	
	    Disp_Ram(18,glcd_raminfo.data[18]); 
	}else if(cmd==OVERVOL){
		if(enable){
	    	glcd_raminfo.data[18]  |=0x08;	
		}else{
			glcd_raminfo.data[18]  &=~0x08;
		}
		Disp_Ram(18,glcd_raminfo.data[18]); 
		glcd_raminfo.data[22]  |=0x10;	
	    Disp_Ram(22,glcd_raminfo.data[22]); 
	}else if(cmd==MissVols){
		if(enable){
	    	glcd_raminfo.data[20]  |=0x08;
		}else{
		    glcd_raminfo.data[20]  &=~0x08;
		}
	    Disp_Ram(20,glcd_raminfo.data[20]); 
	}else if(cmd==SHORTCIR){
	    glcd_raminfo.data[22]  |=0x04;	
	    Disp_Ram(22,glcd_raminfo.data[22]); 
		glcd_raminfo.data[22]  |=0x10;	
	    Disp_Ram(22,glcd_raminfo.data[22]); 
	}else if(cmd==TOOHOT){
		if(enable){
		glcd_raminfo.data[18]  |=0x01;	 
		}else{
		glcd_raminfo.data[18]  &=0xfe;	 
		}	
		Disp_Ram(18,glcd_raminfo.data[18]); 
	}else if(cmd==LEAKCURRENT){
		if(enable){
		glcd_raminfo.data[18]  |=0x02;	 
		}else{
		glcd_raminfo.data[18]  &=0xfd;	 
		}	
		Disp_Ram(18,glcd_raminfo.data[18]); 
	}
	

}


/***********************************************/
/*
    a
  f   b
    g
  e   c
    d    
*/ 
/**********************************************/       
const unsigned char  number_table1[10]={0x0b,0x0f,0x03,0x0f,0x0b,0x02,0x09,0x0d,0x00,0x0f};	
const unsigned char  number_table2[10]={0x07,0x07,0x05,0x06,0x06,0x07,0x07,0x03,0x05,0x05};


const unsigned char  number_table3[10]={0x7b,0x7f,0x53,0x6f,0x6b,0x72,0x79,0x3d,0x50,0x5f};
const unsigned char  number_table4[10]={0xb7,0xf7,0x35,0xf6,0xb6,0x27,0x97,0xd3,0x05,0xf5};
void Disp_Number1(unsigned char channel,unsigned      char number)
{       
     
	if(channel==1){
       if(gSystem_Info.temperenable){
		   glcd_raminfo.data[2]   &=0xf0;
		   glcd_raminfo.data[4]   &=0xf8;
       	}else{
			if(number==11){		
			glcd_raminfo.data[2]   &=0xf0;
			glcd_raminfo.data[4]   &=0xf8;
			glcd_raminfo.data[4]   |=0x02;
			}else{
			glcd_raminfo.data[2]   &=0xf0;
			glcd_raminfo.data[4]   &=0xf8;
			glcd_raminfo.data[2]   |=number_table1[9-number];
			glcd_raminfo.data[4]   |=number_table2[9-number];
			}
       	}
	   Disp_Ram(2,glcd_raminfo.data[2]);
 	   Disp_Ram(4,glcd_raminfo.data[4]);
	}else if(channel==2){
		if(gSystem_Info.temperenable){
			glcd_raminfo.data[6]   &=0xf0;
			glcd_raminfo.data[8]   &=0xf8;
		}else{
			glcd_raminfo.data[6]   &=0xf0;
			glcd_raminfo.data[8]   &=0xf8;
			glcd_raminfo.data[6]   |=number_table1[9-number];
			glcd_raminfo.data[8]   |=number_table2[9-number];
		}
		Disp_Ram(6,glcd_raminfo.data[6]);
		Disp_Ram(8,glcd_raminfo.data[8]);
	}else if(channel==3){
		if(gSystem_Info.temperenable){
			glcd_raminfo.data[10]   &=0xf0;
			glcd_raminfo.data[12]   &=0xf8;
		}else{		
			glcd_raminfo.data[10]   &=0xf0;
			glcd_raminfo.data[12]   &=0xf8;
			glcd_raminfo.data[10]   |=number_table1[9-number];
			glcd_raminfo.data[12]   |=number_table2[9-number];
		}
		Disp_Ram(10,glcd_raminfo.data[10]);
		Disp_Ram(12,glcd_raminfo.data[12]);
	}else if(channel==4){
		if(gSystem_Info.temperenable){

			}else{
				glcd_raminfo.data[14]   &=0xf0;
				glcd_raminfo.data[16]   &=0xf8;
				glcd_raminfo.data[14]   |=number_table1[9-number];
				glcd_raminfo.data[16]   |=number_table2[9-number];
	    }
		Disp_Ram(14,glcd_raminfo.data[14]);
		Disp_Ram(16,glcd_raminfo.data[16]);
	}
	//dot
	glcd_raminfo.data[12]   |=0x08;
	Disp_Ram(12,glcd_raminfo.data[12]);
}



//public 
void Disp_Temper_Unit(void)
{
    //temper
    glcd_raminfo.data[16]  |=0x08;	
	Disp_Ram(16,glcd_raminfo.data[16]);	
	//K3
	glcd_raminfo.data[12]  |=0x08;	
	Disp_Ram(12,glcd_raminfo.data[12]);	

	//T33
	glcd_raminfo.data[0]  |=0x88;	
	Disp_Ram(0,glcd_raminfo.data[0]);	
	//T34

	//T16 
	glcd_raminfo.data[40]  |=0x88;	
	Disp_Ram(40,glcd_raminfo.data[40]);

    //T34
	glcd_raminfo.data[24]  |=0x08;	
	Disp_Ram(24,glcd_raminfo.data[24]);

	//k28
	glcd_raminfo.data[29]  &=0x70f;	
	Disp_Ram(29,glcd_raminfo.data[29]);

	//k11 k15
	glcd_raminfo.data[36]  |=0x88;	
	Disp_Ram(36,glcd_raminfo.data[36]);
	
    //T14
	glcd_raminfo.data[17]  |=0x80;	
	Disp_Ram(17,glcd_raminfo.data[17]);	

	//T13
	glcd_raminfo.data[16]  |=0x88;	
	Disp_Ram(16,glcd_raminfo.data[16]); 
	
    //m1
	glcd_raminfo.data[17]  |=0x08;	
	Disp_Ram(17,glcd_raminfo.data[17]); 
	//m2 T34
	glcd_raminfo.data[40]  |=0x08;	
	Disp_Ram(40,glcd_raminfo.data[40]); 

	//17 number clear
	glcd_raminfo.data[18]&=0xbf;	   
	glcd_raminfo.data[20]&=0x7f;
	//Disp_Ram1(18,glcd_raminfo.data[18]);	
	//Disp_Ram1(20,glcd_raminfo.data[20]);

	//k7
	glcd_raminfo.data[12]|=0x80;	   
	Disp_Ram(12,glcd_raminfo.data[12]); 
 
    //17 number
    glcd_raminfo.data[18]&=0x0f;	 
	glcd_raminfo.data[20]&=0x8f;  
	Disp_Ram(18,glcd_raminfo.data[18]); 
	Disp_Ram(20,glcd_raminfo.data[20]); 

	//18 number
	glcd_raminfo.data[42]&=0x0f;	 
	glcd_raminfo.data[44]&=0x1f;  
	//Disp_Ram(42,glcd_raminfo.data[42]); 
	//Disp_Ram(44,glcd_raminfo.data[44]); 

	//T15
    //k28
	glcd_raminfo.data[24]&=0x7f;	   
	//Disp_Ram1(24,glcd_raminfo.data[24]);

	//glcd_raminfo.data[42]|=0x08;	   
	//Disp_Ram1(42,glcd_raminfo.data[42]);
	//glcd_raminfo.data[44]|=0x10;	   
	//Disp_Ram1(44,glcd_raminfo.data[44]);

	//clear 17
	glcd_raminfo.data[18]&=0x0f;	   
	//Disp_Ram1(18,glcd_raminfo.data[18]);
	glcd_raminfo.data[20]&=0x8f;	   
	//Disp_Ram1(20,glcd_raminfo.data[20]);

    //T29 T32
    glcd_raminfo.data[42]  &=0x07;	
	//Disp_Ram(42,glcd_raminfo.data[42]);
	glcd_raminfo.data[44]  &=0x1f;	
	//Disp_Ram(44,glcd_raminfo.data[44]);

	//k24 k27
	glcd_raminfo.data[54]  &=0x00;	
	glcd_raminfo.data[58]  &=0x00;	
	//Disp_Ram(54,glcd_raminfo.data[54]);
	Disp_Ram(58,glcd_raminfo.data[58]);

	//V
	glcd_raminfo.data[22]  &=0x00;
	Disp_Ram(22,glcd_raminfo.data[22]);

	//T29 add
	glcd_raminfo.data[42]  |=0x08;
	//Disp_Ram(42,glcd_raminfo.data[42]);

	//T15 clear 
	glcd_raminfo.data[40]  &=0xf7;	
	Disp_Ram(40,glcd_raminfo.data[40]);

	//k10
	glcd_raminfo.data[32]  |=0x08;
	Disp_Ram(32,glcd_raminfo.data[32]);

     //K11
	glcd_raminfo.data[36]  &=0xf7;
	Disp_Ram(36,glcd_raminfo.data[36]);

	//k26
	glcd_raminfo.data[46]  &=0xf7;
	//Disp_Ram(46,glcd_raminfo.data[46]);

	//K25
	glcd_raminfo.data[54]  |=0x08;
	//Disp_Ram(54,glcd_raminfo.data[54]);

	
	//K25
	glcd_raminfo.data[50]  &=0xf7;
	//Disp_Ram(50,glcd_raminfo.data[50]);
}




/**************************************************************/


void changeChannels(char channel)
{
	if(DISP_CHANNEL_A==channel){
        
	}else if(DISP_CHANNEL_B==channel){

	}else if(DISP_CHANNEL_C==channel){

	}
}

/*************************************************************/
unsigned int gdispSystick=0;
void dispSystick(void)
{
   gdispSystick++;
}

/*************************************************************
*
*
*
*************************************************************/



	const unsigned char  number_table5[10]={0xb0,0xf0,0x30,0xf0,0xb0,0x20, 0x90,0xd0,0x00,0xf0};	
	const unsigned char  number_table6[10]={0x70,0x70,0x50,0x60,0x60,0x70, 0x70,0x30,0x50,0x50};

	void Disp_Number2(unsigned char channel,unsigned char number)
	{
		 if(channel==1){
		 	 if(gSystem_Info.leakcurenable){
				 glcd_raminfo.data[2]&=0x0f;	
				 glcd_raminfo.data[4]&=0x80;
		 	 }else{
			 	 glcd_raminfo.data[2]&=0x0f;	
				 glcd_raminfo.data[4]&=0x80;	
				 glcd_raminfo.data[2]|=number_table5[9-number];	
				 glcd_raminfo.data[4]|=number_table6[9-number];	
		 	 }
			 Disp_Ram(2,glcd_raminfo.data[2]);	 
			 Disp_Ram(4,glcd_raminfo.data[4]);
		 }else if(channel==2){
		 	 if(gSystem_Info.leakcurenable){
				 glcd_raminfo.data[6]&=0x0f;	
				 glcd_raminfo.data[8]&=0x80;
		 	 }else{
		 	 glcd_raminfo.data[6]&=0x0f;	
			 glcd_raminfo.data[8]&=0x80;
			 glcd_raminfo.data[6]|=number_table5[9-number];		
			 glcd_raminfo.data[8]|=number_table6[9-number];
		 	 }
			 Disp_Ram(6,glcd_raminfo.data[6]);	 
			 Disp_Ram(8,glcd_raminfo.data[8]);
		 }else if(channel==3){
		 	 if(gSystem_Info.leakcurenable){
				 glcd_raminfo.data[10]&=0x0f;	
				 glcd_raminfo.data[12]&=0x80;
		 	 }else{
		 	 glcd_raminfo.data[10]&=0x0f;	
			 glcd_raminfo.data[12]&=0x80;
			 glcd_raminfo.data[10]|=number_table5[9-number];
			 glcd_raminfo.data[12]|=number_table6[9-number];
		 	 }
			 Disp_Ram(10,glcd_raminfo.data[10]);	 
			 Disp_Ram(12,glcd_raminfo.data[12]);
		 }else if(channel==4){
		 	 if(gSystem_Info.leakcurenable){
				 glcd_raminfo.data[14]&=0x0f;	
				 glcd_raminfo.data[16]&=0x88;
		 	 }else{
		 	 glcd_raminfo.data[14]&=0x0f;	
			 glcd_raminfo.data[16]&=0x88;
			 glcd_raminfo.data[14]|=number_table5[9-number];
			 glcd_raminfo.data[16]|=number_table6[9-number];
		 	 }
			 Disp_Ram(14,glcd_raminfo.data[14]);	 
			 Disp_Ram(16,glcd_raminfo.data[16]);
		 }
	}


	
	void Disp_Number3(unsigned char channel,unsigned char number)
	{
		 if(channel==1){
			if(gSystem_Info.cursetmodenable){
			glcd_raminfo.data[26]&=0xf0;   
			glcd_raminfo.data[28]&=0xf8;   
			}else{
			glcd_raminfo.data[26]&=0xf0;	
			glcd_raminfo.data[28]&=0xf8;	
			glcd_raminfo.data[26]|=number_table1[9-number]; //number)table1
			glcd_raminfo.data[28]|=number_table2[9-number]; //table2
			}
			 Disp_Ram(26,glcd_raminfo.data[26]);	 
			 Disp_Ram(28,glcd_raminfo.data[28]);
		 }else if(channel==2){
			if(gSystem_Info.cursetmodenable){
			glcd_raminfo.data[30]&=0xf0;	
			glcd_raminfo.data[32]&=0xf8;
			}else{
			glcd_raminfo.data[30]&=0xf0;	
			glcd_raminfo.data[32]&=0xf8;
			glcd_raminfo.data[30]|=number_table1[9-number]; 	
			glcd_raminfo.data[32]|=number_table2[9-number];
			}
			 Disp_Ram(30,glcd_raminfo.data[30]);	 
			 Disp_Ram(32,glcd_raminfo.data[32]);
		 }else if(channel==3){
			if(gSystem_Info.cursetmodenable){
			glcd_raminfo.data[34]&=0xf0;	
			glcd_raminfo.data[36]&=0xf8;
			}else{
			glcd_raminfo.data[34]&=0xf0;	
			glcd_raminfo.data[36]&=0xf8;
			glcd_raminfo.data[34]|=number_table1[9-number];
			glcd_raminfo.data[36]|=number_table2[9-number];
			}
			 Disp_Ram(34,glcd_raminfo.data[34]);	 
			 Disp_Ram(36,glcd_raminfo.data[36]);
		 }else if(channel==4){
			if(gSystem_Info.cursetmodenable){
			glcd_raminfo.data[38]&=0xf0;	
			glcd_raminfo.data[40]&=0xf8;
			}else{
			glcd_raminfo.data[38]&=0xf0;	
			glcd_raminfo.data[40]&=0xf8;
			glcd_raminfo.data[38]|=number_table1[9-number];
			glcd_raminfo.data[40]|=number_table2[9-number];
			}
			Disp_Ram(38,glcd_raminfo.data[38]);  
			Disp_Ram(40,glcd_raminfo.data[40]);
		 }
	}

	void Disp_Number4(unsigned char channel,unsigned char number)
	{
		//18 clear 
		if(channel==1){
			if(gSystem_Info.volsetmodenable){
			glcd_raminfo.data[26]&=0x0f; 
			glcd_raminfo.data[28]&=0x88; 
			}else{
			glcd_raminfo.data[26]&=0x0f; 
			glcd_raminfo.data[28]&=0x88; 
			glcd_raminfo.data[26]|=number_table5[9-number]; 
			glcd_raminfo.data[28]|=number_table6[9-number]; 
			}
			//Disp_Ram(26,glcd_raminfo.data[26]);	
			//Disp_Ram(28,glcd_raminfo.data[28]);
		}else if(channel==2){
			if(gSystem_Info.volsetmodenable){
			glcd_raminfo.data[30]&=0x0f;
			glcd_raminfo.data[32]&=0x8f;
			}else{
			glcd_raminfo.data[30]&=0x0f;
			glcd_raminfo.data[32]&=0x8f;
			glcd_raminfo.data[30]|=number_table5[9-number];    
			glcd_raminfo.data[32]|=number_table6[9-number];
		    }
		    Disp_Ram(30,glcd_raminfo.data[30]);				
		    Disp_Ram(32,glcd_raminfo.data[32]);	
		}else if(channel==3){
			if(gSystem_Info.volsetmodenable){
			glcd_raminfo.data[34]&=0x0f;
			glcd_raminfo.data[36]&=0x88;
			}else{
			glcd_raminfo.data[34]&=0x0f;
			glcd_raminfo.data[36]&=0x88;
			glcd_raminfo.data[34]|=number_table5[9-number];
			glcd_raminfo.data[36]|=number_table6[9-number];
			}
			Disp_Ram(34,glcd_raminfo.data[34]);	
			Disp_Ram(36,glcd_raminfo.data[36]);
		}else if(channel==4){
			if(gSystem_Info.volsetmodenable){
			glcd_raminfo.data[38]&=0x0f;
			glcd_raminfo.data[40]&=0x88;
			}else{
			glcd_raminfo.data[38]&=0x0f;
			glcd_raminfo.data[40]&=0x88;
			glcd_raminfo.data[38]|=number_table5[9-number];
			glcd_raminfo.data[40]|=number_table6[9-number];
			}
			Disp_Ram(38,glcd_raminfo.data[38]);	
			Disp_Ram(40,glcd_raminfo.data[40]);
		}
	}

void dispVol(float temp)
{ 
	unsigned char _data[10]={0}; 
	unsigned int dot_pos=0;
	sprintf(_data,"%3.1f",temp);
	for(int i=0;i<4;i++){
		if(_data[i]==0x2E){
		dot_pos=i;
		}
	}
		if(dot_pos==1){
		_data[0]=_data[0]-0x30;
		_data[1]=_data[1]-0x30;
		_data[2]=_data[2]-0x30;
		_data[4]=_data[4]-0x30;
		Disp_Number4(1,0); 
		Disp_Number4(2,0); 
		Disp_Number4(3,_data[0]);
		Disp_Number4(4,_data[2]);
	}else{
		_data[0]=_data[0]-0x30;
		_data[1]=_data[1]-0x30;
		_data[2]=_data[2]-0x30;
		_data[4]=_data[4]-0x30;
	 
		Disp_Number4(2,_data[0]); 
		Disp_Number4(3,_data[1]);
		Disp_Number4(4,_data[2]);
	}
	//glcd_raminfo.data[36]  |=0x80;	
	//Disp_Ram(36,glcd_raminfo.data[36]);
}
void Disp_Temper(float tem)
{	 
	unsigned char _data[10]={0};
	unsigned char _temp=0;  

	sprintf(_data,"%3.1f",tem);	
  if(tem<0)
  {
			Disp_Number1(1,11u);
			_temp=_data[1]-0x30;
			Disp_Number1(2, _data[1]-0x30);
			_temp=_data[2]-0x30;
			Disp_Number1(3, _data[2]-0x30);	
			_temp=_data[4]-0x30;
			Disp_Number1(4, _data[4]-0x30);		
	}else{	
		if(_data[3]==0x2E){
			_temp=_data[0]-0x30;
			Disp_Number1(1,_temp);
			_temp=_data[1]-0x30;
			Disp_Number1(2,_temp);
			_temp=_data[2]-0x30;
			Disp_Number1(3,_temp);
			_temp=_data[4]-0x30;
			Disp_Number1(4,_temp);
		}else{
			Disp_Number1(1,0u);
			_temp=_data[0]-0x30;
			Disp_Number1(2,_temp);
			_temp=_data[1]-0x30;
			Disp_Number1(3,_temp);	   
			_temp=_data[3]-0x30;
			Disp_Number1(4,_temp);	
		}				
	}	
}
void Disp_Unit(void)
{
	glcd_raminfo.data[16]  |=0x08;	
	Disp_Ram(16,glcd_raminfo.data[16]);
	glcd_raminfo.data[16]  |=0x08;	
	Disp_Ram(40,glcd_raminfo.data[40]);
}



void dispDot(char channel,char pos)
{


}
void dispMA1(float temp)
{
	unsigned char _data[10]={0};  
	unsigned int dot_pos=0;
	temp=temp*1.0F;
	sprintf(_data,"%4.1f",temp);
	for(int i=0;i<5;i++){		 
		if(_data[i]==0x2E){
			dot_pos=i;
		}
	}
	if(dot_pos==1){
		_data[0]=_data[0]-0x30;
		_data[1]=_data[1]-0x30;
		_data[2]=_data[2]-0x30;
		_data[4]=_data[4]-0x30;
		Disp_Number2(1,0); 
		Disp_Number2(2,0); 
		Disp_Number2(3,_data[0]);
		Disp_Number2(4,_data[2]);
	}else if(dot_pos==2){
		_data[0]=_data[0]-0x30;
		_data[1]=_data[1]-0x30;
		_data[3]=_data[3]-0x30;
		Disp_Number2(1,0); 
		Disp_Number2(2,0); 
		if(_data[0]==0xF0){
		Disp_Number2(3,0);
		}else{
		Disp_Number2(3,_data[0]);
		}
		Disp_Number2(4,_data[1]);
	}
	else if(dot_pos==3){
		_data[0]=_data[0]-0x30;
		_data[1]=_data[1]-0x30;
		_data[2]=_data[2]-0x30;
		_data[4]=_data[4]-0x30;
		Disp_Number2(1,0); 
		Disp_Number2(2,_data[0]); 
		Disp_Number2(3,_data[1]);
		Disp_Number2(4,_data[2]);
	}else if(dot_pos==4){
		_data[0]=_data[0]-0x30;
		_data[1]=_data[1]-0x30;
		_data[2]=_data[2]-0x30;
		_data[3]=_data[3]-0x30;
		//glcd_raminfo.data[12]  |=0x80;	
	    //Disp_Ram(12,glcd_raminfo.data[12]);
	  Disp_Number2(1,_data[0]); 
	  Disp_Number2(2,_data[1]);
	  Disp_Number2(3,_data[2]);
		Disp_Number2(4,_data[3]);
	}
}

void dispMA2(float temp)
{ 
	unsigned char _data[10]={0};  
	unsigned int dot_pos=0;
	temp=temp*1.0F;
	sprintf(_data,"%3.2f",temp);
	for(int i=0;i<4;i++){
	if(_data[i]==0x2E){
	dot_pos=i;
	}
	}
	if(dot_pos==1){
	_data[0]=_data[0]-0x30;
	_data[2]=_data[2]-0x30;
	_data[3]=_data[3]-0x30;
	//Disp_Number3(1,0); 
	Disp_Number3(2,_data[0]); 
	Disp_Number3(3,_data[2]);
	Disp_Number3(4,_data[3]);
	}else if(dot_pos==2){
	_data[0]=_data[0]-0x30;
	_data[1]=_data[1]-0x30;
	_data[3]=_data[3]-0x30;
	_data[4]=_data[4]-0x30;
	//Disp_Number3(1,0); 
	Disp_Number3(1,_data[0]); 
	Disp_Number3(2,_data[1]); 
	Disp_Number3(3,_data[3]);
	Disp_Number3(4,_data[4]);
	}
	else if(dot_pos==3){
	_data[0]=_data[0]-0x30;
	_data[1]=_data[1]-0x30;
	_data[2]=_data[2]-0x30;
	_data[4]=_data[4]-0x30;
	//Disp_Number3(1,_data[0]); 
	Disp_Number3(2,_data[1]); 
	Disp_Number3(3,_data[2]);
	Disp_Number3(4,_data[4]);
	}
	else{
	_data[0]=_data[0]-0x30;
	_data[1]=_data[1]-0x30;
	_data[2]=_data[2]-0x30;
	_data[4]=_data[4]-0x30;
	Disp_Number3(1,_data[0]); 
	Disp_Number3(2,_data[1]); 
	Disp_Number3(3,_data[2]);
	Disp_Number3(4,_data[1]);
	}		
}



void disp_Flag(void)
{
	glcd_raminfo.data[16]  |=0x88;	
	Disp_Ram(16,glcd_raminfo.data[16]);

	glcd_raminfo.data[24]  |=0x08;	
	Disp_Ram(24,glcd_raminfo.data[24]);

	glcd_raminfo.data[40]  |=0x80;	
	Disp_Ram(40,glcd_raminfo.data[40]);

	//k7
	//glcd_raminfo.data[12]  |=0x80;	
	//Disp_Ram(12,glcd_raminfo.data[12]);

	//glcd_raminfo.data[36]  |=0x80;	
	//Disp_Ram(36,glcd_raminfo.data[36]);

    //18
    //glcd_raminfo.data[42]  &=0x0f;
	//glcd_raminfo.data[44]  &=0x1f;
	//Disp_Ram(42,glcd_raminfo.data[42]);
	//Disp_Ram(44,glcd_raminfo.data[44]);

	//glcd_raminfo.data[24]&=0x0f;
	//Disp_Ram(24,glcd_raminfo.data[24]);
	
	glcd_raminfo.data[32]  |=0x08;	
	Disp_Ram(32,glcd_raminfo.data[32]);


	//
	glcd_raminfo.data[46]  &=0x00;	
	Disp_Ram(46,glcd_raminfo.data[46]);
	glcd_raminfo.data[50]  &=0x00;	
	Disp_Ram(50,glcd_raminfo.data[50]);

	glcd_raminfo.data[54]&=0x08; //0xf8; 0x08
	glcd_raminfo.data[56]&=0x00;// ;0xf0; 0x,00
	Disp_Ram(54,glcd_raminfo.data[54]);
	Disp_Ram(56,glcd_raminfo.data[56]);
}

void disp_init(void)
{
	ht1622x_init();
	ht1622x_display_all(0);
    for(int i=0;i<300;i++){
      Disp_Ram(i,0);
 	}
	disp_Flag();
	#if 0
	glcd_raminfo.data[42]  &=0x0f;	
	Disp_Ram(42,glcd_raminfo.data[42]);
	glcd_raminfo.data[44]  &=0x1f;	
	Disp_Ram(44,glcd_raminfo.data[44]);

	glcd_raminfo.data[18]  &=0x0f;	
	Disp_Ram(18,glcd_raminfo.data[18]);
	glcd_raminfo.data[20]  &=0x8f;	
	Disp_Ram(20,glcd_raminfo.data[20]);
	//
	glcd_raminfo.data[36]  |=0x88;	
	Disp_Ram(36,glcd_raminfo.data[36]);
	glcd_raminfo.data[18]  &=0x0f;	
	Disp_Ram(18,glcd_raminfo.data[18]);
	glcd_raminfo.data[20]  &=0x8f;	
	Disp_Ram(20,glcd_raminfo.data[20]);
	glcd_raminfo.data[40]  |=0x80;	
	Disp_Ram(40,glcd_raminfo.data[40]);
	glcd_raminfo.data[16]  |=0x08;	
	Disp_Ram(16,glcd_raminfo.data[16]);
	Disp_Unit();
#endif
}

                                        /*0*/     /*2*/     /*4*//*5*//*6* /*7*//*8*/
const unsigned char  number_table7[10]={0x0d,0x05,0x03,0x07,0x07,0x06,0x06,0x05,0x07,0x07};	
const unsigned char  number_table8[10]={0x0f,0x00,0x0d,0x09,0x02,0x0b,0x0f,0x03,0x0f,0x0b};

void Disp_Number5(unsigned char channel,unsigned char number)
{
	if(channel==1){
	if(gEnternmodeenable[0]){
		glcd_raminfo.data[54]&=0x08; //0xf8; 0x08
		glcd_raminfo.data[56]&=0x00;// ;0xf0; 0x,00
	}else{
		glcd_raminfo.data[54]&=0x08;
		glcd_raminfo.data[56]&=0x00;
		glcd_raminfo.data[54]|=number_table7[number];	
		glcd_raminfo.data[56]|=number_table8[number];	
	}
	Disp_Ram(54,glcd_raminfo.data[54]);	 
	Disp_Ram(56,glcd_raminfo.data[56]);
	}else if(channel==2){
	if(gEnternmodeenable[1]){
		glcd_raminfo.data[50]&=0x08;
		glcd_raminfo.data[52]&=0x00;
	}else{
		glcd_raminfo.data[50]&=0x08;
		glcd_raminfo.data[52]&=0x00;
		glcd_raminfo.data[50]|=number_table7[number];		
		glcd_raminfo.data[52]|=number_table8[number];
	}
		Disp_Ram(50,glcd_raminfo.data[50]);	 
		Disp_Ram(52,glcd_raminfo.data[52]);
	}else if(channel==3){
	if(gEnternmodeenable[2]){
		glcd_raminfo.data[46]&=0x00;
		glcd_raminfo.data[48]&=0x00;
	}else{
		glcd_raminfo.data[46]&=0x00;
		glcd_raminfo.data[48]&=0x00;
		glcd_raminfo.data[46]|=number_table7[number];
		glcd_raminfo.data[48]|=number_table8[number];
		}
		Disp_Ram(46,glcd_raminfo.data[46]);	 
		Disp_Ram(48,glcd_raminfo.data[48]);
	}else if(channel==4){
	if(gEnternmodeenable[3]){
		glcd_raminfo.data[42]&=0x00;
		glcd_raminfo.data[44]&=0x00;
	}
	else{
		glcd_raminfo.data[42]&=0x00;//0x0f;
		glcd_raminfo.data[44]&=0x00;//0x1f;
		glcd_raminfo.data[42]|=number_table7[number];
		glcd_raminfo.data[44]|=number_table8[number];
	}
	Disp_Ram(42,glcd_raminfo.data[42]);	 
	Disp_Ram(44,glcd_raminfo.data[44]);
	}
}


void dispPower(float tem)
{
	unsigned char _data[5]={0};  
	unsigned int dot_pos=0;
	tem=tem*1.0F;
	sprintf(_data,"%1.3f",tem);
	if(_data[1]==0x2E){
		_data[0]=_data[0]-0x30;
		_data[2]=_data[2]-0x30;
		_data[3]=_data[3]-0x30;
		_data[4]=_data[4]-0x30;
		Disp_Number5(1,_data[0]); 
		Disp_Number5(2,_data[2]); 
		Disp_Number5(3,_data[3]);
		Disp_Number5(4,_data[4]);
		
	}
	else if(_data[2]==0x2E){
		_data[0]=_data[0]-0x30;
		_data[1]=_data[1]-0x30;
		_data[3]=_data[3]-0x30;
		Disp_Number5(1,0); 
		Disp_Number5(2,_data[0]); 
		Disp_Number5(3,_data[1]);
		Disp_Number5(4,_data[3]);
	}
	else{
		_data[0]=_data[0]-0x30;
		_data[1]=_data[1]-0x30;
		_data[2]=_data[2]-0x30;
		_data[4]=_data[4]-0x30;
		Disp_Number5(1,_data[0]); 
		Disp_Number5(2,_data[1]); 
		Disp_Number5(3,_data[2]);
		Disp_Number5(4,_data[4]);
	}
}

void disp_Power2(void)
{
	Disp_Number5(1,1); 
	Disp_Number5(2,password[1]); 
	Disp_Number5(3,password[2]);
	Disp_Number5(4,4);


}

void dispPassword(float tem,char i)
{
		unsigned char _data[5]={0};  
		unsigned int dot_pos=0;
		tem=tem*1.0F;
		sprintf(_data,"%1.4f",tem);
		if(i==0){

			  if(_data[1]==0x2E)
				{
					_data[0]=_data[0]-0x30;
					_data[2]=_data[2]-0x30;
					_data[3]=_data[3]-0x30;
					_data[4]=_data[4]-0x30;
				}
				gEnternmodeenable[0]=0;
				gEnternmodeenable[1]=0;
				gEnternmodeenable[2]=0;
				gEnternmodeenable[3]=0;
				glcd_raminfo.data[54]|=0x08;
				Disp_Ram(54,glcd_raminfo.data[54]);
				Disp_Number5(1,_data[0]); 
				Disp_Number5(2,_data[2]); 
				Disp_Number5(3,_data[3]);
				Disp_Number5(4,_data[4]);
				glcd_raminfo.data[42] |=0x08;
			    Disp_Ram(42,glcd_raminfo.data[42]);
		}
		else{
		    Disp_Number5(1,password[0]); 
			Disp_Number5(2,password[1]); 
			Disp_Number5(3,password[2]);
			Disp_Number5(4,password[3]);
			glcd_raminfo.data[42]&=0xf7;
			Disp_Ram(42,glcd_raminfo.data[42]);
			glcd_raminfo.data[54]&=0xf7;
			Disp_Ram(54,glcd_raminfo.data[54]);
		}
}
void disp_MinorStatus(unsigned char channel)
{
	if(gRN_Info.missvol[channel]){
		Disp_Sig(MissVols,1);
	}else{
		Disp_Sig(MissVols,0);
	}
}

unsigned char ncnt_disp=0;
unsigned char channel_status=0;
unsigned int  *padd=0;
unsigned int  *padd2=0;
void dispLCD_Loop(void)
{

   if(channel_status==0)
   {
				Disp_ABC(1,1);
				Disp_ABC(2,0);
				Disp_ABC(3,0);
   }else if(channel_status==1){
				Disp_ABC(2,1);
				Disp_ABC(1,0);
				Disp_ABC(3,0);
   }else if(channel_status==2){
				Disp_ABC(3,1);
				Disp_ABC(1,0);
				Disp_ABC(2,0);
   }

	if(gSystem_Info.tempermode){
		Disp_Temper(gSystem_Info.tempersetup);  
	}else{
		Disp_Temper(gSystem_Info.gathertemper[channel_status]);    
	}

    dispMA2(gSystem_Info.cul[channel_status]);	

   if(gSystem_Info.lcdUpdateenable)
   {
	gSystem_Info.lcdUpdateenable=0;
	if(++channel_status>2){	   	
		channel_status=0;
	}  

	if(gSystem_Info.leakcurmode){
		dispMA1(gSystem_Info.leakcursetup);
	}else{
		dispMA1(gSystem_Info.leakcurent);
	}
		dispVol(gSystem_Info.vol[channel_status]);		
  }
 
  // dispPassword();
   if(enternfactormode==1){
	     dispPassword(1.000,1);

   }else{   
         glcd_raminfo.data[54]&=0x08; //0xf8; 0x08
		 glcd_raminfo.data[56]&=0x00;// ;0xf0; 0x,00
        //dispPassword(gSystem_Info.totalpower,0);
		 Disp_Ram(54,glcd_raminfo.data[54]);	 
	     Disp_Ram(56,glcd_raminfo.data[56]);
		 
		 glcd_raminfo.data[50]&=0x08;
		 glcd_raminfo.data[52]&=0x00;
		 Disp_Ram(50,glcd_raminfo.data[50]);	 
	     Disp_Ram(52,glcd_raminfo.data[52]);
		 
		 glcd_raminfo.data[46]&=0x00;
		 glcd_raminfo.data[48]&=0x00;
		 Disp_Ram(46,glcd_raminfo.data[46]);	 
	     Disp_Ram(48,glcd_raminfo.data[48]);
		 
		 glcd_raminfo.data[42]&=0x00;
		 glcd_raminfo.data[44]&=0x00;
		 Disp_Ram(42,glcd_raminfo.data[42]);	 
	     Disp_Ram(44,glcd_raminfo.data[44]);
   }

    Watch_Loop(channel_status);

	glcd_raminfo.data[46]  &=0xf7;	
	Disp_Ram(46,glcd_raminfo.data[46]);

	glcd_raminfo.data[50]  &=0xf7;	
	Disp_Ram(50,glcd_raminfo.data[50]);

}


void LCD_PowerOn(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, GPIO_PIN_SET);
}
