#ifndef PS2_TASK_H
#define PS2_TASK_H

#include "main.h"

typedef struct 
{
	uint8_t    LX;       //左摇杆,左右方向
	uint8_t    RY;       //右摇杆,前后方向
	uint16_t   Key;      //16个按键值
}TX_PS2_TypeDef;

void ps2_task(void const * argument);

#endif /* LED_H */



