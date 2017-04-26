#ifndef ___BUZZER_H___
#define ___BUZZER_H___

#ifdef BUZZER_GLOBAL
	#define BUZZER_EXT
#else
	#define BUZZER_EXT extern
#endif


#define BUZZER_ON()             __buzzer_on()
#define BUZZER_OFF()            __buzzer_off()

// variable definition

BUZZER_EXT u8 BuzzerStatus;
BUZZER_EXT bool BuzzerEnable;

BUZZER_EXT u8 BeepMode;
BUZZER_EXT u8 FrqMode;

BUZZER_EXT u16 gBuzzerOnWait;
BUZZER_EXT u16 gBuzzerOffWait;
BUZZER_EXT u8 gBuzzerLoop;
BUZZER_EXT s16 BuzzerDelay; 

BUZZER_EXT void stopBuzzer(void);
BUZZER_EXT void startBuzzer(char mode);
BUZZER_EXT void InitBuzzer(void);
BUZZER_EXT void HandleBuzzerTimer();
BUZZER_EXT void setBuzzerLv(void);
BUZZER_EXT void BuzzerBtn(void);
BUZZER_EXT void BuzzerBtn100ms(void);

BUZZER_EXT void __buzzer_on();
BUZZER_EXT void __buzzer_off();
#endif	//___BUZZER_H___

