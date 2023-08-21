// ***********************************************************************************
// Проект:      Lab2_2 
// Программа:   Работа с кнопкой 
// Микроконтроллер: 1986ВЕ92У
// Устройство: Отладочная плата К1986ВЕ92QI
// Файл: menu.h 
// Назначение: Формирование меню      
// Компилятор:  Armcc 5.03.0.76 из комплекта Keil uVision 4.72.1.0 
// ***********************************************************************************

#ifndef __U_MENU
 #define __U_MENU
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "myenum.h"
#include <stdio.h>
#include "common.h"
#include "button.h"
#include "MLT/mlt_lcd.h"
#include "machine.h"
#include "rst.h"
#include "can_.h"
#include "main.h"


// Количество пунктов меню
#define U_MENU_ITEM_COUNT 2

#define U_ID_MENU_ITEM_COUNT 12
extern uint8_t U_MENU_Item; 
extern int8_t ID_zn;
extern int8_t U_ID_MENU_Item;
// Возможные состяния автомата

enum
{
  U_MENU_SM_BUTTON_TEST_PRESS = 1, // Состояние "Ожидание нажатия кнопки"
  U_MENU_SM_BUTTON_PRESS_PAUSE,    // Состояние "Пауза для защиты от дребезга контактов и случайных нажатий"
  U_MENU_SM_BUTTON_PRESS_CONFIRM,  // Состояние "Подтверждение нажатия кнопки"
  U_MENU_SM_BUTTON_TEST_UNPRESS    // Состояние "Ожидание отпускания кнопки"
};

// Инициализировать меню
	void U_MENU_Init (void);

// Задача по работе с меню
	void U_MENU_Task_Function (void const *argument);

// Задача по выводу бегущей строки на ЖКИ
	void U_MENU_Running_String_Task_Function (void);

// Выбираем нужное нам ID передачи 
	char* U_ID_MENU_Prepare_Item(uint8_t itemID);

	char* U_SWRK_MENU_Prepare_Item(uint8_t state, int16_t rpm);

// ЗАдача по выбору бегущей строки на ЖКИ в меню выбора ID 
	void U_ID_MENU_Task_Function (void const *arg); 

//
	void U_CAN_MENU_Task_Function(void);

// 
extern void U_CAN_MAIN_MENU(void );

	uint8_t U_AMRX_Item(uint8_t RPMstate);
	
//Устанавливаем тактирование С внешнего генератора 16мц
	void HSE_INT(void);

	int speed_tr(int rpm);

	void AUTO_RPM(void);
	
	char* U_ID_Select(int8_t idcount);
		
	void print_screen();
	void print_speed_screen(void);
	void print_id_Screen(int8_t ID);
		
#endif 


