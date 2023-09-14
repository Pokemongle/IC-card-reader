#ifndef _UART_H
#define _UART_H
#define ERR_NO        0
#define ERR_TIMEOUT   1
#define ERR_SENDDATA  2
#define ERR_RECVACK   3
#define ERR_RECVDATA  4
//-----------------------------UART1����ʹ�õ�ȫ�ֱ���---------------------------
extern xdata unsigned char gPn532Data[100];            //13.56M RF���ݻ���
extern xdata unsigned char *gpPn532RxBuf, gPn532RxLen; //�����жϺ����������buf
extern xdata unsigned char gi;     //���ڽ������������ack֮���������
//-------------------------------------------------------------------------------

extern unsigned char gTimeOutFlag;
extern unsigned char gUart1RecvFlag;


void UartInit(void);		//115200bps@11.0592MHz
void C51Uart1SendByte(unsigned char num);//����1����һ���ֽ�
void C51InitUart(void);
#endif