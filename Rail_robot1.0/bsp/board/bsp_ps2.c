#include "bsp_ps2.h"
#include "string.h"//strlen

#define DELAY_TIME  HAL_Delay_us(5);

//ps2���ݱ���
PS2_TypeDef ps2_val;

//Button value reading, zero time storage
//����ֵ��ȡ����ʱ�洢
uint16_t Handkey;
//Start the order. Request data
//��ʼ�����������
uint8_t Comd[2]= {0x01,0x42};
//Data store array
//���ݴ洢����
uint8_t Data[9]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//ҡ��ģ��������
#define Rocker_RX 5                
#define Rocker_RY 6
#define Rocker_LX 7
#define Rocker_LY 8

//�ڲ�ʹ�ú���
static void PS2_Cmd(uint8_t CMD);

//static uint8_t PS2_RedLight(void);
//static void PS2_Vibration(uint8_t motor1, uint8_t motor2);
//static void PS2_VibrationMode(void);

static void PS2_ReadData(void);
//static uint16_t PS2_DataKey(void);
//static uint8_t PS2_AnologData(uint8_t button);
static void PS2_ClearData(void);
static void PS2_ShortPoll(void);
static void PS2_EnterConfing(void);
static void PS2_TurnOnAnalogMode(void);
static void PS2_ExitConfing(void);
void PS2_SetInit(void);

void HAL_Delay_us(uint32_t us)//��ʱus����
{//��Ҫcubemx����ʱSYSʱ��Դѡ��SysTick
    __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks :
                    oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}

void PS2_Key_Param_Init(PS2_TypeDef *p)
{
	p->Key = 0;
	p->LX = 128;
	p->LY = 128;
	p->RX = 128;
	p->RX = 128;
	
	p->enum_error = 0;
	p->enum_state = EnumNULL;
	p->usb_ps2_ready = 0;
}

/**************************************************************************
Function: Ps2 handle initializer
Input   : none
Output  : none
�������ܣ�PS2�ֱ���ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void PS2_Init(void)
{
	/*//���ų�ʼ��
//    GPIO_InitTypeDef GPIO_InitStructure;
//	
//	//��Ӧ����ʱ�ӿ���
//	ENABLE_PS2_DI_PIN_CLOCK;
//	ENABLE_PS2_DO_PIN_CLOCK;
//	ENABLE_PS2_CS_PIN_CLOCK;
//	ENABLE_PS2_CLK_PIN_CLOCK;
//	
//	//����ģʽ����
//    GPIO_InitStructure.GPIO_Pin = PS2_DI_PIN;			//�˿�����
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//��ͨ����ģʽ
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100M
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;		//���������ݵĶ�ȡ��������Ϊ����,��ps2���γ�ʱ,��ȡ���ź�ȫΪ0;������Ϊ������ȫΪ1��
//    GPIO_Init(PS2_DI_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin = PS2_DO_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //���
//    GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;  //2M
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�����趨������ʼ��GPIO
//    GPIO_Init(PS2_DO_PORT, &GPIO_InitStructure);
//	
//    GPIO_InitStructure.GPIO_Pin = PS2_CS_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //���
//    GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;  //2M
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�����趨������ʼ��GPIO
//    GPIO_Init(PS2_CS_PORT, &GPIO_InitStructure);
//	
//    GPIO_InitStructure.GPIO_Pin = PS2_CLK_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //���
//    GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;  //2M
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�����趨������ʼ��GPIO
//    GPIO_Init(PS2_CLK_PORT, &GPIO_InitStructure);
	*/
	//PS2�ڲ�������ʼ��
	PS2_SetInit();
	
}


/**************************************************************************
Function: Read the control of the ps2 handle
Input   : none
Output  : none
�������ܣ���ȡPS2�ֱ��Ŀ�����(����ʹ�ú���)
��ڲ�������
����  ֵ����
**************************************************************************/
void PS2_Read(void)
{
    //Reading key
    //��ȡ������ֵ
		ps2_val.Key = PS2_DataKey();
    //Read the analog of the remote sensing x axis on the left
    //��ȡ���ң��X�᷽���ģ����
    ps2_val.LX=PS2_AnologData(Rocker_LX);
    //Read the analog of the directional direction of remote sensing on the left
    //��ȡ���ң��Y�᷽���ģ����
    ps2_val.LY=PS2_AnologData(Rocker_LY);
    //Read the analog of the remote sensing x axis
    //��ȡ�ұ�ң��X�᷽���ģ����
    ps2_val.RX=PS2_AnologData(Rocker_RX);
    //Read the analog of the directional direction of the remote sensing y axis
    //��ȡ�ұ�ң��Y�᷽���ģ����
    ps2_val.RY=PS2_AnologData(Rocker_RY);
}
/**************************************************************************
Function: Send commands to the handle
Input   : none
Output  : none
�������ܣ����ֱ���������
��ڲ�������
����  ֵ����
**************************************************************************/
static void PS2_Cmd(uint8_t CMD)
{
    volatile uint16_t ref=0x01;
    Data[1] = 0;
    for(ref=0x01; ref<0x0100; ref<<=1)
    {
        if(ref&CMD)
        {
            DO_H;     //Output a control bit //���һλ����λ
        }
        else DO_L;
        HAL_Delay_us(16);
        CLK_H;      //Clock lift //ʱ������
        DELAY_TIME;
        CLK_L;
        DELAY_TIME;
        CLK_H;
        HAL_Delay_us(16);
        if( DI)
            Data[1] = ref|Data[1];
        HAL_Delay_us(16);
    }
    HAL_Delay_us(16);
}
/**************************************************************************
Function: Whether it is a red light mode, 0x41= analog green light, 0x73= analog red light
Input   : none
Output  : 0: red light mode, other: other modes
�������ܣ��ж��Ƿ�Ϊ���ģʽ,0x41=ģ���̵ƣ�0x73=ģ����
��ڲ�������
����  ֵ��0�����ģʽ������������ģʽ
**************************************************************************/
//static uint8_t PS2_RedLight(void)
//{
//    CS_L;
//    PS2_Cmd(Comd[0]);  //Start orders //��ʼ����
//    PS2_Cmd(Comd[1]);  //Request data //��������
//    CS_H;
//    if( Data[1] == 0X73)   return 0 ;
//    else return 1;

