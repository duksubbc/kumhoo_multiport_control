#ifndef ___LED_H___
#define ___LED_H___

#ifdef LED_GLOBAL
	#define LED_EXT
#else
	#define LED_EXT extern
#endif

#define  NOARMAL_MODE   0
#define  PAIRING_MODE   1
#define  BATTERY_MODE   2
#define  WRITE_EEPROM_MODE 3

#define  WDT_ENABLE_MODE   4


typedef enum {
  CABLE_READY_MODE,
  CABLE_START_MODE,
  CABLE_ALARM_MODE
} CLedMode;

typedef enum {
  CABLE_TYPE_UNKNOW,
  CABLE_TYPE_R,
  CABLE_TYPE_BR
} CLedType;

//#define  CABLE_READY_MODE     0
//#define  CABLE_START_MODE     1
//#define  CABLE_ALARM_MODE     2

//#define  CABLE_TYPE_R         1
//#define  CABLE_TYPE_BR        2


LED_EXT u8 gRunLedMode;
LED_EXT CLedMode gCLedMode[MAX_CHANNEL_NUMS+1];
LED_EXT CLedType gCLedType[MAX_CHANNEL_NUMS+1];
LED_EXT void do_run_led(void);
LED_EXT void do_c_led(void);
LED_EXT void set_mode_runled(u8 mode);
LED_EXT void set_mode_cled(CLedMode mode,u8 ch);
LED_EXT void init_led();
#endif	//___LED_H___

