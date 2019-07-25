/**
 * Main file for Task1
**/

//#include "your processor's register definitions here"
#include "STM32F103_Regs.h"

//Macro to simplify register access
#define reg(x)          (*(volatile uint32_t*)(x))
#define BIT7    (uint32_t)( 0x00000001 << 7 )
#define IO_PIN (uint32_t)( 0x00000001 << 9 )
#define IO_PORT GPIOB_BSRR
#define IO_RESET(x) ( x << 16 )


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

void send_Str( char* strptr );
void send_Char( char character );

__attribute__ (( section ( ".task1" )))

int main ( void ) {
    reg( GPIOB_CRH )   &= (uint32_t)0xFFFFFF0F;                   //Reset PB9 settings
    reg( GPIOB_CRH )   |= (uint32_t)0x00000020;                   //PB9 general purpose output, push pull, 2Mhz
    while(1){
        uint8_t pin_on;
        pin_on = 0;
        if( pin_on == 1 ){
            reg( IO_PORT ) |= IO_RESET( IO_PIN );
            pin_on = 0;
        }
        else{
            reg( IO_PORT ) |= IO_PIN;
            pin_on = 1;
        }
        send_Str( "\r\nTask 1" );
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