//}
/**************************************************************************
Function: Read the handle data
Input   : none
Output  : none
�������ܣ���ȡ�ֱ�����
��ڲ�������
����  ֵ����
**************************************************************************/
static void PS2_ReadData(void)
{
    volatile uint8_t byte=0;
    volatile uint16_t ref=0x01;
    CS_L;
    PS2_Cmd(Comd[0]);  //Start orders //��ʼ����
    PS2_Cmd(Comd[1]);  //Request data //��������
    for(byte=2; byte<9; byte++) //Start receiving data //��ʼ��������
    {
        for(ref=0x01; ref<0x100; ref<<=1)
        {
            CLK_H;
            DELAY_TIME;
            CLK_L;
            DELAY_TIME;
            CLK_H;
            if(DI)
                Data[byte] = ref|Data[byte];
        }
        HAL_Delay_us(16);
    }
    CS_H;
}
/**************************************************************************
Function: Handle the data of the read 2 and handle only the key parts
Input   : none
Output  : 0: only one button presses the next time; No press
�������ܣ��Զ�������PS2�����ݽ��д���,ֻ����������
��ڲ�������
����  ֵ��0: ֻ��һ����������ʱ����; 1: δ����
**************************************************************************/
uint8_t ps2_default_buf[9] = {0,0x73,0,0xff,0xff,128,128,128,128};

