#ifndef BSP_UART_H
#define BSP_UART_H

#include "main.h"

typedef struct 
{
	
	uint8_t data;          // 接收到的数据
	UART_HandleTypeDef* huart;  // 指向对应的串口句柄
	
} UART_Data_t;

void Usart_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
void any_printf(USART_TypeDef* USARTx,char *format,...);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* BSP_UART_H */







