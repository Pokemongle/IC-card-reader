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
* ��������  PN532SendData
* �������ܣ���������PN532��������
* ���������
 pBuf����Ҫ���͸�PN532��֡����
 len�� pBuf�ĳ���
* ���������
        �����Ƿ�ɹ���־
            �ɹ���SUCCESS
            ʧ�ܣ���Ӧ�Ĵ������
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
* ��������  PN532RecvData
* �������ܣ���������PN532��������
* ���������
        ��
* ���������
        pBuf��
            ����֡����
        �����Ƿ�ɹ���־
            �ɹ���SUCCESS
            ʧ�ܣ���Ӧ�Ĵ������
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
        ; //ֻҪ��һ��Ϊ1���˳�ѭ��

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
* ��������  PN532SendAck
* �������ܣ���������PN532����ACK֡
* ���������
 ��
* ���������
        �����Ƿ�ɹ���־
            �ɹ���SUCCESS
            ʧ�ܣ���Ӧ�Ĵ������
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
* ��������  PN532RecvAck
* �������ܣ���������PN532����ACK֡
* ���������
 ��
* ���������
        pBuf��
            ����ACK֡����
        �����Ƿ�ɹ���־
            �ɹ���SUCCESS
            ʧ�ܣ���Ӧ�Ĵ������
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
        ; //ֻҪ��һ��Ϊ1���˳�ѭ��

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
* ��������  SAMConfiguration
* �������ܣ�����PN532���ڲ�����
* ���������
 mode:
      0x01:normal mode
      0x02:virtual mode
      0x03:wired mode
      0x04:dual mode
 timeout:
      0x00:��ʹ��timeout
      0x01-0xFF����ʱֵ
 irq:
      0x00����ʹ��P70_IRQ
      0x01��ʹ��P70_IRQ
* ���������
 �ɹ���ERR_NO
        ʧ�ܣ�ERR_RECVACK��ERR_RECVDATA
* �������������
        pRfBuf�����룬���ڴ����������Ļ��棬���ⲿ���룬���ڽ���ں����ڲ�����������������
                �������Ŵ�PN532���յĽ��
**********************************************************************************************************/
unsigned char SAMConfiguration(unsigned char *pRfBuf, unsigned char mode, unsigned char timeout, unsigned char irq)
{
    unsigned char temp;
    unsigned char s;
    pRfBuf[0] = 0x00; //ǰ����
    pRfBuf[1] = 0x00; //����ʼ��־2���ֽ�
    pRfBuf[2] = 0xFF;
    pRfBuf[3] = 3 + 1 + 1; // len + 1�ֽڵ�PD0 + 1�ֽڵ�TFI
    pRfBuf[4] = 0x100 - pRfBuf[3];
    pRfBuf[5] = 0xD4; //�����ʾ
    temp = pRfBuf[5];
    pRfBuf[6] = 0x14; //������
    temp += pRfBuf[6];
    pRfBuf[7] = mode;
    temp += pRfBuf[7];
    pRfBuf[8] = timeout;
    temp += pRfBuf[8];
    pRfBuf[9] = irq;
    temp += pRfBuf[9];

    pRfBuf[10] = 0x100 - temp; // DCS
    pRfBuf[11] = 0x00;


    //�ײ㷢�ͺ���
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
* ��������  InListPassiveTarget
* �������ܣ�PN532��⿨
* ���������
 maxTag:
            PN532����ʼ�����ĸ�����PN532һ��ֻ��ͬʱ����2�ſ�
 brty:
            0x00 : 106 kbps type A (ISO/IEC14443 Type A),
            0x01 : 212 kbps (FeliCa polling),
            0x02 : 424 kbps (FeliCa polling),
            0x03 : 106 kbps type B (ISO/IEC14443-3B),
            0x04 : 106 kbps Innovision Jewel tag.
        pData:
* ���������
 �ɹ���ERR_NO
        ʧ�ܣ�ERR_RECVACK��ERR_RECVDATA
* �������������
        pRfBuf�����룬���ڴ����������Ļ��棬���ⲿ���룬���ڽ���ں����ڲ�����������������
                �������Ŵ�PN532���յĽ��
*****************************************************************************************/
unsigned char InListPassiveTarget(unsigned char *pRfBuf, unsigned char maxTag, unsigned char brty,
                                  unsigned char *pData, unsigned char len)
{
    unsigned char i, temp;
    unsigned char s;
    pRfBuf[0] = 0x00; //ǰ����
    pRfBuf[1] = 0x00; //����ʼ��־2���ֽ�
    pRfBuf[2] = 0xFF;
    pRfBuf[3] = len + 1 + 1 + 1 + 1; // len + 1�ֽڵ�maxTag + 1�ֽڵ�brty + 1�ֽڵ�PD0 + 1�ֽڵ�TFI
    pRfBuf[4] = 0x100 - pRfBuf[3];
    pRfBuf[5] = 0xD4; //�����ʾ
    temp = pRfBuf[5];
    pRfBuf[6] = 0x4A; //������
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

    //�ײ㷢�ͺ���
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