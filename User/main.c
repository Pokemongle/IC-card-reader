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
	//��ʼ����
	LcdShowStr(0,0,"Welcome",7);
	C51Delay1us(60000);
	C51Delay1us(60000);
	LcdShowStr(0,0,"1-wakeup",8);
	LcdShowStr(0,1,"2-findcard",10);

	//����KEY1����ʼ���ͻ���ָ��
	while(KEY1);

	//����pn532��Lcd��ʾ"Waking."
	LcdFullClear();
	LcdShowStr(0,0,"Waking up...",12);
	C51Delay1us(60000);
	C51Delay1us(60000);
		//��ʼ���ͻ���ָ��
	startwakeupsend=1;
	TI=1;	
	while(cntwakeupsend<24);//�ȴ����ͽ���
	   //���ͽ�����Lcd��ʾ"Success",һ��ʱ�����ʾӦ����
	startwakeuprec=1;//��ʼ����Ӧ��
	LcdFullClear();
	while(cntwakeuprec<15);//ת��ΪASCII����Lcd�����
	LcdShowStr(0,0,"Success",7);//���ճɹ�
	for(i=0;i<15;i++)
	{
	 	hextoascii(pbufrec[i]);
	}
	LcdShowStr(0,0,"WaitForAscii",12);
	C51Delay1us(60000);
	C51Delay1us(60000);
	LcdShowStr(0,0,Lcdbuf1,16);
	LcdShowStr(0,1,Lcdbuf2,14);
	

	//Lcd��ʾ��������
	cntlcd1=0;
	cntlcd2=0;
	//--------------------------

	//--Ѱ��
		//--����KEY2��ʼѰ��
	while(KEY2);
	//����Ѱ��ָ�Lcd��ʾ"Finding card..."
	LcdFullClear();
	LcdShowStr(0,0,"Finding card...",15);
	C51Delay1us(60000);
	C51Delay1us(60000);
		//��ʼ����Ѱ��ָ��
	startfindcardsend=1;
	TI=1;
	while(cntfindcardsend<11);//�ȴ����ͽ���
		//���ͽ�������ʼ���գ�һ��ʱ���Lcd��ʾӦ��
	startfindcardrec=1;//��ʼ����Ӧ��
	LcdFullClear();
	LcdShowStr(0,0,"Success",7);//���ճɹ�
	while(cntfindcardrec<25);//ת��ΪASCII����Lcd�����
	for(i=19;i<23;i++)//ת��UID��
	{
	 	hextoascii(pbufrec[i]);
		namebuf[i-19]=pbufrec[i];
	}
		//��Ӧ������д

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
		if(startwakeuprec)//���ջ���Ӧ��
		{
		 	if(cntwakeuprec<15)
			{
			 	pbufrec[cntwakeuprec++]=SBUF;
			}
		}
		if(startfindcardrec)//����Ѱ��Ӧ��
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
		if(startwakeupsend)//���ͻ���ָ��
		{
			
		 	if(cntwakeupsend<24)
			{
		 		TXDbuf=wakeupsend[cntwakeupsend++];
				SBUF=TXDbuf;
			}	
		}
		if(startfindcardsend)//����Ѱ��ָ��
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


