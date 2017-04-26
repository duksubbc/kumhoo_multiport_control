#ifndef ___SECURITY_H___
#define ___SECURITY_H___

#ifdef SECURITY_GLOBAL
	#define SECURITY_EXT
#else
	#define SECURITY_EXT extern
#endif

typedef enum {
  SECUR_READY,
  SECUR_START,
  SECUR_ALARM,
  SECUR_STOP,
  SECUR_WAIT,
  SECUR_WAIT2
} securStatus;

typedef enum {
  PAIR_READY,
  PAIR_START,
  PAIR_STOP
} pairStatus;

// variable definition
SECURITY_EXT securStatus mSecurStatus[];
SECURITY_EXT bool  mDCShort;
SECURITY_EXT bool  cSensorEnable;
SECURITY_EXT bool  pSensorEnable;
SECURITY_EXT u8    gMachineStatus;
SECURITY_EXT s16   mDCShort_Delay;

SECURITY_EXT pairStatus mPairStatus;

SECURITY_EXT void init_security();
SECURITY_EXT void securityService(void);
SECURITY_EXT pairStatus getPairStatus(void);

#endif	//___SECURITY_H___

