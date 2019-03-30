#ifndef BOS_CONSOLE_H
#define BOS_CONSOLE_H

#include <stdint.h>

void send_Byte( uint8_t );
void send_Char( char );
void send_Str( char* );
void send_Uint32( uint32_t );
uint8_t char_Byte( void );
char receive_Char( void );
char receive_CharN( void );
void serial_Init( void );
void send_PC( void );

#endif
