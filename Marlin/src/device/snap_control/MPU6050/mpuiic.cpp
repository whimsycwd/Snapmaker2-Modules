#include "mpuiic.h"
#include "../../../HAL/hal_include.h"

HalGPIO iic_sda_pin;
HalGPIO iic_scl_pin;
 
void MPU_IIC_Delay(void)
{
	volatile uint32_t i = 40;
	while(i--);
	// delay_us(2);
}

void MPU_IIC_Init(uint8_t sda_pin, uint8_t scl_pin)
{
	iic_sda_pin.Init(sda_pin, GPIO_OUT_PP);
	iic_scl_pin.Init(scl_pin, GPIO_OUT_PP);
}

void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();
	MPU_IIC_SDA_H;
	MPU_IIC_SCL_H;
	MPU_IIC_Delay();
	MPU_IIC_SDA_L;
	MPU_IIC_Delay();
	MPU_IIC_SCL_L;
}

void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();
	MPU_IIC_SCL_L;
	MPU_IIC_SDA_L;
	MPU_IIC_Delay();
	MPU_IIC_SCL_H;
	MPU_IIC_SDA_H;
	MPU_IIC_Delay();
}


// return 1-fail 0-ok
uint8_t MPU_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	MPU_SDA_IN();
	MPU_IIC_SDA_H;
	MPU_IIC_Delay();
	MPU_IIC_SCL_H;
	MPU_IIC_Delay();
	while (MPU_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL_L;
	return 0;
}

void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL_L;
	MPU_SDA_OUT();
	MPU_IIC_SDA_L;
	MPU_IIC_Delay();
	MPU_IIC_SCL_H;
	MPU_IIC_Delay();
	MPU_IIC_SCL_L;
}

void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL_L;
	MPU_SDA_OUT();
	MPU_IIC_SDA_H;
	MPU_IIC_Delay();
	MPU_IIC_SCL_H;
	MPU_IIC_Delay();
	MPU_IIC_SCL_L;
}

void MPU_IIC_Send_Byte(uint8_t txd)
{
	uint8_t t;
	MPU_SDA_OUT();
	MPU_IIC_SCL_L;
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7)
			MPU_IIC_SDA_H;
		else
			MPU_IIC_SDA_L;
		txd <<= 1;
		MPU_IIC_SCL_H;
		MPU_IIC_Delay();
		MPU_IIC_SCL_L;
		MPU_IIC_Delay();
	}
}

uint8_t MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	MPU_SDA_IN();
	for (i = 0; i < 8; i++)
	{
		MPU_IIC_SCL_L;
		MPU_IIC_Delay();
		MPU_IIC_SCL_H;
		receive <<= 1;
		if (MPU_READ_SDA)
			receive++;
		MPU_IIC_Delay();
	}
	if (!ack)
		MPU_IIC_NAck();
	else
		MPU_IIC_Ack();
	return receive;
}
