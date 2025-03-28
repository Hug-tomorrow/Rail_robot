#include "led_RGB_task.h"
#include "main.h" 

void led_RGB_task(void const * argument)
{
//	Buzzer_AddTask(3, 50);//蜂鸣器响 1 次，持续时间为 110ms，然后停止 110ms	
//	Buzzer_AddTask(2, 100);
	vTaskDelay(1000); 
	while(1)
	{
		// 执行蜂鸣任务
		Buzzer_task(100);
//		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
		
		// 点亮绿灯
		Turn_LED(GREEN_LIGHT);

		// 延时 500ms
		vTaskDelay(100); 

	}       
}













