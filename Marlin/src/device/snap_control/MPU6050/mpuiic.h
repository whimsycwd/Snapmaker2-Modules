#ifndef __MPUIIC_H
#define __MPUIIC_H
#include <stdint.h>

#define MPU_SDA_PIN iic_sda_pin
#define MPU_SCL_PIN iic_scl_pin

#define MPU_SDA_IN()   MPU_SDA_PIN.SetMode(GPIO_IN_FLOATING)
#define MPU_SDA_OUT()  MPU_SDA_PIN.SetMode(GPIO_OUT_PP)

#define MPU_IIC_SCL_H     MPU_SCL_PIN.High()
#define MPU_IIC_SCL_L     MPU_SCL_PIN.Low()
#define MPU_IIC_SDA_H     MPU_SDA_PIN.High()
#define MPU_IIC_SDA_L     MPU_SDA_PIN.Low()

#define MPU_READ_SDA   	MPU_SDA_PIN.Read()


void MPU_IIC_Delay(void);
void MPU_IIC_Init(uint8_t sda_pin, uint8_t scl_pin);
void MPU_IIC_Start(void);
void MPU_IIC_Stop(void);
void MPU_IIC_Send_Byte(uint8_t txd);
uint8_t MPU_IIC_Read_Byte(unsigned char ack);
uint8_t MPU_IIC_Wait_Ack(void);
void MPU_IIC_Ack(void);
void MPU_IIC_NAck(void);

void IMPU_IC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t MPU_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















