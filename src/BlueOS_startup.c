/***
 * Blue OS startup code
***/
#include "BlueOS_startup.h"
#include "BlueOS_clocks.h"
#include "BlueOS_registers.h"
#include "BlueOS_loader.h"
#include "BlueOS_status.h"
#include "BlueOS_console.h"
#include "BlueOS_IO.h"
#include "BlueOS_utilities.h"

/***
 * Vector handler that is called on processor reset
 * This is where code begins executing.
***/
void os_entry( void ){

    disable_irq(); //Don't allow interrupts while starting system clocks

    clocks_Start_sysClock();

    clocks_Start_periphClocks();

    clocks_Start_sysTick();

    status_Startup();

    console_Startup_72Mhz();

    /***
     * At this point the processor should be up and running at 72Mhz
     * and USART 1 is active for console IO
    ***/

    send_Str ( "\nClocks Started" );

    status_Show_Error();

    send_Str( CMD_LINE );

    /***
     * Continue startup
    ***/
    enable_irq();

    io_Startup();

    while(1){
        console_Task();
        status_LED_Task();
    }

    hold_hdlr();                                            //User code shouldn't exit, but just in case, keep processor here.
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
    send_Char( 'h' );
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
