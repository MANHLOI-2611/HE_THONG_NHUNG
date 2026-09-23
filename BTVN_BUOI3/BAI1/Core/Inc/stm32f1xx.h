#ifndef STM32F1XX_H
#define STM32F1XX_H

#include <stdint.h>

/* ================= RCC ================= */

#define RCC_BASE        0x40021000UL
#define RCC_CR          (*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x1C))

/* ================= GPIOA ================= */
#define GPIOA_BASE      0x40010800UL

#define GPIOA_CRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_CRH       (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
#define GPIOA_IDR       (*(volatile uint32_t *)(GPIOA_BASE + 0x08))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))

/* ================= GPIOB ================= */
#define GPIOB_BASE      0x40010C00UL

#define GPIOB_CRL       (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_CRH       (*(volatile uint32_t *)(GPIOB_BASE + 0x04))
#define GPIOB_IDR       (*(volatile uint32_t *)(GPIOB_BASE + 0x08))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + 0x0C))

/* ================= USART1 ================= */
#define USART1_BASE     0x40013800UL

#define USART1_SR       (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR       (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR      (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1      (*(volatile uint32_t *)(USART1_BASE + 0x0C))
#define USART1_CR2      (*(volatile uint32_t *)(USART1_BASE + 0x10))
#define USART1_CR3      (*(volatile uint32_t *)(USART1_BASE + 0x14))

/* ================= I2C1 ================= */
#define I2C1_BASE       0x40005400UL

#define I2C1_CR1        (*(volatile uint32_t *)(I2C1_BASE + 0x00))
#define I2C1_CR2        (*(volatile uint32_t *)(I2C1_BASE + 0x04))
#define I2C1_OAR1       (*(volatile uint32_t *)(I2C1_BASE + 0x08))
#define I2C1_DR         (*(volatile uint32_t *)(I2C1_BASE + 0x10))
#define I2C1_SR1        (*(volatile uint32_t *)(I2C1_BASE + 0x14))
#define I2C1_SR2        (*(volatile uint32_t *)(I2C1_BASE + 0x18))
#define I2C1_CCR        (*(volatile uint32_t *)(I2C1_BASE + 0x1C))
#define I2C1_TRISE      (*(volatile uint32_t *)(I2C1_BASE + 0x20))

/* ================= SysTick ================= */
#define SYSTICK_BASE    0xE000E010UL

#define SYSTICK_CTRL    (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SYSTICK_LOAD    (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SYSTICK_VAL     (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))

#endif
