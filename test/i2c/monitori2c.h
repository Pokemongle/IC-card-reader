/**
  ******************************************************************************
  * @file    sw_i2c.h
  * @author  zhujun
  * @version V1.0
  * @date    2019-07-01
  * @brief   软件模拟I2C相关操作头文件
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2017 MindX</center></h2>
  ******************************************************************************
  */ 
#ifndef __SW_I2C_H__
#define __SW_I2C_H__

#include "stm32f10x.h"

#define I2C_SCL_RCC                 RCC_APB2Periph_GPIOB
#define I2C_SCL_PORT                GPIOB
#define I2C_SCL_PIN                 GPIO_Pin_6

#define I2C_SDA_RCC                 RCC_APB2Periph_GPIOB
#define I2C_SDA_PORT                GPIOB
#define I2C_SDA_PIN                 GPIO_Pin_7

#define SCL_H                       I2C_SCL_PORT->BSRR = I2C_SCL_PIN  
#define SCL_L                       I2C_SCL_PORT->BRR  = I2C_SCL_PIN   
         
#define SDA_H                       I2C_SDA_PORT->BSRR = I2C_SDA_PIN
#define SDA_L                       I2C_SDA_PORT->BRR  = I2C_SDA_PIN  
      
#define SCL_READ                    ((I2C_SCL_PORT->IDR  & I2C_SCL_PIN) != (uint32_t)Bit_RESET) 
#define SDA_READ                    ((I2C_SDA_PORT->IDR  & I2C_SDA_PIN) != (uint32_t)Bit_RESET) 

#define I2C_WIDTH                   5


void I2C_Initial(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
uint8_t I2C_Wait_Ack(void);
void I2C_Send_Byte(uint8_t data);
uint8_t I2C_Read_Byte(void);
uint8_t I2C_Read(uint8_t* pbuf, uint16_t length, uint8_t addr);
uint8_t I2C_Write(uint8_t* pbuf, u16 length, uint8_t addr);

#endif

