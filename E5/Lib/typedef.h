
#ifndef _MT_TYPEDEF_H_
#define _MT_TYPEDEF_H_

#define BYTE_ALIGN   __attribute__ ((packed))

#ifndef _MT_TYPE_CHARS__
#define _MT_TYPE_CHARS__
typedef unsigned   char       u8_t;
typedef signed     char       s8_t;
typedef unsigned   short int  u16_t;
typedef signed     short int  s16_t;
//#ifdef __GNUC__
typedef unsigned   int        u32_t;
typedef signed     int        s32_t;
//#else
//typedef unsigned   long    u32_t;
//typedef signed     long    s32_t;
#endif

#define UDP_LISTEN_PORT 5918
#define DEFAULT_SIP    0x784F36AC
#define DEFAULT_SPORT 7758

#define FUNC_TYPE_NONE  0
#define FUNC_TYPE_FSW   0x80
#define FUNC_TYPE_TSW   0x84
#define FUNC_TYPE_TSWA  0x85
#define FUNC_TYPE_TSWB  0x86
#define FUNC_TYPE_TSWC  0x87
#define FUNC_TYPE_SSW   0x94
#define FUNC_TYPE_SSWA  0x95
#define FUNC_TYPE_SSWB  0x96
#define FUNC_TYPE_SSWC  0x97
#define FUNC_TYPE_COM   0x88
#define FUNC_TYPE_ETH   0x89
#define FUNC_TYPE_EWF   0x8a
#define FUNC_TYPE_ENB   0x8b
#define FUNC_TYPE_E2G   0x8c
#define FUNC_TYPE_E4G   0x8d


#define FUNC_TYPE_E9_801  0xE0
#define FUNC_TYPE_E9_COM  0xE8
#define FUNC_TYPE_E9_ETH  0xE9
#define FUNC_TYPE_E9_WF   0xEA
#define FUNC_TYPE_E9_NB   0xEB
#define FUNC_TYPE_E9_2G   0xEC
#define FUNC_TYPE_E9_4G   0xED


#if defined (NET_INF_AT_4G)	
	#define NET_INF_TYPE FUNC_TYPE_E4G
#elif defined (NET_INF_AT_2G)	
	#define NET_INF_TYPE FUNC_TYPE_E2G
#elif defined (NET_INF_AT_NB)	
	#define NET_INF_TYPE FUNC_TYPE_ENB
#elif defined (NET_INF_AT_WIFI)	
	#define NET_INF_TYPE FUNC_TYPE_EWF
	#define NET_IF_WIFI 1
#endif

#if defined (T9)
	#define NET_INF_MODEL 0x5439
#elif defined (E9)
	#define NET_INF_MODEL 0x4539
#endif

#if defined (E9)
	#define NUMBER_OF_SLOTS     8
	#define NUMBER_OF_NODES     8
	#define NUMBER_OF_EX_NODES  8	
#else
	#define NUMBER_OF_SLOTS     32
	#define NUMBER_OF_NODES     32
	#define NUMBER_OF_EX_NODES  32
#endif

#define NODE_SPN_BASE 0xFFFFFF00

#define uhtonl(x)  (x) = ((x)&0xff)<<24 | ((x)&0xff00)<<8 |((x)&0xff0000)>>8 |((x)&0xff000000)>>24
#define uhtons(x)  (x) = (((x)&0xff)<<8) | (((x)&0xff00)>>8)




typedef enum
{ 
  ERR = 0xE0,
  ERR_FUNC_TYPE,  //0xe1
  ERR_NODE_ID,
  ERR_NODE_MODEL,
	ERR_BUS_BUSY,
	ERR_FIFO_FULL,  //0xe5
	ERR_MEM_OPERATE,
	ERR_PARAMETER,
	ERR_MSG_CRC,
	ERR_MSG_HEAD,
	ERR_MSG_LEN,    //0xea
	ERR_OP_FORBIDDEN,
	ERR_NETWORK_ERROR,
	ERR_NVMEM_ERROR,
}ERR_CODE;



#define ACT_TIMER_LEN   32
#define TIMER_NODE_NUM  31
#define ACT_SCENE_LEN   8
#define SCENE_ACT_NUM   24


typedef union
{
	u16_t all;
	struct{	
		unsigned tri:1;  //three-phase
		unsigned lip:1;  //protection while leak occur
		unsigned lid:1;  //leak detection
		unsigned bch:1;  // branch channel
		unsigned nch:1;	 // earth line avaliable
		unsigned mcb:1;  // micro current break
		unsigned rev:2;
		unsigned cl:5;
		unsigned cc:3;
#define CODE_CC_X 0
#define CODE_CC_C 3
#define CODE_CC_D 3
#define CODE_CC_I 4
	}BYTE_ALIGN bit;
}BYTE_ALIGN a_bit;

