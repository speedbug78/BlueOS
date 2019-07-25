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
#include "BlueOS_switcher.h"

void usage_Fault( uint32_t* stack_ptr );
void mem_Fault( uint32_t* stack_ptr );
void bus_Fault( uint32_t* stack_ptr );
void hard_Fault( uint32_t* stack_ptr );
void hold_Fault( uint32_t* stack_ptr );

void read_cfsr( void );
void unstack_regs( uint32_t* stack_ptr );

/***
 * Vector handler that is called on processor reset
 * This is where code begins executing.
***/
int main( void ){

    disable_irq(); // Don't allow interrupts while starting system clocks

    clocks_Start_sysClock();

    clocks_Start_periphClocks();

    clocks_Start_sysTick();

    switcher_Startup();

    status_Startup();

    console_Startup_72Mhz();

    /***
     * At this point the processor should be up and running at 72Mhz
     * and USART 1 is active for console IO
    ***/

    send_Str ( "\r\nClocks Started" );

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

    hold_hdlr();                                            // User code shouldn't exit, but just in case, keep processor here.
}

/*
Vector handler to hold the processor in an infinite loop
*/
void hold_hdlr( void ){
    asm volatile("mov r0, sp");
    asm volatile("b hold_Fault");
}

void nm_hdlr( void ){
    send_Str( "\r\nNon-maskable interrupt" );
    while(1);
}

void hf_hdlr( void ){
    asm volatile("mov r0, sp");
    asm volatile("b hard_Fault");
}

void mm_hdlr( void ){
    asm volatile("mov r0, sp");
    asm volatile("b mem_Fault");
}

void bf_hdlr( void ){
    asm volatile("mov r0, sp");
    asm volatile("b bus_Fault");
}

void uf_hdlr( void ){
    asm volatile("mov r0, sp");
    asm volatile("b usage_Fault");
}

void sc_hdlr( void ){
    send_Str( "\r\nSystem service call interrupt" );
    while(1);
}

void dm_hdlr( void ){
    send_Str( "\r\nDebug monitor interrupt" );
    while(1);
}

/*
* C functions for faults needing stack data
*/
void hold_Fault( uint32_t* stack_ptr ){
    send_Str( "\nHold Handler" );
    unstack_regs( stack_ptr );
    while(1);
}

void hard_Fault( uint32_t* stack_ptr ){
    uint32_t fault_status;
    send_Str( "\r\nHard Fault" );
    fault_status = reg( SCB_HFSR );
    if( fault_status & BIT30 ){
        send_Str( "\r\nHard fault caused by unhandled fault" );
    }
    if( fault_status & BIT1 ){
        send_Str( "\r\nInvalid vector table read" );
    }

    send_Str( "\r\nSwitcher Debug: " );
    send_Uint32( switch_debug );

    read_cfsr();
    unstack_regs( stack_ptr );
    while(1);
}

void bus_Fault( uint32_t* stack_ptr ){
    send_Str( "\r\nBus Fault" );
    read_cfsr();
    unstack_regs( stack_ptr );
    while(1);
}

void mem_Fault( uint32_t* stack_ptr ){
    send_Str( "\r\nMemory Management Fault" );
    read_cfsr();
    unstack_regs( stack_ptr );
    while(1);
}

void usage_Fault( uint32_t* stack_ptr ){
    send_Str( "\r\nUsage Fault" );
    read_cfsr();
    unstack_regs( stack_ptr );
    while(1);
}


/*
* Helper for parsing the CFSR
*/
void read_cfsr( void ){
    // <PM0056 4.4.10>
    uint32_t fault_status;
    fault_status = reg( SCB_CFSR );

    if( fault_status & BIT25 ){
        send_Str( "\r\nDivide by Zero" );
    }

    if( fault_status & BIT24 ){
        send_Str( "\r\nUnaligned memory access" );
    }

    if( fault_status & BIT19 ){
        send_Str( "\r\nCoprocessor not available" );
    }

    if( fault_status & BIT18 ){
        send_Str( "\r\nInvalid Context or EXC_RETURN value" );
    }

    if( fault_status & BIT17 ){
        send_Str( "\r\nIllegal use of EPSR  (does the target address have LSB=0?)" );
    }

    if( fault_status & BIT16 ){
        send_Str( "\r\nUndefined Instruction" );
    }

    if( fault_status & BIT15 ){
        uint32_t address;
        address = reg( SCB_BFAR );
        send_Str( "\r\nBus fault address: " );
        send_Uint32( address );
    }

    if( fault_status & BIT12 ){
        send_Str( "\r\nBus fault stacking for exception failed" );
    }

    if( fault_status & BIT11 ){
        send_Str( "\r\nBus fault unstacking from exception failed" );
    }

    if( fault_status & BIT10 ){
        send_Str( "\r\nData bus error, unknown address" );
    }

    if( fault_status & BIT9 ){
        send_Str( "\r\nData bus error, pc is valid" );
    }

    if( fault_status & BIT8 ){
        send_Str( "\r\nInstruction bus error" );
    }

    if( fault_status & BIT7 ){
        uint32_t address;
        address = reg( SCB_MMFAR );
        send_Str( "\r\nMemory management fault address: " );
        send_Uint32( address );
    }

    if( fault_status & BIT4 ){
        send_Str( "\r\nMemory management stacking for exception failed" );
    }

    if( fault_status & BIT3 ){
        send_Str( "\r\nMemory management unstacking for exception failed" );
    }

    if( fault_status & BIT1 ){
        send_Str( "\r\nLoad or store attempted to un-accessable location" );
    }

    if( fault_status & BIT0 ){
        send_Str( "\r\nInstruction fetch attempted to access non-execute location" );
    }
}

/*
* Helper for unstacking registers
*/
void unstack_regs( uint32_t* stack_ptr ){
    // <DDI0337E 5.5.1>
    send_Str( "\r\nR0: " );
    send_Uint32( stack_ptr[0] );
    send_Str( "\r\nR1: " );
    send_Uint32( stack_ptr[1] );
    send_Str( "\r\nR2: " );
    send_Uint32( stack_ptr[2] );
    send_Str( "\r\nR3: " );
    send_Uint32( stack_ptr[3] );
    send_Str( "\r\nR12: " );
    send_Uint32( stack_ptr[4] );
    send_Str( "\r\nLink Register (lr): " );
    send_Uint32( stack_ptr[5] );
    send_Str( "\r\nProgram Counter (pc): " );
    send_Uint32( stack_ptr[6] );
    send_Str( "\r\nProgram Status Register (psr): " );
    send_Uint32( stack_ptr[7] );
}
