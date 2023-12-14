#ifndef _USER_
#define _USER_
#include <ny8.h>
#include "NY8_constant.h"

#define	EE_TO_1ms   0x00
#define	EE_TO_2ms	0x02
#define	EE_TO_4ms	0x01
#define	EE_TO_8ms	0x03
#define	EE_TO_16ms	0x04
#define	EE_TO_32ms	0x05

/*---------------- 变量类型 ----------------*/
#define  uint8_t    unsigned char 
#define  uint16_t   unsigned int
#define  uint32_t   unsigned long
typedef unsigned char uchar;
typedef unsigned int uint;
typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;
extern volatile uint8_t Main_Time;

#define UPDATE_REG(x)	__asm__("MOVR _" #x ",F")
#define         false       0
#define         true        1

/*-------- IO口配置 --------*/
#define     PI_A        PORTBbits.PB5
#define     PI_B        PORTBbits.PB4
#define     PI_C        PORTAbits.PA0
#define     PI_IR       PORTAbits.PA1
#define     PI_AC       PORTAbits.PA6

#define     PO_TEST     PORTBbits.PB0
#define     PO_TEST2     PORTBbits.PB3
#define     PO_W        PORTAbits.PA4
#define     PO_Y        PORTAbits.PA2
#define     PO_FAN      PORTAbits.PA7
#define     PO_DIN      PORTBbits.PB1
#define     PO_CLK      PORTBbits.PB2
// #define     PO_STB      PORTBbits.PB1

#define     SDA_T       PORTBbits.PB1
#define     SCL_T       PORTBbits.PB2

#define   DISP_SDA_OUT()        {IOSTB &= 0xFD;}
#define   DISP_SDA_INPUT()      {IOSTB |= 0x02;}

#define     W_PWM_Duty  PWM2DUTY
#define     Y_PWM_Duty  PWM3DUTY
#define         W_PWM_OFF()      (P2CR1 &= 0x7F)
#define         Y_PWM_OFF()      (P3CR1 &= 0x7F)
#define         W_PWM_ON()       (P2CR1 |= 0x80)    //PWM2
#define         Y_PWM_ON()       (P3CR1 |= 0x80)    //PWM3

/*pwm*/
#define     PWM_MAX     200
#define     LM_Level    100
#define     SW_Level    38

#define     Speed_LM            25          //亮度：≈100级
#define     Speed_SW            8           //色温：33级
#define     Speed_Breathe       1           //呼吸
#define     Speed_HalfBreathe   2           //中性光呼吸
#define     Speed_Flash         50          //闪烁

#define     MAX_TUBE_NUM		4	    //数码管最大位数
#define     Display_L           0x38
#define     Display_H           0x76
#define     Display_O           0x3F
#define     Display_F           0x71
//亮度色温
#define     SW_Base             27
#define     SW_MAX              38  //2700+3300=6500
#define     SW_WY               19  //2700+1100=4600
#define     SW_MIN              0
#define     LM_MAX              100
#define     LM_Half              50
#define     LM_MIN              5

/*---------------- 通信接收 ----------------*/
typedef union _CommType
{
	uint32_t All;
	uint8_t One[3];
}CommType;
// extern CommType PairCode;

typedef union _AddrType
{
    uint16_t All;
    uint8_t One[2];
}AddrType;
extern AddrType PairCode;

/*------------------ 标志位 ------------------*/
typedef union 
{
    u8 byte;
    struct 
    {
       u8 b0:1;
       u8 b1:1;
       u8 b2:1;
       u8 b3:1;
       u8 b4:1;
       u8 b5:1;
	   u8 b6:1;
       u8 b7:1;
    }bits;
}FLAG;

