#include "bsp_oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "stdint.h"//unit8

static uint8_t OLED_GRAM[128][8];
/**************************************************************************
Function: Refresh the OLED screen
Input   : Dat: data/command to write, CMD: data/command flag 0, represents the command;1, represents data
Output  : none
函数功能：向OLED写入一个字节
入口参数：dat:要写入的数据/命令，cmd:数据/命令标志 0,表示命令;1,表示数据
返回  值：无
**************************************************************************/
static void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
    uint8_t i;
    if(cmd)
        OLED_RS_Set();
    else
        OLED_RS_Clr();
    for(i=0; i<8; i++)
    {
        OLED_SCLK_Clr();
        if(dat&0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat<<=1;
    }
    OLED_RS_Set();
}

/**************************************************************************
Function: Refresh the OLED screen
Input   : none
Output  : none
函数功能：刷新OLED屏幕
入口参数：无
返回  值：无
**************************************************************************/
void OLED_Refresh_Gram(void)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);    //Set page address (0~7) //设置页地址（0~7）
        OLED_WR_Byte (0x00,OLED_CMD);      //Set the display location - column low address //设置显示位置—列低地址
        OLED_WR_Byte (0x10,OLED_CMD);      //Set the display location - column height address //设置显示位置—列高地址
        for(n=0; n<128; n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
    }
}

/**************************************************************************
Function: Screen clear function, clear the screen, the entire screen is black, and did not light up the same
Input   : none
Output  : none
函数功能：清屏函数,清完屏,整个屏幕是黑色的，和没点亮一样
入口参数：无
返回  值：无
**************************************************************************/
void OLED_Clear(void)
{
    uint8_t i,n;
    for(i=0; i<8; i++)for(n=0; n<128; n++)OLED_GRAM[n][i]=0X00;
    OLED_Refresh_Gram(); //Update the display //更新显示
}

void OLED_ClearBuf(void)
{
    uint8_t i,n;
    for(i=0; i<8; i++)for(n=0; n<128; n++)OLED_GRAM[n][i]=0X00;
}
/**************************************************************************
Function: Draw point
Input   : x,y: starting coordinate;T :1, fill,0, empty
Output  : none
函数功能：画点
入口参数：x,y :起点坐标; t:1,填充,0,清空
返回  值：无
**************************************************************************/
static void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
    uint8_t pos,bx,temp=0;
    if(x>127||y>63)return;//超出范围了.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;
}

/**************************************************************************
Function: Set the coordinates (position) displayed on the screen.
Input   : x, y: starting point coordinates
Output  : none
函数功能：设置汉字在屏幕上显示的坐标（位置）
入口参数: x,y :起点坐标
返回  值：无
**************************************************************************/
static void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}

/**************************************************************************
Function: Find m to the NTH power
Input   : m: base number, n: power number
Output  : none
函数功能：求m的n次方的函数
入口参数：m：底数，n：次方数
返回  值：无
**************************************************************************/
static uint32_t oled_pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}


