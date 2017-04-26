
#define PORTSCAN_GLOBAL

#include <stm8s.h>

#include "main.h"
#include "port.h"
#include "uart.h"


#define PORT_QTY      2            // 버튼의 수 
#define ON_TIME       5             // 버튼 눌림 인식시간(4 x 4line x 2ms = 32ms) 
#define ON_MAX       10

volatile unsigned char  PortStatus[PORT_QTY]=  {0};       // 버튼 1개당 1 byte의 버퍼를 할당 
volatile  unsigned char PortOnTime[PORT_QTY] = {0};

const unsigned char port_table[]=
{
	KEY_PORT1,		
	KEY_PORT2,							
};

// 11: 버튼이 막 눌려짐(Push)
// 10: 버튼이 ON 상태
// 01: 버튼이 막 떨어짐(Pull)
// 00: 버튼이 OFF 상태

static void inc_key(u8 port_no) 
{ 
	u8 byte; 

	byte = PortStatus[port_no];  	        // 버튼의 직전 상태값 가져오기 
	if ((byte & 0x3F) < ON_TIME)	byte++;	// 버튼이 눌려진 시간값을 UP count 
	if ((byte & 0x3F) >= ON_TIME)   	// 버튼이 일정시간동안 눌려져 있었다면 PUSH로 판정 
	{ 
		if ((byte & 0x80) == 0)             // 버튼의 이전상태가 OFF 
		{ 
			  byte = byte | 0xC0;                 // 버튼이 ON상태임을 표시(b7:1) 
			  PushSwitch = port_no;    // [PUSH]로 인식된 버튼의 논리값을 결정 
		}
		else      // 버튼의 이전상태가 ON 
		{
			 byte = byte | 0x80;
			 byte = (byte & (~0x40));		
		}
	}; 
        
	PortStatus[port_no] = byte;       // 버튼의 상태값을 저장                       
} 

static void dec_key(u8  port_no) 
{ 
	u8 byte; 
	byte = PortStatus[port_no];                // 버튼의 직전 상태값 가져오기 
	if ((byte & 0x3F) > 0) 	byte--;       // 버튼이 눌려진 시간값을 DOWN count 
	if ((byte & 0x3F) == 0) 	// 버튼이 일정시간동안 떨어져 있었다면 PULL로 판정 
	{ 
		if ((byte & 0x80) == 0x80)          // 버튼의 이전상태가 ON
		{ 
			byte =( byte & (~ 0x80)); // 버튼 OFF
			byte = byte | 0x40; // Up Detect
			PullSwitch = port_no;        // [PULL]로 인식된 버튼의 논리값을 결정 			
		}
		else
		{
			byte = 0;                                // 버튼이 OFF상태임을 표시(b7:0) 
		}
	};
        
	PortStatus[port_no] = byte;                 // 버튼의 상태값을 저장    
}  


void portScan (void ) 
{ 
      if(TOUCH_IN_CHK) {
        dec_key(1); 
      } else {
        inc_key(1);	//Active Low
      }
} 


mStatus getPortState(char portNumber)
{
  if(portNumber == KEY_PORT1)
    return PortNo1Status;
  else if(portNumber == KEY_PORT2)
    return PortNo2Status;
  else 
    return -1;
}

void setPortState(char portNumber, mStatus status )
{
  disableInterrupts();
  if(portNumber == KEY_PORT1) {
    if(PortNo1Status != status)
      PortNo1Status = status;
  }
  else if(portNumber == KEY_PORT2) {
    if(PortNo2Status != status)
      PortNo2Status = status;
  }
  
  enableInterrupts();
  
  return;
}

void PortAlarm(void)
{
  static char sync = 0;
  
  if((PortNo1Status == PORT_ALARM) && 
     (PortNo2Status == PORT_ALARM)) {
       if(!sync) {
        PORT1_LED_ON;
        LED1_ON;
        PORT2_LED_ON;
        LED2_ON;
        sync = 1;
       }
        PORT1_LED_TOGGLE;
        LED1_TOGGLE;
        PORT2_LED_TOGGLE;
        LED2_TOGGLE;
     } else {
        sync = 0;
        if(PortNo1Status == PORT_START) {
            PORT1_LED_ON;
            LED1_ON;
        } else if(PortNo1Status == PORT_ALARM) {
            PORT1_LED_TOGGLE;
            LED1_TOGGLE;
        } else if((PortNo1Status == PORT_READY) ||
                  (PortNo1Status == PORT_RESET)){
            PORT1_LED_OFF;
            LED1_OFF;
        }

        if(PortNo2Status == PORT_START) {
            PORT2_LED_ON;
            LED2_ON;
        } else if(PortNo2Status == PORT_ALARM) {
            PORT2_LED_TOGGLE;
            LED2_TOGGLE;
        } else if((PortNo2Status == PORT_READY) ||
                  (PortNo2Status == PORT_RESET)){
            PORT2_LED_OFF;
            LED2_OFF;
        }
     } 
}


void RequesID()
{
  static char sync = 0;
  if((PortNo1Status == PORT_INIT) ||(PortNo2Status == PORT_INIT)){
    if(!sync) {
      LED1_ON;
      LED2_OFF;
      sync = 1;
    }
    LED1_TOGGLE;
    LED2_TOGGLE;
  } else if((PortNo1Status == PORT_REQ) || (PortNo2Status == PORT_REQ)) {
    if(sync) {
      LED1_ON;
      LED2_ON;
      sync = 0;
    }
    LED1_TOGGLE;
    LED2_TOGGLE;
  } 
  return;
}

void PortService(void)
{
	char key_value = KEY_NONE;

        //disableInterrupts();
        
	if(PullSwitch !=KEY_NONE) {
		key_value = port_table[PullSwitch];
		PullSwitch = KEY_NONE;
	} else if(PushSwitch != KEY_NONE) {
		key_value = port_table[PushSwitch];
		PushSwitch = KEY_NONE;
	}
	
	switch(key_value)		
	{
		case KEY_PORT1:
		{
			switch ( PortNo1Status) 
			{
                        case PORT_READY:
                        case PORT_RESET:
                          PortNo1Status = PORT_START;
                          break;
                        case PORT_START:
                          PortNo1Status = PORT_ALARM;
                          txBuf[2] =CMD_PORT_EVT; 
                          txBuf[3] =KEY_PORT1; 
                          answer_send();
                          break;
                        case PORT_ALARM:
                        break;
                        case PORT_INIT:
                          /*
                          PortNo1Status = PORT_REQ;
                          PortNo2Status = PORT_REQ;
                          deviceID = 0xFF;
                          txBuf[2] =CMD_ID_REQ; 
                          txBuf[3] =0; 
                          answer_send();
                          */
                          break;
			}
			break;
		}	
		case KEY_PORT2:
		{
			switch ( PortNo2Status) {
                          case PORT_READY:
                          case PORT_RESET:
                            PortNo2Status = PORT_START;
                            break;
                          case PORT_START:
                            PortNo2Status = PORT_ALARM;
                            txBuf[2] =CMD_PORT_EVT; 
                            txBuf[3] =KEY_PORT2; 
                            answer_send();
                            break;
                          case PORT_ALARM:
                            break;
                          case PORT_INIT:
                            PortNo1Status = PORT_REQ;
                            PortNo2Status = PORT_REQ;
                            deviceID = 0xFF;
                            txBuf[2] =CMD_ID_REQ; 
                            txBuf[3] =0; 
                            answer_send();
                            break;
			}
			break;
		}
	}
        
        //enableInterrupts();
}

