#ifndef _PUBLIC_H
#define _PUBLIC_H
extern unsigned char gTimeMult;
void hextoascii(unsigned char dat);//16���Ƶ���ÿһλת��ΪASCII����Lcd�����
void C51InitTimer(unsigned char num);//T0 num*25ms
void Timer2Init(unsigned char num);		//num*25ms@11.0592MHz
void ConfigUART1();//����T1����ͨ��
void ConfigUART2();//
void Delayms(unsigned int ms);//���뼶��ʱ
void C51StopTimer();//ֹͣ��ʱ
void C51Delay1us(unsigned short n);//��ʱn*1us����
void Timer02ms();//2msT0��ʱ
#endif