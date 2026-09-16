#include <stdint.h>

extern unsigned long _estack;      
extern unsigned long _sidata;      
extern unsigned long _sdata;      
extern unsigned long _edata;     
extern unsigned long _sbss;        
extern unsigned long _ebss;       

extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void)             __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void)       __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler(void)       __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler(void)        __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler(void)      __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler(void)             __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler(void)        __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler(void)          __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler(void)         __attribute__ ((weak, alias("Default_Handler")));

__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void)) &_estack,     
    Reset_Handler,                 
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
};

void Reset_Handler(void)
{
    unsigned long *src, *dst;

    src = &_sidata;
    dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    main();

    while (1) { }
}

void Default_Handler(void)
{
    while (1) { }
}