/**************************************************************************
Function: Initialize the OLED
Input   : none
Output  : none
函数功能：初始化OLED
入口参数: 无
返回  值：无
**************************************************************************/
void OLED_Init(void)
{
/*    GPIO_InitTypeDef GPIO_InitStructure;

    ENABLE_OLED_PIN_CLOCK;
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(OLED_SCL_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(OLED_SDA_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = OLED_RES_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(OLED_RES_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = OLED_DC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(OLED_DC_PORT, &GPIO_InitStructure);
	
//	//特殊IO:PC13、14、15的处理
//	//Turn off the external low speed external clock signal function, PC13, PC14, PC15 can be used as normal IO
//    //关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//    PWR_BackupAccessCmd(ENABLE);
//    RCC_LSEConfig(RCC_LSE_OFF);
//    PWR_BackupAccessCmd(DISABLE);
*/

//0.96寸OLED（128×64像素），SSD1306 x的范围是0~127，y的范围是0~7。

    OLED_RST_Clr();
    HAL_Delay(100);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE,OLED_CMD); //Close display //关闭显示
    OLED_WR_Byte(0xD5,OLED_CMD); //The frequency frequency factor, the frequency of the shock //设置时钟分频因子,震荡频率
    OLED_WR_Byte(80,OLED_CMD);   //[3:0], the frequency dividing factor;[7:4], oscillation frequency //[3:0],分频因子;[7:4],震荡频率
    OLED_WR_Byte(0xA8,OLED_CMD); //Set the number of driver paths //设置驱动路数
    OLED_WR_Byte(0X3F,OLED_CMD); //Default 0x3f(1/64) //默认0X3F(1/64)
    OLED_WR_Byte(0xD3,OLED_CMD); //Setting display deviation //设置显示偏移
    OLED_WR_Byte(0X00,OLED_CMD); //Default is 0//默认为0

    OLED_WR_Byte(0x40,OLED_CMD); //Sets the number of rows to display starting line [5:0] //设置显示开始行 [5:0],行数

    OLED_WR_Byte(0x8D,OLED_CMD); //Charge pump setup //电荷泵设置
    OLED_WR_Byte(0x14,OLED_CMD); //Bit2, on/off //bit2，开启/关闭
    OLED_WR_Byte(0x20,OLED_CMD); //Set up the memory address mode //设置内存地址模式
    OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00, column address mode;01, line address mode;10. Page address mode;The default 10; //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_WR_Byte(0xA1,OLED_CMD); //Segment redefine setting,bit0:0,0- >;0;1, 0 - & gt;127; //段重定义设置,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC0,OLED_CMD); //Set the COM scan direction;Bit3:0, normal mode;1, Re-define schema COM[n-1]- >;COM0;N: Number of driving paths//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    OLED_WR_Byte(0xDA,OLED_CMD); //Set the COM hardware pin configuration //设置COM硬件引脚配置
    OLED_WR_Byte(0x12,OLED_CMD); //[5:4]configuration //[5:4]配置

    OLED_WR_Byte(0x81,OLED_CMD); //Contrast Settings //对比度设置
    OLED_WR_Byte(0xEF,OLED_CMD); //1~ 255; Default 0x7f (brightness Settings, the bigger the brighter) //1~255;默认0X7F (亮度设置,越大越亮)
    OLED_WR_Byte(0xD9,OLED_CMD); //Set the pre-charging cycle //设置预充电周期
    OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB,OLED_CMD); //Setting vcomh voltage multiplier//设置VCOMH 电压倍率
    OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WR_Byte(0xA4,OLED_CMD); //Global display; Bit0:1, open; 0, close; (white screen/black screen)//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_WR_Byte(0xA6,OLED_CMD); //Settings display mode; Bit0:1, anti-phase display; 0, normal display//设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_WR_Byte(0xAF,OLED_CMD); //Open display //开启显示
    OLED_Clear();
}

/**************************************************************************
Function: Displays a character, including partial characters, at the specified position
Input   : x,y: starting coordinate;Len: The number of digits;Size: font size;Mode :0, anti-white display,1, normal display
Output  : none
函数功能：在指定位置显示一个字符,包括部分字符
入口参数：x,y :起点坐标; len :数字的位数; size:字体大小; mode:0,反白显示,1,正常显示
返回  值：无
**************************************************************************/
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{
    uint8_t temp,t,t1;
    uint8_t y0=y;
    chr=chr-' '; //Get the offset value //得到偏移后的值
    for(t=0; t<size; t++)
    {
        if(size==12)temp=oled_asc2_1206[chr][t];  //Invoke 1206 font   //调用1206字体
        else temp=oled_asc2_1608[chr][t];		      //Invoke the 1608 font //调用1608字体
        for(t1=0; t1<8; t1++)
        {
            if(temp&0x80)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
}


/**************************************************************************
Function: Displays 2 numbers
Input   : x,y: starting coordinate;Len: The number of digits;Size: font size;Mode: mode, 0, fill mode, 1, overlay mode;Num: value (0 ~ 4294967295);
Output  : none
函数功能：显示2个数字
入口参数：x,y :起点坐标; len :数字的位数; size:字体大小; mode:模式, 0,填充模式, 1,叠加模式; num:数值(0~4294967295);
返回  值：无
**************************************************************************/
void OLED_ShowNumber(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0; t<len; t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
                continue;
            } else enshow=1;

        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1);
    }
}
/**************************************************************************
Function: Display string
Input   : x,y: starting coordinate;*p: starting address of the string
Output  : none
函数功能：显示字符串
入口参数：x,y :起点坐标; *p:字符串起始地址
返回  值：无
**************************************************************************/
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58
    while(*p!='\0')
    {
        if(x>MAX_CHAR_POSX) {
            x=0;
            y+=16;
        }
        if(y>MAX_CHAR_POSY) {
            y=x=0;
            OLED_Clear();
        }
        OLED_ShowChar(x,y,*p,size,1);
        x+=8;
        p++;
    }
}

