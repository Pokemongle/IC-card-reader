//��ʼ�ź�
void I2C_start()
{
    I2C_SCL_1;
    I2C_SDA_1;
    I2C_Delay();
    I2C_SDA_0;
    I2C_Delay();
    I2C_SCL_0;
}
//�����ź�
void I2C_stop()
{
    I2C_SDA_0;
	I2C_SCL_1;
	I2C_Delay();
	I2C_SDA_1;
}
void I2C_sendbyte(uint8_t send_data)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        if(send_data&(0X80>>i)) 
            I2C_SDA_1; 
        else 
            I2C_SDA_0;
    
        I2C_SCL_1; 
        I2C_Delay(); 
        I2C_SCL_0;
    }
}
uint8_t I2C_readbyte(uint8_t ack)
{
    uint8_t read_data=0;
    uint8_t i;
    uint8_t readbit;
    
    for(i=0;i<8;i++)
    {
        if(I2C_SDA_0) 
            readbit=0; 
        else 
            readbit=1;
    
        I2C_Delay();
        I2C_SCL_1;
        I2C_Delay();
        I2C_SCL_0;
            
        read_data= (read_data|readbit)<<1;
    }
    
    I2C_Delay();
    //Ӧ���ź�
    if(ack==0)
        I2C_NACK();
    else
        I2C_ACK();
    
    return Data;
}
void I2C_ACK(void)		//	Ӧ���ź�
{
    I2C_SDA_0;
    I2C_Delay();
    I2C_SCL_1;
    I2C_Delay();
    I2C_SCL_0;
    I2C_SDA_1;
}

void I2C_NACK(void)		//��Ӧ���ź�
{
    I2C_SDA_1;
    I2C_Delay();
    I2C_SCL_1;
    I2C_Delay();
    I2C_SCL_0;
    I2C_Delay();
    I2C_SDA_0;
}
uint8_t waitACK()
{
    uint8_t receive;
    
    I2C_SDA_1;
    I2C_Delay();
    I2C_SCL_1;
    I2C_Delay();
    if(I2C_SDA_1)
        receive=1;
    else   
        receive=0;
    I2C_SCL_0;
    I2C_Delay();
    return receive;   
}
void I2C_write_reg(uint8_t slaveaddr,uint8_t regaddr,uint8_t*writebuffer,uint8_t Wlen)
{
    uint8_t writeflag=0;
    uint8_t i;
    uint8_t error;
    
    I2C_start();						//��ʼ�ź�
    I2C_sendbyte(slaveaddr|0x00);		//Ѱַ�źţ��ӻ���ַ+д����
    
    if(waitACK)							//�ȴ��ӻ�Ӧ���ź�
        error|=0x01;
    
    I2C_sendbyte(regaddr);				//�Ĵ�����ַ
       
    if(waitACK)
        error|=0x02;

    for(i=0;i<Wlen;i++)					//д������
        {
            I2C_sendbyte(writebuffer[i]);
            if(waitACK)
                error|=0x04;
        }
        
    I2C_stop();    						//ֹͣ�ź�
}
void I2C_read_reg(uint8_t slaveaddr,uint8_t regaddr,uint8_t*readbuffer,uint8_t Rlen)
{
   uint8_t error=0;
   uint8_t i=0;
    
   I2C_start();
   I2C_sendbyte(slaveaddr|0x00);
   if(waitACK)
       error|=0x01;
   
   I2C_sendbyte(regaddr);
   if(waitACK)
       error|=0x02;
   
   I2C_sendbyte(slaveaddr|0x01);
   if(waitACK)
       error|=0x04;
   
   for(i=0;i<=(Rlen-1);i++)
   {
       *readbuffer=I2C_readbyte(1);	//ÿ��ȡ��һ���ֽڣ�����Ҫ���ӻ�һ��Ӧ���ź�
       readbuffer++;
   }
   *readbuffer=I2C_readbyte(0);		//���һ���ֽڶ�ȡ�귢�ͷ�Ӧ���ź�
   I2C_stop();
   
}
