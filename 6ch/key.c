
#define KEY_GLOBAL

#include <stm8s.h>

#include "includes.h"
#include "board.h"

#include "main.h"
#include "key.h"
#include "uart.h"

#if defined(MULTI_6CH)

#define KEY_QTY      11            // 버튼의 수 
#define ON_TIME       30            //5        // 버튼 눌림 인식시간 (4 x 4line x 2ms = 32ms) 
#define ON_MAX       10
#define REPEAT_KEY_START       250    //  40*12ms +40  반복키가 처음 발생하는 시점입니다.
#define CONTINUE_KEY_TIME      20     //  반복키가 발생하는 주기입니다.

#else

#define KEY_QTY       6            // 버튼의 수 
#define ON_TIME       30            //5        // 버튼 눌림 인식시간 (4 x 4line x 2ms = 32ms) 
#define ON_MAX       10
#define REPEAT_KEY_START       250    //  40*12ms +40  반복키가 처음 발생하는 시점입니다.
#define CONTINUE_KEY_TIME      20     //  반복키가 발생하는 주기입니다.

#endif

volatile  unsigned  char  KeyStatus[KEY_QTY]=  {0};       // 버튼 1개당 1 byte의 버퍼를 할당 
volatile  unsigned short  KeyOnTime[KEY_QTY] = {0};

const unsigned char key_table[]=
{
	KEY_PAIRING,
        KEY_VOUT_MON,
        KEY_DC_MON,
        KEY_BUZZER_SW,
        KEY_IR_PULG,
        KEY_UPDATE_SW,
        KEY_SENSOR1,
#if defined(MULTI_6CH)
        KEY_SENSOR2,
        KEY_SENSOR3,
        KEY_SENSOR4,
        KEY_SENSOR5,
        KEY_SENSOR6
#endif
};


// 11: 버튼이 막 눌려짐(Push)
// 10: 버튼이 ON 상태
// 01: 버튼이 막 떨어짐(Pull)
// 00: 버튼이 OFF 상태

static void inc_key(u8 key) 
{ 
    u8 byte; 

    byte = KeyStatus[key];  	        // 버튼의 직전 상태값 가져오기 
    if ((byte & 0x3F) < ON_TIME)	byte++;	// 버튼이 눌려진 시간값을 UP count 
    if ((byte & 0x3F) >= ON_TIME)   	// 버튼이 일정시간동안 눌려져 있었다면 PUSH로 판정 
    { 
            if ((byte & 0x80) == 0)         // 버튼의 이전상태가 OFF 
            { 
                      byte = byte | 0xC0;    // 버튼이 ON상태임을 표시(b7:1) 
                      PushKey = key;         // [PUSH]로 인식된 버튼의 논리값을 결정 
            }
            else      // 버튼의 이전상태가 ON 
            {
                     byte = byte | 0x80;
                     byte = (byte & (~0x40));		
            }
    }; 
    
    KeyStatus[key] = byte;       // 버튼의 상태값을 저장                       
} 

static void dec_key(u8  key) 
{ 
    u8 byte; 
    byte = KeyStatus[key];                // 버튼의 직전 상태값 가져오기 
    if ((byte & 0x3F) > 0) 	byte--;       // 버튼이 눌려진 시간값을 DOWN count 
    if ((byte & 0x3F) == 0) 	      // 버튼이 일정시간동안 떨어져 있었다면 PULL로 판정 
    { 
            if ((byte & 0x80) == 0x80)          // 버튼의 이전상태가 ON
            { 
                    byte =( byte & (~ 0x80)); // 버튼 OFF
                    byte = byte | 0x40;      // Up Detect
                    PullKey = key;        // [PULL]로 인식된 버튼의 논리값을 결정 			
            }
            else
            {
                    byte = 0;                                // 버튼이 OFF상태임을 표시(b7:0) 
            }
    };
    
    KeyStatus[key] = byte;                 // 버튼의 상태값을 저장    
}  

