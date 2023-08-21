#include  "menu.h"
#include "common.h"
#include "can_.h"
#include "MDR32F9Qx_can.h"              // Keil::Drivers:CAN
#include "hexto.h"
// Машина состояний для реализации меню
TMachine U_MENU_StateMachine;
// МАшина состоний для реализации режимов
extern volatile CAN_ControlAM CAN_AM_menu;
extern volatile CAN_ControlSTB CAN_STB_menu;
extern volatile _ID_ IDM;
volatile Pressed_Button Pressed_B;
const char* mssg[128];
static char mssg1[128];
uint8_t U_SWRK_MENU_Item;
uint8_t U_RPM_MENU_Item;
const char* charWRKstate; 
const char* charRPMstate;
const char* charIDstate;
int8_t U_ID_MENU_Item;
volatile TVariant32 neededData;
volatile CAN_RxMsgTypeDef rx_frame;
// Выбранный пункт меню
uint8_t U_MENU_Item; 
//fsStatus fs;

char label[12];
uint32_t ser_num;

int64_t FreeSize;

#define StrBufSize 200
char StrBuf[StrBufSize];

#define DirName "M0:\\Dir"
#define FileName "FileDir\\File.txt"


int Radius = 10;
int CircleRadius;
float CircleArea;

uint8_t U_ID_MENU_MAX;
uint8_t CAN_AMState;
volatile int8_t Button_Pointer;
volatile int8_t Work_state;
volatile int8_t Performane_state;  

// Строка для представления времени
uint8_t U_MENU_Time_String[8];  

// Указатель на сообщение, выводимое в качестве пункта меню
char* U_MENU_Message;
char* U_ID_MENU_Message;

// Подготовить строку для выбранного пункта главного меню
char* U_MENU_Prepare_Item(uint8_t item); 

// Подготовить строку для меню настройки ID 
char* U_ID_MENU_Prepare_Item(uint8_t item); 

// Инициализировать меню
void U_MENU_Init (void)
{
  // Инициализация конечного автомата 
  U_Machine_Init(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_PRESS);
	

	// Выбрать начальный пункт меню
	U_MENU_Item = 0;
	U_ID_MENU_Item=0;
	CAN_AMState = 0x10;
	U_ID_MENU_MAX = 3;
}
	

 void U_CAN_MAIN_MENU(void )
	{
U_AMRX_MENU_Prepare_Item(0);

	print_id_Screen(ID_zn);
	print_speed_screen();
	U_Machine_Test_Timer(&U_MENU_StateMachine);
	while(1){
		
	U_CAN_TX_Messages_Init();
	U_CAN_TX_Task_Function();
	U_CAN_RX_Task_Function1();
	print_speed_screen();
	print_id_Screen(ID_zn);
    // Проверить таймер 
    U_Machine_Test_Timer(&U_MENU_StateMachine);
		
		// Проверка состояния конечного автомата
		switch (U_MENU_StateMachine.State)
		{
/********************************************************************************************************/
			// Состояние "Ожидание нажатия кнопки"
			case U_MENU_SM_BUTTON_TEST_PRESS:
				
			// Блок вывода данных на экран 
  			if (U_Machine_Come_From_Another(&U_MENU_StateMachine)) U_Machine_Stay_Here(&U_MENU_StateMachine);
				charRPMstate = U_AMRX_MENU_Prepare_Item(U_RPM_MENU_Item);
				if (charRPMstate) U_MLT_Put_String (charRPMstate, 1);
				charWRKstate = U_SWRK_MENU_Prepare_Item(U_SWRK_MENU_Item,RPM);
				if (charWRKstate) U_MLT_Put_String (charWRKstate, 2);
				charIDstate = U_ID_Select(U_ID_MENU_Item);
				if (charIDstate) U_MLT_Put_String (charIDstate, 7);
			
			// Блок сканирования кнопок 
				 if ( U_RIGHT_Read_Button()){ U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_PRESS_PAUSE); Button_Pointer = 0x001;}
				 if ( U_SEL_Read_Button()) {U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_PRESS_PAUSE);Button_Pointer = 0x010;}
				 if (U_UP_Read_Button()){U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_PRESS_PAUSE);Button_Pointer = 0x011;}
				 if (U_DOWN_Read_Button()){U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_PRESS_PAUSE);Button_Pointer = 0x000;}
				 if( U_LEFT_Read_Button()){U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_PRESS_PAUSE);Button_Pointer = 0x000;}
				 if(U_LEFT_Read_Button()&U_RIGHT_Read_Button()){U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_PRESS_PAUSE); }
			break;
				
