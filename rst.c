#include <stdio.h>
#include "common.h"

//  Свеодиоды для индикации статуса
#define LED_OK       BRD_LED_1
#define LED_FAULT    BRD_LED_2
#define LED_CYCLE    BRD_LED_3

#define LED_SEL_ALL   BRD_LED_1 | BRD_LED_2 | BRD_LED_3

//  Настройки тактирования.
//  Системынй таймер с периодом 1мс используется для отработки таймаутов при работе с картой.
//  Таймер запускается в brdSDIO.c в функции BRD_SDIO_Init(), 
//  которая вызывается при инициализации SD карты - BRD_SD_Init()
#define PLL_MULL      RST_CLK_CPU_PLLmul10
#define PERIOD_1MS    (HSE_Value * (PLL_MULL + 1) / 1000)

uint32_t TmrDelay_ms = 1000;

//  Тесты в цикле - Активным может быть только один
void While_TestFS(void);
void While_SDIO_TestCMD(void);
void While_SDIO_TestCMD_8Clk(void);
void While_SDIO_TestCycles(void);
void While_SDIO_TestRate(void);

void U_RST_Init(void)
{  
	
// Включить тактирование батарейного блока и внутренние генераторы, все остальное сбросить
  RST_CLK_DeInit(); 
	
  // Включить генератор на внешнем кварце
  RST_CLK_HSEconfig (RST_CLK_HSE_ON); 
  while (RST_CLK_HSEstatus() != SUCCESS);

  // Настроить источник и КУ PLL (CPU_C1_SEL = HSE)
  RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10); 
 
  // Включить PLL, но еще не подключать к кристаллу
  RST_CLK_CPU_PLLcmd(ENABLE); 
  while (RST_CLK_CPU_PLLstatus() != SUCCESS);

  // Делитель С3( CPU_C3_SEL = CPU_C2_SEL )
  RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1); 
  
  // На С2 идет с PLL, а не напрямую с С1 (CPU_C2_SEL = PLL)
  RST_CLK_CPU_PLLuse(ENABLE); 
  
  // CPU берет с выхода С3 (а может с выхода HSI,LSI,LSE) (HCLK_SEL = CPU_C3_SEL )
  RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3); 

  // Разрешить тактирование требуемых периферийных устройств
  RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTF, ENABLE);
	
}
