#include "oled_task.h"
#include "main.h"

static int count=1;//����һ���Լӱ���

uint16_t CO_content=0;   //����CO����
uint8_t Point_Number=0;    //����RFID���ݣ���ı��
uint8_t battery_level=0;    //�����ص����ٷֱ�
uint16_t battery_voltage=0;    //�����ص�ѹ
extern uint8_t Chassis_ModeFlag;
extern uint8_t EnableFlag;
extern uint8_t TargetRFID;
uint8_t Point_Target;

void oled_task(void const *pvParameters)
{
	vTaskDelay(30);
	while(1)
	{
		Point_Target=TargetRFID;
		OLED_ClearBuf();
		if(++count>1000) count=0;//����100����
		
		OLED_ShowString(65,0,(unsigned char *)"CO:",16);//��ʾ"CO"
		OLED_ShowNumber(90,0,CO_content,4,16);//��ʾ����"C0_content"
		
		OLED_ShowString(0,15,(unsigned char *)" P:",16);//��ʾ"Point"
		OLED_ShowNumber(25,15,Point_Number,2,16);//��ʾ����"Point_Number"
		
		OLED_ShowString(0,30,(unsigned char *)" M:",16);//��ʾ"Mode"
		OLED_ShowNumber(25,30,Chassis_ModeFlag,2,16);//��ʾ����"Chassis_ModeFlag"
		
		OLED_ShowString(0,45,(unsigned char *)"TP:",16);//
		OLED_ShowNumber(25,45,Point_Target,2,16);
		 
    float showvoltage_num = (float)battery_voltage / 100; // ������ת��Ϊ������		
		OLED_ShowString(65,15,(unsigned char *)" V:",16);
		oled_showfloat(showvoltage_num, 88,15,2,2);		
		
		OLED_ShowString(65,30,(unsigned char *)"RB:   %",16);
		OLED_ShowNumber(85,30,battery_level,3,16);
		
		if(EnableFlag==1)
		{
			OLED_ShowString(0,0,(unsigned char *)"EN: ON ",16);
		}
		else if(EnableFlag==0)
		{
			OLED_ShowString(0,0,(unsigned char *)"EN:OFF",16);
		}
		
		OLED_Refresh_Gram();//ˢ��
//		CO_content=0;   //����CO����
//battery_level=0;    //�����ص����ٷֱ�
//battery_voltage=0;    //�����ص�ѹ
//		OLED_ShowCHinese(0, 0, 0, 16, 16);
		vTaskDelay(200);
	}



}




