#ifndef BSP_UART_H
#define BSP_UART_H

#include "main.h"

typedef struct 
{
	
	uint8_t data;          // ���յ�������
	UART_HandleTypeDef* huart;  // ָ���Ӧ�Ĵ��ھ��
	
} UART_Data_t;

void Usart_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
void any_printf(USART_TypeDef* USARTx,char *format,...);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* BSP_UART_H */







