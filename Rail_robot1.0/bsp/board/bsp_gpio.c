#include "bsp_gpio.h"
#include "main.h"

uint8_t ProximitySwitch_Flag=0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_10)  
    {
			ProximitySwitch_Flag=1;
        // �����߼�
//        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9); 
    }
}



