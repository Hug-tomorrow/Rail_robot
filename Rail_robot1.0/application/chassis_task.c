#include "chassis_task.h"
#include "main.h"
#include "ps2_task.h"

// 定义加速度和减速度的变量
#define ACCELERATION 1500  // 加速度，单位：r/min
#define DECELERATION 1500  // 减速度，单位：r/min

// 定义电机ID
#define MOTOR_RIGHT 1   
#define MOTOR_LEFT  2   

// 死区阈值
#define DEAD_ZONE 15

// 定义电机允许速度范围 额定转速:3000r/min 
#define MAX_MOTOR_SPEED 1500
#define MIN_MOTOR_SPEED -1500

uint8_t Chassis_ModeFlag=0;//底盘模式
uint8_t EnableFlag=0;			 //使能标志
uint8_t CurrentRFID=0;     //当前RFID
uint8_t TargetRFID=99;  //目标RFID:  设置100避免初始化时 CurrentRFID==TargetRFID

uint8_t huchiliang;

Motor_t motor_left;		//储存将要发送的电机数据
Motor_t motor_right;

extern PS2Data_TypeDef PS2_Data;
extern uint8_t Point_Number;    //储存RFID数据：点的编号
extern uint8_t ProximitySwitch_Flag;
//extern osMessageQId ps2QueueHandle;

//int16_t send_speed; //协议速度 0-3000
//int8_t  send_direction; //协议方向

static void chassis_init(void)
{
		NMT_Reset_Node(); // 复位节点
		vTaskDelay(20);
	
		SDO_Acceleration(MOTOR_RIGHT, ACCELERATION); 	SDO_Acceleration(MOTOR_RIGHT, ACCELERATION); 
		SDO_Deceleration(MOTOR_RIGHT, DECELERATION); SDO_Deceleration(MOTOR_RIGHT, DECELERATION);
		SDO_Direction(MOTOR_RIGHT, FORWARD); 		SDO_Direction(MOTOR_RIGHT, FORWARD);

		SDO_Acceleration(MOTOR_LEFT, ACCELERATION); 	SDO_Acceleration(MOTOR_LEFT, ACCELERATION); 
		SDO_Deceleration(MOTOR_LEFT, DECELERATION); 	SDO_Deceleration(MOTOR_LEFT, DECELERATION); 
		SDO_Direction(MOTOR_LEFT, FORWARD); SDO_Direction(MOTOR_LEFT, FORWARD);

		NMT_Start_Node(); 
		vTaskDelay(20);	
}

static void chassis_start(void)
{	
		EnableFlag=1;
	
		SDO_SwitchingState(MOTOR_RIGHT);SDO_SwitchingState(MOTOR_RIGHT);
		SDO_RelayEngage(MOTOR_RIGHT); SDO_RelayEngage(MOTOR_RIGHT); 
		SDO_EnableMotor(MOTOR_RIGHT);SDO_EnableMotor(MOTOR_RIGHT);
		SDO_OperationMode(MOTOR_RIGHT);SDO_OperationMode(MOTOR_RIGHT);

		SDO_SwitchingState(MOTOR_LEFT);  SDO_SwitchingState(MOTOR_LEFT); 
		SDO_RelayEngage(MOTOR_LEFT); SDO_RelayEngage(MOTOR_LEFT); 
		SDO_EnableMotor(MOTOR_LEFT);   SDO_EnableMotor(MOTOR_LEFT); 
		SDO_OperationMode(MOTOR_LEFT); SDO_OperationMode(MOTOR_LEFT); 

}

static void chassis_disable(void)
{
		EnableFlag=0;
	
		SDO_DisableMotor(MOTOR_RIGHT);// 去使能
		SDO_DisableMotor(MOTOR_RIGHT);// 去使能
		SDO_RelayRelease(MOTOR_RIGHT);// 继电器解除吸合
		SDO_RelayRelease(MOTOR_RIGHT);// 继电器解除吸合

		SDO_DisableMotor(MOTOR_LEFT);// 去使能
		SDO_DisableMotor(MOTOR_LEFT);// 去使能
		SDO_RelayRelease(MOTOR_LEFT);// 继电器解除吸合
		SDO_RelayRelease(MOTOR_LEFT);
}
static void chassis_stop(void)
{				
		motor_right.speed=0;					
		motor_left.speed =0;
}


// 设置电机速度和方向
static void set_motor_speed(uint8_t motor, Motor_t motor_data) 
{
		int16_t send_speed; //协议速度 0-3000
		int8_t  send_direction; //协议方向
	
		send_direction = (motor_data.speed >= 0) ? FORWARD : REVERSE;
    send_speed = saturate(abs(motor_data.speed), 0, MAX_MOTOR_SPEED);

    PDO_Direction(motor, send_direction);
    PDO_Motor_RPM(motor, send_speed);
}

