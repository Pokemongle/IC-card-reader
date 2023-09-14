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
	while(sta&=0x80);//Lcd最高位不为1（忙）时，跳出循环
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
	LcdWriteCmd(0x38);//设置2x16显示，5x7方格，8位数据接口
	LcdWriteCmd(0x0C);//显示器开，光标开，光标闪烁
	LcdWriteCmd(0x06);//文字不动，地址加1
	LcdWriteCmd(0x01);//清屏 	
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
	LcdWriteCmd(addr|=0x80);//设置RAM地址	
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