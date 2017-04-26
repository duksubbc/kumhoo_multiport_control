
#define KEY_GLOBAL

#include <stm8s.h>

#include "includes.h"
#include "board.h"

#include "main.h"
#include "key.h"
#include "uart.h"

#if defined(MULTI_6CH)

#define KEY_QTY      11            // ��ư�� �� 
#define ON_TIME       30            //5        // ��ư ���� �νĽð� (4 x 4line x 2ms = 32ms) 
#define ON_MAX       10
#define REPEAT_KEY_START       250    //  40*12ms +40  �ݺ�Ű�� ó�� �߻��ϴ� �����Դϴ�.
#define CONTINUE_KEY_TIME      20     //  �ݺ�Ű�� �߻��ϴ� �ֱ��Դϴ�.

#else

#define KEY_QTY       6            // ��ư�� �� 
#define ON_TIME       30            //5        // ��ư ���� �νĽð� (4 x 4line x 2ms = 32ms) 
#define ON_MAX       10
#define REPEAT_KEY_START       250    //  40*12ms +40  �ݺ�Ű�� ó�� �߻��ϴ� �����Դϴ�.
#define CONTINUE_KEY_TIME      20     //  �ݺ�Ű�� �߻��ϴ� �ֱ��Դϴ�.

#endif

volatile  unsigned  char  KeyStatus[KEY_QTY]=  {0};       // ��ư 1���� 1 byte�� ���۸� �Ҵ� 
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


// 11: ��ư�� �� ������(Push)
// 10: ��ư�� ON ����
// 01: ��ư�� �� ������(Pull)
// 00: ��ư�� OFF ����

static void inc_key(u8 key) 
{ 
    u8 byte; 

    byte = KeyStatus[key];  	        // ��ư�� ���� ���°� �������� 
    if ((byte & 0x3F) < ON_TIME)	byte++;	// ��ư�� ������ �ð����� UP count 
    if ((byte & 0x3F) >= ON_TIME)   	// ��ư�� �����ð����� ������ �־��ٸ� PUSH�� ���� 
    { 
            if ((byte & 0x80) == 0)         // ��ư�� �������°� OFF 
            { 
                      byte = byte | 0xC0;    // ��ư�� ON�������� ǥ��(b7:1) 
                      PushKey = key;         // [PUSH]�� �νĵ� ��ư�� ������ ���� 
            }
            else      // ��ư�� �������°� ON 
            {
                     byte = byte | 0x80;
                     byte = (byte & (~0x40));		
            }
    }; 
    
    KeyStatus[key] = byte;       // ��ư�� ���°��� ����                       
} 

static void dec_key(u8  key) 
{ 
    u8 byte; 
    byte = KeyStatus[key];                // ��ư�� ���� ���°� �������� 
    if ((byte & 0x3F) > 0) 	byte--;       // ��ư�� ������ �ð����� DOWN count 
    if ((byte & 0x3F) == 0) 	      // ��ư�� �����ð����� ������ �־��ٸ� PULL�� ���� 
    { 
            if ((byte & 0x80) == 0x80)          // ��ư�� �������°� ON
            { 
                    byte =( byte & (~ 0x80)); // ��ư OFF
                    byte = byte | 0x40;      // Up Detect
                    PullKey = key;        // [PULL]�� �νĵ� ��ư�� ������ ���� 			
            }
            else
            {
                    byte = 0;                                // ��ư�� OFF�������� ǥ��(b7:0) 
            }
    };
    
    KeyStatus[key] = byte;                 // ��ư�� ���°��� ����    
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

// 1���� ��ư�� ���� �ݺ�Ű ���������� ���� ������ �ð��� ���� Ű���� ���� ��ö� ��ư�� ���� ������ �ð��� Ŭ����
void Repeat_Key_Detect(u8 key_no)
{
	if ((KeyStatus[key_no]&0xC0) == 0x80)                   // ��ư�� ������ ������ �ִ��� Ȯ��
	{      
		KeyOnTime[key_no]++;
		if (KeyOnTime[key_no] == REPEAT_KEY_START)    	// ��ư�� ������ �ð��� ī��Ʈ���ϰ� �߻��ð��� �Ǿ����� Ȯ��
		{
			RepeatKey=key_no;                     // �߻��ð��� �Ǿ�����, �ش� Ű���� �ο�
			//KeyOnTime[key_no] -= CONTINUE_KEY_TIME; // ���� �߻��ֱ� ��ŭ ��ư�� ������ �ð��� ����
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
