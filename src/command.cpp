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
	������� ������� "�����" ������ � ������� ��������� WAKE

 	���������: 		CMD	  - ���� �������� � �������
								N	  - ����� ������������ ���� � ������� (����� ���� ������� Data)
								*Data - ��������� ������� �������� ������������ ����
	������������
	��������:		���

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

Raw_Arr[j+4] = CRC_8_Arr(Raw_Arr, (j+4));	// ���������� ��������� ������ - CRC

j = Array_to_wake(Raw_Arr, (j+5), Send_Arr);

Array_to_USART1(Send_Arr, j);
}


//------------------------------- ����������: --------------------------------
extern uint16_t	Rx_Nbt;      	  		// �������� ���������� ���� � ������
extern char			Rx_Dat[FRAME_IN]; 	// ������ �������� ������

char Tx_Dat_raw[FRAME_OUT];					// "�����" ����� ������
char Tx_Dat[2*FRAME_OUT];						// ������������ ����� � ��������
																		// ����� ������� - � ����� ��������� �������

extern char Command;								// ��� ������� �� ����������
extern StAmpModeRange StAmpMR;			// Step Amp ����� Mode/Range

//float f_val;

// -- ������� ������� ����������� ������ -------------------------------------
void CMD_exec(void)
{
unsigned int length;														// ����� ������������� ������

USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // ����� ����� ������ �������� 
																								// �� ����� ���������� �������
	
Tx_Dat_raw[0] = FEND;														// ������ ������ ������ - ���������
																								// ��� ���� �������
switch (Command)
{	
		// *****************************************************************
		// Link Echo - ������� ��������� ������
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

				Tx_Dat_raw[1] = CMD_LED_ON_OFF;		// ������� ������� �������
/*	
				Tx_Dat_raw[2] = 0x00;							// ����� ���� ������ ������
				Tx_Dat_raw[3] = 0x01;
				Tx_Dat_raw[4] = ERR_NO;						// ���� ������ "��� ������"
			
				Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// ��������� ���� - CRC
				length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // ����� ��������������� �������
				Array_to_USART1(Tx_Dat, length);								// �������� ������			
*/
		break;
		}

		// *****************************************************************
		// Step Amp polarity select
		case CMD_STAMP_POL:	
		{
				if (Rx_Dat[0])	{ST_AMP_POSITIVE;}
				else 						{ST_AMP_NEGATIVE;}

				Tx_Dat_raw[1] = CMD_STAMP_POL;		// ������� ������� �������
/*	
				Tx_Dat_raw[2] = 0x00;							// ����� ���� ������ ������
				Tx_Dat_raw[3] = 0x01;
				Tx_Dat_raw[4] = ERR_NO;						// ���� ������ "��� ������"
			
				Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// ��������� ���� - CRC
				length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // ����� ��������������� �������
				Array_to_USART1(Tx_Dat, length);								// �������� ������
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




		
/*   -------- ����� - ������� �� ���������, ������ ������ --------------------------- */
		
		case CMD_ERR: 										// ��������� ������ ������
    {
		Tx_Dat_raw[1] = CMD_ERR;
/*	
		Tx_Dat_raw[2] = 0x00;							// ����� ���� ������ ������
		Tx_Dat_raw[3] = 0x01;				
		Tx_Dat_raw[4] = ERR_TX;
		Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// ��������� ���� - CRC
		length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // ����� ��������������� �������
		Array_to_USART1(Tx_Dat, length);								// �������� ������ ������
*/
		break;
		}
						
		default: break;
}


// �������� ������� � ������������� �������� �� �������
switch (Command)
{		// �������� ������ �� �������� �������
		case CMD_LED_ON_OFF:
		case CMD_STAMP_POL:
		case CMD_ERR:
												Tx_Dat_raw[2] = 0x00;														// ����� ���� ������ ������
												Tx_Dat_raw[3] = 0x01;
												Tx_Dat_raw[4] = ERR_NO;													// ���� ������ "��� ������"
												Tx_Dat_raw[5] = CRC_8_Arr(Tx_Dat_raw, 5);				// ��������� ���� - CRC
												length = Array_to_wake(Tx_Dat_raw, 6, Tx_Dat);  // ����� ��������������� �������
												Array_to_USART1(Tx_Dat, length);								// �������� ������	

		break;
}





Command = CMD_NOP;	// ������� ����������, ����� ����������� �������
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}