// 处理PS2按键
static void handle_ps2_key(uint16_t key) 
{
    switch (key) 
		{
			case R2_KEY:  //失能
				
					EnableFlag=0;
					chassis_disable();
			
				 break;
			case R1_KEY:  //使能
				
					EnableFlag=1;
			
					SDO_RelayEngage(MOTOR_RIGHT); 
					SDO_RelayEngage(MOTOR_RIGHT);
					SDO_EnableMotor(MOTOR_RIGHT);
					SDO_EnableMotor(MOTOR_RIGHT);
				
					SDO_RelayEngage(MOTOR_LEFT); 
					SDO_RelayEngage(MOTOR_LEFT);
					SDO_EnableMotor(MOTOR_LEFT); 	
					SDO_EnableMotor(MOTOR_LEFT); 		
			
				 break;
			case SELECT_KEY:  //底盘静止
				
					chassis_stop();
					Chassis_ModeFlag=	CHASSIS_STOP;		
			
				 break;
			case L_UP:   //摇杆控制
				
					Chassis_ModeFlag=CHASSIS_ROCKER;
					ProximitySwitch_Flag=0;
				break;
			case L_LEFT:   //定速巡航——1			
				
					Chassis_ModeFlag=CHASSIS_CRUISE1;
					ProximitySwitch_Flag=0;			
//					Point_Number=0;
			
				break;	
			case R_1GREEN:
					TargetRFID=1;Point_Number=0;
				break;
			case R_4PINK:
					TargetRFID=2;Point_Number=0;
				break;
			case R_3BLUE:
					TargetRFID=3;Point_Number=0;
				break;
			case R_2RED:
					TargetRFID=4;Point_Number=0;
				break;
			default:
					// 其他按键处理
					break;
    }
}

static void calculateMotorSpeeds(unsigned char LX, unsigned char RY, int16_t* leftMotorSpeed, int16_t* rightMotorSpeed) 
{
    // 将RY值映射到-1到1的范围（用于前进和后退）
    float ryNormalized = (float)RY / 255.0f * 2 - 1;

    // 将LX值映射到-1到1的范围（用于旋转）
    float lxNormalized = (float)LX / 255.0f * 2 - 1;

    // 计算前进/后退速度
    int16_t forwardSpeed = (int16_t)(ryNormalized * MAX_MOTOR_SPEED);

    // 计算旋转速度
    int16_t rotationSpeed = (int16_t)(lxNormalized * MAX_MOTOR_SPEED);

    // 死区处理
    if (forwardSpeed >= -DEAD_ZONE && forwardSpeed <= DEAD_ZONE) {
        forwardSpeed = 0;
    }
    if (rotationSpeed >= -DEAD_ZONE && rotationSpeed <= DEAD_ZONE) {
        rotationSpeed = 0;
    }

    // 计算左右电机速度
    *leftMotorSpeed = -forwardSpeed + rotationSpeed;  // 左电机 = 前进速度 + 旋转速度
    *rightMotorSpeed = forwardSpeed + rotationSpeed;  // 右电机 = 前进速度 - 旋转速度

    // 限幅
    if (*leftMotorSpeed > MAX_MOTOR_SPEED) *leftMotorSpeed = MAX_MOTOR_SPEED;
    if (*leftMotorSpeed < MIN_MOTOR_SPEED) *leftMotorSpeed = MIN_MOTOR_SPEED;
    if (*rightMotorSpeed > MAX_MOTOR_SPEED) *rightMotorSpeed = MAX_MOTOR_SPEED;
    if (*rightMotorSpeed < MIN_MOTOR_SPEED) *rightMotorSpeed = MIN_MOTOR_SPEED;
}

void RFID_handle()
{
		CurrentRFID=Point_Number;
		if(CurrentRFID==TargetRFID)
		{
	//		if()//限位器触发
	//		{
			Chassis_ModeFlag=	CHASSIS_STOP;
			TargetRFID=99;
//				motor_right.speed=0;					
//				motor_left.speed =0;
			
	//		}	
		}
}

void chassis_task(void const *pvParameters)
{
		// 等待初始化完成
		vTaskDelay(1000);
	
		//底盘初始化
		chassis_init();
		vTaskDelay(100);
	
		chassis_start();
		vTaskDelay(100);
	
    while (1)
    {			
				handle_ps2_key(PS2_Data.Key);
				RFID_handle();
			
				if(Chassis_ModeFlag==CHASSIS_ROCKER)
				{
					calculateMotorSpeeds(PS2_Data.LX, PS2_Data.RY, &motor_left.speed, &motor_right.speed);				
				}
				else if(Chassis_ModeFlag==CHASSIS_CRUISE1)
				{				
					motor_right.speed=CRUISE1_RIGHTSPEED;					
					motor_left.speed =CRUISE1_LEFTSPEED;				
				}
				else
				{
					chassis_stop();
					
				}					
				
				
				// 设置电机速度					
				set_motor_speed(MOTOR_RIGHT,motor_right);
				set_motor_speed(MOTOR_LEFT, motor_left); 
								
				Turn_LED(BLUE_LIGHT);
				
				vTaskDelay(250);
		}
}


