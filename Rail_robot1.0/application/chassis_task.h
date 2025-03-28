#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H

#include "struct_typedef.h"

//����������Ƽ�� 2ms
#define CHASSIS_CONTROL_TIME_MS 20

typedef struct 
{
	uint8_t    LX;       //��ҡ��,���ҷ���
	uint8_t    RY;       //��ҡ��,ǰ����
	uint16_t   Key;      //16������ֵ
}PS2Data_TypeDef;

typedef struct
{
int8_t	mode; //����ģʽ  0-10
int16_t speed; //Э���ٶ� 0-3000
int8_t  direction; //Э�鷽��
int16_t torque; //���� 0-1000
//int32_t position; //Ŀ��λ��
int32_t	acceleration; //Э����ٶ�
int32_t deceleration; //Э����ٶ�
  
}Motor_t;

enum MOTOR_DIRECTION 
{
    FORWARD = 0,
    REVERSE = 64
};

enum CHASSIS_MODE_FLAG 
{
    CHASSIS_STOP=0,
    CHASSIS_ROCKER = 1,
		CHASSIS_CRUISE1 = 2,
		CHASSIS_CRUISE2 = 3,
};

enum CRUISE_VALUE 
{
    CRUISE1_RIGHTSPEED    = 500,
		CRUISE1_LEFTSPEED     = -500,
		CRUISE2_RIGHTSPEED    = 2000,
		CRUISE2_LEFTSPEED     = -2000,
};
void chassis_task(void const *pvParameters);

#endif /* CHASSIS_TASK_H */



