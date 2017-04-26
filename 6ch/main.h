#ifndef ___MAIN_H___
#define ___MAIN_H___


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


// PORTD
#define PAIRING_TIMER   0
#define SYS_TIMER       1
#define MODE_EXC_TIMER  2

#if defined(MULTI_6CH)
//#define my_printf(...) printf(__VA_ARGS__)
#define DBG_LOG(...)  printf(__VA_ARGS__)
#else
#define DBG_LOG(...)
#endif
extern unsigned char		deviceID;
extern unsigned char		cmd;
extern unsigned char		cmd_data;
extern unsigned char		txBuf[SUB_PROTOCOL_LEN];
extern volatile unsigned char	busy_flag;
extern unsigned char            gRunLedMode;
extern u32 systemTimer[2];

extern void IWDG_Config(void);
extern void uDelay(unsigned long nCount);
extern void msDelay(unsigned long nCount);
extern void Delay(unsigned long nCount);
extern void pan_enable(u8 onOff);

#endif	//___MAIN_H___

