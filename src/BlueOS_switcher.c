#include "BlueOS_switcher.h"
#include "BlueOS_utilities.h"
#include "BlueOS_clocks.h"
#include "BlueOS_config.h"
#include "BlueOS_console.h"
#include "BlueOS_startup.h"

/* Static Variables */

static uint32_t* task_Table_Index;
static uint32_t* schedule_Table_Index;

uint32_t* stack_Table[NUMBER_OF_TASKS];

static volatile uint8_t current_task;

static volatile uint8_t current_task_id;

// Number of system "ticks" since powerup.
static volatile uint32_t sys_Ticks;
static volatile uint32_t next_Switch_Time;
static uint8_t switch_On;

uint32_t switch_debug;

/* Local Function Declarations */

static inline void store_psp( void );
static inline void load_psp( void );
static inline void store_context( void );
static inline void set_context( void );

/*
 * Lots of pointers need to be checked in this code and the
 * stack_table isn't setup properly yet.
*/

/***
 *      Startup Functions
***/

void switcher_Startup( void ){
    uint8_t i;

    switch_debug = 0;

    /* Initialize schedule table index */
    schedule_Table_Index = (uint32_t*)SCHEDULE_TABLE_BEGIN;

    /* Initialize task table index */
    task_Table_Index = (uint32_t*)TASK_TABLE_BEGIN;

    sys_Ticks = 0;
    next_Switch_Time = sys_Ticks + ( *SCHEDULE_TABLE_BEGIN & 0x00FFFFFF );
    switch_On = 0;

    /* Initialize stack table and stacks */
    for( i = 0; i < NUMBER_OF_TASKS; i ++ ){
        uint32_t ram_start, ram_length;
        uint32_t code_start;
        code_start = *(task_Table_Index + ( 0 ) + ( NUMBER_OF_TASK_DATA * i ));
        ram_start = *(task_Table_Index + ( 2 ) + ( NUMBER_OF_TASK_DATA * i ));
        ram_length = *(task_Table_Index + ( 3 ) + ( NUMBER_OF_TASK_DATA * i ));

        stack_Table[i] = (uint32_t*)( ram_start + ram_length );

        /*
         * Initialize stacks for tasks (not OS).  This is done so we can perform the first switch
         * just like all the others, rather than having a special case for the first one.
        */
        if( i > 0 ){
            // <DDI0337E 5.5.1>
            uint32_t* stack_pointer;
            stack_pointer = stack_Table[i]; /* Pull end of RAM info from the table */

            stack_pointer -= 8; /* move the stack pointer to bottom of hw stacked data (not including r4-r11 this first time, 32 bytes ). */
            /* Stack initial hw information for task */
            stack_pointer[0] = (uint32_t)i; /* Pass any argument to the task here (r0 value) */
            stack_pointer[1] = 0; /* r1 value */
            stack_pointer[2] = 0; /* r2 value */
            stack_pointer[3] = 0; /* r3 value */
            stack_pointer[4] = 0; /* r12 value */
            stack_pointer[5] = (uint32_t)hold_hdlr; /* lr value (where to go if the task doesn't start or it returns) */
            stack_pointer[6] = code_start; /* pc value needs to be the start of the task's code */
            stack_pointer[7] = 0x01000000; /* default value for the program status register */

            stack_pointer -= 8; /* Move the stack pointer to the bottom of the sw stacked area */
            /* Don't bother stacking sw info */
            stack_Table[i] = stack_pointer;  /* Update the stack pointer in the table */
        }
    }

    current_task_id = ( *schedule_Table_Index & 0xFF000000 );
}

/***
 *      Interrupt Handlers
***/

