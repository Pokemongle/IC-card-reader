//»½ÐÑpn532
void I2C_Send_Byte(uint8_t byte)
{
	uint8_t i = 8;

	while(i)
	{		
		if (byte & 0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
		sw_delay_us(I2C_WIDTH);
        
		SCL_H;
        sw_delay_us(I2C_WIDTH);
        
        /*Wait PN532 Wakeup*/
        while(!SCL_READ)
        {
            SCL_H;
        }
        
        SCL_L;
        sw_delay_us(I2C_WIDTH);
        
        byte <<= 1;
        i--;
	}
    SCL_L;
    sw_delay_us(I2C_WIDTH);
}
