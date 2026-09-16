#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018)

#define GPIOA_CRL   (*(volatile uint32_t *)0x40010800)
#define GPIOA_IDR   (*(volatile uint32_t *)0x40010808)
#define GPIOA_ODR   (*(volatile uint32_t *)0x4001080C)

#define IOPAEN      (1 << 2)

#define BUTTON_PIN  (1 << 0)   // PA0
#define LED_PIN     (1 << 1)   // PA1


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
    // Bật clock GPIOA
    RCC_APB2ENR |= IOPAEN;

    /*
        PA0 = Input Pull-up
        PA1 = Output Push-Pull 2MHz

        PA0: 0x8
        PA1: 0x2

        => CRL[7:0] = 0x28
    */
    GPIOA_CRL &= ~0xFF;
    GPIOA_CRL |= 0x28;

    // PA0 bật pull-up
    GPIOA_ODR |= BUTTON_PIN;

    // PA1 tắt LED ban đầu
    GPIOA_ODR &= ~LED_PIN;


    while (1)
    {
        // Kiểm tra nút được nhấn
        if ((GPIOA_IDR & BUTTON_PIN) == 0)
        {
            // Chống dội nút
            delay(20);

            if ((GPIOA_IDR & BUTTON_PIN) == 0)
            {
                // Chờ người dùng nhả nút
                while ((GPIOA_IDR & BUTTON_PIN) == 0)
                {
                }

                // Chống dội khi nhả
                delay(20);

                // Đổi trạng thái LED
                GPIOA_ODR ^= LED_PIN;
            }
        }
    }
}
