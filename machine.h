// ***********************************************************************************
// Микроконтроллер: 1986ВЕ92У
// Устройство: Отладочная плата К1986ВЕ92QI
// Файл: machine.h 
// Назначение: Конечный автомат 
// Компилятор:  Armcc 5.03.0.76 из комплекта Keil uVision 4.72.1.0 
// ***********************************************************************************

#ifndef __U_MACHINE
 #define __U_MACHINE

#include "common.h"
#include <stdint.h>
#include "can_.h"
#include <string.h>
#include <ctype.h>
#include <math.h>

// Структура конечного автомата 
typedef volatile struct
{       
  // Текущее состояние
	// Состояние 0 считается неопределенным. Его использовать нельзя.
  int8_t State;	 
  // Предыдущее состояние
  int8_t Previous;
  // Следующее состояние
  int8_t Next;
  // Таймер-счетчик
	uint32_t Counter;
	
} TMachine;

typedef volatile struct 
{
	int32_t  Data0;
	uint32_t Data1;
	uint8_t Flag_;
	uint8_t IDRX;
	
	} RXMSG;

typedef volatile struct{
		int8_t Button; 
}Pressed_Button;  


// Инициализация конечного автомата. 
void U_Machine_Init(TMachine *machine,      // Конечный автомат
	                  int8_t start_State      // Начальное состояние
                   );

// Смена состояния конечного автомата
void U_Machine_Change_State(TMachine *machine,	// Конечный автомат 
	                          int8_t newState     // Новое состояние
   	                       );

// Остаться в том же состоянии 
void U_Machine_Stay_Here(TMachine *machine);

// Проверить, пришли из другого состояния? 
int8_t U_Machine_Come_From_Another(TMachine *machine);

// Установить таймер 
void U_Machine_Set_Timer(TMachine *machine,			// Конечный автомат
												 uint32_t Period, 			// Период ожидания, проверок таймера
												 int8_t   State				  // Состояние, в которое перейдет автомат по срабатыванию таймера 
											  );

// Сбросить таймер 
void U_Machine_Reset_Timer(TMachine *machine);

// Проверить таймер 
void U_Machine_Test_Timer(TMachine *machine);

void U_Change_Mode(CAN_ControlAM *mode, int8_t newMode);
void U_Change_Packet(CAN_ControlSTB *packet,int8_t newST );
void U_Button_Pointer(Pressed_Button *button, int8_t newButton);
void U_SVLG_Change(Savelog *save,int8_t newstate);
int U_ID_State(uint8_t itemID);

void Delay(int waitTicks);

#endif

