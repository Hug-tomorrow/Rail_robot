#include "bsp_canopen.h"
#include "main.h"

// 定义常量
#define BASE_COB_ID 0x0600
#define NMT_COB_ID 0x0000
#define PULSE_PER_REV     10000       // 每圈脉冲数

/* CANopen帧ID定义 */
#define RPDO1_COBID       0x200 // RPDO1的COB-ID
#define RPDO2_COBID       0x300 // RPDO2的COB-ID
#define NMT_COBID         0x0000            // NMT命令帧ID

/**
  * @brief          发送电机控制命令
  * @param[in]      motorId: 电机ID，用于指定目标电机
  * @param[in]      command: 命令字节，用于指定命令类型
  * @param[in]      subCommand: 子命令字节，用于指定命令的子类型
  * @param[in]      objectId: 对象字典ID，用于指定命令的目标对象
  * @param[in]      subIndex: 子索引，用于指定对象字典中的子索引
  * @param[in]      value: 命令值，用于指定命令的具体数值
  * @retval         none
  */
void sendCommand(uint8_t motorId, uint8_t command, uint8_t subCommand, uint8_t objectId, uint8_t subIndex, uint8_t value) 
{
    uint8_t data[8] = {command, subCommand, objectId, 0x00, value, 0x00, 0x00, 0x00};
    sendCANMessage(BASE_COB_ID + motorId, data, 8);
//		for (int i = 0; i < 8; i++) {
////    any_printf(USART3, "%02X ", data[i]);  // 以16进制格式打印每个字节
//}
}


// PDO设置电机转速（RPM）0-3000
void PDO_Motor_RPM(uint8_t motorId, int16_t rpm) 
{
    // 转速转换公式：脉冲/秒 = (rpm * 10000) / 60
    int32_t target_speed = (int32_t)((float)rpm * PULSE_PER_REV / 60);
    
    // 限幅保护（根据驱动器手册设置）
    const int32_t MAX_SPEED = 500000; // 最大速度限制
    if (target_speed > MAX_SPEED) target_speed = MAX_SPEED;
    else if (target_speed < 0) target_speed = 0;
    
    // 数据格式：小端模式写入目标速度（单位：脉冲/秒）
    uint8_t data[8] = {0};
    data[0] = target_speed & 0xFF;
    data[1] = (target_speed >> 8) & 0xFF;
		data[2] = (target_speed >> 16) & 0xFF;
    
    // 发送速度指令
    sendCANMessage(RPDO2_COBID + motorId, data, 4);
}

void PDO_Direction(uint8_t motorId,int8_t direction)//PDO-方向    
{
	uint8_t data[8] = {0};
  data[0] = direction;
	sendCANMessage(RPDO1_COBID + motorId, data, 1);
}

// 复位节点
void NMT_Reset_Node(void) 
{
    uint8_t data[8] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(NMT_COB_ID, data, 8);
		
}

//节点进入预操作
void NMT_PreOperational_Node(void)
{
    uint8_t data[8] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(NMT_COB_ID, data, 8);
		
}

// 启动节点远程控制
void NMT_Start_Node(void) 
{
    uint8_t data[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(NMT_COB_ID, data, 8);
}

//停止节点远程控制
void NMT_Stop_Node(void)
{
	uint8_t data[8] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  sendCANMessage(NMT_COB_ID, data, 8);
}

// 切换状态
void SDO_SwitchingState(uint8_t motorId) 
{
    sendCommand(motorId, 0x2b, 0x40, 0x60, 0x00, 0x06);
}

// 继电器吸合
void SDO_RelayEngage(uint8_t motorId) 
{
    sendCommand(motorId, 0x2b, 0x40, 0x60, 0x00, 0x07);
}

// 使能
void SDO_EnableMotor(uint8_t motorId) 
{
    sendCommand(motorId, 0x2b, 0x40, 0x60, 0x00, 0x0f);
}

// 写操作模式
void SDO_OperationMode(uint8_t motorId) 
{
    sendCommand(motorId, 0x2f, 0x60, 0x60, 0x00, 0x03);
}

// 设置加速度
void SDO_Acceleration(uint8_t motorId, int acceleration) 
{
		int32_t value = (int32_t)((float)acceleration * PULSE_PER_REV / 60);
    uint8_t data[8] = {0x23, 0x83, 0x60, 0x00, 0x90, 0xD0, 0x03, 0x00};

    data[4] = value & 0xFF;
    data[5] = (value >> 8) & 0xFF;
    data[6] = (value >> 16) & 0xFF;

    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}
// 设置减速度
void SDO_Deceleration(uint8_t motorId, int deceleration) 
{
    int32_t value = (int32_t)((float)deceleration * PULSE_PER_REV / 60);
    uint8_t data[8] = {0x23, 0x84, 0x60, 0x00, 0x90, 0xD0, 0x03, 0x00};

    data[4] = value & 0xFF;
    data[5] = (value >> 8) & 0xFF;
    data[6] = (value >> 16) & 0xFF;

    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}

// 设置速度
void SDO_Speed(uint8_t motorId, int speed) 
{
    speed = speed * 10000;
    uint8_t data[8] = {0x23, 0xff, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

    data[6] = (speed >> 16) & 0xFF;
    data[5] = (speed >> 8) & 0xFF;
    data[4] = speed & 0xFF;

    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}

// 设置方向
void SDO_Direction(uint8_t motorId, int direction) 
{
    uint8_t data[8] = {0x2f, 0x7E, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

    data[6] = (direction >> 16) & 0xFF;
    data[5] = (direction >> 8) & 0xFF;
    data[4] = direction & 0xFF;

    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}

// 去使能
void SDO_DisableMotor(uint8_t motorId) 
{
    sendCommand(motorId, 0x2b, 0x40, 0x60, 0x00, 0x07);
}

// 继电器解除吸合
void SDO_RelayRelease(uint8_t motorId) 
{
    uint8_t data[8] = {0x2b, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}




