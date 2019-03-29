/***
 * Blue Metal OS startup code
***/
#include "BlueOS_startup.h"
#include "BlueOS_registers.h"
#include "BlueOS_bootloader.h"
#include "BlueOS_debug_messages.h"
#include "BlueOS_status.h"
#include "BlueOS_serial.h"

void boot_entry( void );

/*
Vector handler that is called on processor reset
*/
void reset_hdlr( void ){
    //Configure Clock to 8MHz HSI
    reg( RCC_CR )      |= (uint32_t)0x00000001;                 //Reset RCC clock to default HSI (don't change HSI trim)
    reg( RCC_CFGR )    &= (uint32_t)0xF0FF0000;                 //Set HSI as clock, not divided for AHB, APB1 and APB2.  /2 for ADC.  No clock output
    reg( RCC_CR )      &= (uint32_t)0xFEF6FFFF;                 //Turn off PLL, HSE and Clock security
    reg( RCC_CR )      &= (uint32_t)0xFFFBFFFF;                 //HSE not bypassed
    reg( RCC_CFGR )    &= (uint32_t)0xFF80FFFF;                 //Reset PLL and USB OTG values
    reg( RCC_CIR )      = (uint32_t)0x009F0000;                 //Clear interrupt flags for clock security and PLL, HSE, HSI, LSE and LSI ready

    //Configure Flash
    reg( FLASH_ACR )    = (uint32_t)0x00000030;                 //Reset state of flash. 0 wait states (HSI is 8Mhz), prefetch buffer enabled.

    //Enable Peripheral Clocks
    reg( RCC_APB2ENR ) |= (uint32_t)0x0000401D;                 //Turn on GPIOA, GPIOB, GPIOC, AFIO and USART1

    //Configure PB15 as a push/pull output for power on
    reg( GPIOB_CRH )   &= (uint32_t)0x0FFFFFFF;                 //Reset PB15 settings
    reg( GPIOB_CRH )   |= (uint32_t)0x20000000;                 //PB15 push pull output
    reg( GPIOB_BSRR )  |= (uint16_t)0x8000;                     //Set PB15 to keep processor powered on.

    //Configure PA9 (Tx) and PA10 (Rx)
    reg( GPIOA_CRH )   &= (uint32_t)0xFFFFFF0F;                 //Reset PA9 settings
    reg( GPIOA_CRH )   |= (uint32_t)0x00000090;                 //PA9 alternate function output, push pull, 10Mhz

    status_Init();

    serial_Init();

    send_Str( BOOTLOAD_BANNER );

    boot_entry();

    debug_User_Jump();

    User_Jump();

    hold_hdlr();                                            //User code shouldn't exit, but just in case, keep processor here.
}

/*
Entry conditions for running the bootloader or user code.
Blocks forever waiting for a command on the serial port.
*/
inline void boot_entry( void ){
    while(1){
        char cmd;
        cmd = receive_CharN();
        status_Fast_LED();

        if( cmd == 'b' ){                          //If boot intent is to bootload (lowercase 'b' was sent)
            bootloader();                                //Branch to bootloader
            debug_Boot_Finished();
            return;
        }
        else if( cmd == 'r' ){
            return;
        }
    }
}



/*
Vector handler to hold the processor in an infinite loop
*/
void hold_hdlr( void ){
    while(1);
}
/*
The rest of these handlers are only in the vector table if BMOS_DEBUG is defined.
See BlueMetalOS_vectors.S
*/
void hf_hdlr( void ){
    debug_Hard_Fault();
    while(1);
}

void mm_hdlr( void ){
    send_Char( 'm' );
    while(1);
}

void bf_hdlr( void ){
    send_Char( 'b' );
    while(1);
}

void uf_hdlr( void ){
    send_Char( 'u' );
    while(1);
}

void sc_hdlr( void ){
    send_Char( 's' );
    while(1);
}
