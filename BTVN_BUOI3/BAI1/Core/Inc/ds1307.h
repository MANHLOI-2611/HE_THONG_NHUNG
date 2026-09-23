#ifndef DS1307_H
#define DS1307_H

#include "stm32f1xx.h"

#define DS1307_ADDR 0x68

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} DS1307_Time;

void DS1307_Init(void);

void DS1307_SetTime(DS1307_Time *time);
void DS1307_GetTime(DS1307_Time *time);

uint8_t DS1307_BCDToDec(uint8_t bcd);
uint8_t DS1307_DecToBCD(uint8_t dec);

#endif
