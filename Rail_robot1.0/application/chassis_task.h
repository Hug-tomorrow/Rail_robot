#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H

#include "struct_typedef.h"

//底盘任务控制间隔 2ms
#define CHASSIS_CONTROL_TIME_MS 20

typedef struct 
{
	uint8_t    LX;       //左摇杆,左右方向
	uint8_t    RY;       //右摇杆,前后方向
	uint16_t   Key;      //16个按键值
}PS2Data_TypeDef;

typedef struct
{
int8_t	mode; //操作模式  0-10
int16_t speed; //协议速度 0-3000
int8_t  direction; //协议方向
int16_t torque; //力矩 0-1000
//int32_t position; //目标位置
int32_t	acceleration; //协议加速度
int32_t deceleration; //协议减速度
  
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



