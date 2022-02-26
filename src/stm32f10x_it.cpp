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
//------------------------------- ����������: --------------------------------
char Rx_Sta,							// ��������� �������� ������ ����         	
     Rx_Pre,        			// ���������� �������� ����
     Rx_Cmd,        			// �������� �������     
     Rx_Dat[FRAME_IN], 		// ������ �������� ������
     Rx_Crc;        			// ����������� ����� ������������ ������     

uint16_t Rx_Nbt;      		// ����������� ���������� ���� � ������
uint16_t Rx_Ptr;        	// ������ ������� ����������� ������

char Command;							// ��� ������� �� ����������



extern "C" void USART1_IRQHandler(void)
{
unsigned char data_in;
char Pre;

// -------- If Read Data Register is not empty -----------------------------------
if((USART1->SR & USART_SR_RXNE)!=0)
{
data_in = USART1->DR;  							// ������ ������
	 
  if(data_in == FEND)             	// ���� ���������� ������ ������,
  {
  Rx_Pre = data_in;             		// �� ���������� ���-�����
  Rx_Crc = CRC_INIT;              	// ������������� CRC,
  Rx_Sta = WAIT_CMD;             		// ������� � ��������� ���������,
  CRC_8(data_in, &Rx_Crc);    			// ���������� CRC,
  return;                         	// �����
  }

  if(Rx_Sta == WAIT_FEND)           // -----> ���� �������� FEND,
  return;                           //�� �����	
	
  Pre = Rx_Pre;                			// ���������� ������� ���-�����
  Rx_Pre = data_in;                 // ���������� ���-�����	
	
  if(Pre == FESC)                   // ���� ���-���� ����� FESC,
  {
    if(data_in == TFESC)            // � ���� ������ ����� TFESC,
       data_in = FESC;              // �� �������� ��� �� FESC
    else if(data_in == TFEND)       // ���� ���� ������ ����� TFEND,
           data_in = FEND;          // �� �������� ��� �� FEND
         else
         {
           Rx_Sta = WAIT_FEND;      // ��� ���� ������ �������� ����� ������,
           Command = CMD_ERR;       // ���������� �� FESC, ������
           return;
         }
  }
  else
  {
    if(data_in == FESC)             // ���� ���� ������ ����� FESC, �� ������
      return;                       // ������������ � ���-�����
  }	
	
	

  switch(Rx_Sta)
  {
  
  case WAIT_CMD:                      // -----> �������� ������ �������
    {
      if(data_in & 0x80)              // �������� ���� 7 �������
      {
        Rx_Sta = WAIT_FEND;           // ���� ��� 7 �� ����� ����,
        Command = CMD_ERR;            // �� ������
        break;
      }
      Rx_Cmd = data_in;               // ���������� �������
      CRC_8(data_in, &Rx_Crc);        // ���������� CRC
      Rx_Sta = WAIT_NH;               // ��������� � ������ ���������� ����
      break;
    }
  case WAIT_NH:                      	// -----> �������� ������ ���������� ����
    {								 									// ������� ���� ���-��
      Rx_Nbt = (data_in<<8);

      CRC_8(data_in, &Rx_Crc); 	      // ���������� CRC
			Rx_Sta = WAIT_NL;				  			// ������� � ������ �������� ����� ���-�� ����
      break;
    }
  case WAIT_NL:                      	// -----> �������� ������ ���������� ����
    {								 									// ������� ���� ���-��	
      Rx_Nbt |= data_in;			 				// ������������ ���� ����������

      if(Rx_Nbt > FRAME_IN)          	// ���� ���������� ���� > FRAME_IN,
      {
        Rx_Sta = WAIT_FEND;
        Command = CMD_ERR;            // �� ������
        break;
      }

      CRC_8(data_in, &Rx_Crc); 	      // ���������� CRC
      Rx_Ptr = 0;                     // �������� ������ ������� ������
      Rx_Sta = WAIT_DATA;             // ��������� � ������ ������	  
      break;
    }

  case WAIT_DATA:                     // -----> �������� ������ ������
    {
      if(Rx_Ptr < Rx_Nbt)             // ���� �� ��� ������ �������,
      {
        Rx_Dat[Rx_Ptr++] = data_in;   // �� ���������� ����� ������,
        CRC_8(data_in, &Rx_Crc);  	  // ���������� CRC
        break;
      }

      if(data_in != Rx_Crc)          	// ���� ������� ��� ������, �� �������� CRC
      {
        Rx_Sta = WAIT_FEND;          	// ���� CRC �� ���������,
        Command = CMD_ERR;           	// �� ������
        break;
      }
      Rx_Sta = WAIT_FEND;            	// ����� ������ ��������,
      Command = Rx_Cmd;              	// �������� ������� �� ����������
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
