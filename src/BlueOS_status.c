#include "BlueOS_status.h"
#include "BlueOS_registers.h"

void status_Fast_LED( void ){
    #ifdef MAPLE_MINI
        reg( GPIOB_BSRR ) |= (uint16_t)0x0002;
        for( uint16_t i=0; i<0xFFFF; i++ ){__asm__("");};
        reg( GPIOB_BRR ) |= (uint16_t)0x0002;
        for( uint16_t i=0; i<0xFFFF; i++ ){__asm__("");};
    #endif
    #ifdef BLUE_PILL
        reg( GPIOC_BSRR ) |= (uint16_t)0x2000;
        for( uint16_t i=0; i<0xFFFF; i++ ){__asm__("");};
        reg( GPIOC_BRR ) |= (uint16_t)0x2000;
        for( uint16_t i=0; i<0xFFFF; i++ ){__asm__("");};
    #endif
}

void status_Init( void ){
    #ifdef MAPLE_MINI
        //Configure PB1 as pp output (Maple Mini only)
        reg( GPIOB_CRL )   &= (uint32_t)0xFFFFFF0F;                   //Reset PB1 settings
        reg( GPIOB_CRL )   |= (uint32_t)0x00000020;                   //PB1 general purpose output, push pull, 2Mhz
    #endif

    #ifdef BLUE_PILL
        //Configure PC13 as pp output (Blue Pill only)
        reg( GPIOC_CRH )   &= (uint32_t)0xFF0FFFFF;                   //Reset PC13 settings
        reg( GPIOC_CRH )   |= (uint32_t)0x00200000;                   //PC13 general purpose output, push pull, 2Mhz
    #endif
}