uint16_t PS2_DataKey(void)
{
	static uint8_t last_ps2_data[9] = {0,0,0,0xff,0xff,128,128,128,128};
	static uint8_t ps2_connect_timeout = 0;
	
	//��ȡPS2�ֱ�����
    PS2_ClearData();
    PS2_ReadData();
	
	//����ps2����ģʽ(0x41)������ps2�ֱ�����ģʽ(0x20),�������ģʽ�ֱ��������Ȳ���
	//255Ϊ�쳣����,�����ֱ�ֻ�����˲��ֶ�û����ȫ����ɶ���������
	if( Data[1]==0x41 || Data[1]==0x20 || Data[1]==255 )
	{
		uint8_t save = Data[1];
		PS2_SetInit();//��ʼ��Ϊģ��ģʽ
		memcpy(Data,ps2_default_buf,9);//��һ������ʹ��ģʽ����,��ֹʧ��
		Data[1] = save;//�����ε�ֵд��,��ֹ��ʼ�����ɹ����������˸��ж�
	}
	
	//�������������ݶ���0���������һ�ε�����(ȫ0��ȫ1,���������õĳ�ʼ���������й�)
	uint8_t a=0;
	for(uint8_t i=0;i<9;i++)
	{
		a+=Data[i];
	}
	if(a==0) //����ȫ0˵���ֱ����γ�
	{
		//��״̬�¿ɵ���ps2�ֱ�δ����,����һ�����������ps2����ֵ
		ps2_connect_timeout++;
		if( ps2_connect_timeout> 10 ) 
		{   //�ֱ����ӳ�ʱ,ʹ��Ĭ�ϱ���ֹͣС������
			ps2_connect_timeout = 11, memcpy(last_ps2_data,ps2_default_buf,9);
		}			
		
		//ȫ0ʱʹ����һ�ε�����(���а���ͬʱ���²�Ϊ0)
		memcpy(Data,last_ps2_data,9);
	}
	else
	{
		ps2_connect_timeout = 0;//��ʱ��λ
	}
	
	//���汾������
	memcpy(last_ps2_data,Data,9); 
	
	//This is 16 buttons, pressed down to 0, and not pressed for 1 
	//����16������������Ϊ0��δ����Ϊ1
    Handkey=(Data[4]<<8)|Data[3]; 
	
	//ת��Ϊ ����Ϊ1,δ����Ϊ0����Ӧusb ps2���ݸ�ʽ��
	Handkey = ~Handkey;
	
	//���ؼ�ֵ���
	return Handkey;
}
/**************************************************************************
Function: Get a simulation of a rocker
Input   : Rocker
Output  : Simulation of rocker, range 0~ 256
�������ܣ��õ�һ��ҡ�˵�ģ����
��ڲ�����ҡ��
����  ֵ��ҡ�˵�ģ����, ��Χ0~256
**************************************************************************/
uint8_t PS2_AnologData(uint8_t button)
{
    return Data[button];
}
/**************************************************************************
Function: Clear data buffer
Input   : none
Output  : none
�������ܣ�������ݻ�����
��ڲ�������
����  ֵ����
**************************************************************************/
static void PS2_ClearData(void)
{
    uint8_t a;
    for(a=0; a<9; a++)
        Data[a]=0x00;
}
/******************************************************
Function: Handle vibration function
Input   : motor1: the right small vibrator, 0x00, other
          motor2: the left big shock motor 0x40~ 0xff motor is open, and the value is greater
Output  : none
�������ܣ��ֱ��𶯺���
��ڲ�����motor1:�Ҳ�С�𶯵�� 0x00�أ�������
	        motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
����  ֵ����
******************************************************/
//static void PS2_Vibration(uint8_t motor1, uint8_t motor2)
//{
//    CS_L;
//    HAL_Delay_us(16);
//    PS2_Cmd(0x01); //Start order //��ʼ����
//    PS2_Cmd(0x42); //Request data //��������
//    PS2_Cmd(0X00);
//    PS2_Cmd(motor1);
//    PS2_Cmd(motor2);
//    PS2_Cmd(0X00);
//    PS2_Cmd(0X00);
//    PS2_Cmd(0X00);
//    PS2_Cmd(0X00);
//    CS_H;
//    HAL_Delay_us(16);
//}
/**************************************************************************
Function: Short press
Input   : none
Output  : none
�������ܣ��̰�
��ڲ�������
����  ֵ����
**************************************************************************/
static void PS2_ShortPoll(void)
{
    CS_L;
    HAL_Delay_us(16);
    PS2_Cmd(0x01);
    PS2_Cmd(0x42);
    PS2_Cmd(0X00);
    PS2_Cmd(0x00);
    PS2_Cmd(0x00);
    CS_H;
    HAL_Delay_us(16);
}
/**************************************************************************
Function: Enter configuration
Input   : none
Output  : none
�������ܣ���������
��ڲ�������
����  ֵ����
**************************************************************************/
static void PS2_EnterConfing(void)
{
    CS_L;
    HAL_Delay_us(16);
    PS2_Cmd(0x01);
    PS2_Cmd(0x43);
    PS2_Cmd(0X00);
    PS2_Cmd(0x01);
    PS2_Cmd(0x00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    CS_H;
    HAL_Delay_us(16);
}
/**************************************************************************
Function: Send mode Settings
Input   : none
Output  : none
�������ܣ�����ģʽ����
��ڲ�������
����  ֵ����
**************************************************************************/
static void PS2_TurnOnAnalogMode(void)
{
    CS_L;
    PS2_Cmd(0x01);
    PS2_Cmd(0x44);
    PS2_Cmd(0X00);
    PS2_Cmd(0x01); //analog=0x01;digital=0x00  Software Settings send mode ������÷���ģʽ
    PS2_Cmd(0x03); //0x03 lock storage setup, which cannot be set by the key "mode" set mode. //0x03�������ã�������ͨ��������MODE������ģʽ��
    //0xee non-locking software Settings can be set by the key "mode" set mode.//0xEE������������ã���ͨ��������MODE������ģʽ��
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    CS_H;
    HAL_Delay_us(16);
}
/**************************************************************************
Function: Vibration setting
Input   : none
Output  : none
�������ܣ�������
��ڲ�������
����  ֵ����
**************************************************************************/
//static void PS2_VibrationMode(void)
//{
//    CS_L;
//    HAL_Delay_us(16);
//    PS2_Cmd(0x01);
//    PS2_Cmd(0x4D);
//    PS2_Cmd(0X00);
//    PS2_Cmd(0x00);
//    PS2_Cmd(0X01);
//    CS_H;
//    HAL_Delay_us(16);
//}
/**************************************************************************
Function: Complete and save the configuration
Input   : none
Output  : none
�������ܣ���ɲ���������
��ڲ�������
����  ֵ����
**************************************************************************/
static void PS2_ExitConfing(void)
{
    CS_L;
    HAL_Delay_us(16);
    PS2_Cmd(0x01);
    PS2_Cmd(0x43);
    PS2_Cmd(0X00);
    PS2_Cmd(0x00);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    CS_H;
    HAL_Delay_us(16);
}
/**************************************************************************
Function: Handle configuration initialization
Input   : none
Output  : none
�������ܣ��ֱ����ó�ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void PS2_SetInit(void)
{
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_EnterConfing();		  //Enter configuration mode //��������ģʽ
    PS2_TurnOnAnalogMode();	//The "traffic light" configuration mode and select whether to save //�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
    //PS2_VibrationMode();	//Open vibration mode //������ģʽ
    PS2_ExitConfing();		  //Complete and save the configuration //��ɲ���������
}



