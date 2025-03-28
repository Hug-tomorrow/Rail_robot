#include "bsp_canopen.h"
#include "main.h"

// ���峣��
#define BASE_COB_ID 0x0600
#define NMT_COB_ID 0x0000
#define PULSE_PER_REV     10000       // ÿȦ������

/* CANopen֡ID���� */
#define RPDO1_COBID       0x200 // RPDO1��COB-ID
#define RPDO2_COBID       0x300 // RPDO2��COB-ID
#define NMT_COBID         0x0000            // NMT����֡ID

/**
  * @brief          ���͵����������
  * @param[in]      motorId: ���ID������ָ��Ŀ����
  * @param[in]      command: �����ֽڣ�����ָ����������
  * @param[in]      subCommand: �������ֽڣ�����ָ�������������
  * @param[in]      objectId: �����ֵ�ID������ָ�������Ŀ�����
  * @param[in]      subIndex: ������������ָ�������ֵ��е�������
  * @param[in]      value: ����ֵ������ָ������ľ�����ֵ
  * @retval         none
  */
void sendCommand(uint8_t motorId, uint8_t command, uint8_t subCommand, uint8_t objectId, uint8_t subIndex, uint8_t value) 
{
    uint8_t data[8] = {command, subCommand, objectId, 0x00, value, 0x00, 0x00, 0x00};
    sendCANMessage(BASE_COB_ID + motorId, data, 8);
//		for (int i = 0; i < 8; i++) {
////    any_printf(USART3, "%02X ", data[i]);  // ��16���Ƹ�ʽ��ӡÿ���ֽ�
//}
}


// PDO���õ��ת�٣�RPM��0-3000
void PDO_Motor_RPM(uint8_t motorId, int16_t rpm) 
{
    // ת��ת����ʽ������/�� = (rpm * 10000) / 60
    int32_t target_speed = (int32_t)((float)rpm * PULSE_PER_REV / 60);
    
    // �޷������������������ֲ����ã�
    const int32_t MAX_SPEED = 500000; // ����ٶ�����
    if (target_speed > MAX_SPEED) target_speed = MAX_SPEED;
    else if (target_speed < 0) target_speed = 0;
    
    // ���ݸ�ʽ��С��ģʽд��Ŀ���ٶȣ���λ������/�룩
    uint8_t data[8] = {0};
    data[0] = target_speed & 0xFF;
    data[1] = (target_speed >> 8) & 0xFF;
		data[2] = (target_speed >> 16) & 0xFF;
    
    // �����ٶ�ָ��
    sendCANMessage(RPDO2_COBID + motorId, data, 4);
}

void PDO_Direction(uint8_t motorId,int8_t direction)//PDO-����    
{
	uint8_t data[8] = {0};
  data[0] = direction;
	sendCANMessage(RPDO1_COBID + motorId, data, 1);
}

// ��λ�ڵ�
void NMT_Reset_Node(void) 
{
    uint8_t data[8] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(NMT_COB_ID, data, 8);
		
}

//�ڵ����Ԥ����
void NMT_PreOperational_Node(void)
{
    uint8_t data[8] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(NMT_COB_ID, data, 8);
		
}

// �����ڵ�Զ�̿���
void NMT_Start_Node(void) 
{
    uint8_t data[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(NMT_COB_ID, data, 8);
}

//ֹͣ�ڵ�Զ�̿���
void NMT_Stop_Node(void)
{
	uint8_t data[8] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  sendCANMessage(NMT_COB_ID, data, 8);
}

// �л�״̬
void SDO_SwitchingState(uint8_t motorId) 
{
    sendCommand(motorId, 0x2b, 0x40, 0x60, 0x00, 0x06);
}

// �̵�������
void SDO_RelayEngage(uint8_t motorId) 
{
    sendCommand(motorId, 0x2b, 0x40, 0x60, 0x00, 0x07);
}

// ʹ��
void SDO_EnableMotor(uint8_t motorId) 
{
    sendCommand(motorId, 0x2b, 0x40, 0x60, 0x00, 0x0f);
}

// д����ģʽ
void SDO_OperationMode(uint8_t motorId) 
{
    sendCommand(motorId, 0x2f, 0x60, 0x60, 0x00, 0x03);
}

// ���ü��ٶ�
void SDO_Acceleration(uint8_t motorId, int acceleration) 
{
		int32_t value = (int32_t)((float)acceleration * PULSE_PER_REV / 60);
    uint8_t data[8] = {0x23, 0x83, 0x60, 0x00, 0x90, 0xD0, 0x03, 0x00};

    data[4] = value & 0xFF;
    data[5] = (value >> 8) & 0xFF;
    data[6] = (value >> 16) & 0xFF;

    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}
// ���ü��ٶ�
void SDO_Deceleration(uint8_t motorId, int deceleration) 
{
    int32_t value = (int32_t)((float)deceleration * PULSE_PER_REV / 60);
    uint8_t data[8] = {0x23, 0x84, 0x60, 0x00, 0x90, 0xD0, 0x03, 0x00};

    data[4] = value & 0xFF;
    data[5] = (value >> 8) & 0xFF;
    data[6] = (value >> 16) & 0xFF;

    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}

// �����ٶ�
void SDO_Speed(uint8_t motorId, int speed) 
{
    speed = speed * 10000;
    uint8_t data[8] = {0x23, 0xff, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

    data[6] = (speed >> 16) & 0xFF;
    data[5] = (speed >> 8) & 0xFF;
    data[4] = speed & 0xFF;

    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}

// ���÷���
void SDO_Direction(uint8_t motorId, int direction) 
{
    uint8_t data[8] = {0x2f, 0x7E, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

    data[6] = (direction >> 16) & 0xFF;
    data[5] = (direction >> 8) & 0xFF;
    data[4] = direction & 0xFF;

    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}

// ȥʹ��
void SDO_DisableMotor(uint8_t motorId) 
{
    sendCommand(motorId, 0x2b, 0x40, 0x60, 0x00, 0x07);
}

// �̵����������
void SDO_RelayRelease(uint8_t motorId) 
{
    uint8_t data[8] = {0x2b, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
    sendCANMessage(BASE_COB_ID + motorId, data, 8);
}




