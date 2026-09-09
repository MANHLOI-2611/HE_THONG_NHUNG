#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018)

#define GPIOA_CRL   (*(volatile uint32_t *)0x40010800)
#define GPIOA_ODR   (*(volatile uint32_t *)0x4001080C)

#define IOPAEN (1 << 2)

void delay(volatile uint32_t time)
{
    while (time--)
    {
        for (volatile uint32_t i = 0; i < 800; i++)
        {
            __asm volatile ("nop");
        }
    }
}

int main(void)
{
    /* Bật clock GPIOA */
    RCC_APB2ENR |= IOPAEN;

    /* PA0 - PA7: Output Push-Pull */
    GPIOA_CRL = 0x11111111;

    while (1)
    {
        /* LED chạy PA0 -> PA7 */
        for (int i = 0; i < 8; i++)
        {
            GPIOA_ODR = (1 << i);
            delay(500);
        }

        /* LED chạy PA6 -> PA0 */
        for (int i = 6; i >= 0; i--)
        {
            GPIOA_ODR = (1 << i);
            delay(500);
        }
    }
}
