#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018)
#define GPIOC_CRH   (*(volatile uint32_t *)0x40011004)
#define GPIOC_ODR   (*(volatile uint32_t *)0x4001100C)

static void delay_ms(uint16_t t)
{
    uint32_t i, j;

    for (i = 0; i < t; i++)
    {
        for (j = 0; j < 8000; j++)
        {
            __asm volatile ("nop");
        }
    }
}

int main(void)
{
    // Enable clock cho GPIOC
    RCC_APB2ENR |= 0xFC;

    // PC13 = Output Push-Pull, 10 MHz
    GPIOC_CRH = 0x44344444;

    while (1)
    {
        // Toggle PC13
        GPIOC_ODR ^= (1 << 13);

        delay_ms(100);
    }
}
