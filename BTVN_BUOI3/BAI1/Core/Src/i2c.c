#include "i2c.h"

void I2C1_Init(void)
{
    /* Clock GPIOB */
    RCC_APB2ENR |= (1 << 3);

    /* Clock I2C1 */
    RCC_APB1ENR |= (1 << 21);

    /* PB6 = SCL, PB7 = SDA
       Alternate function open-drain, 50 MHz */
    GPIOB_CRL &= ~((0xF << 24) | (0xF << 28));
    GPIOB_CRL |=  ((0xF << 24) | (0xF << 28));

    /* I2C reset */
    I2C1_CR1 = (1 << 15);
    I2C1_CR1 = 0;

    /*
     * APB1 = 8 MHz
     * I2C standard mode = 100 kHz
     */
    I2C1_CR2 = 8;
    I2C1_CCR = 40;
    I2C1_TRISE = 9;

    /* Own address */
    I2C1_OAR1 = (1 << 14);

    /* Enable I2C */
    I2C1_CR1 = (1 << 0);
}

void I2C1_Start(void)
{
    I2C1_CR1 |= (1 << 8);

    while (!(I2C1_SR1 & (1 << 0)));

    /* Clear START */
    (void)I2C1_SR1;
}

void I2C1_Stop(void)
{
    I2C1_CR1 |= (1 << 9);
}

void I2C1_Write(uint8_t data)
{
    I2C1_DR = data;

    while (!(I2C1_SR1 & (1 << 7)));

    /* BTF */
    while (!(I2C1_SR1 & (1 << 2)));
}

uint8_t I2C1_Read_ACK(void)
{
    uint8_t data;

    I2C1_CR1 |= (1 << 10);

    while (!(I2C1_SR1 & (1 << 6)));

    data = (uint8_t)I2C1_DR;

    return data;
}

uint8_t I2C1_Read_NACK(void)
{
    uint8_t data;

    I2C1_CR1 &= ~(1 << 10);

    while (!(I2C1_SR1 & (1 << 6)));

    data = (uint8_t)I2C1_DR;

    return data;
}

void I2C1_WriteReg(uint8_t device_addr,
                   uint8_t reg_addr,
                   uint8_t data)
{
    uint32_t temp;

    I2C1_Start();

    /* Slave address + Write */
    I2C1_DR = (device_addr << 1);

    /* Wait ADDR */
    while (!(I2C1_SR1 & (1 << 1)));

    /* Clear ADDR */
    temp = I2C1_SR1;
    temp = I2C1_SR2;
    (void)temp;

    I2C1_Write(reg_addr);
    I2C1_Write(data);

    I2C1_Stop();
}

uint8_t I2C1_ReadReg(uint8_t device_addr,
                     uint8_t reg_addr)
{
    uint8_t data;
    uint32_t temp;

    /* START */
    I2C1_Start();

    /* Slave address + Write */
    I2C1_DR = (device_addr << 1);

    while (!(I2C1_SR1 & (1 << 1)));

    /* Clear ADDR */
    temp = I2C1_SR1;
    temp = I2C1_SR2;
    (void)temp;

    /* Register address */
    I2C1_Write(reg_addr);

    /* Repeated START */
    I2C1_CR1 |= (1 << 8);

    while (!(I2C1_SR1 & (1 << 0)));

    /* Slave address + Read */
    I2C1_DR = (device_addr << 1) | 1;

    while (!(I2C1_SR1 & (1 << 1)));

    /* Clear ADDR */
    temp = I2C1_SR1;
    temp = I2C1_SR2;
    (void)temp;

    /* NACK after receiving 1 byte */
    I2C1_CR1 &= ~(1 << 10);

    while (!(I2C1_SR1 & (1 << 6)));

    data = (uint8_t)I2C1_DR;

    I2C1_Stop();

    return data;
}
