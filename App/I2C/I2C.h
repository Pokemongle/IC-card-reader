//--I2C
sbit I2C_SCL=P0^7;
sbit I2C_SDA=P0^6;
#define I2C_Delay()  {_nop_();_nop_();_nop_();_nop_();_nop_();}
bit I2CAddressing(unsigned char addr);
void I2CStart();
void I2CStop();
bit I2CWrite(unsigned char dat);
bit I2CAddressing(unsigned char addr);
unsigned char I2CReadNAK();
unsigned char I2CReadACK();
