/********************************************************************************
* STC12C5A60S2����PN532����Ѱ������
* STC12C5A60S2ͨ��UART��PN532����
* ͨ���ж�ʵ�ֽ���UART���ݣ���ͨ����ʱ�ж�PN532����Ӧ�Ƿ�ʱ
********************************************************************************/
#include "reg52.h"
#include <intrins.h>

#define ERR_NO        0
#define ERR_TIMEOUT   1
#define ERR_SENDDATA  2
#define ERR_RECVACK   3
#define ERR_RECVDATA  4

//-----------------------------UART1����ʹ�õ�ȫ�ֱ���---------------------------
xdata unsigned char gPn532Data[100];            //13.56M RF���ݻ���
xdata unsigned char *gpPn532RxBuf, gPn532RxLen; //�����жϺ����������buf
xdata unsigned char gi;     //���ڽ������������ack֮���������
//-------------------------------------------------------------------------------

unsigned char gTimeMult;
unsigned char gTimeOutFlag;
unsigned char gUart1RecvFlag;

sbit p25 = P2^5;

void C51InitIo(void)
{  
 P2M0 |= 0x20;
 P2M1 |= 0x00;//����P2.5Ϊǿ�������
}

//��ʱʱ��=num*25ms
void C51InitTimer(unsigned char num)
{
 //AUXR |= 0xC0;//bit7->T0x12:0->12T,1->1T;bit6->T1x12:0->12T,1->1T,Ĭ��������Ǵ�ͳ8051�ٶ�
 TMOD |= 0x01;
 TH0 = 0x4C;
 TL0 = 0x00;//0x4C00=19456,(65536-19456)*12/22118400=0.025s=25ms
 gTimeMult = num;//25*40=1000ms=1s
 ET0 = 1;
 TR0 = 1;
}

void C51StopTimer()
{
 TR0 = 0;
}

/**************************************************
Function: init_uart();

Description:
  set uart working mode,
  ʹ�ö��������ʷ�������Ϊ�����ʷ�����
**************************************************/
void C51InitUart(void)
{
 AUXR |= 0x18; //BRTR=1,�������������ʷ�����,S2SMOD=1,����2������*2��S1BRS=1������1ʹ�ö��������ʷ�������Ϊ�����ʷ�����,S1BRS=0������1ʹ�ö�ʱ��1��Ϊ�����ʷ�����
     
 SCON |= 0x50; //UART1������ģʽ1(8λuart,������ʼλ��ֹͣλ��10λ)���ɱ�����f=((2^SMOD)/32)*BRT��ren==1->�����н���
 PCON |= 0x80; //smod=1,
 TMOD |= 0x20; //��ʱ�������뷽ʽ2����TR1���ƶ�ʱ���Ŀ�����ر�
 TH1   = 0xFF; //22.1184MHz:0xF4->9600, 0xFA->19200, 0xFD->38400, 0xFE->57600, 0xFF->115200
 TL1   = 0xFF;
 TR1   = 1;  //������ʱ��1   
 ES    = 1;     //������1�ж�

 S2CON |= 0x50; //UART2������ģʽ1(8λuart,������ʼλ��ֹͣλ��10λ)���ɱ�����f=((2^S2SMOD)/32)*BRT��ren==1->�����н��� 
 BRT    = 0xF4; //BRTx12=0;0xFF->115200,0xF4->9600,0xA0->1200 
 IE2   |= 0x01; //������2�ж�
}

//����1����һ���ֽں���
void C51Uart1SendByte(unsigned char num)
{
 ES = 0;  
 TI = 0;
 SBUF = num;
 while (0==TI);
 TI = 0;
 ES = 1;
}

//---------����2���������һ���ֽں���----------
void C51Uart2SendByte(unsigned char num)
{ 
 IE2   &= 0xFE; //������2�жϣ���Ҫ���ӹرա����ж���䣬��Ȼ����2���������� 
 S2CON = S2CON&0xFD;//��������жϱ�־
 S2BUF = num;
 while (0==(S2CON&0x02));//����1˵���������
 S2CON = S2CON&0xFD;//��������жϱ�־
 IE2   |= 0x01; //������2�ж�
}

