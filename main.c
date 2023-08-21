#include "main.h"
#include "MDR32F9Qx_rst_clk.h"          // Keil::Drivers:RST_CLK                   // define objects in main module
                    // RTOS object definitions

 int main()	{	
	U_RST_Init();
		
	U_MLT_Init();
	
	U_BTN_Init();
	
	U_MENU_Init();
	
	U_CAN_Init_OS();
	
	CAN_Initial();
	
	U_CAN_MAIN_MENU();
}

 void Main_Task_Init (void)
{
	void U_CAN_MAIN_MENU(void );
}


