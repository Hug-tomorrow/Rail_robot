#ifndef PS2_TASK_H
#define PS2_TASK_H

#include "main.h"

typedef struct 
{
	uint8_t    LX;       //��ҡ��,���ҷ���
	uint8_t    RY;       //��ҡ��,ǰ����
	uint16_t   Key;      //16������ֵ
}TX_PS2_TypeDef;

void ps2_task(void const * argument);

#endif /* LED_H */



