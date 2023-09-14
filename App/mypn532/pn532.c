#include<reg52.h>
#include<intrins.h>
#include"pn532.h"
#include"public.h"
#include"uart.h"

void pn532waking()
{
	C51Uart1SendByte(0x55);
	C51Uart1SendByte(0x55);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0xff);
	C51Uart1SendByte(0x03);
	C51Uart1SendByte(0xfd);
	C51Uart1SendByte(0xd4);
	C51Uart1SendByte(0x14);
	C51Uart1SendByte(0x01);
	C51Uart1SendByte(0x17);
	C51Uart1SendByte(0x00);									
}

void pn532findcard()
{
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0x00);	
	C51Uart1SendByte(0xFF);
	C51Uart1SendByte(0x04);
	C51Uart1SendByte(0xFC);
	C51Uart1SendByte(0xD4);
	C51Uart1SendByte(0x4A);
	C51Uart1SendByte(0x02);
	C51Uart1SendByte(0x00);
	C51Uart1SendByte(0xE0);
	C51Uart1SendByte(0x00);									
}

/*****************************************************************
* 函数名：  PN532SendData
* 函数功能：主控器向PN532发送数据
* 输入参数：
 pBuf：需要发送给PN532的帧数据
 len： pBuf的长度
* 输出参数：
        返回是否成功标志
            成功：SUCCESS
            失败：相应的错误代码
********************************************************************/
unsigned char PN532SendData(unsigned char *pBuf, unsigned char len)
{
    unsigned char i;
    for (i = 0; i < len; i++)
    {
        C51Uart1SendByte(pBuf[i]);
    }
    return ERR_NO;
}

/***************************************************
* 函数名：  PN532RecvData
* 函数功能：主控器从PN532接收数据
* 输入参数：
        无
* 输出参数：
        pBuf：
            接收帧缓存
        返回是否成功标志
            成功：SUCCESS
            失败：相应的错误代码
***************************************************/
unsigned char PN532RecvData(unsigned char *pBuf)
{
    gTimeOutFlag = 0;
    gUart1RecvFlag = 0;

    Timer2Init(4);

    gpPn532RxBuf = pBuf; // Start of RX buffer
    gPn532RxLen = 6;     // Load RX byte counter
    gi = 0;
    while (!(gTimeOutFlag || gUart1RecvFlag))
        ; //只要有一个为1则退出循环

    C51StopTimer();
    if (1 == gUart1RecvFlag)
    {
        return ERR_NO;
    }
    else
    {
        return ERR_TIMEOUT;
    }
}

/****************************************
* 函数名：  PN532SendAck
* 函数功能：主控器向PN532发送ACK帧
* 输入参数：
 无
* 输出参数：
        返回是否成功标志
            成功：SUCCESS
            失败：相应的错误代码
*****************************************/
unsigned char PN532SendAck()
{
    unsigned char pAckBuf[6] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
    unsigned char i;

    for (i = 0; i < 6; i++)
    {
        C51Uart1SendByte(pAckBuf[i]);
    }
    return ERR_NO;
}

/***********************************************
* 函数名：  PN532RecvAck
* 函数功能：主控器从PN532接收ACK帧
* 输入参数：
 无
* 输出参数：
        pBuf：
            接收ACK帧缓存
        返回是否成功标志
            成功：SUCCESS
            失败：相应的错误代码
*************************************************/
unsigned char PN532RecvAck(unsigned char *pBuf)
{
    gTimeOutFlag = 0;
    gUart1RecvFlag = 0;

    Timer2Init(1);

    gpPn532RxBuf = pBuf; // Start of RX buffer
    gPn532RxLen = 6;     // Load RX byte counter
    gi = 0;
    while (!(gTimeOutFlag || gUart1RecvFlag))
        ; //只要有一个为1则退出循环

    C51StopTimer();

    if (1 == gUart1RecvFlag)
    {
        return ERR_NO;
    }
    else
    {
        return ERR_TIMEOUT;
    }
}

