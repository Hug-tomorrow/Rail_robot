#ifndef CAN_TXRX_H
#define CAN_TXRX_H

#include "main.h"

typedef enum
{
    CAN_COB_ID = 0x180,
    CAN_M1_ID = 0x181,
    CAN_M2_ID = 0x182,
	
} can_id_e;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void sendCANMessage(uint32_t id, uint8_t *data, uint8_t len) ;

#endif /* CAN_TXRX_H */



