#ifndef STM32F103_REGS
#define STM32F103_REGS

/***
 * Cortex M3 Registers
***/
// <PM0056 4.4.14>
#define SCB_CPUID       0xE000ED00  // Processor identification
#define SCB_ICSR        0xE000ED04  // PendSV and Systick interrupt control
#define SCB_VTOR        0xE000ED08  // Vector table relocation
#define SCB_AIRCR       0xE000ED0C
#define SCB_SCR         0xE000ED10
#define SCB_CCR         0xE000ED14
#define SCB_SHPR1       0xE000ED18
#define SCB_SHPR2       0xE000ED1C
#define SCB_SHPR3       0xE000ED20
#define SCB_SHCRS       0xE000ED24
#define SCB_CFSR        0xE000ED28
#define SCB_HFSR        0xE000ED2C
#define SCB_MMFAR       0xE000ED34
#define SCB_BFAR        0xE000ED38

// <PM0056 4.5.6>
#define SYSTICK_CTRL    0xE000E010
#define SYSTICK_LOAD    0xE000E014
#define SYSTICK_VAL     0xE000E018
#define SYSTICK_CALIB   0xE000E01C

// <PM0056 4.3.11>
#define NVIC_ISER0      0xE000E100

/***
    * AHB Bus
***/
// Internal Flash Memory register locations
#define FLASH_ACR       0x40022000
#define FLASH_KEY       0x40022004
#define FLASH_OPTKEY    0x40022008
#define FLASH_SR        0x4002200C
#define FLASH_CR        0x40022010
#define FLASH_ADR       0x40022014
#define FLASH_OBR       0x4002201C
#define FLASH_WRPR      0x40022020

// Reset and Clock Control (RCC) register locations
#define RCC_CR          0x40021000
#define RCC_CFGR        0x40021004
#define RCC_CIR         0x40021008
#define RCC_APB2RSTR    0x4002100C
#define RCC_APB1RSTR    0x40021010
#define RCC_AHBENR      0x40021014
#define RCC_APB2ENR     0x40021018
#define RCC_APB1ENR     0x4002101C
#define RCC_BDCR        0x40021020
#define RCC_CSR         0x40021024

/***
    * APB2 Bus
***/
// USART1 register locations
#define USART1_SR       0x40013800
#define USART1_DR       0x40013804
#define USART1_BRR      0x40013808
#define USART1_CR1      0x4001380C
#define USART1_CR2      0x40013810
#define USART1_CR3      0x40013814
#define USART1_GTPR     0x40013818

// General Purpose IO (GPIO) Port A register locations
#define GPIOA_CRL       0x40010800
#define GPIOA_CRH       0x40010804
#define GPIOA_IDR       0x40010808
#define GPIOA_ODR       0x4001080C
#define GPIOA_BSRR      0x40010810
#define GPIOA_BRR       0x40010814
#define GPIOA_LCKR      0x40010818

// General Purpose IO (GPIO) Port B register locations
#define GPIOB_CRL       0x40010C00
#define GPIOB_CRH       0x40010C04
#define GPIOB_IDR       0x40010C08
#define GPIOB_ODR       0x40010C0C
#define GPIOB_BSRR      0x40010C10
#define GPIOB_BRR       0x40010C14
#define GPIOB_LCKR      0x40010C18

// General Purpose IO (GPIO) Port C register locations
#define GPIOC_CRL       0x40011000
#define GPIOC_CRH       0x40011004
#define GPIOC_IDR       0x40011008
#define GPIOC_ODR       0x4001100C
#define GPIOC_BSRR      0x40011010
#define GPIOC_BRR       0x40011014
#define GPIOC_LCKR      0x40011018

// Alternate Function IO (AFIO) register locations
#define AFIO_EVCR       0x40010000
#define AFIO_MAPR       0x40010004
#define AFIO_EXTICR1    0x40010008
#define AFIO_EXTICR2    0x4001000C
#define AFIO_EXTICR3    0x40010010
#define AFIO_EXTICR4    0x40010014
#define AFIO_MAPR2      0x4001001C

#define IWDG_KR         0x40003000
#define IWDG_PR         0x40003004
#define IWDG_RLR        0x40003008
#define IWDG_SR         0x4000300C

/***
    * APB1 Bus
***/
#define I2C1_CR1        0x40005400
#define I2C1_CR2        0x40005404
#define I2C1_OAR1       0x40005408
#define I2C1_OAR2       0x4000540C
#define I2C1_DR         0x40005410
#define I2C1_SR1        0x40005414
#define I2C1_SR2        0x40005418
#define I2C1_CCR        0x4000541C
#define I2C1_TRISE      0x40005420

#define I2C2_CR1        0x40005800
#define I2C2_CR2        0x40005804
#define I2C2_OAR1       0x40005808
#define I2C2_OAR2       0x4000580C
#define I2C2_DR         0x40005810
#define I2C2_SR1        0x40005814
#define I2C2_SR2        0x40005818
#define I2C2_CCR        0x4000581C
#define I2C2_TRISE      0x40005820

#endif
