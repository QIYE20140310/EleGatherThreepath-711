#include "Tem_ADC.h"
#include "main.h"
#include "math.h"


static void GetLeak(void);

#define GATHER_CAHNEL_NUMBERS  5U

uint32_t adc_value[5]=0;

uint32_t adc_value1[128][5];
extern ADC_HandleTypeDef hadc1;
float uhADCxConvertedValue[5];

void Tem_AdcInit(void)
{
    for(int i=0;i<5;i++){
       uhADCxConvertedValue[i]=0;
     }
 	HAL_ADC_Start_DMA(&hadc1,&adc_value1[0][0],GATHER_CAHNEL_NUMBERS*128);
}



#define TEM_TABLE_NUMBERS    190U

const float TEMPER_TABLE[TEM_TABLE_NUMBERS][2]={

{-20,   3.201824283},
{-19,   3.196119704},
{-18,   3.190137206},
{-17,   3.183867017},
{-16,   3.177298637},
{-15,   3.170421901},
{-14,   3.163226231},
{-13,   3.155701328},
{-12,   3.147836512},
{-11,   3.139621052},
{-10,   3.131044469},
{-9,    3.122096032},
{-8,    3.112764895},
{-7,    3.103041125},
{-6,    3.09291389},
{-5,    3.082372622},
{-4,    3.071407988},
{-3,    3.060009309},
{-2,    3.048166576},
{-1,    3.035871105},
{0,	    3.023113325},
{1,  	3.009884159},
{2,	    2.996176426},
{3,     2.98197982},
{4, 	2.967289519},
{5, 	2.952096907},
{6, 	2.93639569},
{7,  	2.920180318},
{8,	    2.903444823},
{9,	    2.886187871},
{10,	2.868401779},
{11,	2.850083848},
{12,	2.831234463},
{13,	2.811851169},
{14,	2.791935111},
{15,	2.771484169},
{16,	2.750500655},
{17,	2.728986711},
{18,	2.706947698},
{19,	2.684389613},
{20,	2.661310925},
{21,	2.637726193},
{22,	2.613639168},
{23,	2.589058685},
{24,	2.563995242},
{25,	2.538461538},
{26,	2.512466888},
{27,	2.486029303},
{28,	2.459149977},
{29,	2.431860712},
{30,	2.404169683},
{31,	2.376096086},
{32,	2.347646533},
{33,	2.318859697},
{34,	2.289744375},
{35,	2.260313376},
{36,	2.230608365},
{37,	2.20062298 },
{38,	2.170404601},
{39,	2.139961566},
{40,	2.109323359},
{41,	2.0785167  },
{42,	2.047548865},
{43,	2.016452742},
{44,	1.985258964},
{45,	1.953995187},
{46,	1.922666184},
{47,	1.891308802},
{48,	1.859958108},
{49,	1.828603065},
{50,	1.797313378},
{51,	1.766067555},
{52,	1.734906332},
{53,	1.703869407},
{54,	1.672941525},
{55,	1.642180619},
{56,	1.611588642},
{57,	1.581160477},
{58,	1.550976097},
{59,	1.520992291},
{60,	1.491252238},
{61,	1.461764706},
{62,	1.432568755},
{63,	1.403666245},
{64,	1.375090898},
{65,	1.346797932},
{66,	1.318811287},
{67,	1.291193718},
{68,	1.263926537},
{69,	1.237027235},
{70,	1.210510764},
{71,	1.184344147},
{72,	1.158579741},
{73,	1.133176476},
{74,	1.108182784},
{75,	1.08359863},
{76,	1.059420618},
{77,	1.035693701},
{78,	1.012305026},
{79,	0.989345315},
{80,	0.966800217},
{81,	0.94470773},
{82,	0.922992149},
{83,	0.901686087},
{84,	0.88082252},
{85,	0.860374569},
{86,	0.840311561},
{87,	0.820661157},
{88,	0.801451177},
{89,	0.782581498},
{90,	0.764139344},
{91,	0.746083995},
{92,	0.728437841},
{93,	0.711155566},
{94,	0.69432542},
{95,	0.677761297},
{96,	0.661618207},
{97,	0.645844504},
{98,	0.630457058},
{99,	0.615399843},
{100,	0.600687098},
{101,	0.586332986},
{102,	0.572351564},
{103,	0.558604935},
{104,	0.545255718},
{105,	0.532240208},
{106,	0.519491644},
{107,	0.507097921},
{108,	0.494911172},
{109,	0.483098592},
{110,	0.471590195},
{111,	0.460313226},
{112,	0.449356446},
{113,	0.438645626},
{114,	0.428270581},
{115,	0.418071728},
{116,	0.408138108},
{117,	0.398475967},
{118,	0.389005851},
{119,	0.3798183},
{120,	0.370919258},
{121,	0.362140186},
{122,	0.353659117},
{123,	0.345305318},
{124,	0.337258716},
{125,	0.329346456},
{126,	0.32166065},
{127,	0.314205145},
{128,	0.306893216},
{129,	0.299818171},
{130,	0.292892291},
{131,	0.286117876},
{132,	0.279497193},
{133,	0.273125019},
{134,	0.266911765},
{135,	0.260766255},
{136,	0.254876811},
{137,	0.249058523},
{138,	0.243406712},
{139,	0.237923355},
{140,	0.232515337},
{141,	0.227374302},
{142,	0.222216004},
{143,	0.21723236},
{144,	0.21242515},
{145,	0.207699516},
{146,	0.203153153},
{147,	0.198690558},
{148,	0.194312514},
{149,	0.190019791},
{150,	0.185911107},
{151,	0.181791552},
{152,	0.177858021},
{153,	0.174013262},
{154,	0.170257967},
{155,	0.166592815},
{156,	0.163018473},
{157,	0.159535592},
{158,	0.156144808},
{159,	0.152846743},
{160,	0.149642005},
{161,	0.146531184},
{162,	0.143414214},
{163,	0.140391919},
{164,	0.137464861},
{165,	0.134633585},
{166,	0.131898621},
{167,	0.129158926},
{168,	0.12651622},
{169,	0.123869105},

};

