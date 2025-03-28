#ifndef __BSP_BUZZER_H
#define __BSP_BUZZER_H
#include "main.h"

enum{
	BEEP_OFF = 0,
	BEEP_ON  = 1
};

/*--------Buzzer config--------*/
#define ENABLE_Buzzer_PIN_CLOCK  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define Buzzer_PORT              Buzzer_GPIO_Port
#define Buzzer_PIN               Buzzer_Pin
//#define Buzzer                 PDout(11)
/*----------------------------------*/


/*--------Buzzer Interface Fun --------*/
void Buzzer_Init(void); 

uint8_t Buzzer_AddTask(uint8_t num_time,uint8_t times);
void Buzzer_task(uint8_t rate);
void Buzzer_ClearQueue(void);// 清空任务队列
/*----------------------------------*/


#endif
