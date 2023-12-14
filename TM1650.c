#include "user.h"
const uint8_t fontCode[10] = {   
    0x3F,0x06,0x5B,0x4F,0x66,   //0-4
    0x6D,0x7D,0x07,0x7F,0x6F,   //5-9
}; 
void DelayUs(unsigned char Time)
{
	unsigned char a;
	Time>>=1;		//8M--2T
//	Time>>=2;		//4M--2T
//	Time>>=3;		//2M--2T
//	Time>>=4;		//1M--2T
	for(a=0;a<Time;a++)
	{
		NOP();
	}
} 
void DelayMs(u16 Time)
{
	u16 a,b;
	for(a=0;a<Time;a++)
	{
	 	CLRWDT(); 		
		for(b=0;b<5;b++)
		{
		 	DelayUs(198); 	
		}
	}
}                                                                                                                            

void Delay5us_TM()
{
	unsigned char i;
	i = 6;
	while (--i);
}
/**********************************************
//TM Start//起始位
**********************************************/
void TM_Start()
{
	SCL_T = 1;
	SDA_T = 1;
	Delay5us_TM();
	SDA_T = 0;
    Delay5us_TM();
}
/**********************************************
//TM Stop//结束位
**********************************************/
void TM_Stop()
{
	SCL_T = 1;
	SDA_T = 0;
	Delay5us_TM();
	SDA_T = 1;
}
/**********************************************
//TM Ack//ACK信号
**********************************************/
void TM_Ack()
{
	unsigned char timeout = 1;
    SDA_T = 1;
    Delay5us_TM();
    DISP_SDA_INPUT();
	SCL_T = 1;
	Delay5us_TM();
	// SCL_T = 0;
	while((SDA_T) && (timeout <= 200))
	{
		timeout++;
	}
	SCL_T = 0;
    Delay5us_TM();
    // SDA_T = 0;
    DISP_SDA_OUT();
    Delay5us_TM();
}
/**********************************************
// 通过总线写一个字节
**********************************************/
void Write_TM_Byte(unsigned char TM_Byte)
{
	unsigned char i;
	SCL_T = 0;
	Delay5us_TM();
	for(i=0;i<8;i++)
	{
		if(TM_Byte & 0x80)
			SDA_T = 1;
		else
			SDA_T = 0;
        Delay5us_TM();
		SCL_T = 1;
		Delay5us_TM();
        SCL_T = 0;
		Delay5us_TM();
		TM_Byte <<= 1;
	}
}
/*********************TM1650写数据************************************/
void TM_WrDat(unsigned char add,unsigned char dat)
{
	TM_Start();
	Write_TM_Byte(add);//显存地址
	TM_Ack();
	Write_TM_Byte(dat);//显示数据
	TM_Ack();
	TM_Stop();
}
/*********************TM1650写命令************************************/
void TM_WrCmd(unsigned char Bri)
{
	TM_Start();
	Write_TM_Byte(0x48);//显示模式
	TM_Ack();
	Write_TM_Byte(Bri);
	TM_Ack();
	TM_Stop();
}
void DisplayTM1650(void)
{
    TM_WrDat(0x68,DisplayArray[0]);//DIG1
	TM_WrDat(0x6a,DisplayArray[1]);//DIG2
	TM_WrDat(0x6c,DisplayArray[2]);//DIG3
	TM_WrDat(0x6e,DisplayArray[3]);//DIG4
}
/*********************TM1650初始化************************************/
void TM_Init()
{
	TM_WrCmd(0x61);//1级亮度//8段显示//开显示
    DisplayArray[0] = DisplayArray[1] = DisplayArray[2] = DisplayArray[3] = fontCode[8];
    DisplayTM1650();
    DelayMs(400);
}

void DisAllOFF_TM1650(void)
{
	DisplayArray[0] = DisplayArray[1] = DisplayArray[2] = DisplayArray[3] = 0x00;
	DisplayTM1650();
    TM_WrCmd(0x60);//1级亮度//8段显示//开显示
}

void Display_Proc(void)
{
	u8 Temp;
	if(F_DisUpdate)
	{
		F_DisUpdate = 0;
		if(bONOFF)
		{
			if(Display == 0)		//色温
			{
				Temp = (SwLevel+SW_Base)/10%10;
				DisplayArray[0] = fontCode[Temp];
				Temp = (SwLevel+SW_Base)%10;
				DisplayArray[1] = fontCode[Temp];
				DisplayArray[2] = fontCode[0];
				DisplayArray[3] = fontCode[0];
			}
			else if(Display == 1)	//亮度
			{
				DisplayArray[0] = Display_L;
				if(LmLevel >= 100)
					DisplayArray[1] = fontCode[1];
				else
					DisplayArray[1] = 0x00;
				Temp = LmLevel/10%10;
				DisplayArray[2] = fontCode[Temp];
				Temp = LmLevel%10;
				DisplayArray[3] = fontCode[Temp];
			}
			else if(Display == 2)
			{
				DisplayArray[0] = Display_H;
				DisplayArray[1] = 0x00;
				DisplayArray[2] = 0x00;
				DisplayArray[3] = fontCode[LED_MODE];
			}
		}
		else
		{
			DisplayArray[0] = Display_O;
			DisplayArray[1] = Display_F;
			DisplayArray[2] = Display_F;
			DisplayArray[3] = 0x00;
		}
		DisplayTM1650();
	}
}
