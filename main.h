// ***********************************************************************************
// Проект:      Lab2_2 
// Программа:   Работа с кнопкой 
// Микроконтроллер: 1986ВЕ92У
// Устройство: Отладочная плата К1986ВЕ92QI
// Файл: main.h 
// Назначение: Главный модуль  
// Компилятор:  Armcc 5.03.0.76 из комплекта Keil uVision 4.72.1.0 
// ***********************************************************************************

#ifndef __MAIN_H
  #define __MAIN_H

#include "common.h"

#include "rst.h"
#include "menu.h"
#include "can_.h"
#include "machine.h"

// Инициализация задач RTX
void Main_Task_Init (void);

char* U_AMRX_MENU_Prepare_Item(uint8_t RPMstate);

void print_speed_Screen(void);
 
 
char* U_SWRK_MENU_Prepare_Item(uint8_t state, int16_t rpm);
 
#endif 

