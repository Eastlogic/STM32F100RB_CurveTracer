#include "wake16.h"


/* 	Вычисление контрольной суммы байта данных по полиному W = X^8+X^5+X^4+1 
	
	Параметры: 		b 		- байт входных данных
					crc		- указатель на инициализированное значение CRC
	Возвращаемое
	значение:		нет
*/
void CRC_8(char b, char *crc)
{
  char i;
  
  for(i = 0; i < 8; b = b >> 1, i++)
  if((b ^ *crc) & 1) *crc = ((*crc ^ 0x18) >> 1) | 0x80;
  else *crc = (*crc >> 1) & ~0x80;
}


/* 	Вычисление контрольной суммы массива данных 
	
	Параметры: 		arr 	- указатель на первый элемент массива байт
								length	- длина массива данных
	Возвращаемое
	значение:		CRC		- контрольная сумма
*/
char CRC_8_Arr(char *arr, unsigned int length)
{
char CRC=CRC_INIT;						// Инициализируем CRC значением
  
  while(length)
  {
  CRC_8(*arr, &CRC);
  arr++;
  length--;
  }

  return CRC;
}




/* 	Инициализация автомата приема входного пакета данных 
	
	Параметры: 		нет
	Возвращаемое
	значение:		нет
*/


//------------------------------- Переменные: --------------------------------
extern char Rx_Sta;				//Состояние процесса приема байт         	
extern char Command;			//Код команды на выполнение


void Receive_init(void)
{
Rx_Sta  = WAIT_FEND;
Command = CMD_NOP;
}




/* Преобразование "сырого" массива в выходной массив формата протокола WAKE

 	Параметры: 		char *Init_ar	- указатель на исходный массив данных
					Init_length		- длина исходного массива данных
					char *Res_ar	- указатель на результирующий массив данных
	Возвращаемое
	значение:		j - длина результирующего массива данных
					Результат преобразования сохраняется в массив данных Res_ar
*/


unsigned int Array_to_wake(char *Init_ar, unsigned int Init_length, char *Res_ar)
{
unsigned int i, j;

	j = 0;
		// Обработка массива к отправке
		for (i=0; i<Init_length; i++)
		{
			if ((i==0)||(i==1)) 
			{
			Res_ar[i] = Init_ar[i];
			}
			else
			{
				if ((Init_ar[i]==FEND)||(Init_ar[i]==FESC))
				{
				Res_ar[j++] = FESC;

					if (Init_ar[i]==FEND) {Res_ar[j]   =	TFEND;}
					if (Init_ar[i]==FESC) {Res_ar[j]   =	TFESC;}
				}
				else
				{
					Res_ar[j] = Init_ar[i];
				}
			}		
		j++;
		}
return j;		// Возвращаем длину результирующего массива
}



