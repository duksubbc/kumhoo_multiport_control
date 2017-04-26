#ifndef ___PORT_H___
#define ___PORT_H___

#ifdef PORTSCAN_GLOBAL
	#define PORTSCAN_EXT
#else
	#define PORTSCAN_EXT extern
#endif

typedef enum {
  PORT_READY,
  PORT_START,
  PORT_ALARM,
  PORT_RESET,
  PORT_INIT,
  PORT_REQ
  
} mStatus;

#define KEY_NONE		0xFF
#define KEY_PORT1		0x01
#define KEY_PORT2		0x02


// variable definition
PORTSCAN_EXT mStatus PortNo1Status;
PORTSCAN_EXT mStatus PortNo2Status;

PORTSCAN_EXT u8 PushSwitch;              // PUSH 버튼의 값 
PORTSCAN_EXT u8 PullSwitch;               // PULL 버튼의 값 
PORTSCAN_EXT const unsigned char        port_table[];
PORTSCAN_EXT unsigned char		deviceID;
PORTSCAN_EXT unsigned char		relayState;
PORTSCAN_EXT unsigned char		portStauts;

PORTSCAN_EXT void portScan(void );
PORTSCAN_EXT void PortService(void);
PORTSCAN_EXT mStatus getPortState(char portNumber);
PORTSCAN_EXT void setPortState(char portNumber, mStatus status );

#endif	//___PORT_H___