float testtemper;
float Tem_Gettemplate(float value)
{
	unsigned int indexmax=0;
	unsigned int indexmin=0;
	float    _tem=0;
	if(value<=0.123869105f) return (169);
	if(value>=3.201824283)  return (-20);
	for(int i=1;i<TEM_TABLE_NUMBERS;i++){
		if(TEMPER_TABLE[i][1]<value){
		indexmin=i;				
		break;
		}
	}
	indexmax=indexmin+1;
	_tem=(float)(TEMPER_TABLE[indexmax][0]+TEMPER_TABLE[indexmin][0])/2;
	testtemper=_tem;
	return _tem;
}
	
	const float LeakTable[][2]={
	
	{12,	5},
	{20,   10},
	{37.7, 20},
	{54.5, 30},
	{75.5, 40},
	{91.5, 50}, 
	{110,  60},
	{131.5,70},
	{147,  80},
	{167.5,90},
	{183.8,100},
	{280,150},
	{366,200},
	{457,250},
	{547,300},
	{647,350},
	{740,400},
	{831,450},
	{923,500},
	{1000,550},
	{1100,600},
	{1198,650},
	{1278,700},
	{1375,750},
	{1470,800},
	{1563,850},
	{1645,900},
	{1734,950},
	{1840,1000},
	{1940,1050},
	{2040,1100},
	{2134,1150},
	{2221,1200},
	{2318,1250},
	{2412,1300},
	{2504,1350},
	{2590,1400},
	{2671,1450},
	{2774,1500},
	
	};


	unsigned int indexmax,indexmin;	
float Leak_Gettemplate(float value)
{

	float    _tem=0;
	if(value>2774)   return 1500;
	if(value<12)     return 0;
	for(int i=1;i<39;i++)
	{ 
		if(LeakTable[i][0]>value){
		indexmax=i;
		break;
		}
	}
	indexmin=indexmax+1;
	_tem=(float)(LeakTable[indexmin][1]+LeakTable[indexmax][1])/2;
	return _tem;
}

float Gather_temper[5]={0};
#define ADC_REF   0.000805664f
void Tem_AdcLoop(void)
{
	float tem=0;

	for(unsigned int i=0;i<GATHER_CAHNEL_NUMBERS;i++){		
	uhADCxConvertedValue[i]=(float)(adc_value1[0][i]*ADC_REF);		   
	Gather_temper[i]=Tem_Gettemplate(uhADCxConvertedValue[i]);	  
	gSystem_Info.gathertemper[i]=Tem_Gettemplate(uhADCxConvertedValue[i])-8;
	}	
	GetLeak();
}


