
#define SECURITY_GLOBAL

#include "includes.h"
//#include "security.h"
//#include "led.h"

securStatus mSecurStatus[MAX_CHANNEL_NUMS+1];
u8 gMachineStatus = NOARMAL_MODE;
s16 gPumpWait;
s16 qBeepfrq = 0;

typedef void ( *TFunction)(void);
__far TFunction GoAddress;


pairStatus getPairStatus(void)
{
  return mPairStatus;
}

void stopPair(void)
{
  if(mPairStatus == PAIR_START) {
    if(DC_MON_CHK) {
      set_mode_runled(NOARMAL_MODE);
    } else {
      set_mode_runled(BATTERY_MODE);
    }
  }
}

void startPair(char mode)
{
  if(mode == 0) {
    set_mode_runled(PAIRING_MODE);
  }
}

void startSecur(uint8_t ch)
{
  set_mode_cled(CABLE_START_MODE,ch);
}

void stopAlarm(void)
{
  stopBuzzer();
}

void startAlarm(u8 mode,uint8_t ch)
{
  if(mode == 0) {
    startBuzzer(0);
    set_mode_cled(CABLE_ALARM_MODE,ch);
  } else if(mode == 1) {
    // DC Vout is zero
    startBuzzer(1);
  } else if(mode == 2) {
    startBuzzer(2);
  }
}

void reScanSensor(uint8_t ch)
{
  switch(ch) {
    case SENSOR_CH1:
      if(!SENSOR1_CHK) {
        mSecurStatus[SENSOR_CH1] = SECUR_START;
        startSecur(SENSOR_CH1);
      }
      else {
        mSecurStatus[SENSOR_CH1] = SECUR_READY;
      }
      break;
#if defined(MULTI_6CH)    
    case SENSOR_CH2:
      if(!SENSOR2_CHK) {
        mSecurStatus[SENSOR_CH2] = SECUR_START;
        startSecur(SENSOR_CH2);
      }
      else {
        mSecurStatus[SENSOR_CH2] = SECUR_READY;
      }
      break;
    case SENSOR_CH3:
      if(!SENSOR3_CHK) {
        mSecurStatus[SENSOR_CH3] = SECUR_START;
        startSecur(SENSOR_CH3);
      }
      else {
        mSecurStatus[SENSOR_CH3] = SECUR_READY;
      }
      break;
    case SENSOR_CH4:
      if(!SENSOR4_CHK) {
        mSecurStatus[SENSOR_CH4] = SECUR_START;
        startSecur(SENSOR_CH4);
      }
      else {
        mSecurStatus[SENSOR_CH4] = SECUR_READY;
      }
      break;
    case SENSOR_CH5:
      if(!SENSOR5_CHK) {
        mSecurStatus[SENSOR_CH5] = SECUR_START;
        startSecur(SENSOR_CH5);
      }
      else {
        mSecurStatus[SENSOR_CH5] = SECUR_READY;
      }
      break;
    case SENSOR_CH6:
      if(!SENSOR6_CHK) {
        mSecurStatus[SENSOR_CH6] = SECUR_START;
        startSecur(SENSOR_CH6);
      }
      else {
        mSecurStatus[SENSOR_CH6] = SECUR_READY;
      }
      break;
#endif
  }
}

