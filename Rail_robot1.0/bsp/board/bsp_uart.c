#include "bsp_uart.h"  
#include "main.h"

uint8_t Ux_TxBuff[256];    //����x���ͻ�����

uint8_t ua4_rxBuffer[50]; //DMA���ջ�����

uint8_t ua6_rxBuffer[8];   //���ջ�����

extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;

extern uint16_t CO_content;    //����CO����
extern uint8_t Point_Number;    //����RFID���ݣ���ı��
extern uint8_t battery_level;    //�����ص����ٷֱ�
extern uint16_t battery_voltage;    //�����ص�ѹ

//ͨ�����⴮�ڷ���
void any_printf(USART_TypeDef* USARTx,char *format,...)
{	
	uint8_t i;                                           //����forѭ��
	
	va_list listdata;                                     //����һ��va_list����listdata
	va_start(listdata,format);                            //��listdata����...����Ĳ������Ĳ���
	vsprintf((char *)Ux_TxBuff,format,listdata);          //��ʽ�������������U0_TxBuff
	va_end(listdata);                                     //�ͷ�listdata

	for(i=0;i<strlen((const char*)Ux_TxBuff);i++){        //����U0_TxBuff��������������һ���ֽ�һ���ֽڵ�ѭ������
		while((USARTx->SR&0x40)==0);
		USARTx->DR = Ux_TxBuff[i];
			
	}
	while((USARTx->SR&0x40)==0);	     //�ȵ����һ���ֽ����ݷ�����ϣ����˳�����
}

/*****************  �����ַ��� **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&huart3,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

/**
 * @brief       Rx DMA����ص����� 
 * @param       huart: UART�������ָ��
 * @retval      ��
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
		__HAL_DMA_DISABLE_IT(&hdma_uart4_rx,DMA_IT_HT);//�رմ�������ж�
	} 
	
	if (huart == &huart6) 
	{
		if (ua6_rxBuffer[0] == 0x07 && ua6_rxBuffer[1] == 0x00 && ua6_rxBuffer[2] == 0xEE && ua6_rxBuffer[2] == 0x00)
		{
				Point_Number=(ua6_rxBuffer[4] << 4) | ua6_rxBuffer[5];
		} 
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart6, ua6_rxBuffer, 8);
		__HAL_DMA_DISABLE_IT(&hdma_usart6_rx,DMA_IT_HT);//�رմ�������ж�
	}
}

