#ifndef _UART_H
#define _UART_H
#define ERR_NO        0
#define ERR_TIMEOUT   1
#define ERR_SENDDATA  2
#define ERR_RECVACK   3
#define ERR_RECVDATA  4
//-----------------------------UART1部分使用的全局变量---------------------------
extern xdata unsigned char gPn532Data[100];            //13.56M RF数据缓存
extern xdata unsigned char *gpPn532RxBuf, gPn532RxLen; //用于中断函数处理接收buf
extern xdata unsigned char gi;     //用于接收数据与接收ack之间进行区别
//-------------------------------------------------------------------------------

extern unsigned char gTimeOutFlag;
extern unsigned char gUart1RecvFlag;


void UartInit(void);		//115200bps@11.0592MHz
void C51Uart1SendByte(unsigned char num);//串口1发送一个字节
void C51InitUart(void);
#endif