/***
 * Blue OS startup code
***/
#include "BlueOS_startup.h"
#include "BlueOS_registers.h"
#include "BlueOS_loader.h"
#include "BlueOS_debug_messages.h"
#include "BlueOS_status.h"
#include "BlueOS_console.h"

// Declaration of local file functions
static void boot_entry( void );

/***
 * Vector handler that is called on processor reset
 * This is where code begins executing.
***/
void reset_hdlr( void ){
    /***
     * Configure SYSCLOCK for 72Mhz (HSE is 8Mhz for Blue Pill and Maple Mini)
    ***/
    //Set clock control <RM0008 7.3.1>
    reg( RCC_CR )      |= (uint32_t)0x00010000;                 //Turn on HSE
    while(( reg( RCC_CR ) & (uint32_t)0x00020000 ) == 0 ){}     //Wait for HSE to start up (add timeout?)

    //Configure Flash <RM0008  Section 3.3.3>
    reg( FLASH_ACR )    = (uint32_t)0x0000001A;                 //Reset state of flash. 2 wait states (SYSCLK > 48Mhz), prefetch buffer enabled.

    //Set clock configuration <RM0008  Section 7.3.2>
    reg( RCC_CFGR )    &= ~(uint32_t)0x073FFFF3;                //Reset appropriate clock bits.
    reg( RCC_CFGR )    |= (uint32_t)0x001D0400;                 //AHB, APB2 not divided (72Mhz), APB1 and ADC div 2 (36Mhz).

    //Set clock control <RM0008 7.3.1>
    reg( RCC_CR )      |= (uint32_t)0x01000000;                 //Turn on PLL
    while(( reg( RCC_CR ) & (uint32_t)0x02000000 ) == 0 ){}     //Wait for PLL to start up

    //Set clock configuration <RM0008  Section 7.3.2>
    reg( RCC_CFGR )    |= (uint32_t)0x00000002;                 //Set PLL as system clock source
    while(( reg( RCC_CFGR ) & (uint32_t)0x0000000C ) != 0x08 ){}//Wait for PLL to become clock source

    //Set clock interrupt register <RM0008  Section 7.3.3>
    reg( RCC_CIR )      = (uint32_t)0x009F0000;                 //Clear interrupt flags for clock security and PLL, HSE, HSI, LSE and LSI ready

    /***
     * At this point the processor should be up and running at 72Mhz
     * and USART 1 is active for console IO
    ***/

    /***
     * Configure Peripheral Clocks
    ***/
    //Enable Peripheral Clocks <RM0008  Section 7.3.7>
    reg( RCC_APB2ENR ) |= (uint32_t)0x00004005;                 //Turn on GPIOA, AFIO and USART1

    //Configure PA9 (Tx) and PA10 (Rx) <RM0008  Section 9.2.2>
    reg( GPIOA_CRH )   &= (uint32_t)0xFFFFFF0F;                 //Reset PA9 settings
    reg( GPIOA_CRH )   |= (uint32_t)0x00000090;                 //PA9 alternate function output, push pull, 10Mhz

    /***
     * Continue startup
    ***/
    status_Startup();

    console_Startup();

    boot_entry();

    debug_User_Jump();

    User_Jump();

    hold_hdlr();                                            //User code shouldn't exit, but just in case, keep processor here.
}

/*
Entry conditions for running the loader or user code.
Blocks forever waiting for a command on the serial port.
*/
inline void boot_entry( void ){
    while(1){
        char cmd;
        cmd = receive_CharN();
        status_Fast_LED();

        if( cmd == 'b' ){                          //If boot intent is to bootload (lowercase 'b' was sent)
            loader();                                //Branch to bootloader
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
The rest of these handlers are only in the vector table if BOS_DEBUG is defined.
See BlueOS_vectors.S
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
