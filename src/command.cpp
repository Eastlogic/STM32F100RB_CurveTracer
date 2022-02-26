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

/* 
	Функция посылки "сырых" данных в формате протокола WAKE

 	Параметры: 		CMD	  - поле комманды в посылке
								N	  - число пересылаемых байт в посылке (число байт массива Data)
								*Data - указатель первого элемента пересылаемых байт
	Возвращаемое
	значение:		нет

*/

char Raw_Arr[FRAME_OUT+16];
char Send_Arr[2*(FRAME_OUT+16)];

void Send_WAKE_pckg(unsigned char CMD, unsigned int N, char* Data)
{

unsigned int j;

Raw_Arr[0] = FEND;
Raw_Arr[1] = CMD;
Raw_Arr[2] = (unsigned char)(N>>8);
Raw_Arr[3] = (unsigned char)N;

for (j=0; j<N; j++)
{
Raw_Arr[j+4] = Data[j];
}

Raw_Arr[j+4] = CRC_8_Arr(Raw_Arr, (j+4));	// Дополнение последним байтом - CRC

j = Array_to_wake(Raw_Arr, (j+5), Send_Arr);

Array_to_USART1(Send_Arr, j);
}


//------------------------------- Переменные: --------------------------------
extern uint16_t	Rx_Nbt;      	  		// Принятое количество байт в пакете
extern char			Rx_Dat[FRAME_IN]; 	// Массив принятых данных

char Tx_Dat_raw[FRAME_OUT];					// "Сырой" фрейм ответа
char Tx_Dat[2*FRAME_OUT];						// Обработанный фрейм к отправке
																		// Длина фреймов - с очень небольшим запасом

extern char Command;								// Код команды на выполнение
extern StAmpModeRange StAmpMR;			// Step Amp режим Mode/Range

//float f_val;

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
		Send_WAKE_pckg(CMD_LINK_ECHO, Rx_Nbt, Rx_Dat);			
		break;
		}

		// *****************************************************************
		// Debug command - Led on/off switching
		case CMD_LED_ON_OFF:	
		{
				if (Rx_Dat[0])	{Led1_ON;}
				else 						{Led1_OFF;}

				Tx_Dat_raw[1] = CMD_LED_ON_OFF;		// Возврат команды запроса
/*	
				Tx_Dat_raw[2] = 0x00;							// Число байт данных ответа
				Tx_Dat_raw[3] = 0x01;
				Tx_Dat_raw[4] = ERR_NO;						// Байт данных "нет ошибки"
			
				Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// Последний байт - CRC
				length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // Длина сформированного массива
				Array_to_USART1(Tx_Dat, length);								// Отправка ответа			
*/
		break;
		}

		// *****************************************************************
		// Step Amp polarity select
		case CMD_STAMP_POL:	
		{
				if (Rx_Dat[0])	{ST_AMP_POSITIVE;}
				else 						{ST_AMP_NEGATIVE;}

				Tx_Dat_raw[1] = CMD_STAMP_POL;		// Возврат команды запроса
/*	
				Tx_Dat_raw[2] = 0x00;							// Число байт данных ответа
				Tx_Dat_raw[3] = 0x01;
				Tx_Dat_raw[4] = ERR_NO;						// Байт данных "нет ошибки"
			
				Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// Последний байт - CRC
				length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // Длина сформированного массива
				Array_to_USART1(Tx_Dat, length);								// Отправка ответа
*/
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

					}
					else			// If received unknown parameter
					{
					// "Unknown parameter" feedback  message here

					}
		break;
		}




		
/*   -------- Ответ - Команда не выполнена, ошибка обмена --------------------------- */
		
		case CMD_ERR: 										// Обработка ошибки обмена
    {
		Tx_Dat_raw[1] = CMD_ERR;
/*	
		Tx_Dat_raw[2] = 0x00;							// Число байт данных ответа
		Tx_Dat_raw[3] = 0x01;				
		Tx_Dat_raw[4] = ERR_TX;
		Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// Последний байт - CRC
		length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // Длина сформированного массива
		Array_to_USART1(Tx_Dat, length);								// Отправка фрейма ответа
*/
		break;
		}
						
		default: break;
}


// Отправка ответов с повторяющимся форматом на команды
switch (Command)
{		// Короткие ответы на короткие команды
		case CMD_LED_ON_OFF:
		case CMD_STAMP_POL:
		case CMD_ERR:
												Tx_Dat_raw[2] = 0x00;														// Число байт данных ответа
												Tx_Dat_raw[3] = 0x01;
												Tx_Dat_raw[4] = ERR_NO;													// Байт данных "нет ошибки"
												Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// Последний байт - CRC
												length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // Длина сформированного массива
												Array_to_USART1(Tx_Dat, length);								// Отправка ответа	

		break;
}





Command = CMD_NOP;	// Команда обработана, сброс выполняемой команды
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}



