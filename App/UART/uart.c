#include<reg52.h>
#include"uart.h"


//-----------------------------UART1����ʹ�õ�ȫ�ֱ���---------------------------
xdata unsigned char gPn532Data[100];            //13.56M RF���ݻ���
xdata unsigned char *gpPn532RxBuf, gPn532RxLen; //�����жϺ����������buf
xdata unsigned char gi;     //���ڽ������������ack֮���������
//-------------------------------------------------------------------------------

unsigned char gTimeOutFlag;
unsigned char gUart1RecvFlag;

/**************************************************
Function: init_uart();

Description:
  set uart working mode,
  ʹ�ö��������ʷ�������Ϊ�����ʷ�����
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

//����1����һ���ֽں���
void C51Uart1SendByte(unsigned char num)
{
    ES = 0;
    TI = 0;
    SBUF = num;
    while (0 == TI);
    TI = 0;
    ES = 1;
}

