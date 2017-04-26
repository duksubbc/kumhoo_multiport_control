
#define BUZZER_GLOBAL

#include "includes.h"


//#define BUZZER_OP_FRQ

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

u8 BuzzerStatus;
bool BuzzerEnable;

u8 BeepMode;
u8 FrqMode;

u16 gBuzzerOffWait;
u8 gBuzzerLoop;
s16 BuzzerDelay; 

static s16 qBeepfrqMax = MAX_VOL_FRQ;
static s16 qBeepfrqMin = MIN_VOL_FRQ;
static s16 qBeepfrq = MAX_VOL_FRQ;
//static u16 gBuzzerStep = BUZZER_STEP;
static s16 gBuzzerWait;

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

static void Buzzer_Config(int fre)
{
#ifdef BUZZER_OP_FRQ
  // PWM out config
  uint16_t TIM2_Period;
  
  TIM2_Period = 1000000 / fre;
  
  /* TIM2 Peripheral Configuration for WiegandOut*/ 
  //1. Deinitializes the TIM1 peripheral registers to their default reset values.
  TIM2_DeInit();

  //2. Timer2 Base setting. (Frequency, CounterMode, Counter, Repetition).
  /* Time Base configuration 16MHZ/2(FHSI)/8(PRESCALER) = 1MHz*/
  TIM2_TimeBaseInit(TIM2_PRESCALER_8, TIM2_Period-1); // 10msec

  TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,
               TIM2_Period >> 1,TIM1_OCPOLARITY_HIGH );
  
  TIM2_OC2PreloadConfig(ENABLE);
  
  /* Enables TIM2 peripheral Preload register on ARR */  
  TIM2_ARRPreloadConfig(ENABLE);
  
  TIM2_Cmd(ENABLE);		 
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
#else
  // Occur every 10 msec.
  uint16_t TIM2_Period;
  
  TIM2_Period = 10000;
  
  /* TIM2 Peripheral Configuration for WiegandOut*/ 
  //1. Deinitializes the TIM1 peripheral registers to their default reset values.
  TIM2_DeInit();

  //2. Timer2 Base setting. (Frequency, CounterMode, Counter, Repetition).
  /* Time Base configuration 16MHZ/2(FHSI)/8(PRESCALER) = 1MHz*/
  TIM2_TimeBaseInit(TIM2_PRESCALER_8, TIM2_Period-1); // 10msec

  TIM2_Cmd(ENABLE);		 
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
#endif
}

void callback_timer(void)
{
  static unsigned int count = 0;
  static u8 dir = 0;
  
  if(BuzzerDelay == 0) {
    if(BuzzerStatus == 1) {
      if(gBuzzerWait > 0 ) {
         gBuzzerWait--;
         count++;
        if(BeepMode == 0) {
          if((count % 5) == 0){
            BUZZER_OFF();
            if(dir == 0) {
              qBeepfrq -= 200;
              if(qBeepfrq <= qBeepfrqMin) {
                dir = 1;
                qBeepfrq = qBeepfrqMin;
              }
            } else {
              qBeepfrq += 200;
              if(qBeepfrq >= qBeepfrqMax) {
                dir = 0;
                qBeepfrq = qBeepfrqMax;
              }
            }
#ifdef BUZZER_OP_FRQ
            Buzzer_Config(qBeepfrq);
#else
            //if(qBeepLevel++ >= BUZZER_LEVEL_HI)
            //  qBeepLevel = BUZZER_LEVEL_LOW;
#endif
            BUZZER_ON();
          }
        } else if(BeepMode == 1) {

          if((count % 6) == 0){
            BUZZER_OFF();
            if(1)
            {
              qBeepfrq -= BUZZER_STEP;
              if(qBeepfrq <= qBeepfrqMin) {
                //dir = 1;
                qBeepfrq = qBeepfrqMax;
              }
            }
#ifdef BUZZER_OP_FRQ            
            Buzzer_Config(qBeepfrq);
#endif
            BUZZER_ON();
          }
        }
        
        if(gBuzzerWait == 0) {
          BUZZER_OFF();
          BuzzerStatus = 0;
          gBuzzerWait = BUZZER_OFF_WAIT_TIME;
        }
      }
    } else if(BuzzerStatus == 0) {
      if(gBuzzerWait > 0 ) {
         gBuzzerWait--;
         BUZZER_OFF();
         if(gBuzzerWait == 0) {
          BuzzerStatus = 1;
          if(BeepMode == 0)
            gBuzzerWait = BUZZER_ON_WAIT_TIME;
          else if(BeepMode == 1) {
            gBuzzerWait = BUZZER_ON1_WAIT_TIME;
            gBuzzerLoop ++;
            qBeepfrq = MAX_VOL_FRQ;
            if(gBuzzerLoop > 3) {
              gBuzzerWait = 0;
              gBuzzerLoop = 0;
              stopBuzzer();
            }
          }
        }
      }
    }
  }
  return;
}