/**************************************************************************
Function: Display character
Input   : x: indicates the horizontal coordinates displayed; Y: the vertical coordinates that show the display;
          no: the line number in the array of the Chinese character (module) in the hzk-and "array", which is determined by the line number to determine the characters shown in the array,
          The value of the width of the font here must be consistent with the size of the dot matrix value of the use of the word mold.
          font_height: the font is high for the use of the word mold, because my screen pixels are 32hours, 128----0~ 7, and four bits per page
Output  : none
Note: this method is used to show that the Chinese character must satisfy the size of the word that the word model generates the software to generate the same size as the dot matrix
函数功能：显示汉字
入口参数: x：表示显示的水平坐标; y: 表示显示的垂直坐标;
          no: 表示要显示的汉字（模组）在hzk[][]数组中的行号,通过行号来确定在数组中要显示的汉字,
              这里字体的宽font_width的值必须与用字模制作软件生成字模时的点阵值大小一致;
          font_height:为用字模制作软件生成字模时字体的高,由于我的屏像素为32*128-----0~7共8页，每页4个位
返回  值：无
注意：用这种方法来显示汉字一定要满足用字模生成软件生成的字宽与点阵大小相同才行，否者容易乱码
**************************************************************************/
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no,uint8_t font_width,uint8_t font_height)
{
    uint8_t t, i;
    for(i=0; i<(font_height/8); i++)	//The maximum height of font_height is 32. this screen is only 8 pages (line), four digits per page
        //font_height最大值为32，这张屏只有8个页（行），每页4个位
    {
        OLED_Set_Pos(x,y+i);
        for(t=0; t<font_width; t++)		//The maximum value of font_width is 128. the screen is only that large
            //font_width最大值为128，屏幕只有这么大
        {
            OLED_WR_Byte(Hzk16[(font_height/8)*no+i][t],OLED_DATA);
        }
    }
}

//浮点数分解
static int* FenJie_float(const float fudian)
{
    static int tmp[3];
    float temp;
    temp = fudian;
    if(temp<0) temp = -temp,tmp[2]=-1;
    else tmp[2]=1;

    tmp[0] = (int)temp; 			//获取整数部分
    tmp[1] = (temp - tmp[0])*1000;  //小数部分
    return tmp;
}

