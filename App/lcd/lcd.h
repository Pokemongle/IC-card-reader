void InitLcd();//初始化
void LcdWaitReady();//等待空闲
void LcdWriteCmd(unsigned char cmd);//写指令
void LcdWriteDat(unsigned char dat);//写数据
void LcdSetCursor(unsigned char x,unsigned char y);//设置光标位置
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str,unsigned char len);//显示字符串
void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len);//Lcd区域清除
void LcdFullClear();//Lcd清屏