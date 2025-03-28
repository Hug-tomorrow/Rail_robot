#include "bsp_uart.h"  
#include "main.h"

uint8_t Ux_TxBuff[256];    //串口x发送缓冲区

uint8_t ua4_rxBuffer[50]; //DMA接收缓冲区

uint8_t ua6_rxBuffer[8];   //接收缓冲区

extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;

extern uint16_t CO_content;    //储存CO含量
extern uint8_t Point_Number;    //储存RFID数据：点的编号
extern uint8_t battery_level;    //储存电池电量百分比
extern uint16_t battery_voltage;    //储存电池电压

//通过任意串口发送
void any_printf(USART_TypeDef* USARTx,char *format,...)
{	
	uint8_t i;                                           //用于for循环
	
	va_list listdata;                                     //建立一个va_list变量listdata
	va_start(listdata,format);                            //向listdata加载...代表的不定长的参数
	vsprintf((char *)Ux_TxBuff,format,listdata);          //格式化输出到缓冲区U0_TxBuff
	va_end(listdata);                                     //释放listdata

	for(i=0;i<strlen((const char*)Ux_TxBuff);i++){        //根据U0_TxBuff缓冲区数据量，一个字节一个字节的循环发送
		while((USARTx->SR&0x40)==0);
		USARTx->DR = Ux_TxBuff[i];
			
	}
	while((USARTx->SR&0x40)==0);	     //等到最后一个字节数据发送完毕，再退出函数
}

/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&huart3,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

/**
 * @brief       Rx DMA传输回调函数 
 * @param       huart: UART句柄类型指针
 * @retval      无
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	 if (huart == &huart4) 
	{
		 if (ua4_rxBuffer[0] == 0x02 && ua4_rxBuffer[1] == 0x03 && ua4_rxBuffer[2] == 0x02)
		{
				CO_content = (ua4_rxBuffer[3] << 8) | ua4_rxBuffer[4];
		}
		
		 if (ua4_rxBuffer[0] == 0xDD && ua4_rxBuffer[1] == 0x03 && ua4_rxBuffer[2] == 0x00)
		{
				battery_voltage = (ua4_rxBuffer[4] << 8) | ua4_rxBuffer[5];
				battery_level = ua4_rxBuffer[23];
		} 
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart4, ua4_rxBuffer, 50);
		__HAL_DMA_DISABLE_IT(&hdma_uart4_rx,DMA_IT_HT);//关闭传输过半中断
	} 
	
	if (huart == &huart6) 
	{
		if (ua6_rxBuffer[0] == 0x07 && ua6_rxBuffer[1] == 0x00 && ua6_rxBuffer[2] == 0xEE && ua6_rxBuffer[2] == 0x00)
		{
				Point_Number=(ua6_rxBuffer[4] << 4) | ua6_rxBuffer[5];
		} 
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart6, ua6_rxBuffer, 8);
		__HAL_DMA_DISABLE_IT(&hdma_usart6_rx,DMA_IT_HT);//关闭传输过半中断
	}
}

