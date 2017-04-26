
#define BUZZER_GLOBAL

#include "includes.h"

#define ARRAY_SIZE(X)                (sizeof(X)/sizeof(X[0]))    
#define BUZZER_ON_WAIT_TIME         500  //1 = 10msec
#define BUZZER_ON1_WAIT_TIME         60
#define BUZZER_ON2_WAIT_TIME         60
#define BUZZER_OFF_WAIT_TIME         10

#define MAX_VOL_FRQ    2100
#define MIN_VOL_FRQ    1900
#define BUZZER_STEP    25

//#define BUZZER_ON()             __buzzer_on()
//#define BUZZER_OFF()            __buzzer_off()

#define BUZZER_LEVEL_LOW        1
#define BUZZER_LEVEL_MID        2
#define BUZZER_LEVEL_HI         3

#define BUZZER_PORT             GPIOD
#define BUZZER_BUZ1_PIN         GPIO_PIN_2
#define BUZZER_BUZ3_PIN         GPIO_PIN_3

#if defined(MULTI_6CH)
#undef BUZZER_PORT
#undef BUZZER_BUZ1_PIN
#undef BUZZER_BUZ3_PIN
#define BUZZER_PORT             GPIOC
#define BUZZER_BUZ1_PIN         GPIO_PIN_2
#define BUZZER_BUZ3_PIN         GPIO_PIN_4
#endif

static bool BuzzerEnable;

static u8 qBeepLevel = BUZZER_LEVEL_HI;

void __buzzer_on()
{
  switch(qBeepLevel) {
  case BUZZER_LEVEL_LOW:BUZZER_PORT->ODR |= BUZZER_BUZ1_PIN;break;
  case BUZZER_LEVEL_MID:BUZZER_PORT->ODR |= BUZZER_BUZ3_PIN;break;
  case BUZZER_LEVEL_HI: BUZZER_PORT->ODR |= (BUZZER_BUZ3_PIN|BUZZER_BUZ1_PIN);break;
  }
}

void __buzzer_off()
{
  BUZZER_PORT->ODR &=~(BUZZER_BUZ3_PIN|BUZZER_BUZ1_PIN); 
}

void callback_timer(void)
{
  static unsigned int count = 0;
  if((count++ % 5) == 0){
    BUZZER_ON();
  }
  return;
}

void stopBuzzer(void)
{
  BuzzerEnable = FALSE;
  BUZZER_OFF();
  return;
}

void startBuzzer(char mode)
{
  BuzzerEnable = TRUE;
  BUZZER_ON();
  return;
}

void setBuzzerMode(u8 mode)
{
  return;
}

void setBuzzerLv(void)
{

  switch(qBeepLevel) {
    case BUZZER_LEVEL_LOW: qBeepLevel= BUZZER_LEVEL_MID;break;
    case BUZZER_LEVEL_MID: qBeepLevel=BUZZER_LEVEL_HI;  break;
    case BUZZER_LEVEL_HI:  qBeepLevel=BUZZER_LEVEL_LOW; break;
  }
  
  BUZZER_ON();
  Delay(300);
  BUZZER_OFF();
}

u8 getBuzzerLv(void)
{
  return qBeepLevel;
}

void BuzzerBtn(void)
{
  BUZZER_ON();
  Delay(300);
  BUZZER_OFF();

}

void BuzzerBtn100ms(void)
{
  BUZZER_ON();
  Delay(1000);
  BUZZER_OFF();
}

void InitBuzzer(void)
{

  BuzzerEnable = FALSE;

  qBeepLevel = BUZZER_LEVEL_HI;
  GPIO_Init(BUZZER_PORT, (GPIO_Pin_TypeDef)(BUZZER_BUZ1_PIN|BUZZER_BUZ3_PIN), GPIO_MODE_OUT_PP_LOW_FAST);
  BUZZER_OFF();

  return;
}

void HandleBuzzerTimer()
{
  static int loop = 0;
  if(BuzzerEnable) {
    if(loop == 0) {
      callback_timer();
      loop = 0;
    }
  }
}