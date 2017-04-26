
#define PUMP_GLOBAL

#include "includes.h"
//#define  QC_TEST

#ifndef  QC_TEST
//#define PUMP_ON_WAIT_TIME        800 // 7sec
//#define PUMP_ON_WAIT_TIME        1000 // 7sec
//#define PUMP_ON_WAIT_TIME         750 // 7sec
#define PUMP_ON_WAIT_TIME        3950 // 7sec
//#define PUMP_OFF_WAIT_TIME       3250 // 7sec
#define PUMP_OFF_WAIT_TIME       50 // 7sec

#define PUMP_ON2_WAIT_TIME      30000 // 7sec
#else
//#define PUMP_ON_WAIT_TIME         1000 // 7sec
//#define PUMP_OFF_WAIT_TIME        200 // 7sec
#define PUMP_ON_WAIT_TIME         1000//850    // 7sec
#define PUMP_OFF_WAIT_TIME        10//11150  // 7sec
#define PUMP_ON2_WAIT_TIME        10//30000 // 7sec
#endif

#define SetPwm21(Value)  TIM1->CCR3H = (uint8_t)(Value >> 8);   \
                         TIM1->CCR3L = (uint8_t)(Value);



extern void TIM1_ReConfig(unsigned int fre);
extern void TIM1_Config(void);
s16 gPumpWait;
s16 qBeepfrq = 0;

static void TIM2_Pump_Config(void)
{
  /* TIM2 Peripheral Configuration for WiegandOut*/ 
  //1. Deinitializes the TIM1 peripheral registers to their default reset values.
  TIM2_DeInit();

  //2. Timer2 Base setting. (Frequency, CounterMode, Counter, Repetition).
  /* Time Base configuration 16MHZ/8(FHSI)/2(PRESCALER) = 1MHz*/
  //TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10000); // 10msec
  TIM2_TimeBaseInit(TIM2_PRESCALER_2, 10000-1); // 10msec
  
  //TIM2_OC2PreloadConfig(DISABLE);
  
  //TIM2_ICInit( TIM2_CHANNEL_2, TIM2_ICPOLARITY_RISING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV1, 0x0);
  TIM2_Cmd(ENABLE);
	
  //TIM2_OC2Init(TIM1_OCMODE_TIMING, TIM2_OUTPUTSTATE_ENABLE,CCR2_Val, TIM2_OCPOLARITY_HIGH); 			 
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
}

unsigned int AdcRead(unsigned char port)
{
  ADC1->CSR &=(uint8_t)(~ADC1_CSR_CH);
  ADC1->CSR |=port;
  ADC1_StartConversion();
  while(RESET == ADC1_GetFlagStatus(ADC1_FLAG_EOC));
  ADC1_ClearFlag(ADC1_FLAG_EOC);
  return ADC1_GetConversionValue();
}

void stopPump(void)
{
  if(PumpStatus == PORT_WAIT2) {
    if(gPumpWait > 0 ){
      READYWORK_LED_OFF;
      ONWORK_LED_OFF;
      PUMP_CTRL_OFF;
      TIM2_DeInit();
    }
  }
}

void startPump(char mode)
{
  if(mode == 0) {
    READYWORK_LED_OFF;
    ONWORK_LED_ON;
    PUMP_CTRL_ON;
    gPumpWait = PUMP_ON_WAIT_TIME;
  } else if(mode == 1) {
    READYWORK_LED_OFF;
    ONWORK_LED_OFF;
    PUMP_CTRL_ON;
    gPumpWait = PUMP_ON2_WAIT_TIME;
  }
  TIM2_Pump_Config();
}


void callback_timer(void)
{
  static unsigned int count = 0;
  static u8 dir = 0;
  
  if(PumpStatus == PORT_START) {
    if(gPumpWait > 0 ){
       gPumpWait--;
       count++;
       
      if(BeepMode == 0) {
        if((count % 5) == 0){
          PUMP_CTRL_OFF;
          
          if(dir == 0) {
            qBeepfrq -= 200;
            if(qBeepfrq <= 2500) {
              dir = 1;
              qBeepfrq = 2500;
            }
          } else {
            qBeepfrq += 200;
            if(qBeepfrq >= 3500) {
              dir = 0;
              qBeepfrq = 3500;
            }
          }
          
           TIM1_ReConfig(qBeepfrq);
          
          PUMP_CTRL_ON;
        } 

      } else if(BeepMode == 1) {
        if((count % 25) == 0){
          PUMP_CTRL_OFF;
          
          if(dir == 0) {
            qBeepfrq -= 200;
            if(qBeepfrq <= 2500) {
              dir = 1;
              qBeepfrq = 2500;
            }
            
            qBeepfrq = 3000;
          } else if(dir == 1){
            qBeepfrq += 200;
            if(qBeepfrq >= 3500) {
              dir = 2;
              qBeepfrq = 3500;
            }
            
            qBeepfrq = 1;
          } else if(dir == 2){
            qBeepfrq += 200;
            if(qBeepfrq >= 3500) {
              dir = 2;
              qBeepfrq = 3500;
            }
            
            qBeepfrq = 2000;
          }
          
           TIM1_ReConfig(qBeepfrq);
          
          PUMP_CTRL_ON;
        } 

      } else {
        
      }

       if((gPumpWait % 50) == 0){
         ONWORK_LED_TOGGLE;
       }

       if(gPumpWait == 0) {
         PUMP_CTRL_OFF;
         PumpStatus = PORT_STOP;
         gPumpWait = PUMP_OFF_WAIT_TIME;
       }
    }
    
  } else if(PumpStatus == PORT_WAIT) {
    if(gPumpWait > 0 ){
       gPumpWait--;
       if((gPumpWait % 50) == 0){
         READYWORK_LED_TOGGLE;
       }
       if(gPumpWait == 0) {
         PUMP_CTRL_OFF;
         READYWORK_LED_ON;
         ONWORK_LED_OFF;
         //PUMP_CTRL_OFF;
         PumpStatus = PORT_READY;
         TIM2_DeInit();
         
       }
    }
  } else if(PumpStatus == PORT_STOP) {
    if(gPumpWait > 0 ) {
       gPumpWait--;
       if((gPumpWait % 50) == 0){
         ONWORK_LED_TOGGLE;
       }
       if(gPumpWait == 0) {
#ifdef QC_TEST
         PumpStatus = PORT_START;
         PUMP_CTRL_ON;
         gPumpWait = PUMP_ON_WAIT_TIME;
#else
         READYWORK_LED_ON;
         ONWORK_LED_OFF;
         //PUMP_CTRL_OFF;
         PumpStatus = PORT_READY;
         TIM2_DeInit();
         TIM1_Config();
#endif         
       }
    }
  }
  
  return;
}

