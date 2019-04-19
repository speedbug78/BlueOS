/***
 * This file should be auto genereated to reduce complexity
***/

#include "BlueOS_startup.h"
#include "BlueOS_switcher.h"
#include "BlueOS_clocks.h"

extern uint32_t __StackTop;

// Set the section to be the vector table
__attribute__ (( section ( ".vector_table" )))

// Create a function pointer array for the handler functions
uint32_t* const vectorTable[] = {
    (uint32_t*) 0x200007FF,  //Top of stack as defined in the linker file
    (uint32_t*) os_entry,                //Reset handler
    (uint32_t*) hold_hdlr,               //Non mask handler
    (uint32_t*) hold_hdlr,               //Hard fault handler
    (uint32_t*) hold_hdlr,               //Memory management handler
    (uint32_t*) hold_hdlr,               //Bus fault handler
    (uint32_t*) hold_hdlr,               //Usage fault handler
    (uint32_t*) 0x00000000,
    (uint32_t*) 0x00000000,
    (uint32_t*) 0x00000000,
    (uint32_t*) 0x00000000,
    (uint32_t*) hold_hdlr,               //System call handler
    (uint32_t*) hold_hdlr,               //Debug monitor handler
    (uint32_t*) 0x00000000,
    (uint32_t*) switcher_Handler,        //Task switch handler
    (uint32_t*) sysTick_Handler,         //Timer tick handler
};