//��ʱn*1us����
void C51Delay1us(unsigned short n)
{
 while (--n)
 {
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
 }
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
 WDT_CONTR = 0x3F;
 for (i=0; i<len; i++)
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

 C51InitTimer(4);

 gpPn532RxBuf = pBuf;        //Start of RX buffer
    gPn532RxLen = 6;            // Load RX byte counter
 gi = 0;
 while (!(gTimeOutFlag||gUart1RecvFlag));//ֻҪ��һ��Ϊ1���˳�ѭ��

 C51StopTimer();
 if (1==gUart1RecvFlag)
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
 unsigned char pAckBuf[6] = {0x00,0x00,0xFF,0x00,0xFF,0x00};
 unsigned char i;
 
 for (i=0; i<6; i++)
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

 C51InitTimer(1);

 gpPn532RxBuf = pBuf;        //Start of RX buffer
    gPn532RxLen = 6;            // Load RX byte counter
 gi = 0;
 while (!(gTimeOutFlag||gUart1RecvFlag));//ֻҪ��һ��Ϊ1���˳�ѭ��

 C51StopTimer();

 if (1==gUart1RecvFlag)
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
    pRfBuf[3] = 3 + 1 + 1;//len + 1�ֽڵ�PD0 + 1�ֽڵ�TFI
    pRfBuf[4] = 0x100-pRfBuf[3];
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
   
    pRfBuf[10] = 0x100 - temp; //DCS
    pRfBuf[11] = 0x00;
   
    //�ײ㷢�ͺ���
    s = PN532SendData(pRfBuf,12);
    if (s!=ERR_NO)
    {
        return ERR_SENDDATA;
    }
    s = PN532RecvAck(pRfBuf);
    if (s!=ERR_NO)
    {
        return ERR_RECVACK;
    }
    s = PN532RecvData(pRfBuf);
    if (s!=ERR_NO)
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
    unsigned char i,temp;
    unsigned char s;
    pRfBuf[0] = 0x00; //ǰ����
    pRfBuf[1] = 0x00; //����ʼ��־2���ֽ�
    pRfBuf[2] = 0xFF;
    pRfBuf[3] = len + 1 + 1 + 1 + 1;//len + 1�ֽڵ�maxTag + 1�ֽڵ�brty + 1�ֽڵ�PD0 + 1�ֽڵ�TFI
    pRfBuf[4] = 0x100-pRfBuf[3];
    pRfBuf[5] = 0xD4; //�����ʾ
    temp = pRfBuf[5];
    pRfBuf[6] = 0x4A; //������
    temp += pRfBuf[6];
    pRfBuf[7] = maxTag; 
    temp += pRfBuf[7];
    pRfBuf[8] = brty; 
    temp += pRfBuf[8];
   
    for (i=0; i<len; i++)
    {
        pRfBuf[9+i] = pData[i];
        temp  += pData[i];
    }
   
    pRfBuf[9+i] = 0x100 - temp; //DCS
    pRfBuf[9+i+1] = 0x00; 
   
    //�ײ㷢�ͺ���   
    s = PN532SendData(pRfBuf,9+i+1+1);
    if (s!=ERR_NO)
    {
        return ERR_SENDDATA;
    }
    s = PN532RecvAck(pRfBuf);
    if (s!=ERR_NO)
    {
        return ERR_RECVACK;
    }
    s = PN532RecvData(pRfBuf);
    if (s!=ERR_NO)
    {
        return ERR_RECVDATA;
    }
   
    return ERR_NO;
}
       
void main()
{ 
 unsigned char i,s; 

 C51InitIo();
 C51InitUart();
    
 WDT_CONTR = 0x3F;//ʹ�ܿ��Ź����忴�Ź���־������ģʽ��������Ƶϵ��(scale:bit2-bit0)�����ʱ��=��12*scale*32768��/Fosc
   
 EA = 1;   //cpu�ж�������
 
 p25 = 0;
 C51Delay1us(60000);
 C51Delay1us(60000);
 C51Delay1us(60000);
 C51Delay1us(60000);
 C51Delay1us(60000);
 p25 = 1;

 gTimeOutFlag = 0;
 gUart1RecvFlag = 0;

//--------------����PN532----------------
 C51Uart1SendByte(0x55);
 C51Uart1SendByte(0x55);
 C51Uart1SendByte(0x00);
 C51Uart1SendByte(0x00);
 C51Uart1SendByte(0x00);
 SAMConfiguration(gPn532Data,1,0,1);
 for (i=0; i<gPn532Data[3]+7; i++)
 {
  C51Uart2SendByte(gPn532Data[i]);
 }
//---------------------------------------

 while(1)
 {
  WDT_CONTR = 0x3F;
  
  s = InListPassiveTarget(gPn532Data,1,0,0,0);

  if (ERR_NO==s)
  {
   for (i=0; i<gPn532Data[3]+7; i++)
   {
    C51Uart2SendByte(gPn532Data[i]);
   }
   p25 = 0;
   C51Delay1us(60000);
   C51Delay1us(60000);
   C51Delay1us(60000);
   C51Delay1us(60000);
   C51Delay1us(60000);
   p25 = 1;
  }
 }
}

//�����жϷ������
void Uart1IsrHandler(void) interrupt 4
{ 
 if (RI)
 {
  RI = 0;  
  gPn532RxLen--;
  if (gPn532RxLen)
  {
   *gpPn532RxBuf = SBUF;
   
   gi++;
   if (gi==4)
   {
    if (0x00==*gpPn532RxBuf)//����ACK֡����ͨ֡��һ������������Ҫ����Դ�
    {
     gPn532RxLen = *gpPn532RxBuf + 2;  //����ACK
    }
    else
    {
     gPn532RxLen = *gpPn532RxBuf + 3;  //������ + 1�ֽڵİ�����У�� + 1�ֽڵ�����У�� + 1�ֽڵĺ���
    }
   }
   
   gpPn532RxBuf++;
  }
  else
  {
   *gpPn532RxBuf = SBUF;
   gUart1RecvFlag = 1;
  }  
 }
 if (TI)
 {          
  TI = 0; 
 }
}

void Timer0IsrHandler(void) interrupt 1
{
 TH0 = 0x4C;
 TL0 = 0x00;
 if (--gTimeMult)
  return;
 gTimeOutFlag = 1;
}