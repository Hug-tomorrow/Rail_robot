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
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */ 
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//		RGB_YELLOW();
    CAN_RxHeaderTypeDef RxHeader;
//    uint8_t RxData[8];

    // 读取CAN接收到的数据
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

// 非阻塞版本的发送函数
void sendCANMessage(uint32_t id, uint8_t *data, uint8_t len) 
{
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8]={0};
    uint32_t send_mail_box;

    // 配置报文头部
    TxHeader.StdId = id;  // 设置标准ID
    TxHeader.ExtId = 0x00;  // 设置扩展ID（未使用）
    TxHeader.RTR = CAN_RTR_DATA;  // 数据帧
    TxHeader.IDE = CAN_ID_STD;  // 标准帧
    TxHeader.DLC = len;  // 数据长度
    TxHeader.TransmitGlobalTime = DISABLE;  // 禁用全局时间戳

    // 拷贝数据到发送缓冲区
    for (int i = 0; i < len; i++) {
        TxData[i] = data[i];
    }
		
		uint32_t timeout = 100; // 超时时间（单位：毫秒）
    uint32_t start_time = HAL_GetTick();
		
    // 等待空闲邮箱
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
            // 超时处理
            return;
        }
        // 如果所有邮箱都忙，等待一段时间后重试
        vTaskDelay(1);
    }

    // 发送报文
    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &send_mail_box);
		vTaskDelay(2);
}

