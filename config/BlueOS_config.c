/***
 *
 * Configuration tables for BlueOS
 *
 * This data is loaded into ROM at the address specified in the linker (.ld) file
 *
 * The data is divided to fit into a 1k block of ROM
 *
 * Vector Table     - 336 bytes (84 interrupts),        starting at 0x08000000
 *
 * Schedule Table   - 528 bytes (132 schedule slots),   starting at 0x08000150
 *
 * Task Table       - 160 bytes (10 tasks max),         starting at 0x08000360
 *
 * This file should be auto genereated to reduce complexity
 *
***/

#include "BlueOS_startup.h"
#include "BlueOS_switcher.h"
#include "BlueOS_clocks.h"

extern uint32_t __StackTop;

// Create an array for the handler functions
uint32_t* vectorTable[] __attribute__ (( section ( ".vector_table" ))) = {
    (uint32_t*) &__StackTop,            //Top of stack as defined in the linker file
    (uint32_t*) os_entry,               //Reset handler
    (uint32_t*) nm_hdlr,                //Non mask handler
    (uint32_t*) hf_hdlr,                //Hard fault handler
    (uint32_t*) mm_hdlr,                //Memory management handler
    (uint32_t*) bf_hdlr,                //Bus fault handler
    (uint32_t*) uf_hdlr,                //Usage fault handler
    (uint32_t*) 0x00000000,
    (uint32_t*) 0x00000000,
    (uint32_t*) 0x00000000,
    (uint32_t*) 0x00000000,
    (uint32_t*) sc_hdlr,                //System call handler
    (uint32_t*) dm_hdlr,                //Debug monitor handler
    (uint32_t*) 0x00000000,
    (uint32_t*) switcher_Handler,       //Task switch handler
    (uint32_t*) sysTick_Handler,        //Timer tick handler
};

/*Task Schedule*/
/*ID, Run Time (in Ticks);*/
/*2, 10; 1, 50; 0, 40; 2, 10; 0, 90; 2, 10; 0, 90; 2, 10; 0, 90; 2, 10; 0, 90; 2, 10; 0, 90; 2, 10; 0, 90; 2, 10; 0, 90; 2, 10; 0, 90; 2, 10; 0, 90;*/

// Create an array for the schedule, each uint32_t is divided ID: xx | Run Time: xxxxxx
uint32_t* scheduleTable[] __attribute__ (( section ( ".schedule_table" ))) = {
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x01000032,
    (uint32_t*) 0x00000028,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
    (uint32_t*) 0x0200000A,
    (uint32_t*) 0x0000005A,
};

/*Task Table*/
/* Create an array for the tasks */
/*   Code Start Address   , Code Memory Length    , Ram Start Address     , Ram Length            , ID    */
uint32_t* const taskTable[] __attribute__ (( section ( ".task_table" ))) = {
    (uint32_t*) 0x08000800, (uint32_t*) 0x00001400, (uint32_t*) 0x20000000, (uint32_t*) 0x00000800, /*ID 0*/
    (uint32_t*) 0x08001400, (uint32_t*) 0x00002800, (uint32_t*) 0x20000800, (uint32_t*) 0x00002000, /*ID 1*/
    (uint32_t*) 0x08004000, (uint32_t*) 0x0001C000, (uint32_t*) 0x20002800, (uint32_t*) 0x00002800, /*ID 2*/
};
