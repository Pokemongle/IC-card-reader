#include<reg52.h>
#include<intrins.h>
#include"public.h"
#include"uart.h"
#include"pn532.h"
unsigned char gTimeMult;
extern unsigned char Lcdbuf1[];
extern unsigned char Lcdbuf2[];
extern unsigned char cntlcd1;
extern unsigned char cntlcd2;
//将收到的16进制码每一位变成相应的ASCII码显示在Lcd上
void hextoascii(unsigned char dat)
{
	unsigned char DH;
	unsigned char DL;
	DH=dat/16;
	DL=dat%16;
	if(0x00<=DH&&DH<=0x09)
	{
	 	DH='0'+DH;
	}
	else
	{
	 	DH=DH-0x0A+'A';
	}
	if(cntlcd1>15)
	{
	 	Lcdbuf2[cntlcd2++]=DH;	
	}
	else
	{
	 	Lcdbuf1[cntlcd1++]=DH;
	}
	
	if(0x00<=DL&&DL<=0x09)
	{
	 	DL='0'+DL;
	}
	else
	{
	 	DL=DL-0x0A+'A';
	}
	if(cntlcd1>15)
	{
	 	Lcdbuf2[cntlcd2++]=DL;	
	}
	else
	{
	 	Lcdbuf1[cntlcd1++]=DL;
	}
}

void Timer2Init(unsigned char num)		//num*25ms@11.0592MHz
{
	gTimeMult=num;
	T2MOD = 0;		//初始化模式寄存器
	T2CON = 0;		//初始化控制寄存器
	ET2=0;
	TL2 = 0x00;		//设置定时初始值
	TH2 = 0xA6;		//设置定时初始值
	RCAP2L = 0x00;		//设置定时重载值
	RCAP2H = 0xA6;		//设置定时重载值
	TR2 = 1;		//定时器2开始计时
}

void ConfigUART2()
{
	SCON = 0x50;		//8位数据,可变波特率
	TL2 = 0xE8;		//设置定时初始值
	TH2 = 0xFF;		//设置定时初始值
	ET2=1;
	TR2=1;		//定时器2开始计时
}

void Timer02ms()//2ms
{
	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=0xF8;
	TL0=0xCD;
	ET0=1;
	TR0=1;
}

//T0 定时时间=num*25ms
void C51InitTimer(unsigned char num)
{
    // AUXR |= 0xC0;//bit7->T0x12:0->12T,1->1T;bit6->T1x12:0->12T,1->1T,默认情况下是传统8051速度
    TMOD |= 0x01;
    TH0 = 0x4C;
    TL0 = 0x00;      // 0x4C00=19456,(65536-19456)*12/22118400=0.025s=25ms
    gTimeMult = num; // 25*40=1000ms=1s
    ET0 = 1;
    TR0 = 1;
}

void ConfigUART1()
{
 	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;		//设置定时初始值
	TH1 = 0xFD;		//设置定时重载值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时

	ES=1;
}

//延时n*1us函数
void C51Delay1us(unsigned short n)
{
    while (--n)
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void C51StopTimer()
{
    TR0 = 0;
}