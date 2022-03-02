/*
������ ���������� ���������� ������
*/
#ifndef COMMAND_H_
#define COMMAND_H_

//���� ����������� ������ ����������
#define CMD_LINK_ECHO			0x0A			// ������� �������� �����
#define CMD_LED_ON_OFF			0x0B			// Debug command

// -----------------------------------------------------------------------
#define CMD_STAMP_POL			0x0C			// Step Amp polarity select
#define CMD_STAMP_MODE_RANGE	0x0D			// Step Amp mode (current/voltage)
																				// and range select

#define CMD_STAMP_SET_OUT		0x0E			// Step Amp set out signal


//#define CMD_SET_GAIN	0x0B			// ������� ��������� ������������ �������� PGA1, PGA2
//#define CMD_SEL_R			0x0C			// ������� ���������� ���� R_SEL
//#define CMD_SEL_IV		0x0D			// ������� ���������� ���� IV_SEL

//#define CMD_SET_IV		0x0E			// ������� ��������� ��������� ����/���������� (� �.�.)
//#define CMD_GET_ADCS	0x0F			// ������� ���������� ��� ������ (��������� �����)

//�������� ����� �������� � ��������� �������
//#define FRAME_IN 			32	  		// ������������ ����� ���� ��������� ������ � ������ ����������
//#define FRAME_OUT 		32				// ������������ ����� ������������ �������� ���� ������	



void CMD_exec(void);
#endif
