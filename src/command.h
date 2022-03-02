/*
Модуль реализации выполнения команд
*/
#ifndef COMMAND_H_
#define COMMAND_H_

//Коды специальных команд приложения
#define CMD_LINK_ECHO			0x0A			// Команда проверки связи
#define CMD_LED_ON_OFF			0x0B			// Debug command

// -----------------------------------------------------------------------
#define CMD_STAMP_POL			0x0C			// Step Amp polarity select
#define CMD_STAMP_MODE_RANGE	0x0D			// Step Amp mode (current/voltage)
																				// and range select

#define CMD_STAMP_SET_OUT		0x0E			// Step Amp set out signal


//#define CMD_SET_GAIN	0x0B			// Команда установки коэффициента усиления PGA1, PGA2
//#define CMD_SEL_R			0x0C			// Команда управления реле R_SEL
//#define CMD_SEL_IV		0x0D			// Команда управления реле IV_SEL

//#define CMD_SET_IV		0x0E			// Команда установки выходного тока/напряжения (в о.е.)
//#define CMD_GET_ADCS	0x0F			// Команда считывания АЦП канала (статичный режим)

//Значение длины входящих и исходящих пакетов
//#define FRAME_IN 			32	  		// Максимальное число байт входящего пакета в данной реализации
//#define FRAME_OUT 		32				// Максимальное число передаваемых полезных байт данных	



void CMD_exec(void);
#endif
