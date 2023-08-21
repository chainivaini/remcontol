#include "can_.h"
#include "menu.h"
#include "common.h"
#include "hexto.h"
#include "math.h"
//#include "sdwrk.h"

CAN_RxMsgTypeDef Rxmsg;
uint8_t WRKMENU_Item;
 
CAN_TxMsgTypeDef tx_frame;
extern volatile RXMSG RXmsg;
extern int8_t U_ID_MENU_Item;
uint16_t caseUP_DOWN_RPM;
uint8_t caseAUTO;
U_CAN_Buffer_Def U_CAN_Buffer [U_CAN_RX_MESSAGE_COUNT + U_CAN_TX_MESSAGE_COUNT];

int32_t RPM;
int8_t ID_zn;
volatile CAN_ControlAM CAN_AM_menu;
volatile CAN_ControlSTB CAN_STB_menu;
volatile _ID_ IDM;


void CAN_Initial(void) 
{
		uint32_t i;
	U_ID_Select(0);
	U_Change_Packet(&CAN_STB_menu, STB);
	U_Change_Mode(&CAN_AM_menu, Auto);
	
	PORT_InitTypeDef PortInitStructure;
	CAN_InitTypeDef  CAN_InitStructure;
	
		
	RST_CLK_PCLKcmd(RST_CLK_PCLK_CAN1 | RST_CLK_PCLK_PORTA, ENABLE);
	
	//Конфигурация выводов CAN 
  PortInitStructure.PORT_PULL_UP = PORT_PULL_UP_OFF;
  PortInitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
  PortInitStructure.PORT_PD_SHM = PORT_PD_SHM_OFF;
  PortInitStructure.PORT_PD = PORT_PD_DRIVER;
  PortInitStructure.PORT_GFEN = PORT_GFEN_OFF;
  PortInitStructure.PORT_FUNC = PORT_FUNC_ALTER;
  PortInitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
  PortInitStructure.PORT_MODE = PORT_MODE_DIGITAL;

  // PA6 (CAN1_TX) 
  PortInitStructure.PORT_OE = PORT_OE_OUT;
  PortInitStructure.PORT_Pin = PORT_Pin_6;
  PORT_Init (MDR_PORTA, &PortInitStructure);
  
  // PA7 (CAN1_RX)
  PortInitStructure.PORT_OE = PORT_OE_IN;
  PortInitStructure.PORT_Pin = PORT_Pin_7;
  PORT_Init (MDR_PORTA, &PortInitStructure);
	
	// Настроить тактирование CAN1. Частота тактирования 5 МГц (80 МГц / 16)
	CAN_BRGInit (MDR_CAN1, CAN_HCLKdiv8); 
	
	
	CAN_DeInit(MDR_CAN1);
		
	CAN_StructInit (&CAN_InitStructure);
		
	CAN_InitStructure.CAN_ROP  = DISABLE;        // Запретить приём собственных пакетов
	CAN_InitStructure.CAN_SAP  = DISABLE;        // Разрешить отправку поля подтверждения ACK для собственных пакетов
  CAN_InitStructure.CAN_STM  = DISABLE;          // Выключить режим самотестирования (Self Test mode)  
  CAN_InitStructure.CAN_ROM  = DISABLE;          // Выключить режим только на приём (Read Only mode)
  CAN_InitStructure.CAN_PSEG = CAN_PSEG_Mul_7TQ; // Propagation Time Segment
  CAN_InitStructure.CAN_SEG1 = CAN_SEG1_Mul_8TQ; // Phase Segment 1 Time
  CAN_InitStructure.CAN_SEG2 = CAN_SEG2_Mul_4TQ; // Phase Segment 2 Time 
  CAN_InitStructure.CAN_SJW  = CAN_SJW_Mul_3TQ;  // Максимальный шаг автоподстройки семплирования (CAN Synchronizations jump width time - SJW). 
	                                               // В пределах указанного количества TQ могут автоматически подстраиваться SEG1 и SEG2 для достижения заданной скорости передачи.
																								 // Во избежании остановки CAN из-за помех (см. Errata 0017) надо, чтобы SJW < SEG2
  CAN_InitStructure.CAN_SB   = CAN_SB_1_SAMPLE;  // Режим семплирования. Должно наблюдаться 3 выборки.
  CAN_InitStructure.CAN_BRP  = 9;                // Программируемый предделитель BRP. Коэффициент деления 1 + 1 = 2.
	
	//CAN_InitStructure.CAN_OVER_ERROR_MAX = 10;
  CAN_Init (MDR_CAN1, &CAN_InitStructure);
	
	//Настроить шаблоны отправляемых сообщений
	U_CAN_TX_Messages_Init();
  // Разрешить работу CAN1
  CAN_Cmd (MDR_CAN1, ENABLE);

  // Разрешить аппаратные прерывания от CAN1
  NVIC_EnableIRQ (CAN1_IRQn);

  // Разрешить прерывания от CAN1 по GLB_INT и RX_INT
  CAN_ITConfig (MDR_CAN1, CAN_IT_GLBINTEN | CAN_IT_TXINTEN, ENABLE);

  // Разрешить прерывания CAN1 по приему для всех определенных буферов 
	CAN_TxITConfig (MDR_CAN1, 1 << 0, ENABLE);
	
	for (i = 0; i < U_CAN_RX_MESSAGE_COUNT; i++){
    CAN_RxITConfig (MDR_CAN1, 1 << 1, ENABLE);
		
	}
	MDR_CAN1->RXDLC = 8 << 0;
	
	volatile RXMSG RXmsg;
		
	RXmsg.Flag_ = 0;
	RXmsg.Data0 = 0;

}

