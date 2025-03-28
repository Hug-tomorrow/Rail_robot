#ifndef BSP_CRC32_H
#define BSP_CRC32_H
#include "struct_typedef.h"

extern uint32_t get_crc32_check_sum(uint32_t *data, uint32_t len);
extern bool_t  verify_crc32_check_sum(uint32_t *data, uint32_t len);
extern void append_crc32_check_sum(uint32_t *data, uint32_t len);

uint16_t crc16tablefast(uint8_t *ptr, uint16_t len) ;
int verify_crc16(uint8_t *data, uint16_t length) ;
#endif
