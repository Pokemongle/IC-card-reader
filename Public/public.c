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
//���յ���16������ÿһλ�����Ӧ��ASCII����ʾ��Lcd��
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
	T2MOD = 0;		//��ʼ��ģʽ�Ĵ���
	T2CON = 0;		//��ʼ�����ƼĴ���
	ET2=0;
	TL2 = 0x00;		//���ö�ʱ��ʼֵ
	TH2 = 0xA6;		//���ö�ʱ��ʼֵ
	RCAP2L = 0x00;		//���ö�ʱ����ֵ
	RCAP2H = 0xA6;		//���ö�ʱ����ֵ
	TR2 = 1;		//��ʱ��2��ʼ��ʱ
}

void ConfigUART2()
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TL2 = 0xE8;		//���ö�ʱ��ʼֵ
	TH2 = 0xFF;		//���ö�ʱ��ʼֵ
	ET2=1;
	TR2=1;		//��ʱ��2��ʼ��ʱ
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

//T0 ��ʱʱ��=num*25ms
void C51InitTimer(unsigned char num)
{
    // AUXR |= 0xC0;//bit7->T0x12:0->12T,1->1T;bit6->T1x12:0->12T,1->1T,Ĭ��������Ǵ�ͳ8051�ٶ�
    TMOD |= 0x01;
    TH0 = 0x4C;
    TL0 = 0x00;      // 0x4C00=19456,(65536-19456)*12/22118400=0.025s=25ms
    gTimeMult = num; // 25*40=1000ms=1s
    ET0 = 1;
    TR0 = 1;
}

void ConfigUART1()
{
 	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x20;		//���ö�ʱ��ģʽ
	TL1 = 0xFD;		//���ö�ʱ��ʼֵ
	TH1 = 0xFD;		//���ö�ʱ����ֵ
	ET1 = 0;		//��ֹ��ʱ��%d�ж�
	TR1 = 1;		//��ʱ��1��ʼ��ʱ

	ES=1;
}

//��ʱn*1us����
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