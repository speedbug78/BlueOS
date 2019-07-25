#include "BlueOS_status.h"
#include "BlueOS_registers.h"
#include "BlueOS_console.h"
#include "BlueOS_clocks.h"
#include "BlueOS_switcher.h"
#include "BlueOS_IO.h"

static char*    Last_Error;
static uint8_t  Led_On;
static uint32_t Led_Time;
static uint32_t Led_Wait_Time;

void status_Startup( void ){
    Last_Error = "\nNo Error";
    Led_On = 0;
    Led_Time = sysTick_sysTime();
    Led_Wait_Time = 10000;          /* In time ticks (.1 milliseconds) */
}

void status_LED_Task( void ){

    if( sysTick_sysTime() >= ( Led_Time + Led_Wait_Time )){
        Led_Time = sysTick_sysTime();
        if( Led_On == 1 ){
            reg( LED_PORT ) |= IO_RESET( LED_PIN );
            Led_On = 0;
        }
        else{
            reg( LED_PORT ) |= LED_PIN;
            Led_On = 1;
        }
    }
}

void status_Set_Led_wait( uint32_t wait_time ){
    Led_Wait_Time = wait_time;
}

void status_Set_Error( char* s ){
    send_Str( "\r\nError Set" );
    Last_Error = s;
}

void status_Show_Error( void ){
    send_Str( Last_Error );
}