/******************************************************************************************************/
			// Состояние "Пауза для защиты от дребезга контактов кнопки"
			case U_MENU_SM_BUTTON_PRESS_PAUSE:
				if (U_Machine_Come_From_Another(&U_MENU_StateMachine)){ U_Machine_Stay_Here(&U_MENU_StateMachine);
				U_Machine_Set_Timer(&U_MENU_StateMachine,	30, U_MENU_SM_BUTTON_PRESS_CONFIRM);}
					
			break;
/*********************************************************************************************************/
			// Состояние "Подтверждение нажатия кнопки"
			case U_MENU_SM_BUTTON_PRESS_CONFIRM:
				if (U_Machine_Come_From_Another(&U_MENU_StateMachine))  U_Machine_Stay_Here(&U_MENU_StateMachine);
				
			//Проверка нажатия правой кнопки 
				 if (U_RIGHT_Read_Button())
				{
					U_Button_Pointer(&Pressed_B,Right);
 				 // Перейти к следующему пункту меню, а если оставить еще ли прошли все, то к начальному
			   if (++ U_RPM_MENU_Item == U_RPMstate_ITEM_COUNT) U_RPM_MENU_Item = 0;			
          U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_UNPRESS);
				
				}	
			//Проверка нажатия правой кнопки 				
				if(U_LEFT_Read_Button())
				{
					U_Button_Pointer(&Pressed_B,Left);
					if(++U_ID_MENU_Item == U_ID_MENU_MAX ) U_ID_MENU_Item=0;
					U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_UNPRESS);
				}
				//Проверка нажатия кнопки select
				 if (U_SEL_Read_Button())
				{
					U_Button_Pointer(&Pressed_B,Select);
 				 // Перейти к следующему пункту меню, а если прошли все, то к начальному
			  if (++ U_SWRK_MENU_Item == WRKMENU_ITEM_COUNT) U_SWRK_MENU_Item = 0;			
          U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_UNPRESS);
				}	
			//Проверка нажатия вверх кнопки	
	if (U_UP_Read_Button())
				{
					U_Button_Pointer(&Pressed_B,Up);
					
					switch(CAN_AM_menu.State){
						case Auto: 
							switch(CAN_STB_menu.State ){
								case STB: 
									
								break;
								case WRK1:
									AUTO_RPM();
								break;
								case WRK2:
								break;
							}
						break;
						
						case Manual:
									
							switch(CAN_STB_menu.State){
								case STB:
									//Ничего не происходит, поскольку STB
								break;
								case WRK1:
									RPM = RPM + 0x100000;
									if (RXtoRPM(RPM)  >= 1300 ) RPM = 0x014C0000;
									print_speed_screen();
									U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_UNPRESS);
								break;
								case WRK2:
									if (RXtoRPM(RPM)  >= 1300 ) RPM = 0x014C0000;
									print_speed_screen();
									U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_UNPRESS);									
								break;
							}
						break;
					}
					
				}
			
				if (U_DOWN_Read_Button())
				{
					U_Button_Pointer(&Pressed_B,Down);
					
					switch(CAN_AM_menu.State){
						case Auto: 
							switch(CAN_STB_menu.State ){
								case STB: 
									
								break;
								case WRK1:
									AUTO_RPM();
								break;
								case WRK2:
								break;
							}
						break;
						
						case Manual:
									
							switch(CAN_STB_menu.State){
								case STB:
									//Ничего не происходит, поскольку STB
								break;
								case WRK1:
									RPM = RPM-0x100000;
									if (RXtoRPM(RPM)  <= -1300 ) RPM = 0xFEB40000 ;
									print_speed_screen();
									U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_UNPRESS);
								break;
								case WRK2:
									if (RXtoRPM(RPM)  <= -1300 ) RPM = 0xFEB40000 ;
									print_speed_screen();
									U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_UNPRESS);									
								break;
							}
						break;
					}
					
				}
				if(U_LEFT_Read_Button()&U_RIGHT_Read_Button())
				{
					
				}
								
				// Если нажатие кнопки НЕ подтвердилось сменить состояние конечного автомата
				else U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_PRESS);
				break;
	/*********************************************************************************************************/		
			// Состояние "Ждём отпускания кнопки"
			case U_MENU_SM_BUTTON_TEST_UNPRESS:
			
  			// Если пришли из другого состяния											// Остаться в том же состоянии 
				if (U_Machine_Come_From_Another(&U_MENU_StateMachine))  U_Machine_Stay_Here(&U_MENU_StateMachine);
	
				switch(Pressed_B.Button)
				{							
					case Select:
						if (!U_SEL_Read_Button())	U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_PRESS);
				  break;
					
					case Left:
						if(!U_LEFT_Read_Button())U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_PRESS);
					break;
						
					case Right:
						if (!U_RIGHT_Read_Button())	U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_PRESS);
					break;
						case Down:
						if (!U_DOWN_Read_Button())	U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_PRESS);
					break;
				
					case Up:
						if (!U_UP_Read_Button())U_Machine_Change_State(&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_PRESS);
					break;
				}
			break;
			default:
				U_Machine_Init (&U_MENU_StateMachine, U_MENU_SM_BUTTON_TEST_PRESS);
		}
   }

}



