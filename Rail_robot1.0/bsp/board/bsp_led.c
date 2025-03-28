#include "bsp_led.h"
#include "main.h"

// 定义LED引脚
#define RED_PORT LED_RED_GPIO_Port
#define RED_PIN  LED_RED_Pin

#define GREEN_PORT LED_GREE_GPIO_Port
#define GREEN_PIN  LED_GREE_Pin

#define BLUE_PORT  LED_BLUE_GPIO_Port
#define BLUE_PIN   LED_BLUE_Pin

// 定义LED状态
#define LED_ON     GPIO_PIN_RESET
#define LED_OFF    GPIO_PIN_SET

//亮灭翻转函数
void Turn_LED(uint8_t led)
{
	if(led==RED_LIGHT)
	{
		HAL_GPIO_TogglePin(RED_PORT, RED_PIN);  
	}
	else if(led==GREEN_LIGHT)
	{
		HAL_GPIO_TogglePin(GREEN_PORT,GREEN_PIN);
	}
	else if(led==BLUE_LIGHT)
	{
		HAL_GPIO_TogglePin(BLUE_PORT, BLUE_PIN);
	}
}

// 通用LED控制函数
void Control_LED(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state) {
    HAL_GPIO_WritePin(port, pin, state);
}

// 控制RGB LED颜色
void Set_RGB_Color(GPIO_PinState redState, GPIO_PinState greenState, GPIO_PinState blueState) {
    Control_LED(RED_PORT, RED_PIN, redState);
    Control_LED(GREEN_PORT, GREEN_PIN, greenState);
    Control_LED(BLUE_PORT, BLUE_PIN, blueState);
}

// 优化后的颜色控制函数
void LED_RED(void) {
    Set_RGB_Color(LED_ON, LED_OFF, LED_OFF);
}

void LED_GREEN(void) {
    Set_RGB_Color(LED_OFF, LED_ON, LED_OFF);
}

void LED_BLUE(void) {
    Set_RGB_Color(LED_OFF, LED_OFF, LED_ON);
}
void LED_OUT(uint8_t led) {
	if(led==RED_LIGHT)
	{
		Control_LED(RED_PORT, RED_PIN, LED_OFF);  
	}
	else if(led==GREEN_LIGHT)
	{
		Control_LED(GREEN_PORT, GREEN_PIN, LED_OFF);
	}
	else if(led==BLUE_LIGHT)
	{
		Control_LED(BLUE_PORT, BLUE_PIN, LED_OFF);
	}
}
void RGB_OPEN(void) {
    Set_RGB_Color(LED_ON, LED_ON, LED_ON); // 白色
}

void RGB_OFF(void) {
    Set_RGB_Color(LED_OFF, LED_OFF, LED_OFF);
}

void RGB_YELLOW(void) {
    Set_RGB_Color(LED_ON, LED_ON, LED_OFF); // 黄色
}

void RGB_CYAN(void) {
    Set_RGB_Color(LED_OFF, LED_ON, LED_ON);//紫色
}
void RGB_PURPLE(void) {
    Set_RGB_Color(LED_ON, LED_OFF, LED_ON); // 紫色
}



