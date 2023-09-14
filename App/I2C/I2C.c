#include<reg52.h>
#include<intrins.h>
#include"I2C.h"

bit I2CAddressing(unsigned char addr)
{
 	bit ack;
	I2CStart();
	ack=I2CWrite(addr<<1);
	I2CStop();
	return ack;
}

bit I2CWrite(unsigned char dat)
{
 	bit ack;
	unsigned char mask;
	for(mask=0x80;mask!=0;mask>>=1)
	{
	 	if((mask&dat)==0)
		{
		 	I2C_SDA=0;
		}
		else
		{
		 	I2C_SDA=1;
		}
		I2C_Delay();
		I2C_SCL=1;
		I2C_Delay();
		I2C_SCL=0;
	}
	I2C_SDA=1;
	I2C_Delay();
	I2C_SCL=1;
	ack=I2C_SDA;
	I2C_Delay();
	I2C_SCL=0;
	return ~ack;//0=不存在或忙或写入失败，1=存在且空闲或写入成功
}

void I2CStop()
{
 	I2C_SCL=0;
	I2C_SDA=0;
	I2C_Delay();
	I2C_SCL=1;
	I2C_Delay();
	I2C_SDA=1;
	I2C_Delay();
}

void I2CStart()
{
 	I2C_SDA=1;
	I2C_SCL=1;
	I2C_Delay();
	I2C_SDA=0;
	I2C_Delay();
	I2C_SCL=0;
}

unsigned char I2CReadNAK()
{
 	unsigned char mask;
	unsigned char dat;

	I2C_SDA=1;
	for(mask=0x80;mask!=0;mask>>=1)
	{
	 	I2C_Delay();
		I2C_SCL=1;
		if(I2C_SDA==0)
		{
		 	dat&=~mask;
		}
		else
		{
		 	dat|=mask;
		}
		I2C_Delay();
		I2C_SCL=0;
	}
	I2C_SDA=1;
	I2C_Delay();
	I2C_SCL=1;
	I2C_Delay();
	I2C_SCL=0;
	return dat;
}

unsigned char I2CReadACK()
{
 	unsigned char mask;
	unsigned char dat;

	I2C_SDA=1;
	for(mask=0x80;mask!=0;mask>>1)
	{
	 	I2C_Delay();
		I2C_SCL=1;
		if(I2C_SDA==0)
		{
		 	dat&=~mask;
		}
		else
		{
		 	dat|=mask;
		}
		I2C_Delay();
		I2C_SCL=0;
	}
	I2C_SDA=0;
	I2C_Delay();
	I2C_SCL=1;
	I2C_Delay();
	I2C_SCL=0;

	return dat;
}

