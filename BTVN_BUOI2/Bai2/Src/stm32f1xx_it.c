#include "main.h"
#include "stm32f1xx_it.h"

static volatile uint32_t cnt_led1 = 0;
static volatile uint32_t cnt_led2 = 0;
static volatile uint32_t cnt_led3 = 0;

#define LED1_HALF_PERIOD_MS   5000U
#define LED2_HALF_PERIOD_MS    500U
#define LED3_HALF_PERIOD_MS     50U

void NMI_Handler(void)          { while (1) {} }
void HardFault_Handler(void)    { while (1) {} }
void MemManage_Handler(void)    { while (1) {} }
void BusFault_Handler(void)     { while (1) {} }
void UsageFault_Handler(void)   { while (1) {} }
void SVC_Handler(void)          { }
void DebugMon_Handler(void)     { }
void PendSV_Handler(void)       { }

void SysTick_Handler(void)
{
    HAL_IncTick();

    cnt_led1++;
    if (cnt_led1 >= LED1_HALF_PERIOD_MS)
    {
        cnt_led1 = 0;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
    }

    cnt_led2++;
    if (cnt_led2 >= LED2_HALF_PERIOD_MS)
    {
        cnt_led2 = 0;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    }

    cnt_led3++;
    if (cnt_led3 >= LED3_HALF_PERIOD_MS)
    {
        cnt_led3 = 0;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
    }
}
