
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* RCC */
#define RCC_APB2ENR  (*(volatile uint32_t *)0x40021018UL)

/* GPIOA */
#define GPIOA_CRL    (*(volatile uint32_t *)0x40010800UL)
#define GPIOA_CRH    (*(volatile uint32_t *)0x40010804UL)
#define GPIOA_ODR    (*(volatile uint32_t *)0x4001080CUL)

/* AFIO and EXTI */
#define AFIO_EXTICR1 (*(volatile uint32_t *)0x40010008UL)
#define EXTI_IMR     (*(volatile uint32_t *)0x40010400UL)
#define EXTI_FTSR    (*(volatile uint32_t *)0x4001040CUL)
#define EXTI_PR      (*(volatile uint32_t *)0x40010414UL)

/* NVIC */
#define NVIC_ISER0   (*(volatile uint32_t *)0xE000E100UL)

/* USART1 */
#define USART1_SR    (*(volatile uint32_t *)0x40013800UL)
#define USART1_DR    (*(volatile uint32_t *)0x40013804UL)
#define USART1_BRR   (*(volatile uint32_t *)0x40013808UL)
#define USART1_CR1   (*(volatile uint32_t *)0x4001380CUL)

static QueueHandle_t buttonQueue;
static uint32_t buttonCount = 0;

static void UART_Init(void)
{
    /* Enable GPIOA and USART1 clocks */
    RCC_APB2ENR |= (1U << 2) | (1U << 14);

    /* PA9: Alternate-function push-pull, 50 MHz */
    GPIOA_CRH &= ~(0xFU << 4);
    GPIOA_CRH |=  (0xBU << 4);

    /* PA10: floating input */
    GPIOA_CRH &= ~(0xFU << 8);
    GPIOA_CRH |=  (0x4U << 8);

    /* USART1 baud rate: 115200 at 8 MHz */
    USART1_BRR = 0x45U;

    /* Enable USART1 and transmitter */
    USART1_CR1 = (1U << 13) | (1U << 3);
}

static void UART_SendString(const char *str)
{
    while (*str != '\0')
    {
        while ((USART1_SR & (1U << 7)) == 0U)
        {
        }

        USART1_DR = (uint8_t)*str++;
    }

    while ((USART1_SR & (1U << 6)) == 0U)
    {
    }
}

static void UART_SendNumber(uint32_t number)
{
    char buffer[10];
    uint32_t i = 0;

    if (number == 0U)
    {
        UART_SendString("0");
        return;
    }

    while (number > 0U)
    {
        buffer[i++] = (char)('0' + (number % 10U));
        number /= 10U;
    }

    while (i > 0U)
    {
        char output[2];
        output[0] = buffer[--i];
        output[1] = '\0';
        UART_SendString(output);
    }
}

/* Task receives button events from the queue */
static void ButtonTask(void *argument)
{
    uint32_t event;
    (void)argument;

    for (;;)
    {
        if (xQueueReceive(buttonQueue, &event, portMAX_DELAY) == pdPASS)
        {
            buttonCount++;

            UART_SendString("So lan nhan nut: ");
            UART_SendNumber(buttonCount);
            UART_SendString("\r\n");
        }
    }
}

/* Task announces that the RTOS is running */
static void UARTTask(void *argument)
{
    (void)argument;

    UART_SendString("FreeRTOS START\r\n");

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void EXTI0_IRQHandler(void)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    uint32_t event = 1U;

    if ((EXTI_PR & 1U) != 0U)
    {
        /* Clear EXTI0 pending flag */
        EXTI_PR = 1U;

        /* Send event to queue; never transmit UART inside the ISR */
        (void)xQueueSendFromISR(
            buttonQueue,
            &event,
            &higherPriorityTaskWoken
        );

        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }
}

int main(void)
{
    /* Enable AFIO and GPIOA clocks */
    RCC_APB2ENR |= (1U << 0) | (1U << 2);

    /* PA0 input with pull-up */
    GPIOA_CRL &= ~0xFU;
    GPIOA_CRL |= 0x8U;
    GPIOA_ODR |= 1U;

    UART_Init();

    /* Create queue before enabling EXTI0 */
    buttonQueue = xQueueCreate(8, sizeof(uint32_t));

    if (buttonQueue == NULL)
    {
        while (1)
        {
        }
    }

    if (xTaskCreate(
            ButtonTask, "Button", 160, NULL, 2, NULL
        ) != pdPASS)
    {
        while (1)
        {
        }
    }

    if (xTaskCreate(
            UARTTask, "UART", 160, NULL, 1, NULL
        ) != pdPASS)
    {
        while (1)
        {
        }
    }

    /* Map EXTI0 to PA0 */
    AFIO_EXTICR1 &= ~0xFU;

    /* Trigger on falling edge */
    EXTI_FTSR |= 1U;

    /* Clear pending flag, then enable EXTI0 */
    EXTI_PR = 1U;
    EXTI_IMR |= 1U;
    NVIC_ISER0 = (1U << 6);

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();

    while (1)
    {
    }
}
