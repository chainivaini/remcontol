#include "button.h"
#include <stdbool.h>
#include "menu.h"
#include  "common.h"
#include "machine.h"
#include "myenum.h"
#include "MDR32F9Qx_can.h"             


#ifndef _U_CAN_
	#define _U_CAN_

#define RX_BUFFER_NUMBER 0
#define TX_BUFFER_NUMBER 1								

#define U_WRKstate_ITEM_COUNT 3
#define U_RPMstate_ITEM_COUNT 2
#define WRKMENU_ITEM_COUNT 3 

// **** Отправляемые сообщения ****
// INFO
#define U_CAN_TX_NUMBER_INFO      2                 
#define U_CAN_TX_DLC_INFO         8               

#define U_CAN_RX_NUMBER_INFO         0              
#define U_CAN_RX_ID_MASK_INFO        (0x7FF)<<18  
#define U_CAN_RX_ID_FILTER_INFO      (0x7E8)<<18  
#define U_CAN_RX_DLC_INFO            8            

// Количество видов принимаемых сообщений
#define U_CAN_RX_MESSAGE_COUNT  1

// Количество видов отправляемых сообщений
#define U_CAN_TX_MESSAGE_COUNT  1

extern int8_t ID_MENU_STATE ;
extern int32_t RPM;
typedef volatile struct{
	int8_t State;
	int8_t Previous;
	
} CAN_ControlAM;

typedef volatile struct
{
	int8_t StateNEW;
	int8_t StateOLD;
	int16_t ID;
	int32_t RPM;
	int32_t Temperature;
	char* Flags;
}Savelog;

typedef volatile struct{
	int8_t State;
	int8_t Previous;
		
} CAN_ControlSTB;

typedef volatile struct{	int8_t RX;

}_ID_;
// Объединение для организации буфера приема / передачи
typedef union 
{
  CAN_RxMsgTypeDef RX;  // Буфер для приема
  CAN_TxMsgTypeDef TX;  // Буфер для передачи
	
} U_CAN_Buffer_Def;



// Буфер для приема и отправки сообщений 
extern U_CAN_Buffer_Def U_CAN_Buffer [U_CAN_RX_MESSAGE_COUNT + U_CAN_TX_MESSAGE_COUNT];

// Фильтры принимаемых сообщений 
extern CAN_FilterInitTypeDef U_CAN_RX_Filter [U_CAN_RX_MESSAGE_COUNT];

// Инициализация CAN 
void CAN_Initial(void);

void AUTO_RPM(void);
// Настройка отправляемых сообщений
void U_CAN_TX_Messages_Init();

int U_ID_(int8_t idcount);

void U_CAN_Init_OS (void);

void U_CAN_RX_Task_Function (void );

void U_CAN_TX_Task_Function (void);

void U_CAN_RX_Task_Function1 (void);

void U_CAN_Read_Data (MDR_CAN_TypeDef* CANx, uint32_t BufferNumber, CAN_RxMsgTypeDef* RxMessage);

#endif

