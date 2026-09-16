#include "stm32f1xx.h"

#define BUFFER_SIZE 64

volatile char rx_buffer[BUFFER_SIZE];
volatile uint32_t rx_index = 0;
volatile uint8_t command_ready = 0;

volatile uint8_t led_state = 0;
volatile uint8_t pwm_percent =100;


/* =========================================================
 * UART1
 * PB6 = USART1_TX
 * PB7 = USART1_RX
 * 9600 8N1
 * ========================================================= */

void UART1_Init(void)
{
    /* Enable GPIOB clock */
    RCC->APB2ENR |= (1UL << 3);

    /* Enable AFIO clock */
    RCC->APB2ENR |= (1UL << 0);

    /* Enable USART1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /*
     * USART1 remap:
     * PB6 = TX
     * PB7 = RX
     */
    (*(volatile uint32_t *)0x40010004UL) |= (1UL << 2);

    /* PB6: Alternate function push-pull, 50 MHz */
    (*(volatile uint32_t *)0x40010C00UL) &= ~(0xFUL << 24);
    (*(volatile uint32_t *)0x40010C00UL) |=  (0xBUL << 24);

    /* PB7: Input pull-up */
    (*(volatile uint32_t *)0x40010C00UL) &= ~(0xFUL << 28);
    (*(volatile uint32_t *)0x40010C00UL) |=  (0x8UL << 28);

    (*(volatile uint32_t *)0x40010C04UL) |= (1UL << 7);

    /* 9600 baud, assuming USART clock = 8 MHz */
    USART1->BRR = 0x0341;

    /*
     * UE  = USART enable
     * TE  = transmitter enable
     * RE  = receiver enable
     * RXNEIE = RX interrupt enable
     */
    USART1->CR1 =
        USART_CR1_UE |
        USART_CR1_TE |
        USART_CR1_RE |
        USART_CR1_RXNEIE;

    /*
     * Enable USART1 interrupt in NVIC
     *
     * USART1 IRQ number = 37
     * ISER1 bit = 37 - 32 = 5
     */
    (*(volatile uint32_t *)0xE000E104UL) |= (1UL << 5);
}


/* =========================================================
 * UART SEND
 * ========================================================= */

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


/* =========================================================
 * TIM1 PWM
 *
 * PA8 = TIM1_CH1
 *
 * PWM frequency:
 *
 * Timer clock = 8 MHz
 * PSC = 79
 *
 * Counter clock = 8 MHz / 80 = 100 kHz
 *
 * ARR = 999
 *
 * PWM frequency = 100000 / 1000 = 100 Hz
 * ========================================================= */

void PWM_Init(void)
{
    /* Enable GPIOA clock */
    RCC->APB2ENR |= (1UL << 2);

    /* Enable TIM1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    /*
     * PA8:
     * Alternate function push-pull
     * 50 MHz
     *
     * CRH bits 0..3
     */
    GPIOA->CRH &= ~(0xFUL << 0);
    GPIOA->CRH |=  (0xBUL << 0);

    /* Prescaler */
    TIM1->PSC = 79;

    /* Auto reload */
    TIM1->ARR = 999;

    /*
     * PWM mode 1
     *
     * OC1M = 110
     * CCMR1 bits 4..6
     */
    TIM1->CCMR1 &= ~(7UL << 4);
    TIM1->CCMR1 |=  (6UL << 4);

    /* Enable preload for CCR1 */
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE;

    /* Enable CH1 output */
    TIM1->CCER |= TIM_CCER_CC1E;

    /*
     * TIM1 is an advanced timer.
     * MOE must be enabled.
     */
    TIM1->BDTR |= TIM_BDTR_MOE;

    /* Start with LED OFF */
    TIM1->CCR1 = 0;

    /* Generate update event */
    TIM1->EGR |= TIM_EGR_UG;

    /* Enable timer */
    TIM1->CR1 |= TIM_CR1_CEN;
}


/* =========================================================
 * SET PWM PERCENT
 * ========================================================= */

void PWM_SetPercent(uint8_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    }

    /* Save latest PWM value */
    pwm_percent = percent;

    /*
     * If LED is currently ON,
     * immediately apply new PWM.
     *
     * If LED is OFF,
     * only save the value.
     */
    if (led_state)
    {
        TIM1->CCR1 = ((uint32_t)percent * 999UL) / 100UL;
    }
}


/* =========================================================
 * LED ON
 * ========================================================= */

