#ifndef BSP_CANOPEN_H
#define BSP_CANOPEN_H

#include "main.h"

void sendCommand(uint8_t motorId, uint8_t command, uint8_t subCommand, uint8_t objectId, uint8_t subIndex, uint8_t value);
void PDO_Motor_RPM(uint8_t motorId, int16_t rpm);// PDO设置电机转速（RPM）
void PDO_Direction(uint8_t motorId,int8_t direction);//PDO-方向  
void NMT_Reset_Node(void);// 复位节点
void NMT_PreOperational_Node(void);//节点进入预操作
void NMT_Start_Node(void) ;// 启动节点远程控制
void NMT_Stop_Node(void);//停止节点远程控制
void SDO_SwitchingState(uint8_t motorId) ;// 切换状态
void SDO_RelayEngage(uint8_t motorId);// 继电器吸合
void SDO_EnableMotor(uint8_t motorId);// 使能
void SDO_OperationMode(uint8_t motorId);// 写操作模式
void SDO_Acceleration(uint8_t motorId, int acceleration);// 设置加速度
void SDO_Deceleration(uint8_t motorId, int deceleration) ;
void SDO_Speed(uint8_t motorId, int speed);// 设置速度
void SDO_Direction(uint8_t motorId, int direction);// 设置方向
void SDO_DisableMotor(uint8_t motorId);// 去使能
void SDO_RelayRelease(uint8_t motorId);// 继电器解除吸合

#endif /* BSP_CANOPEN_H */



