#include "BlueOS_systick.h"

//Number of system "ticks" since powerup.
static volatile uint32_t sys_Ticks = 0;
static volatile uint32_t next_Switch_Time = 0;

void sysTick_Startup( void ){
    //Setup tick interval time and start timer
}

void sysTick_Handler( void ){
    //Increment Up Time
    sys_Ticks++;

    if( next_Switch_Time == sys_Ticks ){
        //Set switch (PendSV) interrupt
        reg( SCB_ICSR ) |= 0x00010000000;
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