void KeyScan (void ) 
{ 
      if(PAIRING_CHK) {
        dec_key(0); 
      } else {
        inc_key(0);	//Active Low
      }
      
      //if(VOUT_MON_CHK) {
      //  dec_key(1); 
      //} else {
      //  inc_key(1);	//Active Low
      //}

      //if(DC_MON_CHK) {
      //  dec_key(2); 
      //} else {
      //  inc_key(2);	//Active Low
      //}

      if(BUZZER_SW_CHK) {
        dec_key(3); 
      } else {
        inc_key(3);	//Active Low
      }
      
#if 0
      if( (PAIRING_CHK) && (BUZZER_SW_CHK)) {
        dec_key(5); 
      } else {
        inc_key(5);	//Active Low
      }
#endif

      if(SENSOR1_CHK) {
        dec_key(6); 
      } else {
        inc_key(6);	//Active Low
      }
#if defined(MULTI_6CH)
      if(SENSOR2_CHK) {
        dec_key(7); 
      } else {
        inc_key(7);	//Active Low
      }

      if(SENSOR3_CHK) {
        dec_key(8); 
      } else {
        inc_key(8);	//Active Low
      }

      if(SENSOR4_CHK) {
        dec_key(9); 
      } else {
        inc_key(9);	//Active Low
      }

      if(SENSOR5_CHK) {
        dec_key(10); 
      } else {
        inc_key(10);	//Active Low
      }

      if(SENSOR6_CHK) {
        dec_key(11); 
      } else {
        inc_key(11);	//Active Low
      }
#endif
      
      if(DC_MON_CHK) {
        dec_key(2); 
      } else {
        inc_key(2);	//Active Low
      }
      
      if(IR_SENSOR_CHK) {
        dec_key(4); 
      } else {
        inc_key(4);	//Active Low
      }
}

// 1개의 버튼에 대한 반복키 생성조건을 조사 눌려진 시간에 따라 키값을 생성 잠시라도 버튼을 떼면 눌려진 시간을 클리어
void Repeat_Key_Detect(u8 key_no)
{
	if ((KeyStatus[key_no]&0xC0) == 0x80)                   // 버튼이 완전히 눌려져 있는지 확인
	{      
		KeyOnTime[key_no]++;
		if (KeyOnTime[key_no] == REPEAT_KEY_START)    	// 버튼이 눌려진 시간을 카운트업하고 발생시간이 되었는지 확인
		{
			RepeatKey=key_no;                     // 발생시간이 되었으면, 해당 키값을 부여
			//KeyOnTime[key_no] -= CONTINUE_KEY_TIME; // 다음 발생주기 만큼 버튼이 눌려진 시간을 감산
		}
	}
	else
	{
		KeyOnTime[key_no] = 0;       
	}	
}

void Repeat_key_Scan() 
{
  Repeat_Key_Detect(0);
  Repeat_Key_Detect(3); 
}  

void init_Key (void ) 
{ 
  u8 i = 0;
  
  GPIO_Init(VOUT_PORT, (GPIO_Pin_TypeDef)(VOUT_MON), GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(DC_PORT, (GPIO_Pin_TypeDef)(DC_MON), GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(PAIRING_SW_PORT, (GPIO_Pin_TypeDef)( PAIRING_SW ), GPIO_MODE_IN_PU_NO_IT);
  
  
  GPIO_Init(IR_SENSOR_PORT, (GPIO_Pin_TypeDef)(IR_SENSOR), GPIO_MODE_IN_PU_NO_IT);
  
  //GPIO_Init(IR_SENSOR_PORT, (GPIO_Pin_TypeDef)(IR_SENSOR), GPIO_MODE_OUT_PP_HIGH_FAST);
  
  
  
  GPIO_Init(SENSOR1_PORT, (GPIO_Pin_TypeDef)(SENSOR1), GPIO_MODE_IN_FL_NO_IT);
#if defined(MULTI_6CH)
  GPIO_Init(SENSOR2_PORT, (GPIO_Pin_TypeDef)(SENSOR2), GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(SENSOR3_PORT, (GPIO_Pin_TypeDef)(SENSOR3), GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(SENSOR4_PORT, (GPIO_Pin_TypeDef)(SENSOR4), GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(SENSOR5_PORT, (GPIO_Pin_TypeDef)(SENSOR5), GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(SENSOR6_PORT, (GPIO_Pin_TypeDef)(SENSOR6), GPIO_MODE_IN_FL_NO_IT);
#endif
  
  PushKey   = KEY_NONE;
  PullKey   = KEY_NONE;
  RepeatKey = KEY_NONE;

  for(i = 0 ; i < KEY_QTY ; i++) {
    KeyStatus[i] = 0;
    KeyOnTime[i] = 0;
  }
  
    if(DC_MON_CHK) {
        RUN_LED_ON;
    } else {
       RUN_LED_OFF;
    }
}