void securityService(void)
{
    int i ;
    char key_value = KEY_NONE;
    char press  = 0, release = 0; 
    //static u16 preVal = 0,Val = 0;
    
    //disableInterrupts();

    if(PullKey !=KEY_NONE) {
        key_value = key_table[PullKey];
        PullKey = KEY_NONE;
        release = 1;
    } else if(PushKey != KEY_NONE) {
        key_value = key_table[PushKey];
        PushKey = KEY_NONE;
        press = 1;
    } else if (RepeatKey != KEY_NONE) {
        key_value = key_table[RepeatKey]+0x10;
        RepeatKey = KEY_NONE;
    } else if(IRKey != KEY_NONE) {
        key_value = IRKey;
        IRKey = KEY_NONE;
    }
    
    if(key_value != KEY_NONE) 
    {
         switch(key_value)		
         {
         //case KEY_PAIRING:break;  
         default:break;
         case KEY_DC_MON:
          {
            if(press) {
              if(gMachineStatus == NOARMAL_MODE)
              {
                IWDG_Config();
              }
            } else {
              if(gMachineStatus == BATTERY_MODE)
              {
                IWDG_Config();
              }
            }
            break;
          }
#if 0
          case KEY_VOUT_MON:
          {
            if(DC_MON_CHK) {
              if(press) {
                mDCShort = TRUE;
                V_EN_OFF;
                mDCShort_Delay = 1000;
                //set_mode_runled(BATTERY_MODE);
              } 
            }
            break;
          }
#endif
          case KEY_BUZZER_SW:
          {
            if(press == 1) {
              if(PAIRING_CHK)
                setBuzzerLv();
              else {
                stopBuzzer();
              }
            }
            break;
          }
          
          case KEY_PAIRING:
          {
             if(press == 1) {
                if(!BUZZER_SW_CHK)
                  stopBuzzer();
             }
             break;
          }
          
          case KEY_SENSOR1:
          case KEY_SENSOR2:
          case KEY_SENSOR3:
          case KEY_SENSOR4:
          case KEY_SENSOR5:
          case KEY_SENSOR6:
          {
              uint8_t  ch_num;
              ch_num = (key_value-KEY_SENSOR1)+1;
              
              switch (mSecurStatus[ch_num]) 
              {
                case SECUR_READY:
                  if(press) {
                    mSecurStatus[ch_num] = SECUR_START;
                    startSecur(ch_num);
                  }
                  break;
                case SECUR_START:
                  if(release) {
                    mSecurStatus[ch_num] = SECUR_ALARM;
                    startAlarm(0,ch_num);
                  }
                  break;
                case SECUR_ALARM:
                  if(release) {
                    mSecurStatus[ch_num] = SECUR_ALARM;
                    startAlarm(0,ch_num);
                  }
                  break;  
              }
              break;
          }

          case KEY_IR_PAIRKEY: 
          {
            if(!isValidIRID(IRID)) 
            {
              if(mPairStatus == PAIR_START)
              {
                  remote_ids[IRIDX] = IRID;
                  set_mode_runled(WRITE_EEPROM_MODE);
                  TIM4_ITConfig(TIM4_IT_UPDATE, DISABLE);
                  IRIDX = ir_update_ids(IRIDX);
                  set_mode_runled(PAIRING_MODE);
                  BuzzerBtn();
                  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
              }
            }
            break;
          }
          case KEY_IR_SIRENKEY:
          {
            if(isValidIRID(IRID))
            {
               stopBuzzer();
            }
            break;
          }
          case KEY_IR_LAMPKEY:
          {
            if(isValidIRID(IRID))
            {
              for(i= SENSOR_CH1; i <= MAX_CHANNEL_NUMS ;i++) 
                if(mSecurStatus[i] == SECUR_ALARM) {
                  if(gCLedType[i] == CABLE_TYPE_R) {
                    CABLE_R_LED_OFF(i);
                  } else if(gCLedType[i] == CABLE_TYPE_BR){
                    CABLE_BR_LED_OFF(i);
                  }
                  
                  gCLedMode[i] = CABLE_READY_MODE;
                  mSecurStatus[i] = SECUR_READY;
                  reScanSensor(i);
                }
            }
            break;
          }
          case KEY_IR_PULG:
          {              
              switch (mIrPlugStatus) 
              {
                case IR_UNPLUG:
                  if(press) {
                    mIrPlugStatus = IR_PLUG;
                  }
                  break;
                case IR_PLUG:
                  if(release) {
                    mIrPlugStatus = IR_UNPLUG;
                    startAlarm(2,0);
                  }
                  break;
              }
              break;
          }
          
          case KEY_PAIRING_LONG:
          {
            if(BUZZER_SW_CHK) {
              BuzzerBtn();
              switch (mPairStatus) 
              {
                case PAIR_READY:
                    startPair(0);
                    mPairStatus = PAIR_START;
                    systemTimer[PAIRING_TIMER] = 4000;
                  break;
                case PAIR_START:
                    stopPair();
                    mPairStatus = PAIR_READY;
                  break;
              }
            } else {
              DBG_LOG("2 Btn Long Press \r\n");
            }
            break;
          }
          case KEY_BUZZER_LONG:
          {
            if(PAIRING_CHK) {
              DBG_LOG("2 Btn Long Press \r\n");
            }
            break;
          }
        }
    }

    if(mPairStatus == PAIR_START)
    {
      if(systemTimer[PAIRING_TIMER] == 100)
      {
        BuzzerBtn();
      }
      
      if(systemTimer[PAIRING_TIMER] == 0)
      {
        BuzzerBtn();
        stopPair();
        mPairStatus = PAIR_READY;
      }
    }
    
    if(gMachineStatus == WDT_ENABLE_MODE)
    {
      if(systemTimer[SYS_TIMER] == 0)
      {
#if 1
        gMachineStatus = BATTERY_MODE;
        stopBuzzer();
        for(i= SENSOR_CH1; i <= MAX_CHANNEL_NUMS ;i++)
        {
            if(gCLedType[i] == CABLE_TYPE_R) {
              CABLE_R_LED_OFF(i);
            } else if(gCLedType[i] == CABLE_TYPE_BR){
              CABLE_BR_LED_OFF(i);
            } 
            gCLedMode[i] = CABLE_READY_MODE;
            mSecurStatus[i] = SECUR_READY;
            reScanSensor(i);
        }
   
        //IWDG_Config();
#endif        
      }
    }
    
    //enableInterrupts();
}

