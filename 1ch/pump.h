#ifndef ___PUMP_H___
#define ___PUMP_H___

#ifdef PUMP_GLOBAL
	#define PUMP_EXT
#else
	#define PUMP_EXT extern
#endif

typedef enum {
  PORT_READY,
  PORT_START,
  PORT_STOP,
  PORT_WAIT,
  PORT_WAIT2
} mStatus;

// variable definition
PUMP_EXT mStatus PumpStatus;
PUMP_EXT u8 BeepMode;
PUMP_EXT u8 FrqMode;

PUMP_EXT u16 gPumpOnWait;
PUMP_EXT u16 gPumpOffWait;

PUMP_EXT void pumpService(void);
PUMP_EXT void stopPump(void);
PUMP_EXT void startPump(char mode);
PUMP_EXT void InitPump(void);
PUMP_EXT void callback_timer(void);
#endif	//___PORT_H___

