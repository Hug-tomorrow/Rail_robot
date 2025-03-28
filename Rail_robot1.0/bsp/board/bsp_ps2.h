#ifndef __BSP_PS2_H
#define __BSP_PS2_H
//#include "sys.h"
//#include "usbh_usr.h"
#include "stdint.h"//unit8
#include "stm32f4xx_hal.h"
#include "main.h"

/*--------PS2 config--------*/
////PS2手柄引脚
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

#define DI HAL_GPIO_ReadPin(PS2_DI_GPIO_Port,GPIO_PIN_0)    //Input pin //输入引脚    PS2_DI_PIN_Pin

#define DO_H HAL_GPIO_WritePin(PS2_DO_GPIO_Port,PS2_DO_Pin,GPIO_PIN_SET)    //Command height //命令位高
#define DO_L HAL_GPIO_WritePin(PS2_DO_GPIO_Port,PS2_DO_Pin,GPIO_PIN_RESET)  //Command low //命令位低

#define CS_H HAL_GPIO_WritePin(PS2_CS_GPIO_Port,PS2_CS_Pin,GPIO_PIN_SET)    //Cs pull up //CS拉高   
#define CS_L HAL_GPIO_WritePin(PS2_CS_GPIO_Port,PS2_CS_Pin,GPIO_PIN_RESET)  //Cs drawdown //CS拉低

#define CLK_H HAL_GPIO_WritePin(PS2_CLK_GPIO_Port,PS2_CLK_Pin,GPIO_PIN_SET)  //Clock lift //时钟拉高
#define CLK_L HAL_GPIO_WritePin(PS2_CLK_GPIO_Port,PS2_CLK_Pin,GPIO_PIN_RESET)//Clock down //时钟拉低

//These are stick values
#define PSS_RX 5                //右摇杆X轴数据
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8
/*----------------------------------*/

//ps2 ubs的枚举状态
typedef enum {
	EnumNULL =0, //无状态
	EnumWait ,
	EnumDone ,
}ENUM_STATE_t;

//手柄的数据类型
typedef struct _PS2_
{
	uint8_t    LX;       //左摇杆,左右方向
	uint8_t    LY;       //左摇杆,前后方向
	uint8_t    RX;       //右摇杆,左右方向
	uint8_t    RY;       //右摇杆,前后方向
	uint16_t   Key;      //16个按键值
	
	//usb版本ps2专用变量
	uint8_t    usb_ps2_ready;//USB手柄是否就绪标志位
	uint8_t    usb_ps2_alive;//usb数据接收活性检查
	uint16_t   enum_error;
	ENUM_STATE_t enum_state; //枚举状态
}PS2_TypeDef;

extern PS2_TypeDef ps2_val;

enum {
	PS2_KEY_OFF= 1,
	PS2_KEY_ON = 0
};

//PS2按键枚举
enum 
{
	//选择按键
	SELECT_KEY			= (1 << 0),                   //1
	
	//左右摇杆按下键值
	L_ROCKER_KEY       = (1 << 1),               //2
	R_ROCKER_KEY       = (1 << 2),               //4

	//开始按键                                   
	START_KEY           = (1 << 3),              //8
																							 
	//左按键区域                                 
	L_UP               = (1 << 4),               //16
	L_RIGHT            = (1 << 5),               //32
	L_DOWN             = (1 << 6),               //64
	L_LEFT             = (1 << 7),               //128
																							 
	//左右扳机按键值                             
	L2_KEY             = (1 << 8),	             //256
	R2_KEY             = (1 << 9),               //512
	L1_KEY             = (1 << 10),              //1024
	R1_KEY             = (1 << 11),              //2048
	
	//右按键区域
	R_1GREEN           = (1 << 12),              //4096
	R_2RED             = (1 << 13),              //8192
	R_3BLUE            = (1 << 14),              //16384
	R_4PINK            = (1 << 15)               //32768
};
#define Read_PS2_KEY(mask)     ((ps2_val.Key & (mask)) ? PS2_KEY_ON : PS2_KEY_OFF)


//对外接口
void PS2_Init(void);
void PS2_Read(void);
void PS2_Key_Param_Init(PS2_TypeDef *p);
uint8_t PS2_AnologData(uint8_t button);
uint16_t PS2_DataKey(void);
void HAL_Delay_us(uint32_t us);
void PS2_SetInit(void);
#endif







