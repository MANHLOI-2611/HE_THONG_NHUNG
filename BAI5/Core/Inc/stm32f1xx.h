#ifndef STM32F1XX_H
#define STM32F1XX_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

#define RCC ((RCC_TypeDef *)0x40021000UL)

#define RCC_APB2ENR_IOPAEN    (1UL << 2)
#define RCC_APB2ENR_USART1EN  (1UL << 14)

typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *)0x40010800UL)

#define GPIO_CRH_MODE9        (3UL << 4)
#define GPIO_CRH_CNF9         (3UL << 6)
#define GPIO_CRH_CNF9_1       (1UL << 7)

#define GPIO_CRH_MODE10       (3UL << 8)
#define GPIO_CRH_CNF10        (3UL << 10)
#define GPIO_CRH_CNF10_0      (1UL << 10)

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

#define USART1 ((USART_TypeDef *)0x40013800UL)

#define USART_SR_RXNE         (1UL << 5)
#define USART_SR_TXE          (1UL << 7)

#define USART_CR1_RE          (1UL << 2)
#define USART_CR1_TE          (1UL << 3)
#define USART_CR1_RXNEIE      (1UL << 5)
#define USART_CR1_UE          (1UL << 13)

#define NVIC_ISER0 (*(volatile uint32_t *)0xE000E100UL)
#define NVIC_ISER1 (*(volatile uint32_t *)0xE000E104UL)

#define USART1_IRQn 37

static inline void NVIC_EnableIRQ(int32_t IRQn)
{
    NVIC_ISER1 = (1UL << (IRQn - 32));
}
/* ================= TIM1 DEFINITIONS ================= */

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM_TypeDef;

#define TIM1 ((TIM_TypeDef *)0x40012C00UL)

/* TIM1 bits */
#define TIM_CR1_CEN       (1UL << 0)

#define TIM_EGR_UG        (1UL << 0)

#define TIM_CCMR1_OC1PE   (1UL << 3)

#define TIM_CCER_CC1E     (1UL << 0)

#define TIM_BDTR_MOE      (1UL << 15)

/* RCC TIM1 clock enable */
#define RCC_APB2ENR_TIM1EN    (1UL << 11)
#endif
