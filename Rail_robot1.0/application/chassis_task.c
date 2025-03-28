#include "chassis_task.h"
#include "main.h"
#include "ps2_task.h"

// ������ٶȺͼ��ٶȵı���
#define ACCELERATION 1500  // ���ٶȣ���λ��r/min
#define DECELERATION 1500  // ���ٶȣ���λ��r/min

// ������ID
#define MOTOR_RIGHT 1   
#define MOTOR_LEFT  2   

// ������ֵ
#define DEAD_ZONE 15

// �����������ٶȷ�Χ �ת��:3000r/min 
#define MAX_MOTOR_SPEED 1500
#define MIN_MOTOR_SPEED -1500

uint8_t Chassis_ModeFlag=0;//����ģʽ
uint8_t EnableFlag=0;			 //ʹ�ܱ�־
uint8_t CurrentRFID=0;     //��ǰRFID
uint8_t TargetRFID=99;  //Ŀ��RFID:  ����100�����ʼ��ʱ CurrentRFID==TargetRFID

uint8_t huchiliang;

Motor_t motor_left;		//���潫Ҫ���͵ĵ������
Motor_t motor_right;

extern PS2Data_TypeDef PS2_Data;
extern uint8_t Point_Number;    //����RFID���ݣ���ı��
extern uint8_t ProximitySwitch_Flag;
//extern osMessageQId ps2QueueHandle;

//int16_t send_speed; //Э���ٶ� 0-3000
//int8_t  send_direction; //Э�鷽��

static void chassis_init(void)
{
		NMT_Reset_Node(); // ��λ�ڵ�
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
	
		SDO_DisableMotor(MOTOR_RIGHT);// ȥʹ��
		SDO_DisableMotor(MOTOR_RIGHT);// ȥʹ��
		SDO_RelayRelease(MOTOR_RIGHT);// �̵����������
		SDO_RelayRelease(MOTOR_RIGHT);// �̵����������

		SDO_DisableMotor(MOTOR_LEFT);// ȥʹ��
		SDO_DisableMotor(MOTOR_LEFT);// ȥʹ��
		SDO_RelayRelease(MOTOR_LEFT);// �̵����������
		SDO_RelayRelease(MOTOR_LEFT);
}
static void chassis_stop(void)
{				
		motor_right.speed=0;					
		motor_left.speed =0;
}


// ���õ���ٶȺͷ���
static void set_motor_speed(uint8_t motor, Motor_t motor_data) 
{
		int16_t send_speed; //Э���ٶ� 0-3000
		int8_t  send_direction; //Э�鷽��
	
		send_direction = (motor_data.speed >= 0) ? FORWARD : REVERSE;
    send_speed = saturate(abs(motor_data.speed), 0, MAX_MOTOR_SPEED);

    PDO_Direction(motor, send_direction);
    PDO_Motor_RPM(motor, send_speed);
}

// ����PS2����
static void handle_ps2_key(uint16_t key) 
{
    switch (key) 
		{
			case R2_KEY:  //ʧ��
				
					EnableFlag=0;
					chassis_disable();
			
				 break;
			case R1_KEY:  //ʹ��
				
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
			case SELECT_KEY:  //���̾�ֹ
				
					chassis_stop();
					Chassis_ModeFlag=	CHASSIS_STOP;		
			
				 break;
			case L_UP:   //ҡ�˿���
				
					Chassis_ModeFlag=CHASSIS_ROCKER;
					ProximitySwitch_Flag=0;
				break;
			case L_LEFT:   //����Ѳ������1			
				
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
					// ������������
					break;
    }
}

static void calculateMotorSpeeds(unsigned char LX, unsigned char RY, int16_t* leftMotorSpeed, int16_t* rightMotorSpeed) 
{
    // ��RYֵӳ�䵽-1��1�ķ�Χ������ǰ���ͺ��ˣ�
    float ryNormalized = (float)RY / 255.0f * 2 - 1;

    // ��LXֵӳ�䵽-1��1�ķ�Χ��������ת��
    float lxNormalized = (float)LX / 255.0f * 2 - 1;

    // ����ǰ��/�����ٶ�
    int16_t forwardSpeed = (int16_t)(ryNormalized * MAX_MOTOR_SPEED);

    // ������ת�ٶ�
    int16_t rotationSpeed = (int16_t)(lxNormalized * MAX_MOTOR_SPEED);

    // ��������
    if (forwardSpeed >= -DEAD_ZONE && forwardSpeed <= DEAD_ZONE) {
        forwardSpeed = 0;
    }
    if (rotationSpeed >= -DEAD_ZONE && rotationSpeed <= DEAD_ZONE) {
        rotationSpeed = 0;
    }

    // �������ҵ���ٶ�
    *leftMotorSpeed = -forwardSpeed + rotationSpeed;  // ���� = ǰ���ٶ� + ��ת�ٶ�
    *rightMotorSpeed = forwardSpeed + rotationSpeed;  // �ҵ�� = ǰ���ٶ� - ��ת�ٶ�

    // �޷�
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
	//		if()//��λ������
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
		// �ȴ���ʼ�����
		vTaskDelay(1000);
	
		//���̳�ʼ��
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
				
				
				// ���õ���ٶ�					
				set_motor_speed(MOTOR_RIGHT,motor_right);
				set_motor_speed(MOTOR_LEFT, motor_left); 
								
				Turn_LED(BLUE_LIGHT);
				
				vTaskDelay(250);
		}
}


