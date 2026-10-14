
#include <stdint.h>

extern int main(void);
extern unsigned long _estack;

extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

void Reset_Handler(void);
void Default_Handler(void);

void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI0_IRQHandler(void);

void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));

__attribute__((section(".isr_vector"), used))
const uintptr_t vector_table[] =
{
    (uintptr_t)&_estack,
    (uintptr_t)Reset_Handler,
    (uintptr_t)NMI_Handler,
    (uintptr_t)HardFault_Handler,
    (uintptr_t)MemManage_Handler,
    (uintptr_t)BusFault_Handler,
    (uintptr_t)UsageFault_Handler,
    0,
    0,
    0,
    0,
    (uintptr_t)SVC_Handler,
    (uintptr_t)Default_Handler,
    0,
    (uintptr_t)PendSV_Handler,
    (uintptr_t)SysTick_Handler,

    (uintptr_t)Default_Handler,  /* IRQ0: WWDG */
    (uintptr_t)Default_Handler,  /* IRQ1: PVD */
    (uintptr_t)Default_Handler,  /* IRQ2: TAMPER */
    (uintptr_t)Default_Handler,  /* IRQ3: RTC */
    (uintptr_t)Default_Handler,  /* IRQ4: FLASH */
    (uintptr_t)Default_Handler,  /* IRQ5: RCC */
    (uintptr_t)EXTI0_IRQHandler  /* IRQ6: EXTI0 */
};

void Default_Handler(void)
{
    while (1)
    {
    }
}

void Reset_Handler(void)
{
    unsigned long *src = &_sidata;
    unsigned long *dst = &_sdata;

    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    main();

    while (1)
    {
    }
}
