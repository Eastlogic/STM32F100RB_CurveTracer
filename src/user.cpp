#include "user.h"



// -- Step Amplifier state variables -------------------------------------------
StAmpModeRange StAmpMR;




// -- System clocks init -------------------------------------------------------
void RCC_init(void)
{

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

}



// -- GPIOs init ---------------------------------------------------------------
void GPIO_init(void)
{
GPIO_InitTypeDef GPIO_InitStruct;

/* Configure K1-K6 Step Amp control outputs */
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

GPIO_InitStruct.GPIO_Pin = K1_GPIO_Pin;
GPIO_Init(K1_GPIO, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin = K2_GPIO_Pin;
GPIO_Init(K2_GPIO, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin = K3_GPIO_Pin;
GPIO_Init(K3_GPIO, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin = K4_GPIO_Pin;
GPIO_Init(K4_GPIO, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin = K5_GPIO_Pin;
GPIO_Init(K5_GPIO, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin = K6_GPIO_Pin;
GPIO_Init(K6_GPIO, &GPIO_InitStruct);

// Default Step Amp state
ST_AMP_V_RANGE1;
	
/* Configure USART1_Tx as alternate function push-pull */
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOA, &GPIO_InitStruct);

/* Configure USART1_Rx as input floating */
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOA, &GPIO_InitStruct);

/* Configure Led1, Led2 debug outputs */
GPIO_InitStruct.GPIO_Pin = Led1_PIN;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(Led1_GPIO, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin = Led2_PIN;
GPIO_Init(Led2_GPIO, &GPIO_InitStruct);
}
// *****************************************************************************



// -- Timer 2 init -------------------------------------------------------------
/*void TIM2_init(void)
{
TIM_TimeBaseInitTypeDef 	TimBaseInitStruct;

TimBaseInitStruct.TIM_Prescaler = 300;
TimBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
TimBaseInitStruct.TIM_Period = 0xFFFF;										// Auto-reload register

TimBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseInit(TIM2, &TimBaseInitStruct);


// Timer 2 overflow interrupt enable
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

// Timer 2 enable
TIM_Cmd(TIM2, ENABLE);
}
*/




// -- USART1 init --------------------------------------------------------------
/* 		USARTx configured as followed:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity control
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
*/
void USART1_init(void)
{
USART_InitTypeDef USART_InitStruct;

USART_InitStruct.USART_BaudRate = 115200;
USART_InitStruct.USART_WordLength = USART_WordLength_8b;
USART_InitStruct.USART_StopBits = USART_StopBits_1;
USART_InitStruct.USART_Parity = USART_Parity_No ;
USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

/* Configure the USARTx */ 
USART_Init(USART1, &USART_InitStruct);

USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
/* Enable the USARTx */
USART_Cmd(USART1, ENABLE);
}



void USART1_Transmit(char data)
{
  while(!(USART1->SR & USART_SR_TC)); //Проверка завершения передачи предыдущих данных
  USART1->DR = data; 				  				//Передача данных
}


void USART1_SendData(char* data)
{
		int i = 0;

		while (data[i])
		{
		USART1_Transmit(data[i]);
		i++;
		}
}





//#define FARR_SIZE 16

// Array for float to keep float value in ASCII
//char f_arr[FARR_SIZE];

// Float-to-array conversion with formating for 
// UART output array[]={..., CR, LF}
// Result - CR, LF terminated string, 
// null-symbol added for end of string detection
/*
char* FloatToUartFormat(float f)
{
int n;

n = snprintf(f_arr, FARR_SIZE-3, "%+.5f", f);
f_arr[n++] = '\r';	// Add CR
f_arr[n++] = '\n';  // Add LF
f_arr[n]   = '\0';

return f_arr;
}
*/




// -- ADC1 init ----------------------------------------------------------------
void ADC1_init(void)
{
ADC_InitTypeDef		ADC_Init_struct;

ADC_Init_struct.ADC_Mode = ADC_Mode_Independent;
ADC_Init_struct.ADC_ScanConvMode = ENABLE;
ADC_Init_struct.ADC_ContinuousConvMode = ENABLE;
ADC_Init_struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
ADC_Init_struct.ADC_DataAlign = ADC_DataAlign_Right;
ADC_Init_struct.ADC_NbrOfChannel = ADC_CH_NUMB;			// Число каналов в последовательности преобразования
ADC_Init(ADC1, &ADC_Init_struct); 									// Initialise ADC1

//Порядок оцифровки
ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime);

ADC_Cmd(ADC1, ENABLE);														// Enable ADC1

//Калибровка ADC1
ADC_ResetCalibration(ADC1);												// Enable ADC1 reset calibaration register
while (ADC_GetResetCalibrationStatus(ADC1));			// Check the end of ADC1 reset calibration register

ADC_StartCalibration(ADC1);												// Start ADC1 calibration
while (ADC_GetCalibrationStatus(ADC1));						// Check the end of ADC1 calibration

ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}


// -- Настройка прерываний в NVIC ----------------------------------------------
void NVIC_init(void)
{

NVIC_SetPriority(USART1_IRQn, 0);
NVIC_EnableIRQ(USART1_IRQn);

//NVIC_SetPriority(TIM2_IRQn, 0);
//NVIC_EnableIRQ(TIM2_IRQn);


	

}









