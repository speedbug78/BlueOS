#include "BlueOS_console.h"
#include "BlueOS_registers.h"

static const uint8_t Lookup[17] = {"0123456789ABCDEF"};
static __inline__ void * get_pc( void );


void send_Byte( uint8_t character ){
    uint8_t high, low;
    low = character & 0x0F;
    high = (( character >> 4 ) & 0x0F);
    send_Char( Lookup[high] );
    send_Char( Lookup[low] );
}

//ASCII character to Byte conversion
uint8_t char_Byte( void ){
    uint8_t temp_data, data = 0;

    for( uint8_t i = 0; i < 2; i++ ){
        temp_data = (uint8_t)receive_Char();
        for( uint8_t c = 0; c < 16; c++ ){
            if( Lookup[c] == temp_data ){
                if( i == 0 ){
                    data = c << 4;
                }
                else{
                    data += c;
                }
            }
        }
    }
    return data;                                                        //Return hex byte
}

void send_Uint32( uint32_t data ){
    send_Byte( (uint8_t)( data >> 24 ));
    send_Byte( (uint8_t)( data >> 16 ));
    send_Byte( (uint8_t)( data >> 8 ));
    send_Byte( (uint8_t)( data ));
}

//Send a single character out USART1, blocking
void send_Char( char character ){
    while(( reg( USART1_SR ) & (uint16_t)0x0080) == 0 ){};          //Wait for TX buffer to empty and Rx buffer to empty
    reg( USART1_DR ) = (uint16_t)character & (uint16_t)0x00FF;
}

//Send a string of characters (null terminated) out USART1
void send_Str( char* strptr ){
    while( *strptr != 0 ){
            send_Char( *strptr );
            strptr += 1;
    }
}

//Receive a character from USART1, blocking
char receive_Char( void ){
    while(( reg( USART1_SR ) & (uint16_t)0x0020 ) == 0 ){};          //Wait for character to come in and Tx to be empty
    return (char)((uint16_t)reg( USART1_DR ));
}

//Receive a character from USART1, non-blocking
char receive_CharN( void ){
    if(( reg( USART1_SR ) & (uint16_t)0x0020 ) != 0 ){
        return (char)((uint16_t)reg( USART1_DR ));
    }
    return -1;
}

void serial_Init( void ){
    //Init UART
    reg( USART1_CR1 )   = (uint16_t)0x0000;                 //Turn off USART1 and reset
    reg( USART1_CR2 )   = (uint16_t)0x0000;                 //Reset
    reg( USART1_CR3 )   = (uint16_t)0x0000;                 //Reset
    reg( USART1_GTPR )  = (uint16_t)0x0000;                 //Reset
    reg( USART1_BRR )   = (uint16_t)0x0045;                 //Set for 115200 baud (Mantissa = 4, Fraction = 5)
    reg( USART1_CR1 )  |= (uint16_t)0x340C;                 //Enable USART, transmitter and receiver, even parity
}

void __inline__ send_PC( void ){
    send_Uint32( (uint32_t)get_pc() );
}

static __inline__ void * get_pc( void ){
    void * pc;
    asm( "mov %0, pc" : "=r"( pc ));
    return pc;
}
