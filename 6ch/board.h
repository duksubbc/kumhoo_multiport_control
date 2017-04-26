#ifndef ___BOARD_H___
#define ___BOARD_H___

/*********************************************************************************
	STM8S003F3 16MHz TSSOP20 Pin assign map
        kumyhoo standalong 
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

//-------------------------------------
//	Port Pin definition
//-------------------------------------

// LED

#define CABLE_CH0                0
#define CABLE_CH1                1
#define CABLE_CH2                2
#define CABLE_CH3                3
#define CABLE_CH4                4
#define CABLE_CH5                5
#define CABLE_CH6                6

#define SENSOR_CH0                0
#define SENSOR_CH1                1
#define SENSOR_CH2                2
#define SENSOR_CH3                3
#define SENSOR_CH4                4
#define SENSOR_CH5                5
#define SENSOR_CH6                6


#if defined(FEATURE_STD_ALONE)

#define RUN_LED                 GPIO_PIN_5      // PD5
#define C_LED                   GPIO_PIN_4      // PD4
#define V_EN                    GPIO_PIN_6      // PD6

#define RUN_LED_ON	        GPIOD->ODR |=RUN_LED
#define RUN_LED_OFF	        GPIOD->ODR &=~(RUN_LED)
#define RUN_LED_TOGGLE          GPIOD->ODR^=RUN_LED

#define C_LED_ON	        GPIOD->ODR |=C_LED
#define C_LED_OFF	        GPIOD->ODR &=~(C_LED)
#define C_LED_TOGGLE            GPIOD->ODR^=C_LED

#define V_EN_ON	                GPIOD->ODR |=V_EN
#define V_EN_OFF	        GPIOD->ODR &=~(V_EN)

// Sensor level check
#define SENSOR                  GPIO_PIN_1      // PA1
#define VOUT_MON                GPIO_PIN_2      // PA2
#define DC_MON                  GPIO_PIN_3      // PA3
#define PRT_SW                  GPIO_PIN_5      // PC5

#define SENSOR_CHK              (GPIOA->IDR & SENSOR)
#define VOUT_MON_CHK            (GPIOA->IDR & VOUT_MON)  //short
#define DC_MON_CHK              (GPIOA->IDR & DC_MON)    // 정전
#define PRT_SW_CHK              (GPIOC->IDR & PRT_SW)

// Switch
#define PAIRING                 GPIO_PIN_4      // PC4
#define PAIRING_CHK            (GPIOC->IDR & PAIRING)

// Buzzer
#define BUZZER                  GPIO_PIN_3      // PD3
#if defined(BUZZER_OP_LEVEL)
#define BUZZER_ON	        GPIOD->ODR |=BUZZER
#define BUZZER_OFF              GPIOD->ODR &=~(BUZZER)
#else
#define BUZZER_ON	        do{TIM2_ARRPreloadConfig(ENABLE);TIM2_CCxCmd(TIM2_CHANNEL_2,ENABLE);}while(0);
#define BUZZER_OFF	        do{TIM2_ARRPreloadConfig(DISABLE);TIM2_CCxCmd(TIM2_CHANNEL_2,DISABLE);}while(0);          
#endif

#define IR_Rx                   GPIO_PIN_3      // PC3
#define CURRENT                 GPIO_PIN_7      // PC7
#define TP1_PIN                 GPIO_PIN_2      // PD7

#else

#if defined(KUMHOO_PD)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
////    금오전자 Power Delivery 1-Port
////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define MAX_CHANNEL_NUMS                 1

#define VOUT_PORT              GPIOA
#define VOUT_MON               GPIO_PIN_2      // PA2
#define VOUT_MON_CHK           (VOUT_PORT->IDR & VOUT_MON)  //short

#define DC_PORT                GPIOA
#define DC_MON                 GPIO_PIN_2      // PA3
#define DC_MON_CHK             (DC_PORT->IDR & DC_MON)    // 정전

// Switch
#define PAIRING_SW_PORT        GPIOC
#define PAIRING_SW             GPIO_PIN_4      // PC4
#define PAIRING_CHK            (PAIRING_SW_PORT->IDR & PAIRING_SW)

#define BUZZER_SW_PORT         GPIOC
#define BUZZER_SW              GPIO_PIN_5      // PC5
#define BUZZER_SW_CHK          (BUZZER_SW_PORT->IDR & BUZZER_SW)

#define IR_PORT                GPIOC      // PC3
#define IR_Rx                  GPIO_PIN_3      // PC3
#define IR_Rx_CHK          (IR_PORT->IDR & IR_Rx)
#define IR_EXTI_PORT         EXTI_PORT_GPIOC


#define V_EN                   GPIO_PIN_6      // PD6
#define V_EN_ON	                
#define V_EN_OFF

#define RUN_LED_PORT            GPIOC
#define RUN_LED                 GPIO_PIN_7      // PA3
#define RUN_LED_OFF	        RUN_LED_PORT->ODR |=RUN_LED
#define RUN_LED_ON	        RUN_LED_PORT->ODR &=~(RUN_LED)
#define RUN_LED_TOGGLE          RUN_LED_PORT->ODR^=RUN_LED

#define CH1_LED_PORT            GPIOD
#define CH1_LED                   GPIO_PIN_4      // PD4

#define CR_CH1_LED_OFF	        CH1_LED_PORT->ODR |=CH1_LED
#define CR_CH1_LED_ON	        CH1_LED_PORT->ODR &=~(CH1_LED)
#define CR_CH1_LED_TOGGLE       CH1_LED_PORT->ODR^=CH1_LED

#define CBR_CH1_LED_ON	        CH1_LED_PORT->ODR |=CH1_LED
#define CBR_CH1_LED_OFF	        CH1_LED_PORT->ODR &=~(CH1_LED)
#define CBR_CH1_LED_TOGGLE      CH1_LED_PORT->ODR^=CH1_LED

#define CH1_LED_SW_CHK          (CH1_LED_PORT->IDR & CH1_LED)

#define CABLE_R_LED_ON(n)        { switch(n) {   \
                                  case CABLE_CH1:CR_CH1_LED_ON;break;\
                                  }\
                                 }

#define CABLE_R_LED_OFF(n)       { switch(n) {   \
                                  case CABLE_CH1:CH1_LED_OFF;break;\
                                  }\
                                 }




#define CABLE_BR_LED_ON(n)       { switch(n) {   \
                                  case CABLE_CH1:CBR_CH1_LED_ON;break;\
                                  }\
                                 }

#define CABLE_BR_LED_OFF(n)      { switch(n) {   \
                                  case CABLE_CH1:CBR_CH1_LED_OFF;break;\
                                  }\
                                 }

#define CABLE_LED_TOGGLE(n)    { switch(n) {   \
                                  case CABLE_CH1:CR_CH1_LED_TOGGLE;break;\
                                  }\
                                 }


// Sensor level check
#define SENSOR1_PORT           GPIOA
#define SENSOR1                GPIO_PIN_1      // PE1
#define SENSOR1_CHK            (SENSOR1_PORT->IDR & SENSOR1)

// Buzzer Power down for power save
#define SLB_PORT               GPIOC
#define SLB_PIN                GPIO_PIN_6    
#define SLB_OFF	               SLB_PORT->ODR |=SLB_PIN
#define SLB_ON	               SLB_PORT->ODR &=~(SLB_PIN)
#define SLB_TOGGLE             SLB_PORT->ODR^=SLB_PIN

#elif defined(MULTI_1CH)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
////    금오전자 Multi-controler 1-Port
////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define PAN_ENABLE         FALSE
#define BOARD_VER_170404_1

#define MAX_CHANNEL_NUMS                 1

#define VOUT_PORT              GPIOA
#define VOUT_MON               GPIO_PIN_2      // PA2
#define VOUT_MON_CHK           (VOUT_PORT->IDR & VOUT_MON)  //short

#define DC_PORT                GPIOC
#define DC_MON                 GPIO_PIN_7      // PC7
#define DC_MON_CHK             (DC_PORT->IDR & DC_MON)    // 정전

// Switch
#define PAIRING_SW_PORT        GPIOC
#define PAIRING_SW             GPIO_PIN_4      // PC4
#define PAIRING_CHK            (PAIRING_SW_PORT->IDR & PAIRING_SW)

#define BUZZER_SW_PORT         GPIOC
#define BUZZER_SW              GPIO_PIN_5      // PC5
#define BUZZER_SW_CHK          (BUZZER_SW_PORT->IDR & BUZZER_SW)

#define IR_PORT                GPIOC                       // PC3
#define IR_Rx                    GPIO_PIN_3                
#define IR_Rx_CHK          (IR_PORT->IDR & IR_Rx)
#define IR_EXTI_PORT         EXTI_PORT_GPIOC

#define IR_SENSOR_PORT          GPIOA                       // PD3
#define IR_SENSOR                    GPIO_PIN_2
#define IR_SENSOR_CHK             (IR_SENSOR_PORT->IDR & IR_SENSOR)

#define V_EN                   GPIO_PIN_6      // PD6
#define V_EN_ON	                
#define V_EN_OFF

#define PAN_PORT           0  
#define PAN_PIN               1   
#define PAN_ON	             1    
#define PAN_OFF              0   
#define PAN_TOGGLE        1

#define RUN_LED_PORT            GPIOA
#define RUN_LED                 GPIO_PIN_3      // PA3
#define RUN_LED_OFF	        RUN_LED_PORT->ODR |=RUN_LED
#define RUN_LED_ON	        RUN_LED_PORT->ODR &=~(RUN_LED)
#define RUN_LED_TOGGLE          RUN_LED_PORT->ODR^=RUN_LED

#define CH1_LED_PORT            GPIOD
#define CH1_LED                     GPIO_PIN_4      // PD4

#define CR_CH1_LED_OFF	        CH1_LED_PORT->ODR |=CH1_LED
#define CR_CH1_LED_ON	        CH1_LED_PORT->ODR &=~(CH1_LED)
#define CR_CH1_LED_TOGGLE       CH1_LED_PORT->ODR^=CH1_LED

#define CBR_CH1_LED_ON	        CH1_LED_PORT->ODR |=CH1_LED
#define CBR_CH1_LED_OFF	        CH1_LED_PORT->ODR &=~(CH1_LED)
#define CBR_CH1_LED_TOGGLE      CH1_LED_PORT->ODR^=CH1_LED

#define CH1_LED_SW_CHK          (CH1_LED_PORT->IDR & CH1_LED)

#define CABLE_R_LED_ON(n)        { switch(n) {   \
                                  case CABLE_CH1:CR_CH1_LED_ON;break;\
                                  }\
                                 }

#define CABLE_R_LED_OFF(n)       { switch(n) {   \
                                  case CABLE_CH1:CR_CH1_LED_OFF;break;\
                                  }\
                                 }




#define CABLE_BR_LED_ON(n)       { switch(n) {   \
                                  case CABLE_CH1:CBR_CH1_LED_ON;break;\
                                  }\
                                 }

#define CABLE_BR_LED_OFF(n)      { switch(n) {   \
                                  case CABLE_CH1:CBR_CH1_LED_OFF;break;\
                                  }\
                                 }

#define CABLE_LED_TOGGLE(n)    { switch(n) {   \
                                  case CABLE_CH1:CR_CH1_LED_TOGGLE;break;\
                                  }\
                                 }

// Sensor level check
#define SENSOR1_PORT           GPIOA
#define SENSOR1                GPIO_PIN_1      // PE1
#define SENSOR1_CHK            (SENSOR1_PORT->IDR & SENSOR1)


#elif defined(MULTI_6CH)

//#define BOARD_VER_161024
//#define BOARD_VER_170217
#define BOARD_VER_170404_6
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
////    금오전자 Multi-controler 6-Port
////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#define V_EN_ON	        GPIOD->ODR |=V_EN
//#define V_EN_OFF	        GPIOD->ODR &=~(V_EN)

#define PAN_ENABLE         TRUE
#define V_EN_ON	                
#define V_EN_OFF

#define MAX_CHANNEL_NUMS                 6  

#define V_EN                    GPIO_PIN_6      // PD6

#define VOUT_PORT              GPIOA
#define VOUT_MON               GPIO_PIN_2      // PA2
#define VOUT_MON_CHK           (VOUT_PORT->IDR & VOUT_MON)  //short


#if defined(BOARD_VER_161024)
#define DC_PORT                GPIOD
#define DC_MON                 GPIO_PIN_7      // PB7
#define DC_MON_CHK             (DC_PORT->IDR & DC_MON)    // 정전

#elif defined(BOARD_VER_170217) || defined(BOARD_VER_170404_6)
#define DC_PORT                GPIOB
#define DC_MON                 GPIO_PIN_7      // PB7
#define DC_MON_CHK             (DC_PORT->IDR & DC_MON)    // 정전
#endif

// Switch
#define PAIRING_SW_PORT        GPIOC
#define PAIRING_SW             GPIO_PIN_7
#define PAIRING_CHK            (PAIRING_SW_PORT->IDR & PAIRING_SW)

#define BUZZER_SW_PORT         GPIOC
#define BUZZER_SW              GPIO_PIN_6
#define BUZZER_SW_CHK          (BUZZER_SW_PORT->IDR & BUZZER_SW)

#define IR_PORT                GPIOD                       // PD2
#define IR_Rx                    GPIO_PIN_2
#define IR_Rx_CHK             (IR_PORT->IDR & IR_Rx)
#define IR_EXTI_PORT         EXTI_PORT_GPIOD


#define IR_SENSOR_PORT          GPIOD                       // PD3
#define IR_SENSOR                    GPIO_PIN_3
#define IR_SENSOR_CHK             (IR_SENSOR_PORT->IDR & IR_SENSOR)


#define TP1_PIN                GPIO_PIN_2      // PD7

#define PAN_PORT              GPIOG
#define PAN_PIN                  GPIO_PIN_0
#define PAN_ON	                PAN_PORT->ODR |=PAN_PIN
#define PAN_OFF                 PAN_PORT->ODR &=~(PAN_PIN)
#define PAN_TOGGLE           PAN_PORT->ODR^=PAN_PIN

#define RUN_LED_PORT            GPIOG
#define RUN_LED                 GPIO_PIN_1      // PA3
#define RUN_LED_OFF	        RUN_LED_PORT->ODR |=RUN_LED
#define RUN_LED_ON	        RUN_LED_PORT->ODR &=~(RUN_LED)
#define RUN_LED_TOGGLE          RUN_LED_PORT->ODR^=RUN_LED

#define CH1_LED_PORT            GPIOB
#define CH1_LED                 GPIO_PIN_5
#define CR_CH1_LED_ON	        CH1_LED_PORT->ODR &=~(CH1_LED)
#define CR_CH1_LED_OFF	        CH1_LED_PORT->ODR |=CH1_LED
#define CR_CH1_LED_TOGGLE       CH1_LED_PORT->ODR^=CH1_LED
#define CBR_CH1_LED_ON	        CH1_LED_PORT->ODR |=CH1_LED
#define CBR_CH1_LED_OFF	        CH1_LED_PORT->ODR &=~(CH1_LED)
#define CBR_CH1_LED_TOGGLE      CH1_LED_PORT->ODR^=CH1_LED
#define CH1_LED_SW_CHK          (CH1_LED_PORT->IDR & CH1_LED)

#define CH2_LED_PORT            GPIOB
#define CH2_LED                 GPIO_PIN_4
#define CR_CH2_LED_ON	        CH2_LED_PORT->ODR &=~(CH2_LED)
#define CR_CH2_LED_OFF	        CH2_LED_PORT->ODR |=CH2_LED
#define CR_CH2_LED_TOGGLE       CH2_LED_PORT->ODR^=CH2_LED
#define CBR_CH2_LED_ON	        CH2_LED_PORT->ODR |=CH2_LED
#define CBR_CH2_LED_OFF	        CH2_LED_PORT->ODR &=~(CH2_LED)
#define CBR_CH2_LED_TOGGLE      CH2_LED_PORT->ODR^=CH2_LED
#define CH2_LED_SW_CHK          (CH2_LED_PORT->IDR & CH2_LED)


#define CH3_LED_PORT            GPIOB
#define CH3_LED                 GPIO_PIN_3
#define CR_CH3_LED_ON	        CH3_LED_PORT->ODR &=~(CH3_LED)
#define CR_CH3_LED_OFF	        CH3_LED_PORT->ODR |=CH3_LED
#define CR_CH3_LED_TOGGLE       CH3_LED_PORT->ODR^=CH3_LED
#define CBR_CH3_LED_ON	        CH3_LED_PORT->ODR |=CH3_LED
#define CBR_CH3_LED_OFF	        CH3_LED_PORT->ODR &=~(CH3_LED)
#define CBR_CH3_LED_TOGGLE      CH3_LED_PORT->ODR^=CH3_LED
#define CH3_LED_SW_CHK          (CH3_LED_PORT->IDR & CH3_LED)

#define CH4_LED_PORT            GPIOB
#define CH4_LED                 GPIO_PIN_2
#define CR_CH4_LED_ON	        CH4_LED_PORT->ODR &=~(CH4_LED)
#define CR_CH4_LED_OFF	        CH4_LED_PORT->ODR |=CH4_LED
#define CR_CH4_LED_TOGGLE       CH4_LED_PORT->ODR^=CH4_LED
#define CBR_CH4_LED_ON	        CH4_LED_PORT->ODR |=CH4_LED
#define CBR_CH4_LED_OFF	        CH4_LED_PORT->ODR &=~(CH4_LED)
#define CBR_CH4_LED_TOGGLE      CH4_LED_PORT->ODR^=CH4_LED
#define CH4_LED_SW_CHK          (CH4_LED_PORT->IDR & CH4_LED)

#define CH5_LED_PORT            GPIOB
#define CH5_LED                 GPIO_PIN_1
#define CR_CH5_LED_ON	        CH5_LED_PORT->ODR &=~(CH5_LED)
#define CR_CH5_LED_OFF	        CH5_LED_PORT->ODR |=CH5_LED
#define CR_CH5_LED_TOGGLE       CH5_LED_PORT->ODR^=CH5_LED
#define CBR_CH5_LED_ON	        CH5_LED_PORT->ODR |=CH5_LED
#define CBR_CH5_LED_OFF	        CH5_LED_PORT->ODR &=~(CH5_LED)
#define CBR_CH5_LED_TOGGLE      CH5_LED_PORT->ODR^=CH5_LED
#define CH5_LED_SW_CHK          (CH5_LED_PORT->IDR & CH5_LED)

#define CH6_LED_PORT            GPIOB
#define CH6_LED                 GPIO_PIN_0
#define CR_CH6_LED_ON	        CH6_LED_PORT->ODR &=~(CH6_LED)
#define CR_CH6_LED_OFF	        CH6_LED_PORT->ODR |=CH6_LED
#define CR_CH6_LED_TOGGLE       CH6_LED_PORT->ODR^=CH6_LED
#define CBR_CH6_LED_ON	        CH6_LED_PORT->ODR |=CH6_LED
#define CBR_CH6_LED_OFF	        CH6_LED_PORT->ODR &=~(CH6_LED)
#define CBR_CH6_LED_TOGGLE      CH6_LED_PORT->ODR^=CH6_LED
#define CH6_LED_SW_CHK          (CH6_LED_PORT->IDR & CH6_LED)

      
#define CABLE_R_LED_ON(n)         { switch(n) {   \
                                  case CABLE_CH1:CR_CH1_LED_ON;break;\
                                  case CABLE_CH2:CR_CH2_LED_ON;break;\
                                  case CABLE_CH3:CR_CH3_LED_ON;break;\
                                  case CABLE_CH4:CR_CH4_LED_ON;break;\
                                  case CABLE_CH5:CR_CH5_LED_ON;break;\
                                  case CABLE_CH6:CR_CH6_LED_ON;break;\
                                  }\
                                 }

#define CABLE_R_LED_OFF(n)         { switch(n) {   \
                                  case CABLE_CH1:CR_CH1_LED_OFF;break;\
                                  case CABLE_CH2:CR_CH2_LED_OFF;break;\
                                  case CABLE_CH3:CR_CH3_LED_OFF;break;\
                                  case CABLE_CH4:CR_CH4_LED_OFF;break;\
                                  case CABLE_CH5:CR_CH5_LED_OFF;break;\
                                  case CABLE_CH6:CR_CH6_LED_OFF;break;\
                                  }\
                                 }


#define CABLE_BR_LED_ON(n)         { switch(n) {   \
                                  case CABLE_CH1:CBR_CH1_LED_ON;break;\
                                  case CABLE_CH2:CBR_CH2_LED_ON;break;\
                                  case CABLE_CH3:CBR_CH3_LED_ON;break;\
                                  case CABLE_CH4:CBR_CH4_LED_ON;break;\
                                  case CABLE_CH5:CBR_CH5_LED_ON;break;\
                                  case CABLE_CH6:CBR_CH6_LED_ON;break;\
                                  }\
                                 }

#define CABLE_BR_LED_OFF(n)         { switch(n) {   \
                                  case CABLE_CH1:CBR_CH1_LED_OFF;break;\
                                  case CABLE_CH2:CBR_CH2_LED_OFF;break;\
                                  case CABLE_CH3:CBR_CH3_LED_OFF;break;\
                                  case CABLE_CH4:CBR_CH4_LED_OFF;break;\
                                  case CABLE_CH5:CBR_CH5_LED_OFF;break;\
                                  case CABLE_CH6:CBR_CH6_LED_OFF;break;\
                                  }\
                                 }

#define CABLE_LED_TOGGLE(n)     { switch(n) {   \
                                  case CABLE_CH1:CR_CH1_LED_TOGGLE;break;\
                                  case CABLE_CH2:CR_CH2_LED_TOGGLE;break;\
                                  case CABLE_CH3:CR_CH3_LED_TOGGLE;break;\
                                  case CABLE_CH4:CR_CH4_LED_TOGGLE;break;\
                                  case CABLE_CH5:CR_CH5_LED_TOGGLE;break;\
                                  case CABLE_CH6:CR_CH6_LED_TOGGLE;break;\
                                  }\
                                 }

        
// Sensor level check
#define SENSOR1_PORT           GPIOE
#define SENSOR1                GPIO_PIN_1      // PE1
#define SENSOR1_CHK            (SENSOR1_PORT->IDR & SENSOR1)

#define SENSOR2_PORT           GPIOE
#define SENSOR2                GPIO_PIN_2      // PE1
#define SENSOR2_CHK            (SENSOR2_PORT->IDR & SENSOR2)

#define SENSOR3_PORT           GPIOE
#define SENSOR3                GPIO_PIN_3      // PE1
#define SENSOR3_CHK            (SENSOR3_PORT->IDR & SENSOR3)

#define SENSOR4_PORT           GPIOE
#define SENSOR4                GPIO_PIN_5      // PE1
#define SENSOR4_CHK            (SENSOR4_PORT->IDR & SENSOR4)

#define SENSOR5_PORT           GPIOE
#define SENSOR5                GPIO_PIN_6      // PE1
#define SENSOR5_CHK            (SENSOR5_PORT->IDR & SENSOR5)

#define SENSOR6_PORT           GPIOE
#define SENSOR6                GPIO_PIN_7      // PE1
#define SENSOR6_CHK            (SENSOR6_PORT->IDR & SENSOR6)

#endif

#endif // FEATURE_STD_ALONE

#endif //___BOARD_H___