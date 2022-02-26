/*
�������� ��������� �������� ������ WAKE
*/

#ifndef WAKE16_H_
#define WAKE16_H_


#define FEND  			0xC0	//Frame END
#define FESC  			0xDB	//Frame ESCape
#define TFEND 			0xDC	//Transposed Frame END
#define TFESC 			0xDD	//Transposed Frame ESCape

#define CRC_INIT 		0xDE	//Initial CRC value
#define FRAME_IN 		32    //������������ ����� ���� ��������� ������ � ������ ����������
#define FRAME_OUT 	256	  //������������ ����� ������������ �������� ���� ������								

//���� ������������� ������
#define CMD_NOP     0			//��� ��������
#define CMD_ERR     1			//������ ������ ������

//���� ������:
#define ERR_NO 			0x00  //No error
#define ERR_TX 			0x01  //Rx/Tx error
#define ERR_PA 			0x04  //Parameters value error


//RX process states:
enum { WAIT_FEND,     // �������� ������ FEND
       WAIT_CMD,      // �������� ������ �������
       WAIT_NH,       // �������� ������ ���������� ���� � ������
			 WAIT_NL,		  	// [NH:NL] = 0...65535
       WAIT_DATA,     // ����� ������
       WAIT_CRC       // �������� ����� ������ CRC
       };







// 	���������� ����������� ����� ����� ������
void CRC_8(char b, char *crc);

// ���������� ����������� ����� ������� ������
char CRC_8_Arr(char *arr, unsigned int length);

// ������������� �������� ������ �������� ������ ������
void Receive_init(void);

// �������������� "������" ������� � �������� ������ ������� ��������� WAKE
unsigned int Array_to_wake(char *Init_ar, unsigned int Init_length, char *Res_ar);

#endif


