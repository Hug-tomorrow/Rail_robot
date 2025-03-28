#include "CAN_TXRX.h"
#include "main.h"
#include "chassis_task.h"
uint8_t RxData[8]={0}; 

//static MotorStruct chassis_tx_message;
/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal��CAN�ص�����,���յ������
  * @param[in]      hcan:CAN���ָ��
  * @retval         none
  */ 
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//		RGB_YELLOW();
    CAN_RxHeaderTypeDef RxHeader;
//    uint8_t RxData[8];

    // ��ȡCAN���յ�������
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

//    switch (RxHeader.StdId)
//    {
//        case CAN_M1_ID:
//        case CAN_M2_ID:
//        {
//            static uint8_t i = 0;
//            //get motor id
//            i = RxHeader.StdId - CAN_COB_ID;
//            get_motor_measure(&chassis_tx_message[i], RxData);
////            detect_hook(CHASSIS_MOTOR1_TOE + i);
//            break;
//        }

//        default:
//        {
//            break;
//        }
//    }
//		RGB_OFF();
}

// �������汾�ķ��ͺ���
void sendCANMessage(uint32_t id, uint8_t *data, uint8_t len) 
{
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8]={0};
    uint32_t send_mail_box;

    // ���ñ���ͷ��
    TxHeader.StdId = id;  // ���ñ�׼ID
    TxHeader.ExtId = 0x00;  // ������չID��δʹ�ã�
    TxHeader.RTR = CAN_RTR_DATA;  // ����֡
    TxHeader.IDE = CAN_ID_STD;  // ��׼֡
    TxHeader.DLC = len;  // ���ݳ���
    TxHeader.TransmitGlobalTime = DISABLE;  // ����ȫ��ʱ���

    // �������ݵ����ͻ�����
    for (int i = 0; i < len; i++) {
        TxData[i] = data[i];
    }
		
		uint32_t timeout = 100; // ��ʱʱ�䣨��λ�����룩
    uint32_t start_time = HAL_GetTick();
		
    // �ȴ���������
    while (1) 
		{
        uint32_t tsr = hcan1.Instance->TSR;
        if (tsr & CAN_TSR_TME0) {
            send_mail_box = 0;
            break;
        } else if (tsr & CAN_TSR_TME1) {
            send_mail_box = 1;
            break;
        } else if (tsr & CAN_TSR_TME2) {
            send_mail_box = 2;
            break;
        }
				if (HAL_GetTick() - start_time >= timeout) {
            // ��ʱ����
            return;
        }
        // ����������䶼æ���ȴ�һ��ʱ�������
        vTaskDelay(1);
    }

    // ���ͱ���
    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &send_mail_box);
		vTaskDelay(2);
}