void switcher_PendSV_Handler( void ){
    //uint32_t current_task_address;
    uint32_t table_data;

    /* Don't allow other interrupts to happen during context switching */
    asm( "cpsid i" );   // Disable interrupts

    /* Clear switch (PendSV) interrupt */
    reg( SCB_ICSR ) |= 0x08000000;

    /*
     * If this isn't the OS, store its stack pointer and context.
    */
    if( current_task_id != 0 ){
        uint32_t* stack_address;

        /* Store Context */
        asm(
        "MRS %0, psp                    \n\t"   /* Get the stack pointer from the psp into r0 */
        "STMDB %0!, {r4-r11}            \n\t"   /* Store multiple registers onto the stack, update address in r0 */
        "MSR psp, %0"
        : "=r" ( stack_address )                /* outputs */
        :                                       /* inputs */
        :                                       /* clobbers */
        );

        /* Store PSP */
        stack_Table[current_task_id] = stack_address;
    }

    switch_debug = 2;

    /*
     * OS is the only one using the MSP so no need to store its pointer.
     * And it's context should be already stored on the MSP?
    */

    /* Increment the current_task to the next one in the schedule */
    schedule_Table_Index ++;
    /* If we have finished this schedule iteration go back to the beginning */
    if( (uint32_t)schedule_Table_Index >= (uint32_t)SCHEDULE_TABLE_END ){
        schedule_Table_Index = (uint32_t*)SCHEDULE_TABLE_BEGIN;
    }

    switch_debug = 3;

    table_data = (uint32_t)( *schedule_Table_Index );

    switch_debug = 4;

    /* Update current_task_id with the new task */
    current_task_id = ((uint32_t)( table_data & 0xFF000000 ) >> 24 );

    /* Update switch time for next task */
    next_Switch_Time = sys_Ticks + ( table_data & 0x00FFFFFF );

    /*send_Str( "\nTD: " );
    send_Uint32( table_data );
    send_Str( "\nID: " );
    send_Uint32( current_task_id );
    send_Str( "\n" );*/

    switch_debug = 5;

    /* Set new task's stack pointer according to ARM info center section 2.3.7 */
    if( current_task_id != 0 ){
        /* Load the next task's stack pointer into the psp */
        uint32_t stack_address;

        /* Load the next task's stack pointer into sp */
        stack_address = (uint32_t)stack_Table[current_task_id];

        /* Set next task's context */
        asm(
        "LDMFD %0!, {r4-r11}        \n\t"           /* Load multiple registers from the stack */
        "MSR   psp, %0              \n\t"           /* Return the (modified) stack pointer back to the psp */
        "LDR   lr , =0xFFFFFFFD     \n\t"           /* Write Special code into LR to go to non-privileged mode and use PSP */
        "CPSIE i                    "               /* Enable interrupts */
        :                                           /* outputs */
        : "r" ( stack_address )                     /* inputs */
        :                                           /* clobbers */
        );
    }
    else{   /* Set the processor to use the MSP */
        asm(
        "LDR   lr , =0xFFFFFFF9     \n\t"           /* Write Special code into LR to go to privileged mode and use MSP */
        "CPSIE i                    "               /* Enable interrupts */
        );
    }

    //current_task_address = stack_Table[current_task_id][8];

    switch_debug = 6;

    //send_Uint32( current_task_id );
    //send_Uint32( current_task_address );
    //send_Char( '\r' );
    //send_Char( '\n' );
}

void switcher_SysTick_Handler( void ){
    /* Increment Up Time */
    sys_Ticks ++;

    if( next_Switch_Time == sys_Ticks && switch_On ){
        /* Set PendSV interrupt (task switcher) */
        reg( SCB_ICSR ) |= BIT28;
    }
}

/***
 *      Getters & Setters
***/

/* Getter to return system time. */
uint32_t sysTick_sysTime( void ){
    return sys_Ticks;
}

/* Getter to return current task. */
uint8_t switcher_Get_Curent_Task( void ){
    return current_task_id;
}

/* Setter to set next switch time. */
void switcher_Set_Switch_Time( uint32_t time ){
    next_Switch_Time = time;
}

/* Setter to start task switching. */
void switcher_Start_Task_Switching( void ){
    uint8_t i;
    for( i = 0; i < NUMBER_OF_TASKS; i ++ ){
        send_Str( "\r\nTask: " );
        send_Uint32( i );
        send_Str( "   Stack: " );
        send_Uint32( (uint32_t)stack_Table[i] );
    }
    next_Switch_Time = sys_Ticks + ( *schedule_Table_Index & 0x00FFFFFF );
    switch_On = 1;
}

/* Setter to stop task switching. */
void switcher_Stop_Task_Switching( void ){
    switch_On = 0;
}


/***
 *      Helper Functions
***/

/* Save the old task's stack pointer to the PSP */
static inline void store_psp( void ){
    void * sp;

    asm( "MRS %0, psp" : "=r"( sp ));   /* Get the stack pointer from the psp into sp */
    stack_Table[current_task_id] = sp;
    /* Send stack pointer to debug to track this task */
}

/* Load the new task's stack pointer into the Program Stack Pointer (PSP) */
static inline void load_psp( void ){
    void * sp;

    sp = stack_Table[current_task_id];
    asm( "MSR psp, %0" : "=r"( sp ));   /* Place the stack pointer into the psp register */
}

static inline void store_context( void ){
    uint32_t temp;

    asm(
    "MRS %0, psp\n\t"                   /* Get the stack pointer from the psp into temp */
    "STMDB %0, {r4-r11}\n\t"            /* Store multiple registers onto the stack */
    "MSR psp, %0" : "=r" (temp));       /* Return the (modified) stack pointer back to the psp */
}

static inline void set_context( void ){
    uint32_t temp;

    asm(
    "MRS %0, psp\n\t"                   /* Get the stack pointer from the psp into temp */
    "LDMFD %0!, {r4-r11}\n\t"           /* Load multiple registers from the stack */
    "MSR psp, %0" : "=r" (temp));       /* Return the (modified) stack pointer back to the psp */
}