#if 1
void pumpService(void)
{
    char key_value = KEY_NONE;
    u16 adc = 0 ;
    static u16 frq = 0; 
    static u16 preVal = 0,Val = 0; 
    //disableInterrupts();

    if(PullKey !=KEY_NONE) {
        key_value = key_table[PullKey];
        PullKey = KEY_NONE;
    } else if(PushKey != KEY_NONE) {
        //key_value = key_table[PushKey];
        PushKey = KEY_NONE;
    } else if (RepeatKey != KEY_NONE) {
        key_value = KEY_TOUCH_LONG;
        RepeatKey = KEY_NONE;
    }
    
    switch(key_value)		
    {
        case KEY_TOUCH:
        {
            switch (PumpStatus) 
            {
            case PORT_READY:
              PumpStatus = PORT_START;
              BeepMode = 0;
              FrqMode = 0;
              qBeepfrq = 3500;
              TIM1_ReConfig(qBeepfrq);
              startPump(0);
              
              break;
            case PORT_START:
              FrqMode++;
              if(FrqMode > 2)
                FrqMode =0;
              
              BeepMode++;
              if(BeepMode > 2)
                BeepMode =0;  
              
#if 0
              
              PUMP_CTRL_OFF;
              
              if(FrqMode == 0)
               TIM1_ReConfig(2300);
              else if(FrqMode == 1)
               TIM1_ReConfig(2700);
              else
               TIM1_ReConfig(2800);
              PUMP_CTRL_ON;
#endif              
              break;  
              
              
            case PORT_WAIT2:
              PumpStatus = PORT_READY;
              READYWORK_LED_ON;
              break;
            }
            break;
        }
        case KEY_TOUCH_LONG:
        {
            switch (PumpStatus) 
            {
            case PORT_READY:
              //PumpStatus = PORT_WAIT;
              //startPump(1);
              break;
            case PORT_WAIT:
              //PumpStatus = PORT_WAIT2;
              //stopPump();
              break;
              
            case PORT_START:
               BeepMode++;
              if(BeepMode > 2)
                BeepMode =0;  
              //PumpStatus = PORT_READY;
              //READYWORK_LED_ON;
              //ONWORK_LED_OFF;
              //PUMP_CTRL_OFF;
              break;  
            }
            break;
        }
    }
     
    //enableInterrupts();
}
#else
void pumpService(void)
{
    char key_value = KEY_NONE;

    //disableInterrupts();
    
    if(PullKey !=KEY_NONE) {
        key_value = key_table[PullKey];
        PullKey = KEY_NONE;
    } else if(PushKey != KEY_NONE) {
        key_value = key_table[PushKey];
        PushKey = KEY_NONE;
    } else if (RepeatKey != KEY_NONE) {
        key_value = KEY_TOUCH_LONG;
        RepeatKey = KEY_NONE;
    }
    
    switch(key_value)		
    {
        case KEY_TOUCH:
        {
            switch (PumpStatus) 
            {
            case PORT_READY:
              PumpStatus = PORT_START;
              READYWORK_LED_TOGGLE;
              ONWORK_LED_TOGGLE;
              //startPump(0);
              break;
            case PORT_START:
              PumpStatus = PORT_READY;
              READYWORK_LED_TOGGLE;
              ONWORK_LED_TOGGLE;;
              break;
            }
            break;
        }
        case KEY_TOUCH_LONG:
        {
            switch (PumpStatus) 
            {
            case PORT_READY:
              PumpStatus = PORT_WAIT;
              startPump(1);
              break;
            case PORT_WAIT:
              PumpStatus = PORT_WAIT2;
              stopPump();
              break;
            }
            break;
        }
    }
     
    //enableInterrupts();
}

#endif

void InitPump(void)
{
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)( ONWORK_LED ), GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)( READYWORK_LED ), GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)( PUMP_CTRL ), GPIO_MODE_OUT_PP_LOW_SLOW);
  
  READYWORK_LED_ON;
  ONWORK_LED_OFF;
  PUMP_CTRL_OFF;
  BeepMode = 0;
  return;
}
