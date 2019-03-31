#include "BlueOS_systick.h"

//Number of system "ticks" since powerup.
static volatile uint32_t sys_Ticks = 0;
static volatile uint32_t next_Switch_Time = 0;

void sysTick_Startup( void ){
    //Setup tick interval time and start timer
    reg( SYSTICK_CTRL ) |= (uint32_t)0x00000004;    //Set Systick to use AHB (72Mhz) clock.
    reg( SYSTICK_LOAD )  = (uint32_t)(7200 - 1);          //Systick re-load interval (.1mS).
    reg( SYSTICK_VAL )   = (uint32_t)0;             //Reset Systick timer value.
    reg( SCB_SHPR3 )    |= (uint32_t)0xF0000000;    //Set Systick interrupt priority to highest.
    reg( SYSTICK_CTRL ) |= (uint32_t)0x00000003;    //Enable the Systick timer and interrupt.
}

void sysTick_Handler( void ){
    //Increment Up Time
    sys_Ticks++;

    if( next_Switch_Time == sys_Ticks ){
        //Set switch (PendSV) interrupt
        reg( SCB_ICSR ) |= 0x10000000;
    }
}

//Getter to return system time.
uint32_t sysTick_Time( void ){
    return sys_Ticks;
}

//Setter to set next switch time.
void sysTick_Set_Switch_Time( uint32_t time ){
    next_Switch_Time = time;
}
