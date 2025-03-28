#ifndef __BSP_OLED_H
#define __BSP_OLED_H			  	 
#include "stm32f4xx_hal.h"
#include "main.h"

#define OLED_CMD  0	//Command //Ð´ÃüÁî
#define OLED_DATA 1	//Data //Ð´Êý¾Ý

#define CNSizeWidth  16
#define CNSizeHeight 16

/*--------OLED config--------*/
//#define ENABLE_OLED_PIN_CLOCK  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)

//#define OLED_SCL_PORT     GPIOD //port
//#define OLED_SDA_PORT     GPIOD
//#define OLED_RES_PORT     GPIOD
//#define OLED_DC_PORT      GPIOD

//#define OLED_SCL_PIN      GPIO_PIN_7 //pin
//#define OLED_SDA_PIN      GPIO_PIN_6
//#define OLED_RES_PIN      GPIO_PIN_5
//#define OLED_DC_PIN       GPIO_PIN_4

#define OLED_SCLK_Clr()  HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_RESET)   //SCL
#define OLED_SCLK_Set()  HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_SET)   //SCL

#define OLED_SDIN_Clr()  HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_RESET)   //SDA
#define OLED_SDIN_Set()  HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_SET)  //SDA

#define OLED_RST_Clr()   HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_RESET)   //RES
#define OLED_RST_Set()   HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_SET)   //RES

#define OLED_RS_Clr()    HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_RESET)   //DC
#define OLED_RS_Set()    HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_SET)   //DC
/*----------------------------------*/



/*--------OLED Interface Fun--------*/
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Refresh_Gram(void);		
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNumber(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);
void oled_showfloat(const float needtoshow,uint8_t show_x,uint8_t show_y,uint8_t zs_num,uint8_t xs_num);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no,uint8_t font_width,uint8_t font_height);	\
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[]);
extern const unsigned char gImage_usb_bmp[];
void OLED_Refresh_Line(void);
void OLED_ClearBuf(void);
/*----------------------------------*/


#endif  
	 