//函数功能：OLED显示浮点数
//入口参数：需要显示的浮点数、x坐标、y坐标、整数显示的个数、小数显示的个数
//返回值：无
void oled_showfloat(const float needtoshow,uint8_t show_x,uint8_t show_y,uint8_t zs_num,uint8_t xs_num)
{
     static int* p;
    p = FenJie_float(needtoshow);
    // 去掉符号位显示部分

    // 整数部分
    OLED_ShowNumber(show_x, show_y, p[0], zs_num, 16);
    show_x = show_x + zs_num * 8;
    // 小数点
    OLED_ShowChar(show_x, show_y, '.', 16, 1);
    show_x = show_x + 8;
    // 小数部分
    if (p[1] < 100) { // 需要补零
        if (xs_num == 3) {
            OLED_ShowNumber(show_x, show_y, 0, 1, 16);  // 补第一个0
            show_x = show_x + 8;
            if (p[1] >= 10) {
                OLED_ShowNumber(show_x, show_y, p[1], 2, 16);  // 后两位不需要补0
                show_x = show_x + 16;
            } else {
                OLED_ShowNumber(show_x, show_y, 0, 1, 16); // 补第二个0
                show_x = show_x + 8;
                OLED_ShowNumber(show_x, show_y, p[1], 1, 16);
                show_x = show_x + 8;
            }
        } else {
            if (p[1] >= 0 && p[1] < 100) {
                OLED_ShowNumber(show_x, show_y, 0, 1, 16);  // 补第一个0
                show_x = show_x + 8;
            }
            OLED_ShowNumber(show_x, show_y, p[1] / 10, 1, 16);
            show_x = show_x + 8;
        }
    } else {
        if (xs_num == 3) {
            OLED_ShowNumber(show_x, show_y, p[1], 3, 16);
            show_x = show_x + 24;
        } else {
            OLED_ShowNumber(show_x, show_y, p[1] / 10, 2, 16);
            show_x = show_x + 16;
        }
    }
}
/*
//函数功能：OLED显示浮点数
//入口参数：需要显示的浮点数、x坐标、y坐标、整数显示的个数、小数显示的个数
//返回值：无
void oled_showfloat(const float needtoshow,uint8_t show_x,uint8_t show_y,uint8_t zs_num,uint8_t xs_num)
{
    static int* p;
    p = FenJie_float(needtoshow);
    //符号位显示
    if(p[2]>0) OLED_ShowChar(show_x,show_y,'+',12,1);
    else OLED_ShowChar(show_x,show_y,'-',12,1);
    //整数部分
    OLED_ShowNumber(show_x+8,show_y,p[0],zs_num,12);
    //小数点
    OLED_ShowChar(show_x+6+8*zs_num,show_y,'.',12,1);
    //小数部分
    if(p[1]<100)//需要补零
    {
        if(xs_num==3)
        {
            OLED_ShowNumber(show_x+12+8*zs_num,show_y,0,1,12);	 //补第一个0
            if(p[1]>=10)
                OLED_ShowNumber(show_x+18+8*zs_num,show_y,p[1],2,12);	 //后两位不需要补0
            else
                OLED_ShowNumber(show_x+18+8*zs_num,show_y,0,1,12), //补第二个0
                                OLED_ShowNumber(show_x+24+8*zs_num,show_y,p[1],1,12);
        }
        else
        {
            if(p[1]>=0&&p[1]<100)
                OLED_ShowNumber(show_x+12+8*zs_num,show_y,0,1,12);	 //补第一个0
            OLED_ShowNumber(show_x+18+8*zs_num,show_y,p[1]/10,1,12);
        }
    }
    else
    {
        if(xs_num==3)
        {
            OLED_ShowNumber(show_x+12+8*zs_num,show_y,p[1],3,12);
        }
        else
            OLED_ShowNumber(show_x+12+8*zs_num,show_y,p[1]/10,2,12);
    }
}
*/
//画图函数
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[])
{ 	
 	unsigned int j=0; //定义变量
 	unsigned char x,y; //定义变量
  
 	if(y1%8==0) y=y1/8;   //判断终止页是否为8的整数倍
 	 else y=y1/8+1;

	for(y=y0;y<y1;y++) //从起始页开始，画到终止页
	{
		OLED_Set_Pos(x0,y); //在页的起始列开始画
		for(x=x0;x<x1;x++) //画x1 - x0 列
			{
				OLED_WR_Byte(BMP[j++],OLED_DATA);	//画图片的点    	
			}
	}
}

void OLED_Refresh_Line(void)
{
	uint8_t i,n;		    
	for(i=0;i<2;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //Set page address (0~7) //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //Set the display location - column low address //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //Set the display location - column height address //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//usb bmp
const unsigned char gImage_usb_bmp[] = { /* 0X72,0X01,0X00,0X40,0X00,0X3F, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XC0,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,
0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0XE0,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XE0,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0XF8,0X08,
0X0C,0X0C,0X0C,0XFF,0X00,0X00,0XC0,0XC0,0X80,0X80,0X80,0X80,0XC0,0XE0,0XB0,0X98,
0X88,0X88,0X88,0X9C,0X9C,0X9C,0X80,0X80,0XC0,0XC0,0X80,0X80,0X00,0X00,0XFF,0X01,
0X01,0X3D,0X25,0X25,0X3D,0X01,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X07,0X08,
0X08,0X08,0X08,0XFF,0X00,0X00,0X01,0X01,0X00,0X00,0X00,0X01,0X03,0X06,0X0C,0X0C,
0X0C,0X0C,0X0C,0X0C,0X00,0X00,0X00,0X02,0X01,0X01,0X00,0X00,0X00,0X00,0XFF,0X40,
0X40,0X5E,0X52,0X52,0X5E,0X40,0X7F,0X7F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X01,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};



