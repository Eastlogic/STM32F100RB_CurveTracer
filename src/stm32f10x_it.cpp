/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "user.h"
#include "wake16.h"
#include "command.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
extern "C" void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
extern "C" void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
extern "C" void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
extern "C" void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
extern "C" void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
extern "C" void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
extern "C" void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
extern "C" void PendSV_Handler(void)
{
}




extern "C" void SysTick_Handler(void)
{
}







/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
	USART1 Interrupt Handler 
*******************************************************************************/
//------------------------------- Переменные: --------------------------------
char Rx_Sta,							// Состояние процесса приема байт         	
     Rx_Pre,        			// Предыдущий принятый байт
     Rx_Cmd,        			// Принятая команда     
     Rx_Dat[FRAME_IN], 		// Массив принятых данных
     Rx_Crc;        			// Контрольная сумма принимаемого пакета     

uint16_t Rx_Nbt;      		// Принимаемое количество байт в пакете
uint16_t Rx_Ptr;        	// Индекс массива принимаемых данных

char Command;							// Код команды на выполнение



extern "C" void USART1_IRQHandler(void)
{
unsigned char data_in;
char Pre;

// -------- If Read Data Register is not empty -----------------------------------
if((USART1->SR & USART_SR_RXNE)!=0)
{
data_in = USART1->DR;  							// Чтение данных
	 
  if(data_in == FEND)             	// Если обнаружено начало фрейма,
  {
  Rx_Pre = data_in;             		// то сохранение пре-байта
  Rx_Crc = CRC_INIT;              	// Инициализация CRC,
  Rx_Sta = WAIT_CMD;             		// Переход в следующее состояние,
  CRC_8(data_in, &Rx_Crc);    			// Обновление CRC,
  return;                         	// Выход
  }

  if(Rx_Sta == WAIT_FEND)           // -----> Если ожидание FEND,
  return;                           //то выход	
	
  Pre = Rx_Pre;                			// Сохранение старого пре-байта
  Rx_Pre = data_in;                 // Обновление пре-байта	
	
  if(Pre == FESC)                   // Если пре-байт равен FESC,
  {
    if(data_in == TFESC)            // а байт данных равен TFESC,
       data_in = FESC;              // то заменить его на FESC
    else if(data_in == TFEND)       // Если байт данных равен TFEND,
           data_in = FEND;          // то заменить его на FEND
         else
         {
           Rx_Sta = WAIT_FEND;      // для всех других значений байта данных,
           Command = CMD_ERR;       // следующего за FESC, ошибка
           return;
         }
  }
  else
  {
    if(data_in == FESC)             // Если байт данных равен FESC, он просто
      return;                       // запоминается в пре-байте
  }	
	
	

  switch(Rx_Sta)
  {
  
  case WAIT_CMD:                      // -----> ожидание приема команды
    {
      if(data_in & 0x80)              // Проверка бита 7 команды
      {
        Rx_Sta = WAIT_FEND;           // Если бит 7 не равен нулю,
        Command = CMD_ERR;            // то ошибка
        break;
      }
      Rx_Cmd = data_in;               // Сохранение команды
      CRC_8(data_in, &Rx_Crc);        // Обновление CRC
      Rx_Sta = WAIT_NH;               // Переходим к приему количества байт
      break;
    }
  case WAIT_NH:                      	// -----> Ожидание приема количества байт
    {								 									// Старший байт кол-ва
      Rx_Nbt = (data_in<<8);

      CRC_8(data_in, &Rx_Crc); 	      // Обновление CRC
			Rx_Sta = WAIT_NL;				  			// Переход к приему младшего байта кол-ва байт
      break;
    }
  case WAIT_NL:                      	// -----> Ожидание приема количества байт
    {								 									// Младший байт кол-ва	
      Rx_Nbt |= data_in;			 				// Конкатенация байт количества

      if(Rx_Nbt > FRAME_IN)          	// Если количество байт > FRAME_IN,
      {
        Rx_Sta = WAIT_FEND;
        Command = CMD_ERR;            // то ошибка
        break;
      }

      CRC_8(data_in, &Rx_Crc); 	      // Обновление CRC
      Rx_Ptr = 0;                     // Обнуляем индекс массива данных
      Rx_Sta = WAIT_DATA;             // Переходим к приему данных	  
      break;
    }

  case WAIT_DATA:                     // -----> Ожидание приема данных
    {
      if(Rx_Ptr < Rx_Nbt)             // Если не все данные приняты,
      {
        Rx_Dat[Rx_Ptr++] = data_in;   // то сохранение байта данных,
        CRC_8(data_in, &Rx_Crc);  	  // Обновление CRC
        break;
      }

      if(data_in != Rx_Crc)          	// Если приняты все данные, то проверка CRC
      {
        Rx_Sta = WAIT_FEND;          	// Если CRC не совпадает,
        Command = CMD_ERR;           	// то ошибка
        break;
      }
      Rx_Sta = WAIT_FEND;            	// Прием пакета завершен,
      Command = Rx_Cmd;              	// загрузка команды на выполнение
      break;
    }
  }

	
}// end USART_SR_RXNE Handler

}// End USART1_IRQ Handler 

// ---------------------------------------------------------------------------














//uint16_t adc_result;
//float    f_result;
/*******************************************************************************
	TIM2 Interrupt Handler 
*******************************************************************************/
/*
extern "C" void TIM2_IRQHandler(void)
{
static bool flag = false;


		if (TIM_GetITStatus(TIM2, TIM_IT_Update) !=RESET)
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

						if (flag)
						{
						// Turn off led
						GPIO_ResetBits(Led1_GPIO, Led1_PIN);
						flag = false; 
						}
						else
						{
						// Turn on led
						GPIO_SetBits(Led1_GPIO, Led1_PIN);
						flag = true;
						}
		
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC != SET));
		adc_result = ADC1->DR;
		
		f_result = adc_result*100;
		f_result /= 4095;
		f_result -= 50;

		USART1_SendData(FloatToUartFormat(f_result));
		}
}
*/







/******************* (C) EASTLOGIC Digital Lab. ****************/