#define GATHER_LEAKCURRENT  (200U)
float   gLeakCurrent=0;
float   gGatherLeakCurrent[GATHER_LEAKCURRENT]={0};
float Template_Smooth(float vlaue)
{
	return (vlaue/GATHER_LEAKCURRENT);
}



float testFactoryadc=0;

#define BOARD_NUMBER  1U
void GetLeak(void)
{
		float _temp;
		float _total=0;
		for(int i=0;i<128;i++){
			_total+=adc_value1[i][4];
		}
		_total=_total/128;
		
		//uhADCxConvertedValue[4]=uhADCxConvertedValue[4]*1000;
		_total=_total;
		//_temp=Template_Smooth(uhADCxConvertedValue[4]);
		testFactoryadc=_total;
	    #if 0
		if(testFactoryadc<=60.27){
		gLeakCurrent=0.5321*testFactoryadc-2.319;
		}else if((testFactoryadc>60.27)&&(testFactoryadc<=170.32)){	
		gLeakCurrent=0.4543*testFactoryadc+2.6147;
		}
		else if((testFactoryadc>170.32)&&(testFactoryadc<=217.62)){	
		gLeakCurrent=0.422*testFactoryadc+7.981;
		}
		else if((testFactoryadc>217.62)&&(testFactoryadc<=662.5)){	
		gLeakCurrent=0.4496*testFactoryadc+2.1635;
		}
		else if((testFactoryadc>662.5)&&(testFactoryadc<=1105)){	
		gLeakCurrent=0.4519*testFactoryadc+0.5649;
		}else if((testFactoryadc>1105)&&(testFactoryadc<=1555.5)){	
		gLeakCurrent=0.4438*testFactoryadc+9.5151;
		}
		else if((testFactoryadc>1555.5)&&(testFactoryadc<=2010.8)){	
		gLeakCurrent=0.4393*testFactoryadc+16.606;
		}
		else if((testFactoryadc>2010.8)&&(testFactoryadc<=2465.7)){	
		gLeakCurrent=0.4396*testFactoryadc+15.877;
		}else{
		gLeakCurrent=0.4373*testFactoryadc+21.745;
		}
		#elif 0//finsh
		if(testFactoryadc<=60.875){
		gLeakCurrent=0.461894*testFactoryadc+0.958429;
		}else if((testFactoryadc>60.875)&&(testFactoryadc<=175.3999)){	
		gLeakCurrent=0.454346*testFactoryadc+1.461764;
		}
		else if((testFactoryadc>175.3999)&&(testFactoryadc<=220.125)){	
		gLeakCurrent=0.447177*testFactoryadc+1.565132;
		}
		else if((testFactoryadc>220.125)&&(testFactoryadc<=670.974976)){	
		gLeakCurrent=0.443607*testFactoryadc+2.351105;
		}
		else if((testFactoryadc>670.974976)&&(testFactoryadc<=1119.050049)){	
		gLeakCurrent=0.446354*testFactoryadc+0.507782;
		}else if((testFactoryadc>1119.050049)&&(testFactoryadc<=1572.449951)){	
		gLeakCurrent=0.441112*testFactoryadc+6.373962;;
		}
		else if((testFactoryadc>1572.449951)&&(testFactoryadc<=2029.199951)){	
		gLeakCurrent=0.437876*testFactoryadc+11.461426;
		}
		else if((testFactoryadc>2029.199951)&&(testFactoryadc<=2490.774902)){	
		gLeakCurrent=0.433299*testFactoryadc+20.749573;
		}else if((testFactoryadc>2490.774902)&&(testFactoryadc<2949.625)){
		 gLeakCurrent=0.435872*testFactoryadc+14.340454;
		}
		else{
		gLeakCurrent=0.435872*testFactoryadc+14.340454;
		}

        #elif 1//
		if(testFactoryadc<=60.775002){
		gLeakCurrent=0.485142*testFactoryadc+0.515465;
		}else if((testFactoryadc>60.775002)&&(testFactoryadc<=173.800003)){	
		gLeakCurrent=0.442380*testFactoryadc+3.114355;
		}
		else if((testFactoryadc>173.800003)&&(testFactoryadc<=217.649994)){	
		gLeakCurrent=0.456100*testFactoryadc+0.729744;
		}
		else if((testFactoryadc>217.649994)&&(testFactoryadc<=663.400024)){	
		gLeakCurrent=0.448682*testFactoryadc+0.344368;
		}
		else if((testFactoryadc>663.400024)&&(testFactoryadc<=1105.925049)){	
		gLeakCurrent=0.442952*testFactoryadc+1.175140;//
		}else if((testFactoryadc>1105.925049)&&(testFactoryadc<=1556.349976)){	
		gLeakCurrent=0.444025*testFactoryadc+3.941467;;
		}
		else if((testFactoryadc>1556.349976)&&(testFactoryadc<=2008.324951)){	
		gLeakCurrent=0.442502*testFactoryadc+5.311462;
		}
		else if((testFactoryadc>2008.324951)&&(testFactoryadc<=2465.274902)){	
		gLeakCurrent=0.437685*testFactoryadc+12.986938;
		}else if((testFactoryadc>2465.274902)&&(testFactoryadc<2922.100098)){
		gLeakCurrent=0.437804*testFactoryadc+20.692261;
		}
		else{
		gLeakCurrent=0.437804*testFactoryadc+10.592261;
		}
		#else //finish
	    if(testFactoryadc<=56.049999){
		gLeakCurrent=0.470312*testFactoryadc+3.639037;
		}else if((testFactoryadc>56.049999)&&(testFactoryadc<=166.774994)){	
		gLeakCurrent=0.445569*testFactoryadc+4.389545;
		}
		else if((testFactoryadc>166.774994)&&(testFactoryadc<=210.750000)){	
		gLeakCurrent=0.454804*testFactoryadc+4.150101;
		}
		else if((testFactoryadc>210.750000)&&(testFactoryadc<=650.775024)){	
		gLeakCurrent=0.434520*testFactoryadc+4.209991;
		}
		else if((testFactoryadc>650.775024)&&(testFactoryadc<=1088.50000)){	
		gLeakCurrent=0.46420*testFactoryadc+2.65762;////
		}else if((testFactoryadc>1088.50000)&&(testFactoryadc<=1531.0000)){	
		gLeakCurrent=0.451977*testFactoryadc+2.022583;;
		}
		else if((testFactoryadc>1531.0000)&&(testFactoryadc<=1978.324951)){	
		gLeakCurrent=0.443102*testFactoryadc+15.486389;
		}
		else if((testFactoryadc>1978.324951)&&(testFactoryadc<=2427.449951)){	
		gLeakCurrent=0.445301*testFactoryadc+10.031494;
		}else if((testFactoryadc>2427.449951)&&(testFactoryadc<2878.399902))
	   {
		gLeakCurrent=0.443508*testFactoryadc+23.406006;
		}else{
		gLeakCurrent=0.445301*testFactoryadc+19.031494;
		}
	    #endif

	    if(gLeakCurrent<=10){
		     gSystem_Info.leakcurent=0;
	   	}else{
	       gSystem_Info.leakcurent=gLeakCurrent;
	   	}
}




