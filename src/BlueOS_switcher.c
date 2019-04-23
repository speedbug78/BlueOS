#include "BlueOS_switcher.h"
#include "BlueOS_utilities.h"
#include "BlueOS_config.h"
#include "BlueOS_console.h"
#include "BlueOS_startup.h"

static volatile uint8_t current_task;

static volatile uint8_t current_task_id;

//Local Functions
static inline void store_psp( void );
static inline void load_psp( void );
static inline void store_context( void );
static inline void set_context( void );

static uint32_t* task_Table_Index;
static uint32_t* schedule_Table_Index;

uint32_t* stack_Table[NUMBER_OF_TASKS];

//Lots of pointers need to be checked in this code and the
// stack_table isn't setup properly yet.

void switcher_Startup( void ){
    uint8_t i;

    //Initialize schedule table index
    schedule_Table_Index = (uint32_t*)SCHEDULE_TABLE_BEGIN;

    //Initialize task table index
    task_Table_Index = (uint32_t*)TASK_TABLE_BEGIN;

    //Initialize stack table and stacks
    for( i = 0; i < NUMBER_OF_TASKS; i ++ ){
        uint32_t ram_start, ram_length, code_start;
        code_start = *(task_Table_Index + ( 0 ) + ( NUMBER_OF_TASK_DATA * i ));
        ram_start = *(task_Table_Index + ( 2 ) + ( NUMBER_OF_TASK_DATA * i ));
        ram_length = *(task_Table_Index + ( 3 ) + ( NUMBER_OF_TASK_DATA * i ));

        stack_Table[i] = (uint32_t*)( ram_start + ram_length );

        //Initialize stacks for tasks (not OS).  This is done so we can perform the first switch
        //just like all the others, rather than having a special case for the first one.
        if( i > 0 ){
            stack_Table[i] -= 1;
            *stack_Table[i] = (uint32_t)i; //Pass any argument to the task here? (r0 value)
            stack_Table[i] -= 1;
            *stack_Table[i] = 0; //r1 value
            stack_Table[i] -= 1;
            *stack_Table[i] = 0; //r2 value
            stack_Table[i] -= 1;
            *stack_Table[i] = 0; //r3 value
            stack_Table[i] -= 1;
            *stack_Table[i] = 0; //r12 value
            stack_Table[i] -= 1;
            *stack_Table[i] = code_start; //pc value needs to be the start of the task's code
            stack_Table[i] -= 1;
            *stack_Table[i] = (uint32_t)hold_hdlr; //lr value (where to go if the task doesn't start or it returns)
            stack_Table[i] -= 1;
            *stack_Table[i] = 0x21000000; //default value for the program status register
            stack_Table[i] -= 8;    //move the stack pointer to include registers stacked by software (value doesn't matter?).
        }
    }

    current_task_id = *schedule_Table_Index & 0xFF000000;
}

void switcher_Handler( void ){
    disable_irq(); //Don't allow other interrupts to happen during context switching
    //Clear switch (PendSV) interrupt
    reg( SCB_ICSR ) |= 0x08000000;

    store_context();

    //If this isn't the OS, store its stack pointer.
    //OS is the only one using the MSP so no need to store.
    if( current_task_id != 0 ){
        store_psp();
    }

    //Increment the current_task to the next one in the schedule
    schedule_Table_Index ++;
    //If we have finished this schedule iteration go back to the beginning
    if( schedule_Table_Index >= SCHEDULE_TABLE_END ){
        schedule_Table_Index = (uint32_t*)SCHEDULE_TABLE_BEGIN;
    }

    //Update current_task_id with the new task
    current_task_id = *schedule_Table_Index & 0xFF000000;

    send_Str( "\nNew Task ID: " );
    send_Uint32( current_task_id );
    send_Str( "\n" );

    //Set new task's stack pointer according to ARM info center section 2.3.7
    if( current_task_id != 0 ){
        //Load the next task's stack pointer into the psp
        load_psp();
        //Set next task's context
        set_context();
        enable_irq();
        asm( "ldr pc, =0xFFFFFFFD" );
    }
    else{   //Set the processor to use the MSP
        //Set next task's context
        set_context();
        enable_irq();
        asm( "ldr pc, =0xFFFFFFF9" );
    }
}

//Save the old task's stack pointer to the PSP
static inline void store_psp( void ){
    void * sp;

    asm( "MRS %0, psp" : "=r"( sp ));   //Get the stack pointer from the psp into sp
    stack_Table[current_task_id] = sp;
    //Send stack pointer to debug to track this task
}

//Load the new task's stack pointer into the Program Stack Pointer (PSP)
static inline void load_psp( void ){
    void * sp;

    sp = stack_Table[current_task_id];
    asm( "MSR psp, %0" : "=r"( sp ));   //Place the stack pointer into the psp register
}

static inline void store_context( void ){
    uint32_t temp;

    asm(
    "MRS %0, psp\n\t"                   //Get the stack pointer from the psp into temp
    "STMDB %0, {r4-r11}\n\t"            //Store multiple registers onto the stack
    "MSR psp, %0" : "=r" (temp));       //Return the (modified) stack pointer back to the psp
}

static inline void set_context( void ){
    uint32_t temp;

    asm(
    "MRS %0, psp\n\t"                   //Get the stack pointer from the psp into temp
    "LDMFD %0!, {r4-r11}\n\t"           //Load multiple registers from the stack
    "MSR psp, %0" : "=r" (temp));       //Return the (modified) stack pointer back to the psp
}
