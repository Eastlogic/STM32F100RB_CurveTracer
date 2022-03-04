#include "stm32f10x.h"
#include "user.h"						// User service routines
#include "my_stack.h"				// Simple user stack implementation
#include "wake16.h"					// Wake16 data transfer protocol
#include "command.h"				// Parser of received command to perform


//Stack<XY> xy_stack(100);

extern char Command;		// ������� �� ����������


int main(void)
{
RCC_init();
GPIO_init();
DAC_init();

//TIM2_init();	
USART1_init();
ADC1_init();

NVIC_init();						// ���������� ������ ���������� � ����������� � NVIC
__enable_irq();


	while(1)
	{
			if (Command != CMD_NOP)	// ���� ������ ����� �������
			{
			CMD_exec();
			}		
	}



}