//Формирование пакетов отпракляемых сообщений 
 void U_CAN_TX_Messages_Init ()
{  
	volatile TVariant32 data;
 volatile RXMSG RXmsg;

	switch(CAN_STB_menu.State)
 {
	 case STB:
	{  
		tx_frame.ID = ID_zn<<18;
		tx_frame.PRIOR_0 = 1; 
		tx_frame.IDE = CAN_ID_STD; 
		tx_frame.DLC = 8; 
		tx_frame.Data[0] = 0x00000000; 
		tx_frame.Data[1] = 0x00000000;
		break;
	}
	 case WRK1:
  {
		tx_frame.ID = ID_zn<<18;
		tx_frame.PRIOR_0 = 1; 
		tx_frame.IDE = CAN_ID_STD; 
		tx_frame.DLC = 8; 
		tx_frame.Data[0] = 0x00000080; 
		tx_frame.Data[1] = U_RXdata_inverse(RPM); 
		break;
		 
  }
 case WRK2:
  {
		tx_frame.ID = ID_zn<<18;
		tx_frame.PRIOR_0 = 1; 
		tx_frame.IDE = CAN_ID_STD; 
		tx_frame.DLC = 8; 
		tx_frame.Data[0] = 0x00000080; 
		tx_frame.Data[1] = U_RXdata_inverse(RPM);
		
		break;
   }
	}
 CAN_Transmit(MDR_CAN1, TX_BUFFER_NUMBER, &tx_frame);
}


void U_CAN_Init_OS (void)
{
	uint32_t i;
	
  // Разрешить прием для всех определенных для приема буферов
	for (i = 0; i < U_CAN_RX_MESSAGE_COUNT; i++)
    CAN_Receive (MDR_CAN1, i, ENABLE);
	
  // Сконфигурировать буферы для передачи
	for (i = U_CAN_RX_MESSAGE_COUNT; i < U_CAN_RX_MESSAGE_COUNT + U_CAN_TX_MESSAGE_COUNT; i++)
	{
		MDR_CAN1->BUF_CON[i] = CAN_BUF_CON_EN & ~CAN_BUF_CON_RX_TXN;		
	}
	
}

// Функция отпраления сообщения по каналу CAN
 void U_CAN_TX_Task_Function (void)
{
	
	volatile TVariant32 currentData;    // Текущие обрабатываемые данные
	
	U_CAN_TX_Messages_Init ();
	MDR_CAN1->CAN_BUF[1].ID=ID_zn<<18;
	MDR_CAN1->TXID = ID_zn<<18;
	CAN_Transmit(MDR_CAN1, TX_BUFFER_NUMBER, &tx_frame);

}				
				
// Функция полученяи 
void U_CAN_RX_Task_Function1 (void ){
	/// ОШибка в неправильности задания буфера обмена  на принятие сообщения
	// Структура для приема кадра 
	// Структура для приема кадра 
	CAN_RxMsgTypeDef rx_frame;
	volatile RXMSG RXmsg;
	char mssg[128];
	
	// Извлечение данных из буфера 
	CAN_GetRawReceivedData(MDR_CAN1, 0, &rx_frame);
	
	RXmsg.Flag_ = MDR_CAN1->RXDATAL;
	RXmsg.Data0 = U_RXdata_inverse(MDR_CAN1->RXDATAH);
	sprintf(mssg, "RPM_RX: %0i",RXtoRPM(U_RXdata_inverse(MDR_CAN1->RXDATAH)));
	U_MLT_Put_String(mssg, 4);
	sprintf(mssg, "%s",dec2bin((MDR_CAN1->RXDATAL)));
	U_MLT_Put_String(mssg, 5);
	sprintf(mssg,"T: %0i C",U_TEMPdata_inverse(MDR_CAN1->RXDATAL));
	U_MLT_Put_String(mssg, 6);
	
	
}


void U_SAVE_DATA (Savelog *save)
{
	switch(save->StateNEW)
	{
		case ON:
		
		break;
		
		case OFF:
			
		break;
	}
}
/*
void sdwork()
{
		// Initialize the M0: drive.
  fs = finit ("M0:");
	
  // Mount the M0: drive.
  fs = fmount ("M0:");
  
  fs = fvol ("M0:", label, &ser_num);
	
	FreeSize = ffree ("M0:");
	
	fs = fpwd ("M0:", StrBuf, StrBufSize);
	
	fs = fmkdir (DirName);
	
  fout = fopen (FileName, "w");
	
  if (fout != NULL)
	{
    // Write data to file
		fprintf (fout, "Площадь круга радиуса %i равна %f.\n", Radius, 3.1415926*Radius*Radius);
    fclose (fout);
	}
	
	fin = fopen (FileName, "r"); 
	
  if (fin != NULL)
	{
    // Read data from file
		fscanf (fin, "Площадь круга радиуса %i равна %f.\n", &CircleRadius, &CircleArea);
    fclose (fin);
	}
	
	// The drive is no more needed.
	// Unmount the M0: drive.
  fs = funmount ("M0:");
	
	// Uninitialize the M0: drive.
  fs = funinit ("M0:");
	
}

*/