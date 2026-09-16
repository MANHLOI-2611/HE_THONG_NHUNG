.syntax unified
.cpu cortex-m3
.thumb

.global g_pfnVectors
.global Reset_Handler

.extern main

.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object

g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

    .word WWDG_IRQHandler
    .word PVD_IRQHandler
    .word TAMPER_IRQHandler
    .word RTC_IRQHandler
    .word FLASH_IRQHandler
    .word RCC_IRQHandler
    .word EXTI0_IRQHandler
    .word EXTI1_IRQHandler
    .word EXTI2_IRQHandler
    .word EXTI3_IRQHandler
    .word EXTI4_IRQHandler
    .word DMA1_Channel1_IRQHandler
    .word DMA1_Channel2_IRQHandler
    .word DMA1_Channel3_IRQHandler
    .word DMA1_Channel4_IRQHandler
    .word DMA1_Channel5_IRQHandler
    .word DMA1_Channel6_IRQHandler
    .word DMA1_Channel7_IRQHandler
    .word ADC1_2_IRQHandler
    .word USB_HP_CAN1_TX_IRQHandler
    .word USB_LP_CAN1_RX0_IRQHandler
    .word CAN1_RX1_IRQHandler
    .word CAN1_SCE_IRQHandler
    .word EXTI9_5_IRQHandler
    .word TIM1_BRK_IRQHandler
    .word TIM1_UP_IRQHandler
    .word TIM1_TRG_COM_IRQHandler
    .word TIM1_CC_IRQHandler
    .word TIM2_IRQHandler
    .word TIM3_IRQHandler
    .word TIM4_IRQHandler
    .word I2C1_EV_IRQHandler
    .word I2C1_ER_IRQHandler
    .word I2C2_EV_IRQHandler
    .word I2C2_ER_IRQHandler
    .word SPI1_IRQHandler
    .word SPI2_IRQHandler
    .word USART1_IRQHandler
    .word USART2_IRQHandler
    .word USART3_IRQHandler
    .word EXTI15_10_IRQHandler
    .word RTCAlarm_IRQHandler
    .word USBWakeUp_IRQHandler

.text

Reset_Handler:
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bge zero_bss
    ldr r3, [r0]
    str r3, [r1]
    adds r0, r0, #4
    adds r1, r1, #4
    b copy_data

zero_bss:
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0

zero_loop:
    cmp r1, r2
    bge call_main
    str r3, [r1]
    adds r1, r1, #4
    b zero_loop

call_main:
    bl main

hang:
    b hang

.thumb_func
Default_Handler:
    b .

.macro WEAK_DEFAULT_HANDLER name
    .weak \name
    .set \name, Default_Handler
.endm

WEAK_DEFAULT_HANDLER NMI_Handler
WEAK_DEFAULT_HANDLER HardFault_Handler
WEAK_DEFAULT_HANDLER MemManage_Handler
WEAK_DEFAULT_HANDLER BusFault_Handler
WEAK_DEFAULT_HANDLER UsageFault_Handler
WEAK_DEFAULT_HANDLER SVC_Handler
WEAK_DEFAULT_HANDLER DebugMon_Handler
WEAK_DEFAULT_HANDLER PendSV_Handler
WEAK_DEFAULT_HANDLER SysTick_Handler
WEAK_DEFAULT_HANDLER WWDG_IRQHandler
WEAK_DEFAULT_HANDLER PVD_IRQHandler
WEAK_DEFAULT_HANDLER TAMPER_IRQHandler
WEAK_DEFAULT_HANDLER RTC_IRQHandler
WEAK_DEFAULT_HANDLER FLASH_IRQHandler
WEAK_DEFAULT_HANDLER RCC_IRQHandler
WEAK_DEFAULT_HANDLER EXTI0_IRQHandler
WEAK_DEFAULT_HANDLER EXTI1_IRQHandler
WEAK_DEFAULT_HANDLER EXTI2_IRQHandler
WEAK_DEFAULT_HANDLER EXTI3_IRQHandler
WEAK_DEFAULT_HANDLER EXTI4_IRQHandler
WEAK_DEFAULT_HANDLER DMA1_Channel1_IRQHandler
WEAK_DEFAULT_HANDLER DMA1_Channel2_IRQHandler
WEAK_DEFAULT_HANDLER DMA1_Channel3_IRQHandler
WEAK_DEFAULT_HANDLER DMA1_Channel4_IRQHandler
WEAK_DEFAULT_HANDLER DMA1_Channel5_IRQHandler
WEAK_DEFAULT_HANDLER DMA1_Channel6_IRQHandler
WEAK_DEFAULT_HANDLER DMA1_Channel7_IRQHandler
WEAK_DEFAULT_HANDLER ADC1_2_IRQHandler
WEAK_DEFAULT_HANDLER USB_HP_CAN1_TX_IRQHandler
WEAK_DEFAULT_HANDLER USB_LP_CAN1_RX0_IRQHandler
WEAK_DEFAULT_HANDLER CAN1_RX1_IRQHandler
WEAK_DEFAULT_HANDLER CAN1_SCE_IRQHandler
WEAK_DEFAULT_HANDLER EXTI9_5_IRQHandler
WEAK_DEFAULT_HANDLER TIM1_BRK_IRQHandler
WEAK_DEFAULT_HANDLER TIM1_UP_IRQHandler
WEAK_DEFAULT_HANDLER TIM1_TRG_COM_IRQHandler
WEAK_DEFAULT_HANDLER TIM1_CC_IRQHandler
WEAK_DEFAULT_HANDLER TIM2_IRQHandler
WEAK_DEFAULT_HANDLER TIM3_IRQHandler
WEAK_DEFAULT_HANDLER TIM4_IRQHandler
WEAK_DEFAULT_HANDLER I2C1_EV_IRQHandler
WEAK_DEFAULT_HANDLER I2C1_ER_IRQHandler
WEAK_DEFAULT_HANDLER I2C2_EV_IRQHandler
WEAK_DEFAULT_HANDLER I2C2_ER_IRQHandler
WEAK_DEFAULT_HANDLER SPI1_IRQHandler
WEAK_DEFAULT_HANDLER SPI2_IRQHandler
WEAK_DEFAULT_HANDLER USART2_IRQHandler
WEAK_DEFAULT_HANDLER USART3_IRQHandler
WEAK_DEFAULT_HANDLER EXTI15_10_IRQHandler
WEAK_DEFAULT_HANDLER RTCAlarm_IRQHandler
WEAK_DEFAULT_HANDLER USBWakeUp_IRQHandler
