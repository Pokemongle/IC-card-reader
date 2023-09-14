#ifndef _PUBLIC_H
#define _PUBLIC_H
extern unsigned char gTimeMult;
void hextoascii(unsigned char dat);//16进制的码每一位转换为ASCII码在Lcd上输出
void C51InitTimer(unsigned char num);//T0 num*25ms
void Timer2Init(unsigned char num);		//num*25ms@11.0592MHz
void ConfigUART1();//配置T1串口通信
void ConfigUART2();//
void Delayms(unsigned int ms);//毫秒级延时
void C51StopTimer();//停止延时
void C51Delay1us(unsigned short n);//延时n*1us函数
void Timer02ms();//2msT0定时
#endif