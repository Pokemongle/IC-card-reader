void pn532waking();
void pn532findcard();
unsigned char PN532SendData(unsigned char *pBuf, unsigned char len);
unsigned char PN532RecvData(unsigned char *pBuf);
unsigned char PN532SendAck();
unsigned char PN532RecvAck(unsigned char *pBuf);
unsigned char SAMConfiguration(unsigned char *pRfBuf, unsigned char mode, unsigned char timeout, unsigned char irq);
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
                                  unsigned char *pData, unsigned char len);