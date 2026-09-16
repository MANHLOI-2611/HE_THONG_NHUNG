#include <stdint.h>
#define PERIPH_BASE        0x40000000UL
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000UL)

#define RCC_BASE            (PERIPH_BASE + 0x00021000UL)
#define GPIOA_BASE           (APB2PERIPH_BASE + 0x0800UL)

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

#define RCC     ((RCC_TypeDef *) RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *) GPIOA_BASE)

#define RCC_APB2ENR_IOPAEN   (1UL << 2)
static void delay(volatile uint32_t count)
{
    while (count--) {
        __asm__ volatile ("nop");
    }
}
static void GPIO_Config(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL = 0x44444444UL;
  GPIOA->CRH = 0x11111111UL;
  GPIOA->ODR &= ~(0xFF00UL);
}
int main(void)
{
    GPIO_Config();

    while (1) {
        uint32_t idr = GPIOA->IDR;         
        uint32_t input_val = idr & 0xFFUL; 

        uint32_t inverted = (~input_val) & 0xFFUL; 
      
        GPIOA->ODR = (GPIOA->ODR & 0x00FFUL) | (inverted << 8);

        delay(5000); 
    }
}
