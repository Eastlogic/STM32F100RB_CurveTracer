/*
Описание протокола передачи данных WAKE
*/

#ifndef WAKE16_H_
#define WAKE16_H_


#define FEND  			0xC0	//Frame END
#define FESC  			0xDB	//Frame ESCape
#define TFEND 			0xDC	//Transposed Frame END
#define TFESC 			0xDD	//Transposed Frame ESCape

#define CRC_INIT 		0xDE	//Initial CRC value
#define FRAME_IN 		32    //Максимальное число байт входящего пакета в данной реализации
#define FRAME_OUT 	256	  //Максимальное число передаваемых полезных байт данных								

//Коды универсальных команд
#define CMD_NOP     0			//Нет операции
#define CMD_ERR     1			//Ошибка приема пакета

//Коды ошибок:
#define ERR_NO 			0x00  //No error
#define ERR_TX 			0x01  //Rx/Tx error
#define ERR_PA 			0x04  //Parameters value error


//RX process states:
enum { WAIT_FEND,     // Ожидание приема FEND
       WAIT_CMD,      // Ожидание приема команды
       WAIT_NH,       // Ожидание приема количества байт в пакете
			 WAIT_NL,		  	// [NH:NL] = 0...65535
       WAIT_DATA,     // Прием данных
       WAIT_CRC       // Ожидание конца приема CRC
       };







// 	Вычисление контрольной суммы байта данных
void CRC_8(char b, char *crc);

// Вычисление контрольной суммы массива данных
char CRC_8_Arr(char *arr, unsigned int length);

// Инициализация автомата приема входного пакета данных
void Receive_init(void);

// Преобразование "сырого" массива в выходной массив формата протокола WAKE
unsigned int Array_to_wake(char *Init_ar, unsigned int Init_length, char *Res_ar);

#endif


