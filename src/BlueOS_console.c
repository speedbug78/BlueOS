#include "BlueOS_console.h"
#include "BlueOS_registers.h"
#include "BlueOS_switcher.h"

static const uint8_t Lookup[17] = {"0123456789ABCDEF"};

static char command_Buffer[C_BUFF_LEN];
static uint8_t c_Buff_Index;                /* Points to the next empty slot */
static uint32_t console_Timeout;

static inline void * get_pc( void );
static void console_Command( void );

void console_Command( void ){
    //send_Str( (char*)command_Buffer );
    if( command_Buffer[0] == 'L' ){
        send_Str( "\nLoad" );
    }
    else if( command_Buffer[0] == '\n' ){
        send_Str( "\nLine Feed" );
    }
    else if( command_Buffer[0] == '\r' ){
        send_Str( "\nCarriage Return" );
    }
    else if( command_Buffer[0] ==  'T' ){
        send_Str( "\nTime: " );
        send_Uint32( sysTick_sysTime() );
    }
    else if( command_Buffer[0] == 'S' ){
        send_Str( "\nStart switching" );
        switcher_Start_Task_Switching();
    }
    else if( command_Buffer[0] == 's' ){
        send_Str( "\nStop switching" );
        switcher_Stop_Task_Switching();
    }

    /* Erase command buffer */
    command_Buffer[0] = 0;
}

void console_Task( void ){
    char letter;

    letter = receive_CharN();

    /* If there isn't a character waiting just return */
    if( letter == 0xff ){
        return;
    }

    /* Submit command buffer */
    if( letter == '\n' || letter == '\r' ){
        console_Command();
        c_Buff_Index = 0;
        send_Str( CMD_LINE );
        return;
    }

    /* Handle backspace characters */
    if( letter == '\b' ){
        if( c_Buff_Index > 0 ){
            c_Buff_Index --;
            send_Str( "\b \b" );
        }
        return;
    }

    /* Add characters to the command buffer (preserve null termination) */
    if( c_Buff_Index < ( C_BUFF_LEN )){
        command_Buffer[c_Buff_Index] = letter;
        c_Buff_Index ++;
        send_Char( letter );    //Echo letter
        return;
    }
    /* Reset the command line and buffer if we have too many characters entered */
    else{
        c_Buff_Index = 0;
        send_Str( CMD_LINE );
        return;
    }
}

void console_Startup_8Mhz( void ){
    /* Init UART */
    reg( USART1_CR1 )   = (uint16_t)0x0000;                 //Turn off USART1 and reset
    reg( USART1_CR2 )   = (uint16_t)0x0000;                 //Reset
    reg( USART1_CR3 )   = (uint16_t)0x0000;                 //Reset
    reg( USART1_GTPR )  = (uint16_t)0x0000;                 //Reset
    reg( USART1_BRR )   = (uint16_t)0x0045;                 //Set for 115200 baud @8mhz (Mantissa = 4, Fraction = 5)
    reg( USART1_CR1 )  |= (uint16_t)0x340C;                 //Enable USART, transmitter and receiver, even parity

    for( c_Buff_Index = 0; c_Buff_Index < C_BUFF_LEN; c_Buff_Index++ ){
        command_Buffer[c_Buff_Index] = 0;                   //Initialize command_Buffer
    }
    c_Buff_Index = 0;                                       //Initialize the command buffer index

    send_Str( CMD_LINE );
}

void console_Startup_72Mhz( void ){
    /* Init UART */
    reg( USART1_CR1 )   = (uint16_t)0x0000;                 //Turn off USART1 and reset
    reg( USART1_CR2 )   = (uint16_t)0x0000;                 //Reset
    reg( USART1_CR3 )   = (uint16_t)0x0000;                 //Reset
    reg( USART1_GTPR )  = (uint16_t)0x0000;                 //Reset
    reg( USART1_BRR )   = (uint16_t)0x0271;                 //Set for 115200 baud @72mhZ (Mantissa = 39, Fraction = 1)
    reg( USART1_CR1 )  |= ( BIT2 |  BIT3 );                 //Enable transmitter and receiver
    reg( USART1_CR1 )  |= BIT13;                            //Enable UART

    console_Timeout = 0xFFFF;

    for( c_Buff_Index = 0; c_Buff_Index < C_BUFF_LEN; c_Buff_Index++ ){
        command_Buffer[c_Buff_Index] = 0;                   //Initialize command_Buffer
    }
    c_Buff_Index = 0;                                       //Initialize the command buffer index

    send_Str( CMD_LINE );
}

/* Send 1 hex byte out of the UART */
void send_Byte( uint8_t character ){
    uint8_t high, low;
    low = character & 0x0F;
    high = (( character >> 4 ) & 0x0F);
    send_Char( Lookup[high] );
    send_Char( Lookup[low] );
}

/* ASCII character to Byte conversion */
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
    return data;                                                        /* Return hex byte */
}

void send_Uint32( uint32_t data ){
    send_Byte( (uint8_t)( data >> 24 ));
    send_Byte( (uint8_t)( data >> 16 ));
    send_Byte( (uint8_t)( data >> 8 ));
    send_Byte( (uint8_t)( data ));
}

/* Send a single character out USART1, blocking */
void send_Char( char character ){
    uint32_t i;
    i = 0;
    while((( reg( USART1_SR ) & BIT7 ) == 0 ) && ( i < console_Timeout )){ i++; };                      /* Wait for TX buffer to empty */
    reg( USART1_DR ) = (uint16_t)character & (uint16_t)0x00FF;
}

/* Send a string of characters (null terminated) out USART1 */
void send_Str( char* strptr ){
    while(( *strptr != 0 )){
            send_Char( *strptr );
            strptr += 1;
    }
}

/* Receive a character from USART1, blocking */
char receive_Char( void ){
    while(( reg( USART1_SR ) & BIT5 ) == 0 ){};                       /* Wait for character to come in */
    return (char)((uint16_t)reg( USART1_DR ));
}

/* Receive a character from USART1, non-blocking */
char receive_CharN( void ){
    if(( reg( USART1_SR ) & BIT5 ) != 0 ){
        return (char)((uint16_t)reg( USART1_DR ));
    }
    return 0xff;
}

void inline send_PC( void ){
    send_Uint32( (uint32_t)get_pc() );
}

static inline void * get_pc( void ){
    void * pc;
    asm( "mov %0, pc" : "=r"( pc ));
    return pc;
}
