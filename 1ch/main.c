/*********************************************************************************
	Project:		Relay Slave
	Company:
	MCU:	        STM8S003F3 16MHz/8 = 2MHz (Flash: 8KB, RAM: 1KB)
	Compiler:		IAR STM8 1.41.1
	Programmer:		SY Jang
	Begin:		2014-11-14

	System Resources:
		UART1:		RS485
*********************************************************************************/

#include <stm8s.h>
#include "includes.h"
#include "board.h"

unsigned char deviceID = 0;
u16 systemTimer[2] = {0,0};

//----------------------
//	prototypes
//----------------------
void init_port(void);
void init_mcu(void);

void init_SystemTimer(void)
{
  /* TIM1 Peripheral Configuration for WiegandOut*/ 
   //1. Deinitializes the TIM1 peripheral registers to their default reset values.
  //TIM1_DeInit();
  //2. Timer1 Base setting. (Frequency, CounterMode, Counter, Repetition).
  /* Time Base configuration 16MHZ/2(FHSI)/8(PRESCALER) = 1MHz*/
  TIM1_TimeBaseInit(7, TIM1_COUNTERMODE_UP, 99, 0); 
  
  TIM1_Cmd(ENABLE);
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
  
  //TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_8, 99); // 10khz (100usec)	
  TIM4_Cmd(ENABLE);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  return;
}

/**
  * @brief  Configures the IWDG to generate a Reset if it is not refreshed at the
  *         correct time. 
  * @param  None
  * @retval None
  */
void IWDG_Config(void)
{
  /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG counter clock: LSI/128 */
  IWDG_SetPrescaler(IWDG_Prescaler_128);
  
  /* Set counter reload value to obtain 250ms IWDG TimeOut.
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LSI/128)
                         = 0.25s / (LsiFreq/128)
                         = LsiFreq/(128 * 4)
                         = LsiFreq/512
   */
  IWDG_SetReload((uint8_t)(512));
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
  
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
}


void main(void)         
{
  u8 loop = 0;
  init_mcu();
  init_port();
  init_led();
  
  init_SystemTimer();
  init_Key();
  InitBuzzer();
  Init_IR();
  init_security();
  Debug_init();
  
  RUN_LED_ON;
#if 1
  for(loop = 0; loop < 18;loop++)
  {
    Delay(1000);
  }
#endif
  RUN_LED_OFF;
  
  enableInterrupts();				// Enable all interrupts
  #ifdef USE_POWERSAVE
  wfi();					// wait interrupt, CPU off
  #endif

  while(1)
  {
      securityService();
      
      #ifdef USE_POWERSAVE
      if(is_powersave_req())
      {
        wfi();				        // wait interrupt, CPU off
      }
      #endif	
      //IWDG_ReloadCounter();  
  }
}

void init_port(void)
{
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
}

void init_mcu(void)
{
    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV2); // 16MHz / 2 = 8MHz
    while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) != SET);
}

void Delay(unsigned long nCount)
{
  // master clk is 8MMz
  //
  while(nCount != 0)
  {
          nCount--;
  }
}

void uDelay(unsigned long nCount)
{
  u8 loop ; // master clk is 8MMz
  while(nCount != 0)
  {
    loop = 8;
    while(loop-- != 0);
    nCount--;
  }
}

void msDelay(unsigned long nCount)
{
  // master clk is 8MMz
  //
  while(nCount != 0)
  {
     uDelay(1000);
     nCount--;
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(u8* file, u32 line)
{
  while (1)
  {
  }
}
#endif

