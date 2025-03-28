#include "ps2_task.h"
#include "main.h"
#include "chassis_task.h"

extern osMessageQId ps2QueueHandle;
PS2Data_TypeDef PS2_Data;

void ps2_task(void const * argument)
{		
    while (1) 
		{					
			PS2_Data.Key=PS2_DataKey();
      PS2_Data.LX=PS2_AnologData(PSS_LX);  
			PS2_Data.RY=PS2_AnologData(PSS_RY);//0-255   Ç°ºó
					
      vTaskDelay(20);
    }
}



