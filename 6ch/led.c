/*
*
*/
#define  LED_GLOBAL
#include "includes.h"
#include "board.h"
#include "led.h"

#define TYPE_CHECK_DELAY        10

CLedMode gCLedMode[MAX_CHANNEL_NUMS+1];
static u16 runLed_count = 0;
static u16 cLed_count[MAX_CHANNEL_NUMS+1];
  
static bool cable_check(u8 ch)
{
  bool ret = FALSE;
  switch(ch) {
  case CABLE_CH1:
    GPIO_Init(CH1_LED_PORT, (GPIO_Pin_TypeDef)(CH1_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CH1_LED_PORT, (GPIO_Pin_TypeDef)(CH1_LED), GPIO_MODE_IN_FL_NO_IT);
    msDelay(TYPE_CHECK_DELAY);
    ret = (CH1_LED_SW_CHK)?TRUE:FALSE;
    GPIO_Init(CH1_LED_PORT, (GPIO_Pin_TypeDef)(CH1_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    break;
#if defined(MULTI_6CH)
  case CABLE_CH2:
    GPIO_Init(CH2_LED_PORT, (GPIO_Pin_TypeDef)(CH2_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CH2_LED_PORT, (GPIO_Pin_TypeDef)(CH2_LED), GPIO_MODE_IN_FL_NO_IT);
    msDelay(TYPE_CHECK_DELAY);
    ret = (CH2_LED_SW_CHK)?TRUE:FALSE ;
    GPIO_Init(CH2_LED_PORT, (GPIO_Pin_TypeDef)(CH2_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    break;
  case CABLE_CH3:
    GPIO_Init(CH3_LED_PORT, (GPIO_Pin_TypeDef)(CH3_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CH3_LED_PORT, (GPIO_Pin_TypeDef)(CH3_LED), GPIO_MODE_IN_FL_NO_IT);
    msDelay(TYPE_CHECK_DELAY);
    ret = (CH3_LED_SW_CHK)?TRUE:FALSE ;
    GPIO_Init(CH3_LED_PORT, (GPIO_Pin_TypeDef)(CH3_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    break;
  case CABLE_CH4:
    GPIO_Init(CH4_LED_PORT, (GPIO_Pin_TypeDef)(CH4_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CH4_LED_PORT, (GPIO_Pin_TypeDef)(CH4_LED), GPIO_MODE_IN_FL_NO_IT);
    msDelay(TYPE_CHECK_DELAY);
    ret = (CH4_LED_SW_CHK)?TRUE:FALSE ;
    GPIO_Init(CH4_LED_PORT, (GPIO_Pin_TypeDef)(CH4_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    break;
  case CABLE_CH5:
    GPIO_Init(CH5_LED_PORT, (GPIO_Pin_TypeDef)(CH5_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CH5_LED_PORT, (GPIO_Pin_TypeDef)(CH5_LED), GPIO_MODE_IN_FL_NO_IT);
    msDelay(TYPE_CHECK_DELAY);
    ret = (CH5_LED_SW_CHK)?TRUE:FALSE ;
    GPIO_Init(CH5_LED_PORT, (GPIO_Pin_TypeDef)(CH5_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    break;
  case CABLE_CH6:
    GPIO_Init(CH6_LED_PORT, (GPIO_Pin_TypeDef)(CH6_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CH6_LED_PORT, (GPIO_Pin_TypeDef)(CH6_LED), GPIO_MODE_IN_FL_NO_IT);
    msDelay(TYPE_CHECK_DELAY);
    ret = (CH6_LED_SW_CHK)?TRUE:FALSE ;
    GPIO_Init(CH6_LED_PORT, (GPIO_Pin_TypeDef)(CH6_LED), GPIO_MODE_OUT_PP_LOW_FAST);
    break;
#endif
  }
  
  return ret;
}
  
void set_mode_runled(u8 mode)
{
  gRunLedMode = mode;
  runLed_count = 0;
  return;
}

void set_mode_cled(CLedMode mode,u8 ch)
{
  switch(mode) {
  case CABLE_READY_MODE:
    {
      switch(ch) {
      case CABLE_CH1:
        GPIO_Init(CH1_LED_PORT, (GPIO_Pin_TypeDef)(CH1_LED), GPIO_MODE_IN_FL_NO_IT);
        break;
#if defined(MULTI_6CH)    
      case CABLE_CH2:
        GPIO_Init(CH2_LED_PORT, (GPIO_Pin_TypeDef)(CH2_LED), GPIO_MODE_IN_FL_NO_IT);
        break;
      case CABLE_CH3:
          GPIO_Init(CH3_LED_PORT, (GPIO_Pin_TypeDef)(CH3_LED), GPIO_MODE_IN_FL_NO_IT);
          break;
      case CABLE_CH4:
          GPIO_Init(CH4_LED_PORT, (GPIO_Pin_TypeDef)(CH4_LED), GPIO_MODE_IN_FL_NO_IT);
          break;
      case CABLE_CH5:
          GPIO_Init(CH5_LED_PORT, (GPIO_Pin_TypeDef)(CH5_LED), GPIO_MODE_IN_FL_NO_IT);
          break;
      case CABLE_CH6:
          GPIO_Init(CH6_LED_PORT, (GPIO_Pin_TypeDef)(CH6_LED), GPIO_MODE_IN_FL_NO_IT);
          break;
#endif
      }
    }
    break;
  case CABLE_START_MODE:
    {
      if(cable_check(ch)) {
         gCLedType[ch] = CABLE_TYPE_R;
         if(gMachineStatus == BATTERY_MODE)
         {
           gCLedType[ch] = CABLE_TYPE_BR;
         }
         DBG_LOG("Port[%d] is CABLE_TYPE_R \r\n",ch);
      }
      else {
         gCLedType[ch] = CABLE_TYPE_BR;
         DBG_LOG("Port[%d] is CABLE_TYPE_BR \r\n",ch);
      }
    }
    break;
  }
  
  gCLedMode[ch] = mode;
  cLed_count[ch] = 0;
  return;
}

void do_run_led()
{
  runLed_count++;
  
  switch(gRunLedMode) {
    case NOARMAL_MODE:
      if(runLed_count == 100)
      {
        runLed_count = 0;
        RUN_LED_ON ;
      }
      break;
    case PAIRING_MODE:
      if(runLed_count == 250)
      {
        runLed_count = 0;
        RUN_LED_TOGGLE;
      }
      break;
    case BATTERY_MODE:
      if(runLed_count == 500)
      {
        runLed_count = 0;
        RUN_LED_TOGGLE;
        //RUN_LED_OFF;
      }
      break;
    case WRITE_EEPROM_MODE:
      if(runLed_count == 100)
      {
        runLed_count = 0;
        RUN_LED_TOGGLE;
      }
      break;
  }
}

void do_c_led()
{
  int i ;

  for(i= CABLE_CH1 ; i <= MAX_CHANNEL_NUMS ; i++) {
    cLed_count[i]++;
    switch(gCLedMode[i]) {
     case CABLE_READY_MODE:
          cLed_count[i] = 0;
        break;
     case CABLE_START_MODE:
        cLed_count[i] = 0;
        if(gCLedType[i] == CABLE_TYPE_R)
        {
          CABLE_R_LED_ON(i);
        }
        else
        {
          CABLE_BR_LED_ON(i);
        }
        break;   
     case CABLE_ALARM_MODE:
        if(cLed_count[i] == 500)
        {
          cLed_count[i] = 0;
          CABLE_LED_TOGGLE(i);
        }
        break;
    }
  }
}

void init_led()
{
  int i;
  GPIO_Init(RUN_LED_PORT, (GPIO_Pin_TypeDef)(RUN_LED ), GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(CH1_LED_PORT, (GPIO_Pin_TypeDef)(CH1_LED), GPIO_MODE_IN_FL_NO_IT);
#if defined(MULTI_6CH)
  GPIO_Init(CH2_LED_PORT, (GPIO_Pin_TypeDef)(CH2_LED), GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(CH3_LED_PORT, (GPIO_Pin_TypeDef)(CH3_LED), GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(CH4_LED_PORT, (GPIO_Pin_TypeDef)(CH4_LED), GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(CH5_LED_PORT, (GPIO_Pin_TypeDef)(CH5_LED), GPIO_MODE_IN_FL_NO_IT);  
  GPIO_Init(CH6_LED_PORT, (GPIO_Pin_TypeDef)(CH6_LED), GPIO_MODE_IN_FL_NO_IT);
  //GPIO_Init(CH2_LED_PORT, (GPIO_Pin_TypeDef)(CH2_LED), GPIO_MODE_OUT_PP_LOW_FAST);
  //GPIO_Init(CH3_LED_PORT, (GPIO_Pin_TypeDef)(CH3_LED), GPIO_MODE_OUT_PP_LOW_FAST);
  //GPIO_Init(CH4_LED_PORT, (GPIO_Pin_TypeDef)(CH4_LED), GPIO_MODE_OUT_PP_LOW_FAST);
  //GPIO_Init(CH5_LED_PORT, (GPIO_Pin_TypeDef)(CH5_LED), GPIO_MODE_OUT_PP_LOW_FAST);  
  //GPIO_Init(CH6_LED_PORT, (GPIO_Pin_TypeDef)(CH6_LED), GPIO_MODE_OUT_PP_LOW_FAST);
#endif
  
  //CBR_CH1_LED_OFF;
#if defined(MULTI_6CH)
  //CBR_CH2_LED_OFF;
  //CBR_CH3_LED_OFF;
  //CBR_CH4_LED_OFF;
  //CBR_CH5_LED_OFF;
  //CBR_CH6_LED_OFF;
#endif
    
  gRunLedMode = NOARMAL_MODE;
  
  for(i= CABLE_CH1; i <= MAX_CHANNEL_NUMS;i++)
    gCLedMode[i] = CABLE_READY_MODE;

}

