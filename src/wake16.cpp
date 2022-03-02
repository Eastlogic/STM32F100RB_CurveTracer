#include "wake16.h"


/* 	���������� ����������� ����� ����� ������ �� �������� W = X^8+X^5+X^4+1 
	
	���������: 		b 		- ���� ������� ������
					crc		- ��������� �� ������������������ �������� CRC
	������������
	��������:		���
*/
void CRC_8(char b, char *crc)
{
  char i;
  
  for(i = 0; i < 8; b = b >> 1, i++)
  if((b ^ *crc) & 1) *crc = ((*crc ^ 0x18) >> 1) | 0x80;
  else *crc = (*crc >> 1) & ~0x80;
}


/* 	���������� ����������� ����� ������� ������ 
	
	���������: 		arr 	- ��������� �� ������ ������� ������� ����
								length	- ����� ������� ������
	������������
	��������:		CRC		- ����������� �����
*/
char CRC_8_Arr(char *arr, unsigned int length)
{
char CRC=CRC_INIT;						// �������������� CRC ���������
  
  while(length)
  {
  CRC_8(*arr, &CRC);
  arr++;
  length--;
  }

  return CRC;
}




/* 	������������� �������� ������ �������� ������ ������ 
	
	���������: 		���
	������������
	��������:		���
*/


//------------------------------- ����������: --------------------------------
extern char Rx_Sta;				//��������� �������� ������ ����         	
extern char Command;			//��� ������� �� ����������


void Receive_init(void)
{
Rx_Sta  = WAIT_FEND;
Command = CMD_NOP;
}




/* �������������� "������" ������� � �������� ������ ������� ��������� WAKE

 	���������: 		char *Init_ar	- ��������� �� �������� ������ ������
					Init_length		- ����� ��������� ������� ������
					char *Res_ar	- ��������� �� �������������� ������ ������
	������������
	��������:		j - ����� ��������������� ������� ������
					��������� �������������� ����������� � ������ ������ Res_ar
*/


unsigned int Array_to_wake(char *Init_ar, unsigned int Init_length, char *Res_ar)
{
unsigned int i, j;

	j = 0;
		// ��������� ������� � ��������
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
return j;		// ���������� ����� ��������������� �������
}