void LED_ON(void)
{
    led_state = 1;

    /*
     * Apply the latest saved PWM percentage.
     */
    TIM1->CCR1 =
        ((uint32_t)pwm_percent * 999UL) / 100UL;
}


/* =========================================================
 * LED OFF
 * ========================================================= */

void LED_OFF(void)
{
    led_state = 0;

    /*
     * Set duty cycle to 0%.
     */
    TIM1->CCR1 = 0;
}


/* =========================================================
 * STRING COMPARE
 * ========================================================= */

uint8_t StringEqual(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (*a != *b)
        {
            return 0;
        }

        a++;
        b++;
    }

    if (*a == '\0' && *b == '\0')
    {
        return 1;
    }

    return 0;
}


/* =========================================================
 * PROCESS PWM COMMAND
 *
 * Format:
 *
 * PWM:50%
 *
 * PWM:100%
 *
 * PWM:25%
 * ========================================================= */

void Process_PWM_Command(const char *cmd)
{
    uint32_t value = 0;
    uint8_t i = 4;

    /*
     * cmd:
     *
     * P W M : 5 0 %
     * 0 1 2 3 4 5 6
     */

    while (cmd[i] >= '0' && cmd[i] <= '9')
    {
        value = value * 10UL + (uint32_t)(cmd[i] - '0');
        i++;
    }

    if (cmd[i] != '%')
    {
        UART1_SendString("PWM ERROR\r\n");
        return;
    }

    if (value > 100)
    {
        UART1_SendString("PWM ERROR\r\n");
        return;
    }

    PWM_SetPercent((uint8_t)value);

    UART1_SendString("PWM OK\r\n");
}


/* =========================================================
 * PROCESS COMMAND
 *
 * ON!
 * OFF!
 * PWM:50%!
 * Status!
 * ========================================================= */

void Process_Command(void)
{
    if (StringEqual((const char *)rx_buffer, "ON"))
    {
        LED_ON();

        UART1_SendString("LED ON\r\n");
    }
    else if (StringEqual((const char *)rx_buffer, "OFF"))
    {
        LED_OFF();

        UART1_SendString("LED OFF\r\n");
    }
    else if (
        rx_buffer[0] == 'P' &&
        rx_buffer[1] == 'W' &&
        rx_buffer[2] == 'M' &&
        rx_buffer[3] == ':'
    )
    {
        Process_PWM_Command((const char *)rx_buffer);
    }
    else if (StringEqual((const char *)rx_buffer, "Status"))
    {
        if (led_state)
        {
            UART1_SendString("Status: ON\r\n");
        }
        else
        {
            UART1_SendString("Status: OFF\r\n");
        }
    }
    else
    {
        UART1_SendString("UNKNOWN COMMAND\r\n");
    }
}


/* =========================================================
 * USART1 INTERRUPT HANDLER
 * ========================================================= */

void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_RXNE)
    {
        char c;

        /*
         * Reading DR clears RXNE.
         */
        c = (char)(USART1->DR & 0xFF);

        /*
         * ! = end of command
         */
        if (c == '!')
        {
            rx_buffer[rx_index] = '\0';

            command_ready = 1;

            rx_index = 0;
        }
        else
        {
            if (rx_index < BUFFER_SIZE - 1)
            {
                rx_buffer[rx_index] = c;

                rx_index++;
            }
            else
            {
                /*
                 * Buffer overflow.
                 * Reset buffer.
                 */
                rx_index = 0;
            }
        }
    }
}


/* =========================================================
 * MAIN
 * ========================================================= */

int main(void)
{
    UART1_Init();

    PWM_Init();

    UART1_SendString("STM32 BAI5 READY\r\n");
UART1_SendString("CR1=");
if (USART1->CR1 & USART_CR1_RXNEIE)
    UART1_SendString("RXIE_ON\r\n");
else
    UART1_SendString("RXIE_OFF\r\n");

UART1_SendString("NVIC=");
if ((*(volatile uint32_t *)0xE000E104UL) & (1UL << 5))
    UART1_SendString("IRQ_ON\r\n");
else
    UART1_SendString("IRQ_OFF\r\n");

    UART1_SendString("Commands:\r\n");
    UART1_SendString("ON!\r\n");
    UART1_SendString("OFF!\r\n");
    UART1_SendString("PWM:50%!\r\n");
    UART1_SendString("Status!\r\n");

    while (1)
    {
        /*
         * Command is received by UART interrupt.
         *
         * Main loop only processes the completed command.
         */
        if (command_ready)
        {
            command_ready = 0;

            Process_Command();
        }
    }
}
