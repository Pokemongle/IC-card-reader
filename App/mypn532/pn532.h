void pn532waking();
void pn532findcard();
unsigned char PN532SendData(unsigned char *pBuf, unsigned char len);
unsigned char PN532RecvData(unsigned char *pBuf);
unsigned char PN532SendAck();
unsigned char PN532RecvAck(unsigned char *pBuf);
unsigned char SAMConfiguration(unsigned char *pRfBuf, unsigned char mode, unsigned char timeout, unsigned char irq);
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
                                  unsigned char *pData, unsigned char len);