void stopBuzzer(void)
{
  if(gBuzzerWait > 0 ){
    TIM2_DeInit();
    gBuzzerWait = 0;
    BUZZER_OFF();
  }
  
  BuzzerEnable = FALSE;
}


void startBuzzer(char mode)
{
  BuzzerStatus = 1;
  BuzzerEnable = TRUE;
  BUZZER_ON();
  
  if(mode == 0) {   
    BuzzerStatus = 1;
    BeepMode = 0;
    qBeepfrq = 3200;
    qBeepfrqMax = 3200;
    qBeepfrqMin = 2000;
    gBuzzerWait = BUZZER_ON_WAIT_TIME;
    Buzzer_Config(qBeepfrq);
    BuzzerDelay = 0;
  } else if(mode == 1) {
  
    BuzzerStatus = 1;
    BeepMode = 1;
    qBeepfrq = MAX_VOL_FRQ;
    qBeepfrqMax = MAX_VOL_FRQ;
    qBeepfrqMin = MIN_VOL_FRQ;
    gBuzzerWait = BUZZER_ON1_WAIT_TIME;
    Buzzer_Config(qBeepfrq);
    BuzzerDelay = 3000;
  } else if(mode == 2) {
    BuzzerStatus = 1;
    BeepMode = 0;
    qBeepfrq = 3200;
    qBeepfrqMax = 3200;
    qBeepfrqMin = 2000;
    gBuzzerWait = BUZZER_ON_WAIT_TIME;
    Buzzer_Config(qBeepfrq);
    BuzzerDelay = 0;
  }
}

void setBuzzerMode(u8 mode)
{
  BeepMode = mode;
}

void setBuzzerLv(void)
{
#ifndef BUZZER_OP_FRQ
  switch(qBeepLevel) {
    case BUZZER_LEVEL_LOW: qBeepLevel= BUZZER_LEVEL_MID;break;
    case BUZZER_LEVEL_MID: qBeepLevel=BUZZER_LEVEL_HI;  break;
    case BUZZER_LEVEL_HI:  qBeepLevel=BUZZER_LEVEL_LOW; break;
  }
  
  BUZZER_ON();
  Delay(300);
  BUZZER_OFF();
#endif
}

u8 getBuzzerLv(void)
{
  return qBeepLevel;
}


void BuzzerBtn(void)
{
#ifndef BUZZER_OP_FRQ
  BUZZER_ON();
  Delay(300);
  BUZZER_OFF();
#endif
}

void BuzzerBtn100ms(void)
{
#ifndef BUZZER_OP_FRQ
  BUZZER_ON();
  Delay(1000);
  BUZZER_OFF();
#endif
}

void InitBuzzer(void)
{
  BuzzerStatus = 0;
  BeepMode = 0;
  gBuzzerWait = 0;
  gBuzzerLoop = 0;
  BuzzerEnable = FALSE;
  BuzzerDelay = -1;

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