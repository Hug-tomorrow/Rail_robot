#include "led_RGB_task.h"
#include "main.h" 

void led_RGB_task(void const * argument)
{
//	Buzzer_AddTask(3, 50);//�������� 1 �Σ�����ʱ��Ϊ 110ms��Ȼ��ֹͣ 110ms	
//	Buzzer_AddTask(2, 100);
	vTaskDelay(1000); 
	while(1)
	{
		// ִ�з�������
		Buzzer_task(100);
//		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
		
		// �����̵�
		Turn_LED(GREEN_LIGHT);

		// ��ʱ 500ms
		vTaskDelay(100); 

	}       
}













