#ifndef BSP_CANOPEN_H
#define BSP_CANOPEN_H

#include "main.h"

void sendCommand(uint8_t motorId, uint8_t command, uint8_t subCommand, uint8_t objectId, uint8_t subIndex, uint8_t value);
void PDO_Motor_RPM(uint8_t motorId, int16_t rpm);// PDO���õ��ת�٣�RPM��
void PDO_Direction(uint8_t motorId,int8_t direction);//PDO-����  
void NMT_Reset_Node(void);// ��λ�ڵ�
void NMT_PreOperational_Node(void);//�ڵ����Ԥ����
void NMT_Start_Node(void) ;// �����ڵ�Զ�̿���
void NMT_Stop_Node(void);//ֹͣ�ڵ�Զ�̿���
void SDO_SwitchingState(uint8_t motorId) ;// �л�״̬
void SDO_RelayEngage(uint8_t motorId);// �̵�������
void SDO_EnableMotor(uint8_t motorId);// ʹ��
void SDO_OperationMode(uint8_t motorId);// д����ģʽ
void SDO_Acceleration(uint8_t motorId, int acceleration);// ���ü��ٶ�
void SDO_Deceleration(uint8_t motorId, int deceleration) ;
void SDO_Speed(uint8_t motorId, int speed);// �����ٶ�
void SDO_Direction(uint8_t motorId, int direction);// ���÷���
void SDO_DisableMotor(uint8_t motorId);// ȥʹ��
void SDO_RelayRelease(uint8_t motorId);// �̵����������

#endif /* BSP_CANOPEN_H */



