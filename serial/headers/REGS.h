
#pragma once

#define REGSIZE 0x90

enum WitRegType {
  SAVE 		      = 0x00,
  CALSW 		    = 0x01,
  RSW 		      = 0x02,
  RRATE		      = 0x03,
  BAUD 		      = 0x04,
  AXOFFSET	    = 0x05,
  AYOFFSET	    = 0x06,
  AZOFFSET	    = 0x07,
  GXOFFSET	    = 0x08,
  GYOFFSET	    = 0x09,
  GZOFFSET	    = 0x0a,
  HXOFFSET	    = 0x0b,
  HYOFFSET	    = 0x0c,
  HZOFFSET	    = 0x0d,
  D0MODE		    = 0x0e,
  D1MODE		    = 0x0f,
  D2MODE		    = 0x10,
  D3MODE		    = 0x11,
  D0PWMH		    = 0x12,
  D1PWMH		    = 0x13,
  D2PWMH		    = 0x14,
  D3PWMH		    = 0x15,
  D0PWMT		    = 0x16,
  D1PWMT		    = 0x17,
  D2PWMT		    = 0x18,
  D3PWMT		    = 0x19,
  IICADDR		    = 0x1a,
  LEDOFF 		    = 0x1b,
  MAGRANGX	    = 0x1c,
  MAGRANGY	    = 0x1d,
  MAGRANGZ	    = 0x1e,
  BANDWIDTH	    = 0x1f,
  GYRORANGE	    = 0x20,
  ACCRANGE	    = 0x21,
  SLEEP         = 0x22,
  ORIENT		    = 0x23,
  AXIS6         = 0x24,
  FILTK         = 0x25,
  GPSBAUD		    = 0x26,
  READADDR	    = 0x27,
  BWSCALE		    = 0x28,
  MOVETHR		    = 0x28,
  MOVESTA		    = 0x29,
  ACCFILT		    = 0x2A,
  GYROFILT	    = 0x2b,
  MAGFILT		    = 0x2c,
  POWONSEND	    = 0x2d,
  VERSION		    = 0x2e,
  CCBW			    = 0x2f,
  YYMM				  = 0x30,
  DDHH				  = 0x31,
  MMSS				  = 0x32,
  MS					  = 0x33,
  AX					  = 0x34,
  AY					  = 0x35,
  AZ					  = 0x36,
  GX					  = 0x37,
  GY					  = 0x38,
  GZ					  = 0x39,
  HX					  = 0x3a,
  HY					  = 0x3b,
  HZ					  = 0x3c,
  Roll				  = 0x3d,
  Pitch				  = 0x3e,
  Yaw					  = 0x3f,
  TEMP				  = 0x40,
  D0Status		  = 0x41,
  D1Status		  = 0x42,
  D2Status		  = 0x43,
  D3Status		  = 0x44,
  PressureL		  = 0x45,
  PressureH		  = 0x46,
  HeightL			  = 0x47,
  HeightH			  = 0x48,
  LonL				  = 0x49,
  LonH				  = 0x4a,
  LatL				  = 0x4b,
  LatH				  = 0x4c,
  GPSHeight     = 0x4d,
  GPSYAW        = 0x4e,
  GPSVL				  = 0x4f,
  GPSVH				  = 0x50,
  q0					  = 0x51,
  q1					  = 0x52,
  q2					  = 0x53,
  q3					  = 0x54,
  SVNUM				  = 0x55,
  PDOP				  = 0x56,
  HDOP				  = 0x57,
  VDOP				  = 0x58,
  DELAYT			  = 0x59,
  XMIN          = 0x5a,
  XMAX          = 0x5b,
  BATVAL        = 0x5c,
  ALARMPIN      = 0x5d,
  YMIN          = 0x5e,
  YMAX          = 0x5f,
  GYROZSCALE		= 0x60,
  GYROCALITHR   = 0x61,
  ALARMLEVEL    = 0x62,
  GYROCALTIME		= 0x63,
  REFROLL			  = 0x64,
  REFPITCH		  = 0x65,
  REFYAW			  = 0x66,
  GPSTYPE       = 0x67,
  TRIGTIME      = 0x68,
  KEY           = 0x69,
  WERROR        = 0x6a,
  TIMEZONE      = 0x6b,
  CALICNT       = 0x6c,
  WZCNT         = 0x6d,
  WZTIME        = 0x6e,
  WZSTATIC      = 0x6f,
  ACCSENSOR 	  = 0x70,
  GYROSENSOR 	  = 0x71,
  MAGSENSOR 	  = 0x72,
  PRESSENSOR 	  = 0x73,
  MODDELAY      = 0x74,

  ANGLEAXIS     = 0x75,
  XRSCALE			  = 0x76,
  YRSCALE			  = 0x77,
  ZRSCALE			  = 0x78,

  XREFROLL		  = 0x79,
  YREFPITCH		  = 0x7a,
  ZREFYAW			  = 0x7b,

  ANGXOFFSET		= 0x7c,
  ANGYOFFSET		= 0x7d,
  ANGZOFFSET		= 0x7e,

