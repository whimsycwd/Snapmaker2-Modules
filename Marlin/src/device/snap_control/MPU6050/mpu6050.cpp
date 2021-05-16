#include "mpu6050.h"
#include "../snap_time.h"

static uint8_t mpu_addr = 0x68;

// return 1-fail 0-ok
uint8_t MPU_Init(uint8_t sda_pin, uint8_t scl_pin, uint8_t addr)
{
	uint8_t res;
	MPU_IIC_Init(sda_pin, scl_pin);
	mpu_addr = addr;
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80);
	SnapTimer::SnapTimer::DelayMs(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00);
	MPU_Set_Gyro_Fsr(3);  //±2000dps
	MPU_Set_Accel_Fsr(0); //±2g
	MPU_Set_Rate(50);
	MPU_Write_Byte(MPU_INT_EN_REG, 0X00);
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00);
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80);
	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if (res == mpu_addr)
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01);
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00);
		MPU_Set_Rate(50);
	}
	else
		return 1;
	return mpu_dmp_init(sda_pin, scl_pin);
}

//fsr:0,±250dps; 1,±500dps; 2,±1000dps; 3,±2000dps
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3); //设置陀螺仪满量程范围
}
//fsr:0,±2g; 1,±4g; 2,±8g; 3,±16g
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3); //设置加速度传感器满量程范围
}

uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data = 0;
	if (lpf >= 188)
		data = 1;
	else if (lpf >= 98)
		data = 2;
	else if (lpf >= 42)
		data = 3;
	else if (lpf >= 20)
		data = 4;
	else if (lpf >= 10)
		data = 5;
	else
		data = 6;
	return MPU_Write_Byte(MPU_CFG_REG, data);
}

//rate:4~1000(Hz)
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if (rate > 1000)
		rate = 1000;
	if (rate < 4)
		rate = 4;
	data = 1000 / rate - 1;
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data);
	return MPU_Set_LPF(rate / 2);
}

// return temp * 100
short MPU_Get_Temperature(void)
{
	uint8_t buf[2];
	short raw;
	float temp;
	MPU_Read_Len(mpu_addr, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((uint16_t)buf[0] << 8) | buf[1];
	temp = 36.53 + ((double)raw) / 340;
	return temp * 100;
	;
}

uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
	uint8_t buf[6], res;
	res = MPU_Read_Len(mpu_addr, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*gx = ((uint16_t)buf[0] << 8) | buf[1];
		*gy = ((uint16_t)buf[2] << 8) | buf[3];
		*gz = ((uint16_t)buf[4] << 8) | buf[5];
	}
	return res;
	;
}

uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
	uint8_t buf[6], res;
	res = MPU_Read_Len(mpu_addr, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*ax = ((uint16_t)buf[0] << 8) | buf[1];
		*ay = ((uint16_t)buf[2] << 8) | buf[3];
		*az = ((uint16_t)buf[4] << 8) | buf[5];
	}
	return res;
	;
}

uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	uint8_t i;
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr << 1) | 0);
	if (MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	for (i = 0; i < len; i++)
	{
		MPU_IIC_Send_Byte(buf[i]);
		if (MPU_IIC_Wait_Ack())
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_Stop();
	return 0;
}

uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr << 1) | 0);
	if (MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr << 1) | 1);
	MPU_IIC_Wait_Ack();
	while (len)
	{
		if (len == 1)
			*buf = MPU_IIC_Read_Byte(0);
		else
			*buf = MPU_IIC_Read_Byte(1);
		len--;
		buf++;
	}
	MPU_IIC_Stop();
	return 0;
}

uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((mpu_addr << 1) | 0);
	if (MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Send_Byte(data);
	if (MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Stop();
	return 0;
}

uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t res;
	
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((mpu_addr << 1) | 0);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((mpu_addr << 1) | 1);
	MPU_IIC_Wait_Ack();
	res = MPU_IIC_Read_Byte(0);
	MPU_IIC_Stop();
	return res;
}
