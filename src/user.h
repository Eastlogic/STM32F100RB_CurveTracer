#ifndef USER_H_
#define USER_H_

#include "stm32f10x.h"
#include <stdio.h>

// ---------- Привязка выводов МК ----------------------------------


// -- PC8, PC9 - led outputs for debug purposes --------------------
#define Led1_GPIO		GPIOC
#define Led2_GPIO		GPIOC
#define Led1_PIN		GPIO_Pin_8
#define Led2_PIN		GPIO_Pin_9

#define Led1_ON			GPIO_SetBits(Led1_GPIO, Led1_PIN);
#define Led1_OFF		GPIO_ResetBits(Led1_GPIO, Led1_PIN);

// -- Definitions for Step Amplifier state vontrol -----------------
// K1-K6 -> PB10-PB15 
#define 		K1_GPIO					GPIOB
#define 		K2_GPIO					GPIOB
#define 		K3_GPIO					GPIOB
#define 		K4_GPIO					GPIOB
#define 		K5_GPIO					GPIOB
#define 		K6_GPIO					GPIOB

#define 		K1_GPIO_Pin			GPIO_Pin_10
#define 		K2_GPIO_Pin			GPIO_Pin_11
#define 		K3_GPIO_Pin			GPIO_Pin_12
#define 		K4_GPIO_Pin			GPIO_Pin_13
#define 		K5_GPIO_Pin			GPIO_Pin_14
#define 		K6_GPIO_Pin			GPIO_Pin_15


// -- Тип - перечень состояний режима Step Amplifier ---------------
typedef enum
{
Mode_Range_First = 0,
ModeI_Range1 = Mode_Range_First,
ModeI_Range2,
ModeI_Range3,
ModeI_Range4,
ModeI_Range5,
ModeI_Range6,
ModeI_Range7,
ModeI_Range8,
ModeV_Range1,
ModeV_Range2,
ModeV_Range3,
ModeV_Range4,
ModeV_Range5,
ModeV_Range6,
ModeV_Range7,
ModeV_Range8,
Mode_Range_Last = ModeV_Range8
}
StAmpModeRange;


// -- Макросы управления режимами Step Amplifier -------------------

#define ST_AMP_POSITIVE				GPIO_ResetBits(K1_GPIO, K1_GPIO_Pin);
#define ST_AMP_NEGATIVE				GPIO_SetBits(K1_GPIO, K1_GPIO_Pin);

#define ST_AMP_I_RANGE1				GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K2_GPIO, K2_GPIO_Pin);\
									StAmpMR = ModeI_Range1;

#define ST_AMP_I_RANGE2				GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									StAmpMR = ModeI_Range2;

#define ST_AMP_I_RANGE3				GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_SetBits(K5_GPIO, K5_GPIO_Pin);\
									StAmpMR = ModeI_Range3;

#define ST_AMP_I_RANGE4				GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K5_GPIO, K5_GPIO_Pin);\
									StAmpMR = ModeI_Range4;

#define ST_AMP_I_RANGE5				GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_SetBits(K4_GPIO, K4_GPIO_Pin);\
									StAmpMR = ModeI_Range5;

#define ST_AMP_I_RANGE6				GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K4_GPIO, K4_GPIO_Pin);\
									StAmpMR = ModeI_Range6;

#define ST_AMP_I_RANGE7				GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_SetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_SetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_SetBits(K6_GPIO, K6_GPIO_Pin);\
									StAmpMR = ModeI_Range7;

#define ST_AMP_I_RANGE8				GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_SetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_SetBits(K6_GPIO, K6_GPIO_Pin);\
									StAmpMR = ModeI_Range8;

#define ST_AMP_V_RANGE1				GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_SetBits(K3_GPIO, K3_GPIO_Pin);\
									StAmpMR = ModeV_Range1;

#define ST_AMP_V_RANGE2				GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K3_GPIO, K3_GPIO_Pin);\
									StAmpMR = ModeV_Range2;

#define ST_AMP_V_RANGE3				GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_SetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_SetBits(K5_GPIO, K5_GPIO_Pin);\
									StAmpMR = ModeV_Range3;

#define ST_AMP_V_RANGE4				GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_SetBits(K5_GPIO, K5_GPIO_Pin);\
									StAmpMR = ModeV_Range4;

#define ST_AMP_V_RANGE5				GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_SetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_SetBits(K4_GPIO, K4_GPIO_Pin);\
									StAmpMR = ModeV_Range5;

#define ST_AMP_V_RANGE6				GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);\
									GPIO_SetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_SetBits(K4_GPIO, K4_GPIO_Pin);\
									StAmpMR = ModeV_Range6;

#define ST_AMP_V_RANGE7				GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_SetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_SetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_SetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_SetBits(K6_GPIO, K6_GPIO_Pin);\
									StAmpMR = ModeV_Range7;

#define ST_AMP_V_RANGE8				GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_SetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_SetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_SetBits(K6_GPIO, K6_GPIO_Pin);\
									StAmpMR = ModeV_Range8;
/*
#define ST_AMP_NULL					GPIO_ResetBits(K1_GPIO, K1_GPIO_Pin);\
									GPIO_ResetBits(K2_GPIO, K2_GPIO_Pin);\
									GPIO_ResetBits(K3_GPIO, K3_GPIO_Pin);\
									GPIO_ResetBits(K4_GPIO, K4_GPIO_Pin);\
									GPIO_ResetBits(K5_GPIO, K5_GPIO_Pin);\
									GPIO_ResetBits(K6_GPIO, K6_GPIO_Pin);
*/





// X, Y data struct, for graph point storage
typedef struct
{
uint16_t x;
uint16_t y;
}XY;




// -- System clocks init -------------------------------
void RCC_init(void);

// -- GPIOs init ---------------------------------------
void GPIO_init(void);

// -- DAC init -----------------------------------------
void DAC_init(void);

// -- Timer2 init --------------------------------------
//void TIM2_init(void);

// -- USART1 init --------------------------------------
void USART1_init(void);


#define RCC_ADC_Prescaler RCC_PCLK2_Div2		// ADC Prescaler

// Nmbr of channels in conversion sequence
#define ADC_CH_NUMB	1

#define 		ADC_SampleTime 		ADC_SampleTime_239Cycles5
// -- ADC1 init ----------------------------------------
void ADC1_init(void);


void USART1_Transmit(char data);
void USART1_SendData(char* data);

// Float-to-array conversion
//char* FloatToUartFormat(float f);

// -- NVIC init ----------------------------------------
void NVIC_init(void);




#endif