/*********************************************************************************************************
* 函数名：  SAMConfiguration
* 函数功能：设置PN532的内部参数
* 输入参数：
 mode:
      0x01:normal mode
      0x02:virtual mode
      0x03:wired mode
      0x04:dual mode
 timeout:
      0x00:不使用timeout
      0x01-0xFF：超时值
 irq:
      0x00：不使用P70_IRQ
      0x01：使用P70_IRQ
* 输出参数：
 成功：ERR_NO
        失败：ERR_RECVACK，ERR_RECVDATA
* 输入输出参数：
        pRfBuf：输入，用于存放输入参数的缓存，从外部传入，用于解决在函数内部定义过大的数组问题
                输出，存放从PN532接收的结果
**********************************************************************************************************/
unsigned char SAMConfiguration(unsigned char *pRfBuf, unsigned char mode, unsigned char timeout, unsigned char irq)
{
    unsigned char temp;
    unsigned char s;
    pRfBuf[0] = 0x00; //前导码
    pRfBuf[1] = 0x00; //包开始标志2个字节
    pRfBuf[2] = 0xFF;
    pRfBuf[3] = 3 + 1 + 1; // len + 1字节的PD0 + 1字节的TFI
    pRfBuf[4] = 0x100 - pRfBuf[3];
    pRfBuf[5] = 0xD4; //方向标示
    temp = pRfBuf[5];
    pRfBuf[6] = 0x14; //命令码
    temp += pRfBuf[6];
    pRfBuf[7] = mode;
    temp += pRfBuf[7];
    pRfBuf[8] = timeout;
    temp += pRfBuf[8];
    pRfBuf[9] = irq;
    temp += pRfBuf[9];

    pRfBuf[10] = 0x100 - temp; // DCS
    pRfBuf[11] = 0x00;


    //底层发送函数
    s = PN532SendData(pRfBuf, 12);
    if (s != ERR_NO)
    {
        return ERR_SENDDATA;
    }
    s = PN532RecvAck(pRfBuf);
    if (s != ERR_NO)
    {
        return ERR_RECVACK;
    }
    s = PN532RecvData(pRfBuf);
    if (s != ERR_NO)
    {
        return ERR_RECVDATA;
    }

    return ERR_NO;
}

/****************************************************************************************
* 函数名：  InListPassiveTarget
* 函数功能：PN532检测卡
* 输入参数：
 maxTag:
            PN532最大初始化卡的个数，PN532一次只能同时处理2张卡
 brty:
            0x00 : 106 kbps type A (ISO/IEC14443 Type A),
            0x01 : 212 kbps (FeliCa polling),
            0x02 : 424 kbps (FeliCa polling),
            0x03 : 106 kbps type B (ISO/IEC14443-3B),
            0x04 : 106 kbps Innovision Jewel tag.
        pData:
* 输出参数：
 成功：ERR_NO
        失败：ERR_RECVACK，ERR_RECVDATA
* 输入输出参数：
        pRfBuf：输入，用于存放输入参数的缓存，从外部传入，用于解决在函数内部定义过大的数组问题
                输出，存放从PN532接收的结果
*****************************************************************************************/
unsigned char InListPassiveTarget(unsigned char *pRfBuf, unsigned char maxTag, unsigned char brty,
                                  unsigned char *pData, unsigned char len)
{
    unsigned char i, temp;
    unsigned char s;
    pRfBuf[0] = 0x00; //前导码
    pRfBuf[1] = 0x00; //包开始标志2个字节
    pRfBuf[2] = 0xFF;
    pRfBuf[3] = len + 1 + 1 + 1 + 1; // len + 1字节的maxTag + 1字节的brty + 1字节的PD0 + 1字节的TFI
    pRfBuf[4] = 0x100 - pRfBuf[3];
    pRfBuf[5] = 0xD4; //方向标示
    temp = pRfBuf[5];
    pRfBuf[6] = 0x4A; //命令码
    temp += pRfBuf[6];
    pRfBuf[7] = maxTag;
    temp += pRfBuf[7];
    pRfBuf[8] = brty;
    temp += pRfBuf[8];

    for (i = 0; i < len; i++)
    {
        pRfBuf[9 + i] = pData[i];
        temp += pData[i];
    }

    pRfBuf[9 + i] = 0x100 - temp; // DCS
    pRfBuf[9 + i + 1] = 0x00;

    //底层发送函数
    s = PN532SendData(pRfBuf, 9 + i + 1 + 1);
    if (s != ERR_NO)
    {
        return ERR_SENDDATA;
    }
    s = PN532RecvAck(pRfBuf);
    if (s != ERR_NO)
    {
        return ERR_RECVACK;
    }
    s = PN532RecvData(pRfBuf);
    if (s != ERR_NO)
    {
        return ERR_RECVDATA;
    }

    return ERR_NO;
}