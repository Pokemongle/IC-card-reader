#include<reg52.h>
#include"lcd.h"

//--lcd1602
#define DB P1
sbit RS=P2^7;
sbit RW=P2^6;
sbit EN=P2^5;

void LcdWaitReady()
{
	unsigned char sta;
 	DB=0xFF;
	RS=0;
	RW=1;
	do
	{
		EN=1;
		sta=DB;
		EN=0; 	
	}
	while(sta&=0x80);//Lcd���λ��Ϊ1��æ��ʱ������ѭ��
}

void LcdWriteCmd(unsigned char cmd)
{
	LcdWaitReady();
	RS=0;
	RW=0;
	DB=cmd;
	EN=1;
	EN=0;	
}

void LcdWriteDat(unsigned char dat)
{
	LcdWaitReady();
	RS=1;
	RW=0;
	DB=dat;
	EN=1;
	EN=0;
}

void InitLcd()
{
	LcdWriteCmd(0x38);//����2x16��ʾ��5x7����8λ���ݽӿ�
	LcdWriteCmd(0x0C);//��ʾ��������꿪�������˸
	LcdWriteCmd(0x06);//���ֲ�������ַ��1
	LcdWriteCmd(0x01);//���� 	
}

void LcdSetCursor(unsigned char x,unsigned char y)
{
	unsigned char addr;
	if(y==0)
	{
	 	addr=0x00+x;
	}
	else
	{
	 	addr=0x40+x;
	}
	LcdWriteCmd(addr|=0x80);//����RAM��ַ	
}

void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str,unsigned char len)
{
 	LcdSetCursor(x,y);
	while(len--)
	{
	 	LcdWriteDat(*str++);
	}
}

void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len)
{
 	LcdSetCursor(x,y);
	while(len--)
	{
	 	LcdWriteDat(' ');
	}
}

void LcdFullClear()
{
 	LcdWriteCmd(0x01);
}