void init_security(void)
{  
  int i;
  // I/O Init
  //GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)( V_EN|TP1_PIN ), GPIO_MODE_OUT_PP_HIGH_SLOW);
  //GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)( V_EN ), GPIO_MODE_OUT_PP_HIGH_SLOW);
  
  for(i= SENSOR_CH1; i <= MAX_CHANNEL_NUMS ;i++)
    mSecurStatus[i] = SECUR_READY;
  mPairStatus = PAIR_READY;
  
  mDCShort_Delay = -1;
  
  //if(!DC_MON_CHK)
  //  mDCShort = TRUE;
  //else {
  //  mDCShort = FALSE;
  //  V_EN_ON;
  //}
#if 0
  if(!VOUT_MON_CHK)
    mDCShort = TRUE;
  else {
    mDCShort = FALSE;
    V_EN_ON;
  }
#endif
  
  if(!SENSOR1_CHK) {
    mSecurStatus[SENSOR_CH1] = SECUR_START;
    startSecur(SENSOR_CH1);
    cSensorEnable = TRUE;
  }
  else {
    mSecurStatus[SENSOR_CH1] = SECUR_READY;
    cSensorEnable = FALSE;
  }
  
#if defined(MULTI_6CH) 
  if(!SENSOR2_CHK) {
    mSecurStatus[SENSOR_CH2] = SECUR_START;
    startSecur(SENSOR_CH2);
    cSensorEnable = TRUE;
  }
  else {
    mSecurStatus[SENSOR_CH2] = SECUR_READY;
    cSensorEnable = FALSE;
  }
  
  if(!SENSOR3_CHK) {
    mSecurStatus[SENSOR_CH3] = SECUR_START;
    startSecur(SENSOR_CH3);
    cSensorEnable = TRUE;
  }
  else{
    mSecurStatus[SENSOR_CH3] = SECUR_READY;
    cSensorEnable = FALSE;
  }

  if(!SENSOR4_CHK) {
    mSecurStatus[SENSOR_CH4] = SECUR_START;
    startSecur(SENSOR_CH4);
    cSensorEnable = TRUE;
  }
  else{
    mSecurStatus[SENSOR_CH4] = SECUR_READY;
    cSensorEnable = FALSE;
  }
  
  if(!SENSOR5_CHK) {
    mSecurStatus[SENSOR_CH5] = SECUR_START;
    startSecur(SENSOR_CH5);
    cSensorEnable = TRUE;
  }
  else{
    mSecurStatus[SENSOR_CH5] = SECUR_READY;
    cSensorEnable = FALSE;
  }
  
  if(!SENSOR6_CHK) {
    mSecurStatus[SENSOR_CH6] = SECUR_START;
    startSecur(SENSOR_CH6);
    cSensorEnable = TRUE;
  }
  else{
    mSecurStatus[SENSOR_CH6] = SECUR_READY;
    cSensorEnable = FALSE;
  }
#endif
  
  if(DC_MON_CHK) {
    gMachineStatus = NOARMAL_MODE;    
    pan_enable(TRUE);
    //set_mode_runled(NOARMAL_MODE);
  } else {
    gMachineStatus = BATTERY_MODE;
    pan_enable(FALSE);
    //set_mode_runled(BATTERY_MODE);
  }
  
  set_mode_runled(gMachineStatus);

  return;
}
