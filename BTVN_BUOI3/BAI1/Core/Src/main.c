#include "stm32f1xx.h"
#include "i2c.h"
#include "ds1307.h"

void UART1_Init(void)
{
    RCC_APB2ENR |= (1 << 2);
    RCC_APB2ENR |= (1 << 14);

    /* PA9 = TX */
    GPIOA_CRH &= ~(0xF << 4);
    GPIOA_CRH |=  (0xB << 4);

    /* PA10 = RX */
    GPIOA_CRH &= ~(0xF << 8);
    GPIOA_CRH |=  (0x4 << 8);

    /* HSI 8 MHz -> 115200 */
    USART1_BRR = 0x45;

    USART1_CR1 =
        (1 << 13) |
        (1 << 3)  |
        (1 << 2);
}

void UART1_SendChar(char c)
{
    while (!(USART1_SR & (1 << 7)));
    USART1_DR = c;
}

void UART1_SendString(const char *s)
{
    while (*s)
        UART1_SendChar(*s++);
}

uint8_t UART1_GetChar(void)
{
    while (!(USART1_SR & (1 << 5)));
    return (uint8_t)USART1_DR;
}

uint8_t ToDigit(char c)
{
    return c - '0';
}

void UART1_Send2Digit(uint8_t value)
{
    UART1_SendChar((value / 10) + '0');
    UART1_SendChar((value % 10) + '0');
}

void Delay1s(void)
{
    volatile uint32_t i;

    for (i = 0; i < 800000; i++);
}

void PrintTime(void)
{
    DS1307_Time time;

    DS1307_GetTime(&time);

    UART1_SendString("TIME: ");

    UART1_Send2Digit(time.hours);
    UART1_SendChar(':');

    UART1_Send2Digit(time.minutes);
    UART1_SendChar(':');

    UART1_Send2Digit(time.seconds);

    UART1_SendChar(' ');

    UART1_Send2Digit(time.date);
    UART1_SendChar('/');

    UART1_Send2Digit(time.month);
    UART1_SendChar('/');

    UART1_Send2Digit(time.year);

    UART1_SendString("\r\n");
}

int main(void)
{
    char cmd[21];
    uint8_t index = 0;
    char c;

    DS1307_Time time;

    UART1_Init();
    I2C1_Init();
    DS1307_Init();

    UART1_SendString("\r\nDS1307 READY\r\n");
    UART1_SendString("SET HH:MM:SS DD/MM/YY\r\n");

    /*
     * Chờ lệnh SET
     */
    while (1)
    {
        c = UART1_GetChar();

        if (index < 21)
        {
            cmd[index] = c;
            index++;
        }

        /*
         * Đủ 21 ký tự:
         *
         * SET 22:30:00 23/09/26
         * 012345678901234567890
         */
        if (index == 21)
        {
            if (cmd[0] == 'S' &&
                cmd[1] == 'E' &&
                cmd[2] == 'T' &&
                cmd[3] == ' ' &&
                cmd[6] == ':' &&
                cmd[9] == ':' &&
                cmd[12] == ' ' &&
                cmd[15] == '/' &&
                cmd[18] == '/')
            {
                time.hours =
                    ToDigit(cmd[4]) * 10 +
                    ToDigit(cmd[5]);

                time.minutes =
                    ToDigit(cmd[7]) * 10 +
                    ToDigit(cmd[8]);

                time.seconds =
                    ToDigit(cmd[10]) * 10 +
                    ToDigit(cmd[11]);

                time.date =
                    ToDigit(cmd[13]) * 10 +
                    ToDigit(cmd[14]);

                time.month =
                    ToDigit(cmd[16]) * 10 +
                    ToDigit(cmd[17]);

                time.year =
                    ToDigit(cmd[19]) * 10 +
                    ToDigit(cmd[20]);

                DS1307_SetTime(&time);

                UART1_SendString("\r\nSET OK\r\n");

                break;
            }

            UART1_SendString("\r\nFORMAT ERROR\r\n");

            index = 0;
        }
    }

    /*
     * SET xong:
     * DS1307 tự chạy và liên tục gửi thời gian
     */
    while (1)
    {
        PrintTime();

        Delay1s();
    }
}