typedef union
{
	u8_t all;
	struct{	
		unsigned disc:1;  //disable remote control
		unsigned disd:1;  //disable app display
		unsigned disn:1;  //disable	net control
		unsigned dism:1;  //disable	miss phase on tri-phase node
		unsigned disu:1;  //disable	un-balance phase protection
		unsigned diso:1;  //disable	phase order protection
		unsigned type:2;  // branch channel
	}BYTE_ALIGN bit;
}BYTE_ALIGN s_bit;

typedef union
{
	u32_t all;
	struct{	
		unsigned zno:8;   //three-phase
		unsigned disc:1;  //disable remote control
		unsigned disd:1;  //disable app display
		unsigned disn:1;  //disable	net control
		unsigned dism:1;  //disable	miss phase on tri-phase node
		unsigned disu:1;  //disable	un-balance phase protection
		unsigned diso:1;  //disable	phase order protection
		unsigned type:2;  // branch channel
	}BYTE_ALIGN bit;
}BYTE_ALIGN alm_bit;



typedef struct  //80 bytes
{
	u8_t  nno;
#define NET_NODE_NO  0
	u8_t  type;
	u16_t model;
	u16_t ver;
	u32_t ser;
	u16_t rev;
	s_bit ctrl;
	u8_t  zno;
	u16_t vol_max;   //volate upper limit
	u16_t vol_min;   //volate lower limit
	u16_t vol_stp;   //volate step
	u16_t lki_max;   //leak current upper limit	
	u16_t lki_stp;   //leak current step	
	u16_t pwr_max;   //power upper limit
	u16_t pwr_stp;   //power step
	u16_t pwr_min;   //power lower limit
	s16_t tmp_max;   //temperature upper limit
	u16_t tmp_stp;   //temperature step
	u16_t cur_max;   //current upper limit
	u16_t cur_stp;   //current step
	alm_bit   alm;   //alarm 
	u32_t lkdt;  //leak current detection time
	u32_t lkdi;   //leak current detection interval
	u8_t rev2[6];      //reserved
	u8_t name[24];
}BYTE_ALIGN sw_conf;

typedef union
{
	u16_t all;
	struct{	
		unsigned dhcp:1;       //dhcp enable
		unsigned dis_dns:1;    //dns resolve diable
		unsigned heartbeat: 1; //heart beat message 
		unsigned snet_init: 1;		
#define RUN_MODE_NORMAL 0
#define RUN_MODE_CONFIG 1
#define RUN_MODE_RESET  2
		unsigned run_mode:2;
		unsigned rev:10;
	}BYTE_ALIGN bit;
}BYTE_ALIGN n_bit;

typedef struct   //80 bytes
{
	u8_t  nno;
#define NET_NODE_NO  0
	u8_t  type;
#define FUNC_TYPE_NONE  0
	u16_t model;
	u16_t ver;
	u8_t  mac[6];
	
	n_bit ctrl;
	u32_t ip;
	u32_t nm;
	u32_t gw;
	u32_t sip;
	u16_t sport;
	u32_t dns;
	u16_t rtvi;
	u16_t lport; 
	s8_t  zone; 
	u32_t lip;
	u8_t  pwd[8];
	u8_t  rev[3];
	u8_t  name[24];
}BYTE_ALIGN net_conf;

typedef union
{
	net_conf net;
	sw_conf  sw;
}BYTE_ALIGN node_conf;

typedef struct  //44bytes
{
	u8_t  nno;
	u8_t  type;
	u16_t rev;
	char  ssid[20];
	char  pwd[20];
	//char  doman[32];
	//u32_t dns;
}BYTE_ALIGN net_wlan;

typedef struct   // 44 bytes
{
	u8_t  nno;
	u8_t  type;
	u16_t rev;
	u16_t cur_a_max;   //current upper limit of channel A
	u16_t cur_a_min;   //current lower limit of channel B
	u16_t cur_b_max;   //current upper limit of channel B
	u16_t cur_b_min;   //current lower limit of channel B
	u16_t cur_c_max;   //current upper limit of channel C
	u16_t cur_c_min;   //current lower limit of channel C
	u8_t  cur_a_stp;   //current step of channel A
	u8_t  cur_b_stp;   //current step of channel B
	u8_t  cur_c_stp;   //current step of channel C
	u8_t  rev1;   //current step of channel C
	u16_t pwr_a_max;   //power upper limit of channel A
	u16_t pwr_a_min;   //power lower limit of channel A
	u16_t pwr_b_max;   //power upper limit of channel B
	u16_t pwr_b_min;   //power lower limit of channel B
	u16_t pwr_c_max;   //power upper limit of channel C
	u16_t pwr_c_min;   //power lower limit of channel C
	u8_t  pwr_a_stp;   //power step of channel A
	u8_t  pwr_b_stp;   //power step of channel B
	u8_t  pwr_c_stp;   //power step of channel C
	u8_t  rev2;   //power step of channel C
	u16_t rev3[4];   //power lower limit of channel C
}BYTE_ALIGN tsw_conf;

typedef union
{
	net_wlan net;
	tsw_conf tsw;
}BYTE_ALIGN node_ex;

