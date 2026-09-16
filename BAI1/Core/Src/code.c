#include "stm32f1xx.h"

#define BUFFER_SIZE 128

#define MA_LOP  "HE_THONG_NHUNG"
#define MA_NHOM "05"

volatile char rx_buffer[BUFFER_SIZE];
volatile uint32_t rx_index = 0;


/* ================= UART1 INIT ================= */

void UART1_Init(void)
{
    /* Enable GPIOB clock */
    RCC->APB2ENR |= (1UL << 3);

    /* Enable AFIO clock */
    RCC->APB2ENR |= (1UL << 0);

    /* Enable USART1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;


    /*
     * USART1 REMAP
     * PB6 = TX
     * PB7 = RX
     */
    (*(volatile uint32_t *)0x40010004UL) |= (1UL << 2);


    /*
     * PB6 = TX
     * Output 50 MHz
     * Alternate Function Push-Pull
     *
     * MODE6 = 11
     * CNF6  = 10
     */
    (*(volatile uint32_t *)0x40010C00UL) &= ~(0xFUL << 24);
    (*(volatile uint32_t *)0x40010C00UL) |=  (0xBUL << 24);


    /*
     * PB7 = RX
     * Input Pull-up
     *
     * MODE7 = 00
     * CNF7  = 10
     */
    (*(volatile uint32_t *)0x40010C00UL) &= ~(0xFUL << 28);
    (*(volatile uint32_t *)0x40010C00UL) |=  (0x8UL << 28);

    /* PB7 Pull-up */
    (*(volatile uint32_t *)0x40010C04UL) |= (1UL << 7);


    /*
     * Baudrate = 9600
     * USART1 clock = 8 MHz
     */
    USART1->BRR = 0x0341;


    /*
     * Enable USART1
     * TX + RX
     */
    USART1->CR1 =
        USART_CR1_UE |
        USART_CR1_TE |
        USART_CR1_RE;
}


/* ================= UART SEND ================= */

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


/* ================= UART RECEIVE ================= */

char UART1_ReceiveChar(void)
{
    while (!(USART1->SR & USART_SR_RXNE))
    {
    }

    return (char)(USART1->DR & 0xFF);
}


/* ================= SEND MESSAGE ================= */

void Send_Message(void)
{
    UART1_SendString(MA_LOP);
    UART1_SendString(MA_NHOM);
    UART1_SendString(": ");

    UART1_SendString((const char *)rx_buffer);

    UART1_SendString("\n\r");
}


/* ================= MAIN ================= */

int main(void)
{
    char c;

    UART1_Init();

    /* Test UART */
    UART1_SendString("STM32 UART READY\r\n");

    while (1)
    {
        c = UART1_ReceiveChar();

        if (c == '!')
        {
            rx_buffer[rx_index] = '\0';

            Send_Message();

            rx_index = 0;
        }
        else
        {
            if (rx_index < BUFFER_SIZE - 1)
            {
                rx_buffer[rx_index] = c;
                rx_index++;
            }
        }
    }
}


/* ================= INTERRUPT ================= */

void USART1_IRQHandler(void)
{
}
