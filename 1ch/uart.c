
#include <stm8s.h>
#include "includes.h"
#include "main.h"
#include "uart.h"
#define DEFAULT_BRATE   9600

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
unsigned char			cmd;
unsigned char                   cmd_data;
static unsigned char		rxBuf[SUB_PROTOCOL_LEN];
unsigned char		        txBuf[SUB_PROTOCOL_LEN];
#endif

void Debug_init(void)
{
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
    //UART2_DeInit();
    UART2_Init((uint32_t)DEFAULT_BRATE, UART2_WORDLENGTH_8D, UART2_STOPBITS_1,UART2_PARITY_NO,
                            UART2_SYNCMODE_CLOCK_DISABLE, /*UART2_MODE_TX_ENABLE*/ UART2_MODE_TXRX_ENABLE);
    //UART2_SetAddress(0x02);
    //UART2_WakeUpConfig(UART1_WAKEUP_ADDRESSMARK);
    UART2_ITConfig(UART2_IT_RXNE, ENABLE);
#else
    //UART1_Init((uint32_t)DEFAULT_BRATE, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,UART1_PARITY_NO,
    //                    UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_RX_ENABLE);
    //UART1_ITConfig(UART1_IT_RXNE, ENABLE);
    UART1->CR1 = 0x00;  
    UART1->CR3 = 0x00;  
    UART1->BRR1 = 0x34;  
    UART1->BRR2 = 0x01;
    UART1->CR2 = 0x24;
#endif
}


void xputc(char data)
{    
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
      while((UART2->SR & (uint8_t)UART2_FLAG_TXE) == 0);
      UART2->DR = data;
#endif
}

void xputs(char *str)
{    
  int i;
  if(str == 0)
    return;
  
  for (i = 0; i < strlen(str); i++)
   xputc(str[i]);
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
//========================================
// STM8S003F UART1 RX Interrupt
//========================================
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
	unsigned char	data;
        
        //UART1_ClearITPendingBit(UART1_IT_RXNE);

	data = ((uint8_t)UART2->DR);
        
        if(data == 0x7F)
        {
          data = 0x79;
          xputc(data);
          
          //TIM3->EGR = 01;
          //data = TIM3->SR1 ;
          //TIM3->SR1 = 00;
          //asm("JP $8000");
        } else if(data == 0x00) {
          
          data = 0x79;
          xputc(data);
          
          data = 0x05; //the number
          xputc(data);
          data = 0x10; // version
          xputc(data);
          data = 0x00;
          xputc(data);
          data = 0x11;
          xputc(data);
          data = 0x21;
          xputc(data);
          data = 0x31;
          xputc(data);
          data = 0x43;
          xputc(data);
          data = 0x79;
          xputc(data);
          
          IWDG_Config();
        }
          
          //asm("LDW X,  SP ");
          //asm("LD  A,  $FF");
          //asm("LD  XL, A  ");
          //asm("LDW SP, X  ");
          //asm("JPF $6000");
 
          //IWDG_Config();
        

}
#endif
