#ifndef ___BSP_H___
#define ___BSP_H___

/*********************************************************************************
	STM8S003F3 16MHz TSSOP20 Pin assign map
        orig
----------------------------------------------------------------------------------
	PIN No.	PORT/Default(Remap)			FUNCTION
----------------------------------------------------------------------------------
	 1	UART1_CK/TIM2_CH1/BEEP/PD4		(OUT) CON_485
	 2	UART1_TX/AIN5/PD5			(OUT) RS485_TX
	 3	UART1_RX/AIN6/PD6			(IN)  RS485_RX
	 4	NRST					____Pullup_VCC (RESET, 10K + VCC)
	 5	OSCIN/PA1				____16MHz X-TAL
	 6	OSCOUT/PA2				____16MHz X-TAL
	 7	VSS					____GND
	 8	VCAP					____VCAP (1uF to GND)
	 9	VDD					____VDD
	10	TIM2_CH3/PA3/[SPI_NSS]			(IN)  ID_BIT0
--------------------------------------------------------------------
	11	PB5/I2C_SDA/[TIM1_BKIN]			(IN)  ID_BIT7
	12	PB4/I2C_SCL/[ADC_ETR]			(IN)  ID_BIT6
	13	PC3/TIM1_CH3/[TLI][TIM1_CH1]		(IN)  ID_BIT5
	14	PC4/TIM1_CH4/CLK_CCO/AIN2/[TIM1_CH2N]	(IN)  ID_BIT4
	15	PC5/SPI_SCK/[TIM2_CH1]			(IN)  ID_BIT3
	16	PC6/SPI_MOSI/[TIM1_CH1]			(IN)  ID_BIT2
	17	PC7/SPI_MISO/[TIM1_CH2]			(IN)  ID_BIT1
	18	PD1/SWIM				____SWIM
	19	PD2/AIN3/[TIM2_CH3]			(OUT) COIL_C
	20	PD3/AIN4/TIM2_CH2/ADC_ETR		(OUT) COIL_O


        kumyhoo water pump
----------------------------------------------------------------------------------
	PIN No.	PORT/Default(Remap)			FUNCTION
----------------------------------------------------------------------------------
	 1	UART1_CK/TIM2_CH1/BEEP/PD4		(IN) -
	 2	UART1_TX/AIN5/PD5			(OUT) RS485_TX
	 3	UART1_RX/AIN6/PD6			(IN)  RS485_RX
	 4	NRST					____Pullup_VCC (RESET, 10K + VCC)
	 5	OSCIN/PA1				____16MHz X-TAL
	 6	OSCOUT/PA2				____16MHz X-TAL
	 7	VSS					____GND
	 8	VCAP					____VCAP (1uF to GND)
	 9	VDD					____VDD
	10	TIM2_CH3/PA3/[SPI_NSS]			(IN)  -
--------------------------------------------------------------------
	11	PB5/I2C_SDA/[TIM1_BKIN]			(IN)  -
	12	PB4/I2C_SCL/[ADC_ETR]			(IN)  -
	13	PC3/TIM1_CH3/[TLI][TIM1_CH1]		(OUT)  EN_12V
	14	PC4/TIM1_CH4/CLK_CCO/AIN2/[TIM1_CH2N]	(OUT)  LED1
	15	PC5/SPI_SCK/[TIM2_CH1]			(OUT)  EN_BLUE_LED
	16	PC6/SPI_MOSI/[TIM1_CH1]			(IN)  TOUCH_IN
	17	PC7/SPI_MISO/[TIM1_CH2]			(OUT)  EN_RED_LED
	18	PD1/SWIM				____SWIM
	19	PD2/AIN3/[TIM2_CH3]			(IN) -
	20	PD3/AIN4/TIM2_CH2/ADC_ETR		(IN) -
**********************************************************************************/

#include "protocol.h"


//-------------------------------------
//	CONFIGURATION
//-------------------------------------
//#define USE_POWERSAVE			// 통신문제 있음. Debugger not working??


