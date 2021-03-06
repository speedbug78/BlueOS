/**
 * Main file for Task2
**/

//#include "your processor's register definitions here"
#include "STM32F103_Regs.h"

//Macro to simplify register access
#define reg(x)          (*(volatile uint32_t*)(x))
#define BIT7    (uint32_t)( 0x00000001 << 7 )

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

void send_Str( char* strptr );
void send_Char( char character );

__attribute__ (( section ( ".task2" )))

int main ( void ) {
    while(1){
        send_Str( "\nTask 2" );
    }
    return 0;
}

//Send a single character out USART1, blocking
void send_Char( char character ){
    uint32_t i;
    i = 0;
    while((( reg( USART1_SR ) & BIT7 ) == 0 ) && ( i < 0xFFFF )){ i++; };                      //Wait for TX buffer to empty
    reg( USART1_DR ) = (uint16_t)character & (uint16_t)0x00FF;
}

//Send a string of characters (null terminated) out USART1
void send_Str( char* strptr ){
    while(( *strptr != 0 )){
            send_Char( *strptr );
            strptr += 1;
    }
}
