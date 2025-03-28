#include "uart_Transmit_task.h"
#include "main.h"

uint8_t CO_TxBuff[8]={0x02,0x03,0x00,0x00,0x00,0x01,0x84,0x39};//CO气体检测仪问询报文
uint8_t battery_TxBuff[7]={0xDD,0xA5,0x03,0x00,0xFF,0xFD,0x77};//电池问询报文

void UART_TransmitTask(void const *pvParameters)
{
	vTaskDelay(1000);
	uint16_t battery_count=0;
	
	while(1)
	{
		
		HAL_UART_Transmit_DMA(&huart4,CO_TxBuff,8);
		vTaskDelay(100);
		if(battery_count==600)
		{
			HAL_UART_Transmit_DMA(&huart4,battery_TxBuff,7);
			battery_count=0;
			vTaskDelay(100);
		}
		else if(battery_count==0)
		{
			HAL_UART_Transmit_DMA(&huart4,battery_TxBuff,7);
		}
		battery_count++;
	}
}




