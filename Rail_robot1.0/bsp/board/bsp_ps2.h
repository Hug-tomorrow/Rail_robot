#ifndef __BSP_PS2_H
#define __BSP_PS2_H
//#include "sys.h"
//#include "usbh_usr.h"
#include "stdint.h"//unit8
#include "stm32f4xx_hal.h"
#include "main.h"

/*--------PS2 config--------*/
////PS2�ֱ�����
//#define ENABLE_PS2_DI_PIN_CLOCK  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
//#define ENABLE_PS2_DO_PIN_CLOCK  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
//#define ENABLE_PS2_CS_PIN_CLOCK  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
//#define ENABLE_PS2_CLK_PIN_CLOCK RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)

//#define PS2_DI_PORT  GPIOE
//#define PS2_DO_PORT  GPIOE
//#define PS2_CS_PORT  GPIOE
//#define PS2_CLK_PORT GPIOE

//#define PS2_DI_PIN_Pin  GPIO_PIN_0
//#define PS2_DO_PIN_Pin  GPIO_PIN_1
//#define PS2_CS_PIN_Pin  GPIO_PIN_2
//#define PS2_CLK_PIN_Pin GPIO_PIN_3

#define DI HAL_GPIO_ReadPin(PS2_DI_GPIO_Port,GPIO_PIN_0)    //Input pin //��������    PS2_DI_PIN_Pin

#define DO_H HAL_GPIO_WritePin(PS2_DO_GPIO_Port,PS2_DO_Pin,GPIO_PIN_SET)    //Command height //����λ��
#define DO_L HAL_GPIO_WritePin(PS2_DO_GPIO_Port,PS2_DO_Pin,GPIO_PIN_RESET)  //Command low //����λ��

#define CS_H HAL_GPIO_WritePin(PS2_CS_GPIO_Port,PS2_CS_Pin,GPIO_PIN_SET)    //Cs pull up //CS����   
#define CS_L HAL_GPIO_WritePin(PS2_CS_GPIO_Port,PS2_CS_Pin,GPIO_PIN_RESET)  //Cs drawdown //CS����

#define CLK_H HAL_GPIO_WritePin(PS2_CLK_GPIO_Port,PS2_CLK_Pin,GPIO_PIN_SET)  //Clock lift //ʱ������
#define CLK_L HAL_GPIO_WritePin(PS2_CLK_GPIO_Port,PS2_CLK_Pin,GPIO_PIN_RESET)//Clock down //ʱ������

//These are stick values
#define PSS_RX 5                //��ҡ��X������
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8
/*----------------------------------*/

//ps2 ubs��ö��״̬
typedef enum {
	EnumNULL =0, //��״̬
	EnumWait ,
	EnumDone ,
}ENUM_STATE_t;

//�ֱ�����������
typedef struct _PS2_
{
	uint8_t    LX;       //��ҡ��,���ҷ���
	uint8_t    LY;       //��ҡ��,ǰ����
	uint8_t    RX;       //��ҡ��,���ҷ���
	uint8_t    RY;       //��ҡ��,ǰ����
	uint16_t   Key;      //16������ֵ
	
	//usb�汾ps2ר�ñ���
	uint8_t    usb_ps2_ready;//USB�ֱ��Ƿ������־λ
	uint8_t    usb_ps2_alive;//usb���ݽ��ջ��Լ��
	uint16_t   enum_error;
	ENUM_STATE_t enum_state; //ö��״̬
}PS2_TypeDef;

extern PS2_TypeDef ps2_val;

enum {
	PS2_KEY_OFF= 1,
	PS2_KEY_ON = 0
};

//PS2����ö��
enum 
{
	//ѡ�񰴼�
	SELECT_KEY			= (1 << 0),                   //1
	
	//����ҡ�˰��¼�ֵ
	L_ROCKER_KEY       = (1 << 1),               //2
	R_ROCKER_KEY       = (1 << 2),               //4

	//��ʼ����                                   
	START_KEY           = (1 << 3),              //8
																							 
	//�󰴼�����                                 
	L_UP               = (1 << 4),               //16
	L_RIGHT            = (1 << 5),               //32
	L_DOWN             = (1 << 6),               //64
	L_LEFT             = (1 << 7),               //128
																							 
	//���Ұ������ֵ                             
	L2_KEY             = (1 << 8),	             //256
	R2_KEY             = (1 << 9),               //512
	L1_KEY             = (1 << 10),              //1024
	R1_KEY             = (1 << 11),              //2048
	
	//�Ұ�������
	R_1GREEN           = (1 << 12),              //4096
	R_2RED             = (1 << 13),              //8192
	R_3BLUE            = (1 << 14),              //16384
	R_4PINK            = (1 << 15)               //32768
};
#define Read_PS2_KEY(mask)     ((ps2_val.Key & (mask)) ? PS2_KEY_ON : PS2_KEY_OFF)


//����ӿ�
void PS2_Init(void);
void PS2_Read(void);
void PS2_Key_Param_Init(PS2_TypeDef *p);
uint8_t PS2_AnologData(uint8_t button);
uint16_t PS2_DataKey(void);
void HAL_Delay_us(uint32_t us);
void PS2_SetInit(void);
#endif







