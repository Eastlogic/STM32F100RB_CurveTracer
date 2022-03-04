/*
Parser of received command to be performed
*/

#include "stm32f10x.h"
#include "wake16.h"
#include "user.h"
#include "command.h"


void Array_to_USART1(char *arr, unsigned int length)
{
int i;
		for(i=0; i<length; i++)
		{
			while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
    	USART_SendData(USART1, arr[i]);
		}
}



//------------------------------- Переменные: --------------------------------
extern uint16_t	Rx_Nbt;      	  		// Принятое количество байт в пакете
extern char			Rx_Dat[FRAME_IN]; 	// Массив принятых данных

char Tx_Dat_raw[FRAME_OUT];					// "Сырой" фрейм ответа
char Tx_Dat[2*FRAME_OUT];						// Обработанный фрейм к отправке
																		// Длина фреймов - с очень небольшим запасом

extern char Command;								// Код команды на выполнение
extern StAmpModeRange StAmpMR;			// Step Amp режим Mode/Range

uint16_t	StAmpDACOut;							// Step Amp output value


// -- Функция разбора выполняемых команд -------------------------------------
void CMD_exec(void)
{
unsigned int length;														// Длина отправляемого фрейма

USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // Прием новых команд запрещен 
																								// до конца выполнения текущей
	
Tx_Dat_raw[0] = FEND;														// Начало пакета ответа - одинаково
																								// для всех ответов
switch (Command)
{	
		// *****************************************************************
		// Link Echo - возврат пришедших данных
		case CMD_LINK_ECHO:	
		{		
				//Send_WAKE_pckg(CMD_LINK_ECHO, Rx_Nbt, Rx_Dat);
				Tx_Dat_raw[1] = CMD_LINK_ECHO;		// Возврат команды запроса				
				break;
		}

		// *****************************************************************
		// Debug command - Led on/off switching
		case CMD_LED_ON_OFF:	
		{
				if (Rx_Dat[0])	{Led1_ON;}
				else 						{Led1_OFF;}

				Tx_Dat_raw[1] = CMD_LED_ON_OFF;		// Возврат команды запроса

		break;
		}

		// *****************************************************************
		// Step Amp polarity select
		case CMD_STAMP_POL:	
		{
				if (Rx_Dat[0])	{ST_AMP_POSITIVE;}
				else 						{ST_AMP_NEGATIVE;}

				Tx_Dat_raw[1] = CMD_STAMP_POL;		// Возврат команды запроса

		break;
		}

		// *****************************************************************
		// Step Amplifier mode and range select
		case CMD_STAMP_MODE_RANGE:	
		{

				if ((Rx_Dat[0]>=Mode_Range_First) && (Rx_Dat[0]<=Mode_Range_Last))
				{
						switch(Rx_Dat[0])
						{
						case ModeI_Range1: 	ST_AMP_I_RANGE1; 	break;
						case ModeI_Range2: 	ST_AMP_I_RANGE2; 	break;
						case ModeI_Range3: 	ST_AMP_I_RANGE3; 	break;
						case ModeI_Range4: 	ST_AMP_I_RANGE4; 	break;
						case ModeI_Range5: 	ST_AMP_I_RANGE5; 	break;
						case ModeI_Range6: 	ST_AMP_I_RANGE6; 	break;
						case ModeI_Range7: 	ST_AMP_I_RANGE7; 	break;
						case ModeI_Range8: 	ST_AMP_I_RANGE8; 	break;

						case ModeV_Range1: 	ST_AMP_V_RANGE1; 	break;
						case ModeV_Range2: 	ST_AMP_V_RANGE2; 	break;
						case ModeV_Range3: 	ST_AMP_V_RANGE3; 	break;
						case ModeV_Range4: 	ST_AMP_V_RANGE4; 	break;
						case ModeV_Range5: 	ST_AMP_V_RANGE5; 	break;
						case ModeV_Range6: 	ST_AMP_V_RANGE6; 	break;
						case ModeV_Range7: 	ST_AMP_V_RANGE7; 	break;
						case ModeV_Range8: 	ST_AMP_V_RANGE8; 	break;
						}
					
				// Successfull feedback message here...
				Tx_Dat_raw[1] = CMD_STAMP_MODE_RANGE;
				}
				else 		// If received unknown parameter
				{
				// "Unknown parameter" feedback  message here
				Tx_Dat_raw[1] = CMD_ERR;
				}
		break;
		}

		// *****************************************************************
		// Step Amplifier set out signal
		case CMD_STAMP_SET_OUT:	
		{

				StAmpDACOut = (Rx_Dat[0]<<8) | Rx_Dat[1];
		
				if (StAmpDACOut <= 0xFFF)					// If valid value received
				{
				DAC_SetChannel1Data(DAC_Align_12b_R, StAmpDACOut);
				
				// Successfull feedback message here...
				Tx_Dat_raw[1] = CMD_STAMP_SET_OUT;
				}
				else
				{
				// "Unknown parameter" feedback  message here
				Tx_Dat_raw[1] = CMD_ERR;
				}

		break;
		}

		
/*   -------- Ответ - Команда не выполнена, ошибка обмена --------------------------- */
		
		case CMD_ERR: 				// Обработка ошибки обмена
		{
		Tx_Dat_raw[1] = CMD_ERR;
		break;
		}
						
		default: break;
}



// ********** Отправка ответов на входящие команды **************************

if(CMD_WITH_SHORT_RESPONSE(Command))	// Отправка ответов с коротким форматом
{
		Tx_Dat_raw[2] = 0x00;		// Число байт данных ответа
		Tx_Dat_raw[3] = 0x01;

		switch (Command)
		{		// Короткие ответы на короткие команды

		case CMD_LINK_ECHO:

							Tx_Dat_raw[4] = Rx_Dat[0];											// Байт данных "нет ошибки"
							Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// Последний байт - CRC
		break;

		case CMD_LED_ON_OFF:
		case CMD_STAMP_POL:	

							Tx_Dat_raw[4] = ERR_NO;													// Байт данных "нет ошибки"
							Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// Последний байт - CRC
		break;
		
		case CMD_STAMP_MODE_RANGE:
		case CMD_STAMP_SET_OUT:
					
							if ((Tx_Dat_raw[1] == CMD_STAMP_MODE_RANGE) || (Tx_Dat_raw[1] == CMD_STAMP_SET_OUT))
							{Tx_Dat_raw[4] = ERR_NO;}

							if (Tx_Dat_raw[1] == CMD_ERR)
							{Tx_Dat_raw[4] = ERR_PA;}

							Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// Последний байт - CRC
		break;

		case CMD_ERR:
		
							Tx_Dat_raw[4] = ERR_TX;													// Байт данных "нет ошибки"
							Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// Последний байт - CRC
		break;

		}

		length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // Длина сформированного массива
		Array_to_USART1(Tx_Dat, length);								// Отправка ответа	
}
else												// Отправка ответов с длинным форматом
{

}



Command = CMD_NOP;	// Команда обработана, сброс обработанной команды
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}		
// end of CMD_exec()