#define  ELE_GATHER_NUMBERS 200U
float    EleADCCalue[ELE_GATHER_NUMBERS];
uint32_t EleFinish=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static unsigned int index=0;
	unsigned int m=0;
	float        _total=0;
	if(htim->Instance==TIM2){
		if(EleFinish==0){
			for(int i=0;i<128;i++){
                _total+=adc_value1[m][4];
			}
			_total=_total/128;
			EleADCCalue[index]=_total*ADC_REF*1000;

			if(++index>=ELE_GATHER_NUMBERS){
				index=0;
				EleFinish=1;
			}
		}
	}	
}

extern TIM_HandleTypeDef  htim2;
float  gEleGather=0;
float  testgLeak=0;
void   EleHandle_Loop(void)
{
	float _tem=0;
	if(EleFinish==1){   
	HAL_TIM_Base_Stop_IT(&htim2);
	for(int i=0;i<ELE_GATHER_NUMBERS;i++){
		_tem+=EleADCCalue[i]*EleADCCalue[i];
	}
	gEleGather=(_tem/(ELE_GATHER_NUMBERS-1));	  
	gEleGather=sqrt(gEleGather); 
	if(gEleGather<600){
    testgLeak=gEleGather/1.325;
	}else{
	testgLeak=gEleGather*1.65016;
	}
	EleFinish=0;
	HAL_TIM_Base_Start_IT(&htim2);
	}
}



