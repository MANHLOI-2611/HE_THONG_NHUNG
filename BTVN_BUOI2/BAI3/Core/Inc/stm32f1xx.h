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
typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
} ADC_TypeDef;

#define ADC1 ((ADC_TypeDef *)0x40012400UL)

/* ADC SR */
#define ADC_SR_EOC       (1UL << 1)

/* ADC CR2 */
#define ADC_CR2_ADON     (1UL << 0)
#define ADC_CR2_CAL      (1UL << 2)
#define ADC_CR2_RSTCAL   (1UL << 3)
#define ADC_CR2_CONT     (1UL << 1)
#define ADC_CR2_EXTTRIG  (1UL << 20)
#define ADC_CR2_SWSTART  (1UL << 22)

/* ADC1 clock enable */
#define RCC_APB2ENR_ADC1EN (1UL << 9)
#endif
