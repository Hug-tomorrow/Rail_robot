#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "main.h"

extern uint8_t ProximitySwitch_Flag;

// ����GPIO�������
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif





