#include "bsp_crc32.h"
#include "main.h"
uint16_t a,b;

extern CRC_HandleTypeDef hcrc;

uint32_t get_crc32_check_sum(uint32_t *data, uint32_t len)
{
    return HAL_CRC_Calculate(&hcrc, data, len);
}

bool_t verify_crc32_check_sum(uint32_t *data, uint32_t len)
{
    static uint32_t crc32;
    crc32 = get_crc32_check_sum(data, len-1);
    return (crc32 == data[len-1]);
}
void append_crc32_check_sum(uint32_t *data, uint32_t len)
{
    uint32_t crc32;
    crc32 = get_crc32_check_sum(data, len-1);
    data[len-1] = crc32;
}
unsigned short CRC16_MODBUS(unsigned char* data_value, size_t data_length) 
{
	int i;
	unsigned short crc_value = 0xffff;
	unsigned short poly = 0xA001;
	while (data_length--) 
	{
		crc_value ^= *data_value++;
		for (i = 0; i < 8; i++) 
		{
			if (crc_value & 0x0001) 
			{
				crc_value = (crc_value >> 1) ^ poly;
			} 
			else 
			{
				crc_value = crc_value >> 1;
			}
		}
	}
	return crc_value;
}

// 校验接收数据的CRC
int verify_crc16(uint8_t *data, uint16_t length) 
{
	uint16_t crc = CRC16_MODBUS(data, length - 2); // 计算数据部分的CRC
	uint16_t received_crc =data[length - 2] | (data[length - 1]<< 8);; // 获取接收数据中的CRC
a=crc;b=received_crc;
//	printf(" crc: %04X\n", crc);
//	printf(" received_crc: %04X\n", received_crc);

	return (crc == received_crc); // 比较计算出的CRC和接收的CRC是否一致
}


