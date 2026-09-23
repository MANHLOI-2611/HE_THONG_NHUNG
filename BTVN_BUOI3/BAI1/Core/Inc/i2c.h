#ifndef I2C_H
#define I2C_H

#include "stm32f1xx.h"

void I2C1_Init(void);

void I2C1_Start(void);
void I2C1_Stop(void);

void I2C1_Write(uint8_t data);
uint8_t I2C1_Read_ACK(void);
uint8_t I2C1_Read_NACK(void);

void I2C1_WriteReg(uint8_t device_addr,
                   uint8_t reg_addr,
                   uint8_t data);

uint8_t I2C1_ReadReg(uint8_t device_addr,
                     uint8_t reg_addr);

#endif