char* U_SWRK_MENU_Prepare_Item(uint8_t state, int16_t rpm)
	{
		switch(state)
		{
			case 0:		
				U_Change_Packet(&CAN_STB_menu,STB);
				return "STD";
			case 1:	
				U_Change_Packet(&CAN_STB_menu,WRK1);
				return "WRK1";
			case 2:
				U_Change_Packet(&CAN_STB_menu,WRK2);
				return "WRK2";
			default:
				return 0;
		} 
	}
	
char* U_AMRX_MENU_Prepare_Item(uint8_t RPMstate)
	{
		switch(RPMstate)
		{
			case 0:
				U_Change_Mode(&CAN_AM_menu, Auto);
 				return "Auto";
			case 1:		
				U_Change_Mode(&CAN_AM_menu, Manual);
				return "Manual";
			default:
				return 0;}
	}

	void print_speed_screen(void)
{
	 sprintf(mssg, "RPM_TX: %0i",RXtoRPM(RPM));
		U_MLT_Put_String(mssg, 3);
}
	void print_id_Screen(int8_t ID)
	{
		sprintf(mssg1, "%s",U_ID_Select(ID));
		U_MLT_Put_String(mssg1, 7);
	}

	void AUTO_RPM(void){
	switch(Pressed_B.Button){
		case Up:
			while(RXtoRPM(RPM)<1300){
				print_speed_screen();
				Delay(200);
				RPM = RPM + 0x00100000;
				U_CAN_TX_Messages_Init();
				U_CAN_RX_Task_Function1();
				U_CAN_TX_Task_Function();
				
			}
			while(RXtoRPM(RPM) > 0){
				print_speed_screen();
				Delay(200);
				RPM = RPM - 0x00100000;
				U_CAN_TX_Messages_Init();
				U_CAN_RX_Task_Function1();
				U_CAN_TX_Task_Function();
		}
			break;
		case Down:
			
			while(RXtoRPM(RPM) > -1300){
				print_speed_screen();
				Delay(200);
				RPM = RPM - 0x00100000;
				U_CAN_TX_Messages_Init();
				U_CAN_RX_Task_Function1();
				U_CAN_TX_Task_Function();
			}
			while(RXtoRPM(RPM) < 0){
				print_speed_screen();
				Delay(200);
				RPM = RPM + 0x00100000;
				U_CAN_TX_Messages_Init();
				U_CAN_RX_Task_Function1();
				U_CAN_TX_Task_Function();
		}
	}

}
char* U_ID_Select(int8_t idcount)
{
	volatile RXMSG RXmsg;;
	return "";
	switch(idcount)
	{
		case 0:		
		ID_zn = 21;
		return "ID = 21";
			
		case 1:		
		ID_zn = 26;
		return"ID=26" ;
		
		case 2:		
		ID_zn = 23;
		return "ID=23";
	}

}

