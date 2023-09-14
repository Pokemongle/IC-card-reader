#include<reg52.h>
#include<intrins.h>
#include<string.h>
#include"public.h"
#include"lcd.h"
#include"pn532.h"
#include"uart.h"

//--Keyborad
sbit KEY1=P0^0;
sbit KEY2=P0^1;
//--UART
unsigned char cntTxd=0;
unsigned char *ptrTxd;
unsigned char cntRxd=0;
unsigned char *ptrRxd;
unsigned char RXDbuf=0;
unsigned char TXDbuf=0;
//--pn532
bit startwakeupsend=0;
bit startwakeuprec=0;
bit startfindcardsend=0;
bit startfindcardrec=0;
unsigned char cntwakeupsend=0;
unsigned char cntwakeuprec=0;
unsigned char cntfindcardsend=0;
unsigned char cntfindcardrec=0;
unsigned char code wakeupsend[24]={
	0x55,0x55,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFF,0x03,
	0xFD,0xD4,0x14,0x01,0x17,0x00
};
unsigned char code findcard[11]=
{
	0x00,0x00,0xFF,0x04,
	0xFC,0xD4,0x4A,0x02,
	0x00,0xE0,0x00
};

unsigned char pbufrec[30]={0};


//--public
unsigned char Lcdbuf1[16]={0};
unsigned char Lcdbuf2[16]={0};
unsigned char cntlcd1=0;
unsigned char cntlcd2=0;
unsigned char namelib[][3]=
{
 	{'x','z','y'}
};
unsigned char UIDlib[][4]=
{
 	{0x54,0xF4,0x15,0x6E}
};
unsigned char flagname=0x02;
unsigned char namebuf[4]={0};
//--test

unsigned char libcount=1;
void LcdFindName(unsigned char *pbuf);
void LcdFindName(unsigned char *pbuf)
{
	unsigned char m=0;
 	for(m=0;m<libcount;m++)
	{
	 	unsigned char n;
		for(n=0;n<4;n++)
		{
		 	if(*(pbuf+n)!=UIDlib[m][n])
			{
			 	break;
			}
		}
		if(n==4)
		{
		 	flagname=m;
			return ;
		}
	}	
}

void main()
{
	unsigned int i;
	EA=1;
	C51InitUart();
	InitLcd();
///*	
	//初始界面
	LcdShowStr(0,0,"Welcome",7);
	C51Delay1us(60000);
	C51Delay1us(60000);
	LcdShowStr(0,0,"1-wakeup",8);
	LcdShowStr(0,1,"2-findcard",10);

	//键盘KEY1，开始发送唤醒指令
	while(KEY1);

	//唤醒pn532，Lcd显示"Waking."
	LcdFullClear();
	LcdShowStr(0,0,"Waking up...",12);
	C51Delay1us(60000);
	C51Delay1us(60000);
		//开始发送唤醒指令
	startwakeupsend=1;
	TI=1;	
	while(cntwakeupsend<24);//等待发送结束
	   //发送结束，Lcd显示"Success",一段时间后显示应答码
	startwakeuprec=1;//开始接收应答
	LcdFullClear();
	while(cntwakeuprec<15);//转化为ASCII码在Lcd屏输出
	LcdShowStr(0,0,"Success",7);//接收成功
	for(i=0;i<15;i++)
	{
	 	hextoascii(pbufrec[i]);
	}
	LcdShowStr(0,0,"WaitForAscii",12);
	C51Delay1us(60000);
	C51Delay1us(60000);
	LcdShowStr(0,0,Lcdbuf1,16);
	LcdShowStr(0,1,Lcdbuf2,14);
	

	//Lcd显示参数清零
	cntlcd1=0;
	cntlcd2=0;
	//--------------------------

	//--寻卡
		//--按下KEY2开始寻卡
	while(KEY2);
	//发送寻卡指令，Lcd显示"Finding card..."
	LcdFullClear();
	LcdShowStr(0,0,"Finding card...",15);
	C51Delay1us(60000);
	C51Delay1us(60000);
		//开始发送寻卡指令
	startfindcardsend=1;
	TI=1;
	while(cntfindcardsend<11);//等待发送结束
		//发送结束，开始接收，一段时间后Lcd显示应答
	startfindcardrec=1;//开始接收应答
	LcdFullClear();
	LcdShowStr(0,0,"Success",7);//接收成功
	while(cntfindcardrec<25);//转化为ASCII码在Lcd屏输出
	for(i=19;i<23;i++)//转换UID码
	{
	 	hextoascii(pbufrec[i]);
		namebuf[i-19]=pbufrec[i];
	}
		//对应名字缩写

	LcdFindName(namebuf);
	LcdShowStr(0,0,"WaitForAscii...",15);
	C51Delay1us(60000);
	LcdFullClear();
	LcdShowStr(0,0,Lcdbuf1,8);
	LcdShowStr(10,0,namelib[flagname],3);
	while(1);
//*/
}

void IntUart()interrupt 4
{

///*
	if(RI)
	{
	 	RI=0;
		if(startwakeuprec)//接收唤醒应答
		{
		 	if(cntwakeuprec<15)
			{
			 	pbufrec[cntwakeuprec++]=SBUF;
			}
		}
		if(startfindcardrec)//接收寻卡应答
		{
		 	if(cntfindcardrec<25)
			{
			 	pbufrec[cntfindcardrec++]=SBUF;
			}
		}
	}
 	if(TI)
	{
	 	TI=0;
		if(startwakeupsend)//发送唤醒指令
		{
			
		 	if(cntwakeupsend<24)
			{
		 		TXDbuf=wakeupsend[cntwakeupsend++];
				SBUF=TXDbuf;
			}	
		}
		if(startfindcardsend)//发送寻卡指令
		{
		 	if(cntfindcardsend<11)
			{
			 	TXDbuf=findcard[cntfindcardsend++];
				SBUF=TXDbuf;
			}
		}
		  
	}

//*/

/*
	if(RI)
	{
		RI=0;
	 	if(cnttest<10)
		{
			RXDbuf=SBUF;
			SBUF=RXDbuf;
			
		}	
	}
	if(TI)
	{
		TI=0;
	}
*/
			
}