extern volatile FLAG A_USER_FLAG;
#define A_USER_FLAG_VALUE       A_USER_FLAG.byte
#define bTMR1ms                 A_USER_FLAG.bits.b0
#define bTMR2ms                 A_USER_FLAG.bits.b1
#define bTMR8ms                 A_USER_FLAG.bits.b2
#define bKeyChange              A_USER_FLAG.bits.b3
#define bWrite_Done             A_USER_FLAG.bits.b4
#define bCode_One                  A_USER_FLAG.bits.b5
#define bONOFF                  A_USER_FLAG.bits.b6
#define bJump                   A_USER_FLAG.bits.b7

extern volatile FLAG B_USER_FLAG;
#define B_USER_FLAG_VALUE       B_USER_FLAG.byte
#define F_LongRec               B_USER_FLAG.bits.b0
#define F_A_HIGHT               B_USER_FLAG.bits.b1
#define F_B_HIGHT               B_USER_FLAG.bits.b2
#define F_A_Status              B_USER_FLAG.bits.b3
#define F_B_Status              B_USER_FLAG.bits.b4
#define F_Start_B               B_USER_FLAG.bits.b5
#define F_Start_A               B_USER_FLAG.bits.b6
#define F_DisUpdate             B_USER_FLAG.bits.b7

extern volatile FLAG C_USER_FLAG;
#define C_USER_FLAG_VALUE       C_USER_FLAG.byte
#define AUTO_ON_POWER           C_USER_FLAG.bits.b0
#define F_RecStart               C_USER_FLAG.bits.b1
#define F_Change               C_USER_FLAG.bits.b2
#define F_PairCode              C_USER_FLAG.bits.b3
#define F_RightCode              C_USER_FLAG.bits.b4
#define F_StatusL               C_USER_FLAG.bits.b5
#define F_RecOne               C_USER_FLAG.bits.b6
#define Read_EEPROM             C_USER_FLAG.bits.b7

extern volatile FLAG D_USER_FLAG;
#define D_USER_FLAG_VALUE       D_USER_FLAG.byte
#define bAch                    D_USER_FLAG.bits.b0
#define bAcOne                  D_USER_FLAG.bits.b1
#define ON_OFF_FLASH            D_USER_FLAG.bits.b2

extern uint8_t RecCode_RAM;


/*-------- main.c --------*/
extern volatile u8 Display;
extern volatile u8 DisRAM;
/*-------- sys.c --------*/
void USER_Init(void);
void IO_Init();
void Timer_Init(void);
void PWM_Init();
void Var_Init();
void Set_DDram();
uint8_t Power_On_Test();

/*-------- key.c --------*/
void Scan_Key(void);
void Scan_A_B(void);
/*-------- ac.c --------*/
void AC_Check(void);
void AC_Proc(void);
/*-------- led.c --------*/
void SET_Duty(void);
void ON_Light(void);
void OFF_Light(void);
void Luminance_UP(void);
void Luminance_DOWN(void);
void Color_UP(void);
void Color_DOWN(void);
void SW_Change(void);
void MODE_Change(void);
void MODE_UP(void);
void MODE_DOWN(void);
void Breathe_SW();
void Set_PWM_Duty();
extern u8 speed_cnt;

/*-------- TM1650.c --------*/
void TM_Init();
void DisplayTM1650(void);
void DisAllOFF_TM1650(void);
void Display_Proc(void);
void TM_WrCmd(unsigned char Bri);

extern u8 LED_MODE;
extern u8 W_Target;
extern u8 Y_Target;
extern u8 W_Cur;
extern u8 Y_Cur;
extern u8 LmRAM;
extern u8 SwRAM;
extern u8 LmLevel;
extern u8 SwLevel;
extern u8 ChangeSpeed;
extern uint8_t DisplayArray[MAX_TUBE_NUM];

/*-------- communicate.c --------*/
//void InfraredReception(void);
void RecServer(void);
void Communicate(void);

void PairCode_Deal(void);
void AUTO_ON_LIGHT(void);

void Read_EEPROM_Data(void);
void Write_EEPROM_Data(void);
void Read_Data(void);

#define     ADDR_CODE           0x1794

#endif