typedef union
{
	u16_t all;
	struct{
		unsigned key:1;
		unsigned init:1;
		unsigned new_alrm:1;
		unsigned snet_up: 1;  //
		unsigned eth_up:1;    //
		unsigned ip_ok:1;     //
		unsigned com_ok:1;    //
		unsigned dis_sent: 1; //
		unsigned dhcp:1;
	#define RUN_MODE_NORMAL 0
	#define RUN_MODE_CONFIG 1
	#define RUN_MODE_RESET  2		
		unsigned mode:2;      //
	} BYTE_ALIGN bit;
} BYTE_ALIGN net_sta;

typedef struct    //22 bytes
{
	u8_t nno;
	u8_t type;
	net_sta ctrl;
	u32_t  sip;
	u32_t  ip;
	u32_t  gw;
	u32_t  ps;
	u8_t   rtvi;
	u8_t   on_time;
} BYTE_ALIGN net_status;


typedef union
{
	u32_t all;
	struct{
		unsigned sc:1;   //short current alarm 0
		unsigned sg:1;   //surge alarm
		unsigned ol:1;   //over load alarm
		unsigned ot:1;   //over temperature alarm
		
		unsigned oli:1;  //over leak current alarm
		unsigned oc:1;   //over current alarm
		unsigned ov:1;   //over voltage alarm
		unsigned lip:1;  //leak current protection normal 7
		
		unsigned ltc:1;  //leakage current test un-completed 8
		unsigned ipm:1;  //imput phase missed alarm
		unsigned spk:1;  //spark alarm
		unsigned uv:1;   //unber voltage alarm
		
		unsigned pov:1;  //over voltage pre-alarm
		unsigned puv:1;  //under voltage pre-alarm
		unsigned plk:1;  //leak current pre-alarm
		unsigned poc:1;  //over current pre-alarm 15
		
		unsigned dis_con:1;  //remote control frobidden
		unsigned dis_mph:1;  //disable miss phase protection
		unsigned dis_ubl:1;  //disable unbalance protection
		unsigned dis_pha:1;  //disable phase order protection
		unsigned rev1:2; //reserved
		unsigned pub:1;  //phase unbalanced alarm
		unsigned acb:1;  //phase acb 
		
		unsigned rev2:6;//reserved 
		unsigned on:1;   //node on
		unsigned dv:1;   //data valid
	} BYTE_ALIGN bit;
} BYTE_ALIGN sw_alm;

typedef struct     //22 bytes
{    
	u8_t   nno;
	u8_t   type;
	u16_t  vol;
	u16_t  lki;
	u16_t  pwr;
	s16_t  tmp;
	u16_t  cur;
	u16_t  cur2;
	sw_alm alm;
	u32_t  ps;
} BYTE_ALIGN sw_status;

typedef union
{
	sw_status sw;
	net_status net;
}BYTE_ALIGN node_status;

typedef struct    //20 bytes
{
	u8_t  nno;
	u8_t  type;
	u8_t  dn;
	u32_t vol;
	u16_t lki;
	s16_t tmp; 
	u32_t cur;
	u32_t ps;
} BYTE_ALIGN sw_acm_sum;

typedef struct{
	u32_t time;
	u32_t ps[NUMBER_OF_SLOTS+1];
}BYTE_ALIGN ps_info;


typedef struct
{
	u32_t addr;
#define CONF_DATA_TYPE     0x4D
	u8_t  type;
#define CONF_DATA_VER      0xA1
	u8_t  ver;
#define CONF_DATA_REV      0x05
	u8_t  rev;
	u8_t  num;
}BYTE_ALIGN conf_index;

typedef struct
{
	conf_index  head;
	node_conf   conf[NUMBER_OF_NODES];
	node_ex     ex_conf[NUMBER_OF_EX_NODES];
	node_status stts[NUMBER_OF_NODES];
	ps_info     ps_acm;
}BYTE_ALIGN node_mem;


typedef struct    //15 bytes
{
	u8_t  nno;
	u8_t  type;
	u8_t  dn;
	u16_t vol;
	u16_t lki;
	s16_t tmp; 
	u16_t cur;
	u32_t ps;
} BYTE_ALIGN sw_acm_t;

typedef struct    //15 bytes
{
	u8_t  nno;
	u8_t  type;
	u8_t  dn;
	u16_t flow;
	u16_t syson;
	u32_t ps_a;  //????
	u32_t ps;    //????
} BYTE_ALIGN net_acm;

typedef struct
{
	u16_t sn;                  //serial number of avg data
	u8_t  type;                //type of avg data
#define MEM_AVG_HOUR  0xf1
#define MEM_AVG_DAY   0xf2
#define MEM_AVG_MONTH 0xf4
	u8_t  num;                 //number of nodes
	u32_t time;                //system time
}BYTE_ALIGN acm_head;

typedef union
{
	sw_acm_t sw;
	net_acm net;
}BYTE_ALIGN node_acm;

typedef struct
{
	acm_head head;
	node_acm node[NUMBER_OF_NODES];
}BYTE_ALIGN mem_acm_t;

#endif
