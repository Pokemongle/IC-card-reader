void InitLcd();//��ʼ��
void LcdWaitReady();//�ȴ�����
void LcdWriteCmd(unsigned char cmd);//дָ��
void LcdWriteDat(unsigned char dat);//д����
void LcdSetCursor(unsigned char x,unsigned char y);//���ù��λ��
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str,unsigned char len);//��ʾ�ַ���
void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len);//Lcd�������
void LcdFullClear();//Lcd����