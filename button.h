#ifndef __U_BUTTON
 #define __U_BUTTON

#include "common.h"

// Линия для кнопок
#define U_UP_BTN_PIN PORT_Pin_5
#define U_SEL_BTN_PIN PORT_Pin_2
#define U_DOWN_BTN_PIN PORT_Pin_1
#define U_LEFT_BTN_PIN PORT_Pin_3
#define U_RIGHT_BTN_PIN PORT_Pin_6
// Инициализировать кнопки
void U_BTN_Init (void);

// Получить состояние кнопки UP
uint8_t U_UP_Read_Button (void);

// Получить состояние кнопки SELECT
uint8_t U_SEL_Read_Button (void);

// Получить состояние кнопки DOWN
uint8_t U_DOWN_Read_Button (void);

// Получить состояние кнопки LEFT
uint8_t U_LEFT_Read_Button (void);

// Получить состояние кнопки RIGHT
uint8_t U_RIGHT_Read_Button (void);

#endif 

