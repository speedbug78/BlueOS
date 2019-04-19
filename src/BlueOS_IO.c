#include "BlueOS_IO.h"
#include "BlueOS_status.h"
#include "BlueOS_registers.h"

void io_Startup( void ){
    //Configure PA9 (Tx) and PA10 (Rx) <RM0008  Section 9.2.2>
    reg( GPIOA_CRH )   &= (uint32_t)0xFFFFFF0F;                       //Reset PA9 settings
    reg( GPIOA_CRH )   |= (uint32_t)0x00000090;                       //PA9 alternate function output, push pull, 10Mhz

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
