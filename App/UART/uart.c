#include<reg52.h>
#include"uart.h"


//-----------------------------UART1部分使用的全局变量---------------------------
xdata unsigned char gPn532Data[100];            //13.56M RF数据缓存
xdata unsigned char *gpPn532RxBuf, gPn532RxLen; //用于中断函数处理接收buf
xdata unsigned char gi;     //用于接收数据与接收ack之间进行区别
//-------------------------------------------------------------------------------

unsigned char gTimeOutFlag;
unsigned char gUart1RecvFlag;

/**************************************************
Function: init_uart();

Description:
  set uart working mode,
  使用独立波特率发生器作为波特率发生器
**************************************************/
void C51InitUart(void)
{
    T2MOD = 0x01;
	T2CON = 0x30;
	TH2 = 0xFF;
	TL2 = 0xFD;
	RCAP2H = 0xFF;
	RCAP2L = 0xFD;
	SCON = 0x50;
	PCON = 0x00;
	TR2 = 1;
	ES = 1;
}

//串口1发送一个字节函数
void C51Uart1SendByte(unsigned char num)
{
    ES = 0;
    TI = 0;
    SBUF = num;
    while (0 == TI);
    TI = 0;
    ES = 1;
}

