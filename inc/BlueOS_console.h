#ifndef BOS_CONSOLE_H
#define BOS_CONSOLE_H

#include "BlueOS_defines.h"

#define C_BUFF_LEN  255

#define CMD_LINE    "\nBlue_OS\\>"

void console_Startup_8Mhz( void );
void console_Startup_72Mhz( void );
void console_Task( void );

void send_Byte( uint8_t );
void send_Char( char );
void send_Str( char* );
void send_Uint32( uint32_t );
uint8_t char_Byte( void );
char receive_Char( void );
char receive_CharN( void );
void send_PC( void );

#endif
