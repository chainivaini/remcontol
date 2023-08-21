// ***********************************************************************************
// Микроконтроллер: 1986ВЕ92У
// Устройство: Отладочная плата К1986ВЕ92QI
// Файл: machine.с 
// Назначение: Конечный автомат 
// Компилятор:  Armcc 5.03.0.76 из комплекта Keil uVision 4.72.1.0 
// ***********************************************************************************
#include "machine.h"
#include "can_.h"

// Инициализация конечного автомата. 
void U_Machine_Init(TMachine *machine,      // Конечный автомат
	                  int8_t start_State      // Начальное состояние
                   )
{
  machine->State = start_State; 
  machine->Previous = 0;	    	      // Предыдущее состояние не определено
  machine->Next = 0;			            // Следующее состояние не определено, т.е. таймер не установлен
  machine->Counter = 0;								// Обнулить внутренний таймер-счетчик		
}

// Смена состояния конечного автомата
void U_Machine_Change_State(TMachine *machine,	     // Конечный автомат
	                          int8_t newState          // Новое состояние
   	                       )
{ 
  if (machine->State != newState) 
  {
    machine->Previous = machine->State;
    machine->State = newState;
    machine->Next = 0;  			
    machine->Counter = 0;
  }
}   	

// Остаться в том же состоянии 
void U_Machine_Stay_Here(TMachine *machine)
{
  machine->Previous = machine->State;
  machine->Next = 0;  			
  machine->Counter = 0;
}

// Проверить, пришли из другого состояния? 
int8_t U_Machine_Come_From_Another(TMachine *machine)
{
  return machine->State != machine->Previous;
}

// Установить таймер 
void U_Machine_Set_Timer(TMachine *machine,			// Конечный автомат
												 uint32_t Period, 			// Период ожидания, проверок таймера
												 int8_t   State				  // Состояние, в которое перейдет автомат по срабатыванию таймера 
												)
{
	machine->Next = State;
  machine->Counter = Period;
}

// Сбросить таймер 
void U_Machine_Reset_Timer(TMachine *machine)
{
	machine->Next = 0;
  machine->Counter = 0;
}

// Проверить таймер 
void U_Machine_Test_Timer(TMachine *machine)
{
	// Если установлен таймер
	if (machine->Next != 0)
	{
		// Если таймер сработал
  	if (machine->Counter <= 1)
		{
			// Переводим автомат в требуемое состояние
			machine->State = machine->Next;
			// Сбрасываем таймер
    	machine->Next = 0;
      machine->Counter = 0;
		}
		else
			// Считаем дальше
			machine->Counter --;
  }
		
}

void Delay(int waitTicks)
{
int i;
	 for (i = 0; i <waitTicks; i++)
	{
		U_CAN_TX_Task_Function ();
		U_CAN_RX_Task_Function1();
	}
}

void U_Change_Mode(CAN_ControlAM *mode, int8_t newMode){
	mode->Previous = mode->State;
	mode->State = newMode;
}

void U_Change_Packet(CAN_ControlSTB *packet,int8_t newST ){
	packet->State = newST;
}

void U_SVLG_Change(Savelog *save,int8_t newstate)
{
	save->StateNEW = newstate;
	
}

void U_Button_Pointer(Pressed_Button *button, int8_t newButton){
	button->Button = newButton;
}

/*******************************************************************************
Преобразовывает переменную UINT16 в строку
*******************************************************************************/
