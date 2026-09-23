#include "ds1307.h"
#include "i2c.h"

uint8_t DS1307_BCDToDec(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8_t DS1307_DecToBCD(uint8_t dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

void DS1307_Init(void)
{
    uint8_t seconds;

    seconds = I2C1_ReadReg(DS1307_ADDR, 0x00);

    /* Nếu CH = 1 thì bật lại dao động */
    if (seconds & 0x80)
    {
        seconds &= 0x7F;
        I2C1_WriteReg(DS1307_ADDR, 0x00, seconds);
    }
}

void DS1307_SetTime(DS1307_Time *time)
{
    uint32_t temp;

    I2C1_Start();

    /* DS1307 address + WRITE */
    I2C1_DR = (DS1307_ADDR << 1);

    /* Wait ADDR */
    while (!(I2C1_SR1 & (1 << 1)));

    /* Clear ADDR */
    temp = I2C1_SR1;
    temp = I2C1_SR2;
    (void)temp;

    /* Register 0x00 */
    I2C1_Write(0x00);

    /* Seconds */
    I2C1_Write(
        DS1307_DecToBCD(time->seconds) & 0x7F
    );

    /* Minutes */
    I2C1_Write(
        DS1307_DecToBCD(time->minutes)
    );

    /* Hours */
    I2C1_Write(
        DS1307_DecToBCD(time->hours)
    );

    /* Day */
    I2C1_Write(
        DS1307_DecToBCD(time->day)
    );

    /* Date */
    I2C1_Write(
        DS1307_DecToBCD(time->date)
    );

    /* Month */
    I2C1_Write(
        DS1307_DecToBCD(time->month)
    );

    /* Year */
    I2C1_Write(
        DS1307_DecToBCD(time->year)
    );

    I2C1_Stop();
}

void DS1307_GetTime(DS1307_Time *time)
{
    time->seconds =
        DS1307_BCDToDec(
            I2C1_ReadReg(DS1307_ADDR, 0x00) & 0x7F
        );

    time->minutes =
        DS1307_BCDToDec(
            I2C1_ReadReg(DS1307_ADDR, 0x01) & 0x7F
        );

    time->hours =
        DS1307_BCDToDec(
            I2C1_ReadReg(DS1307_ADDR, 0x02) & 0x3F
        );

    time->day =
        DS1307_BCDToDec(
            I2C1_ReadReg(DS1307_ADDR, 0x03) & 0x07
        );

    time->date =
        DS1307_BCDToDec(
            I2C1_ReadReg(DS1307_ADDR, 0x04) & 0x3F
        );

    time->month =
        DS1307_BCDToDec(
            I2C1_ReadReg(DS1307_ADDR, 0x05) & 0x1F
        );

    time->year =
        DS1307_BCDToDec(
            I2C1_ReadReg(DS1307_ADDR, 0x06)
        );
}
