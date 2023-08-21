#include "button.h"

// Инициализировать кнопки
void U_BTN_Init (void)
{
	// Структура для инициализации портов	
	PORT_InitTypeDef PortInitStructure;
	// Разрешить тактирование порта B
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTB | RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTD, ENABLE);

  // Обозначаем кнопку UP
	PORT_StructInit (&PortInitStructure);
	PortInitStructure.PORT_Pin   = U_UP_BTN_PIN;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (MDR_PORTB, &PortInitStructure);
	
	// Обозначаем кнопку SELECT
	PortInitStructure.PORT_Pin   = U_SEL_BTN_PIN;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (MDR_PORTC, &PortInitStructure);
	
	// Обозначаем кнопку DOWN
	PortInitStructure.PORT_Pin   = U_DOWN_BTN_PIN;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (MDR_PORTE, &PortInitStructure);
	
	// Обозначаем кнопку LEFT
	PortInitStructure.PORT_Pin   = U_LEFT_BTN_PIN;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (MDR_PORTE, &PortInitStructure);
	
	// Обозначаем кнопку RIGHT 
	PortInitStructure.PORT_Pin   = U_RIGHT_BTN_PIN;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (MDR_PORTB, &PortInitStructure);

}

// *********************************************************************UP
uint8_t U_UP_Read_Button (void)
{
	// Прочитать состояние входа, к которому подключена кнопка
	if(PORT_ReadInputDataBit ( MDR_PORTB, U_UP_BTN_PIN))
	  // Кнопка отпущена
	  return 0;
	else
	  // Кнопка нажата
		return 1;
}

// *********************************************************************SELECT 
uint8_t U_SEL_Read_Button (void)
{
	// Прочитать состояние входа, к которому подключена кнопка
	if(PORT_ReadInputDataBit ( MDR_PORTC, U_SEL_BTN_PIN))
	  // Кнопка отпущена
	  return 0;
	else
	  // Кнопка нажата
		return 1;
}

// *********************************************************************DOWN
uint8_t U_DOWN_Read_Button (void)
{
	// Прочитать состояние входа, к которому подключена кнопка
	if(PORT_ReadInputDataBit ( MDR_PORTE, U_DOWN_BTN_PIN))
	  // Кнопка отпущена
	  return 0;
	else
	  // Кнопка нажата
		return 1;
}

// ************************************************************************LEFT
uint8_t U_LEFT_Read_Button (void)
{
	// Прочитать состояние входа, к которому подключена кнопка
	if(PORT_ReadInputDataBit ( MDR_PORTE, U_LEFT_BTN_PIN))
	  // Кнопка отпущена
	  return 0;
	else
	  // Кнопка нажата
		return 1;
}

// ************************************************************************RIGHT
uint8_t U_RIGHT_Read_Button (void)
{
	// Прочитать состояние входа, к которому подключена кнопка
	if(PORT_ReadInputDataBit ( MDR_PORTB, U_RIGHT_BTN_PIN))
	  // Кнопка отпущена
	  return 0;
	else
	  // Кнопка нажата
		return 1;
}