  NUMBERID1     = 0x7f,
  NUMBERID2     = 0x80,
  NUMBERID3     = 0x81,
  NUMBERID4     = 0x82,
  NUMBERID5     = 0x83,
  NUMBERID6     = 0x84,

  XA85PSCALE    = 0x85,
  XA85NSCALE    = 0x86,
  YA85PSCALE    = 0x87,
  YA85NSCALE    = 0x88,
  XA30PSCALE    = 0x89,
  XA30NSCALE    = 0x8a,
  YA30PSCALE    = 0x8b,
  YA30NSCALE    = 0x8c,

  CHIPIDL       = 0x8D,
  CHIPIDH       = 0x8E
};

#define REGINITFLAG REGSIZE-1 


/* AXIS6 */
#define ALGRITHM9 0
#define ALGRITHM6 1

/************CALSW**************/
#define NORMAL            0x00
#define CALGYROACC        0x01
#define CALMAG            0x02
#define CALALTITUDE       0x03
#define CALANGLEZ         0x04
#define CALACCL           0x05
#define CALACCR           0x06
#define CALMAGMM          0x07
#define CALREFANGLE		    0x08
#define CALMAG2STEP		    0x09
//#define CALACCX         0x09
//#define ACC45PRX        0x0A
//#define ACC45NRX        0x0B
//#define CALACCY         0x0C
//#define ACC45PRY        0x0D
//#define ACC45NRY     	  0x0E
//#define CALREFANGLER    0x0F
//#define CALACCINIT      0x10
//#define CALREFANGLEINIT 0x11
#define CALHEXAHEDRON		  0x12

/***************STATUS***************/
enum WitCodes {
  WIT_OK =       0,   // No error
  WIT_BUSY =    -1,   // Busy
  WIT_TIMEOUT = -2,   // Timeout
  WIT_ERROR =   -3,   // Generic Error
  WIT_NOMEM =   -4,   // No memory
  WIT_EMPTY =   -5,   // Resource is empty
  WIT_INVAL =   -6    // Invalid argument
};


/***********BUFFER SIZE***********/
#define WIT_DATA_BUFF_SIZE 256

/************OUTPUTHEAD**************/
enum WitOutputHeadType {
  WIT_TIME       = 0x50,
  WIT_ACC        = 0x51,
  WIT_GYRO       = 0x52,
  WIT_ANGLE      = 0x53,
  WIT_MAGNETIC   = 0x54,
  WIT_DPORT      = 0x55,
  WIT_PRESS      = 0x56,
  WIT_GPS        = 0x57,
  WIT_VELOCITY   = 0x58,
  WIT_QUATER     = 0x59,
  WIT_GSA        = 0x5A,
  WIT_REGVALUE   = 0x5F
};

/************RSW**************/
enum WitContentType {
  RSW_TIME 	 =  0x01,
  RSW_ACC		 =  0x02,
  RSW_GYRO	 =  0x04,
  RSW_ANGLE	 =  0x08,
  RSW_MAG		 =  0x10,
  RSW_PORT	 =  0x20,
  RSW_PRESS	 =  0x40,
  RSW_GPS		 =  0x80,
  RSW_V      =  0x100,
  RSW_Q      =  0x200,
  RSW_GSA		 =  0x400,
  RSW_MASK	 =  0xfff
};
WitContentType operator+(WitContentType a, WitContentType b);

/**RRATE*****/
enum WitOutputRateType {
  RRATE_NONE	= 0x0d,
  RRATE_02HZ	= 0x01,
  RRATE_05HZ	= 0x02,
  RRATE_1HZ 	= 0x03,
  RRATE_2HZ 	= 0x04,
  RRATE_5HZ 	= 0x05,
  RRATE_10HZ	= 0x06,
  RRATE_20HZ	= 0x07,
  RRATE_50HZ	= 0x08,
  RRATE_100HZ	= 0x09,
  RRATE_125HZ	= 0x0a,	//only WT931
  RRATE_200HZ	= 0x0b,
  RRATE_ONCE 	= 0x0c
};

/* BAUD */
enum WitBaudRateType {
  WIT_BAUD_4800	    = 1,
  WIT_BAUD_9600	    = 2,
  WIT_BAUD_19200	  = 3,
  WIT_BAUD_38400	  = 4,
  WIT_BAUD_57600	  = 5,
  WIT_BAUD_115200	  = 6,
  WIT_BAUD_230400	  = 7,
  WIT_BAUD_460800	  = 8,
  WIT_BAUD_921600	  = 9
};

/* KEY */
#define KEY_UNLOCK	0xB588

/* SAVE */
#define SAVE_PARAM	0x00
#define SAVE_SWRST	0xFF

/* ORIENT */
#define ORIENT_HERIZONE	0
#define ORIENT_VERTICLE	1

/* BANDWIDTH */
enum WitBandwidthType {
  BANDWIDTH_256HZ	= 0,
  BANDWIDTH_184HZ	= 1,
  BANDWIDTH_94HZ	= 2,
  BANDWIDTH_44HZ	= 3,
  BANDWIDTH_21HZ	= 4,
  BANDWIDTH_10HZ	= 5,
  BANDWIDTH_5HZ	  = 6
};
