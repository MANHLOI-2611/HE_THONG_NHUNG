#include "stm32f1xx.h"

#define MA_LOP  "HE_THONG_NHUNG"
#define MA_NHOM "05"

/* ================= UART1 ================= */

void UART1_Init(void)
{
    /* Enable GPIOB clock */
    RCC->APB2ENR |= (1UL << 3);

    /* Enable AFIO clock */
    RCC->APB2ENR |= (1UL << 0);

    /* Enable USART1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* USART1 Remap: PB6 = TX, PB7 = RX */
    (*(volatile uint32_t *)0x40010004UL) |= (1UL << 2);

    /* PB6 = Alternate Function Push-Pull, 50 MHz */
    (*(volatile uint32_t *)0x40010C00UL) &= ~(0xFUL << 24);
    (*(volatile uint32_t *)0x40010C00UL) |=  (0xBUL << 24);

    /* PB7 = Input Pull-up/Pull-down */
    (*(volatile uint32_t *)0x40010C00UL) &= ~(0xFUL << 28);
    (*(volatile uint32_t *)0x40010C00UL) |=  (0x8UL << 28);

    /* PB7 Pull-up */
    (*(volatile uint32_t *)0x40010C04UL) |= (1UL << 7);

    /* 9600 baud, USART1 clock = 8 MHz */
    USART1->BRR = 0x0341;

    /* Enable USART1, TX and RX */
    USART1->CR1 =
        USART_CR1_UE |
        USART_CR1_TE |
        USART_CR1_RE;
}

void UART1_SendChar(char c)
{
    while (!(USART1->SR & USART_SR_TXE))
    {
    }

    USART1->DR = (uint8_t)c;
}

void UART1_SendString(const char *str)
{
    while (*str)
    {
        UART1_SendChar(*str);
        str++;
    }
}

/* ================= ADC1 ================= */

/*
 * PA7 = ADC1_IN7
 */

void ADC1_Init(void)
{
    /* Enable GPIOA clock */
    RCC->APB2ENR |= (1UL << 2);

    /* Enable ADC1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /*
     * PA7 = Analog input
     * MODE = 00
     * CNF  = 00
     */
    GPIOA->CRL &= ~(0xFUL << 28);

    /* ADC clock = PCLK2 / 6 = 8 MHz / 6 */
    RCC->CFGR &= ~(7UL << 14);
    RCC->CFGR |=  (2UL << 14);

    /* Turn on ADC */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* Small delay */
    for (volatile int i = 0; i < 10000; i++)
    {
    }

    /* Reset calibration */
    ADC1->CR2 |= ADC_CR2_RSTCAL;

    while (ADC1->CR2 & ADC_CR2_RSTCAL)
    {
    }

    /* Start calibration */
    ADC1->CR2 |= ADC_CR2_CAL;

    while (ADC1->CR2 & ADC_CR2_CAL)
    {
    }

    /* Select channel 7 */
    ADC1->SQR3 = 7;

    /* One conversion */
    ADC1->SQR1 = 0;

}

uint16_t ADC1_Read(void)
{
    ADC1->SR &= ~ADC_SR_EOC;

    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while (!(ADC1->SR & ADC_SR_EOC))
    {
    }

    return (uint16_t)ADC1->DR;
}

/* ================= Delay ================= */

void Delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        for (volatile uint32_t j = 0; j < 8000; j++)
        {
        }
    }
}

/* ================= Integer to String ================= */

void UART1_SendNumber(uint32_t number)
{
    char buffer[12];
    int i = 0;

    if (number == 0)
    {
        UART1_SendChar('0');
        return;
    }

    while (number > 0)
    {
        buffer[i++] = '0' + (number % 10);
        number /= 10;
    }

    while (i > 0)
    {
        UART1_SendChar(buffer[--i]);
    }
}

/* ================= MAIN ================= */

int main(void)
{
    uint16_t adc_value;
    uint32_t voltage_mv;

    UART1_Init();
    ADC1_Init();

    UART1_SendString("STM32 ADC LDR READY\r\n");

    while (1)
    {
        /* Read PA7 */
        adc_value = ADC1_Read();

        /*
         * STM32F103 ADC = 12 bit
         * ADC range = 0 ... 4095
         * Vref = 3.3 V
         *
         * Voltage(mV) = ADC * 3300 / 4095
         */
        voltage_mv = ((uint32_t)adc_value * 3300UL) / 4095UL;

        UART1_SendString(MA_LOP);
        UART1_SendString(MA_NHOM);
        UART1_SendString(": ADC = ");

        UART1_SendNumber(adc_value);

        UART1_SendString(", Voltage = ");

        UART1_SendNumber(voltage_mv / 1000);
        UART1_SendChar('.');
        UART1_SendNumber(voltage_mv % 1000);

        UART1_SendString(" V\r\n");

        /* Send every 1 second */
        Delay_ms(1000);
    }
}

void USART1_IRQHandler(void)
{
}
