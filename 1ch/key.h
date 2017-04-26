#ifndef ___KEY_H___
#define ___KEY_H___

#ifdef KEY_GLOBAL
	#define KEYSCAN_EXT
#else
	#define KEYSCAN_EXT extern
#endif

#define PAIRKEY		0x10
#define SIRENKEY	0x40
#define LAMPKEY		0x60
          
#define KEY_NONE	        0xFF

#define KEY_BASE	        0x00
#define KEY_PAIRING	        0x01
#define KEY_VOUT_MON            0x02
#define KEY_DC_MON	        0x03
#define KEY_BUZZER_SW           0x04
#define KEY_IR_PULG             0x05
#define KEY_UPDATE_SW   	0x06

#define KEY_SENSOR1	        0x07
#define KEY_SENSOR2	        0x08
#define KEY_SENSOR3	        0x09
#define KEY_SENSOR4	        0x0A
#define KEY_SENSOR5	        0x0B
#define KEY_SENSOR6	        0x0C

#define KEY_IR_PAIRKEY          0x10
#define KEY_IR_SIRENKEY         0x40
#define KEY_IR_LAMPKEY          0x60

#define KEY_IR_UNPULG           0x80
#define KEY_IR_REMOVE           0x80

#define KEY_PAIRING_LONG	0x11
#define KEY_VOUT_MON_LONG       0x12
#define KEY_DC_MON_LONG	        0x13
#define KEY_BUZZER_LONG         0x14
#define KEY_IR_PULG_LONG        0x15

#define KEY_SENSOR_LONG	        0x17
#define KEY_SENSOR2_LONG	0x18
#define KEY_SENSOR3_LONG	0x19
#define KEY_SENSOR4_LONG	0x1A
#define KEY_SENSOR5_LONG	0x1B
#define KEY_SENSOR6_LONG	0x1C

// variable definition
//KEYSCAN_EXT mStatus PortNo1Status;
//KEYSCAN_EXT mStatus PortNo2Status;

KEYSCAN_EXT u8 PushKey;              // PUSH 버튼의 값 
KEYSCAN_EXT u8 PullKey;               // PULL 버튼의 값 
KEYSCAN_EXT u8 RepeatKey;
KEYSCAN_EXT const unsigned char       key_table[];

KEYSCAN_EXT void KeyScan(void );
KEYSCAN_EXT void init_Key(void);
KEYSCAN_EXT void Repeat_key_Scan(void);

#endif	//___PORT_H___