//-------------------------------------
//	Port Pin definition
//-------------------------------------

// PORTA
#define ID_BIT0		        GPIO_PIN_3

// PORTC
#define RED_LED                 GPIO_PIN_7      // PC7
#define ONWORK_LED              RED_LED
#define PORT1_LED	        ONWORK_LED 
#define PORT1_LED_ON	        GPIOC->ODR |=PORT1_LED
#define PORT1_LED_OFF	        GPIOC->ODR &=~(PORT1_LED)
#define PORT1_LED_TOGGLE        GPIOC->ODR^=PORT1_LED
#define PORT1_LED_CHK           (GPIOC->IDR & PORT1_LED)

#define ONWORK_LED_ON	        GPIOC->ODR |=ONWORK_LED
#define ONWORK_LED_OFF	        GPIOC->ODR &=~(ONWORK_LED)
#define ONWORK_LED_TOGGLE       GPIOC->ODR^=ONWORK_LED
#define ONWORK_LED_CHK          (GPIOC->IDR & ONWORK_LED)


#define BLUE_LED                GPIO_PIN_5      //PC5
#define READYWORK_LED           BLUE_LED
#define PORT2_LED	        READYWORK_LED 
#define PORT2_LED_ON	        GPIOC->ODR |= PORT2_LED
#define PORT2_LED_OFF	        GPIOC->ODR &=~(PORT2_LED)
#define PORT2_LED_TOGGLE        GPIOC->ODR^=PORT2_LED
#define PORT2_LED_CHK           (GPIOC->IDR & PORT2_LED)

#define READYWORK_LED_ON	GPIOC->ODR |=READYWORK_LED
#define READYWORK_LED_OFF	GPIOC->ODR &=~(READYWORK_LED)
#define READYWORK_LED_TOGGLE    GPIOC->ODR^=READYWORK_LED
#define OREADYWORK_LED_CHK      (GPIOC->IDR & READYWORK_LED)

#define PORT1_SEN	        GPIO_PIN_4 // PB4
#define PORT1_SEN_CHK           (GPIOB->IDR & PORT1_SEN)

#define TOUCH_IN                GPIO_PIN_6 //PC6
#define PORT2_SEN	        TOUCH_IN 
#define PORT2_SEN_CHK           (GPIOC->IDR & PORT2_SEN)
#define TOUCH_IN_CHK            PORT2_SEN_CHK

#if 0
#define PUMP_CTRL               GPIO_PIN_3
#define PUMP_CTRL_ON	        GPIOC->ODR |= PUMP_CTRL
#define PUMP_CTRL_OFF	        GPIOC->ODR &=~(PUMP_CTRL)
#define PUMP_CTRL_CHK           (GPIOC->IDR & PUMP_CTRL)
#else
  /* Enables TIM1 peripheral Preload register on ARR */  
#define PUMP_CTRL               GPIO_PIN_3
#define PUMP_CTRL_ON	        do{TIM1_ARRPreloadConfig(ENABLE);TIM1_CtrlPWMOutputs(ENABLE);}while(0);
#define PUMP_CTRL_OFF	        do{TIM1_ARRPreloadConfig(DISABLE);TIM1_CtrlPWMOutputs(DISABLE);}while(0);
#define PUMP_CTRL_CHK           
#endif

// LED
#define LED1		        GPIO_PIN_4
#define LED1_OFF	        GPIOC->ODR |=LED1
#define LED1_ON	                GPIOC->ODR &=~(LED1)
#define LED1_TOGGLE             GPIOC->ODR^=LED1
#define LED1_CHK                (GPIOC->IDR & LED1)

#define LED2		        GPIO_PIN_4  //GPIO_PIN_3
#define LED2_OFF	        GPIOC->ODR |=LED2
#define LED2_ON	                GPIOC->ODR &=~(LED2)
#define LED2_TOGGLE             GPIOC->ODR^=LED2
#define LED2_CHK                (GPIOC->IDR & LED2)

#endif //___BSP_H___