#ifndef BOS_LOADER
#define BOS_LOADER

#include "BlueOS_types.h"

void loader( void );
void send_Char( char character );
void send_Str( char* strptr );
char receive_Char( void );
char receive_CharN( void );
void erase_Flash( void );
void unlock_Flash( void );
void lock_Flash( void );
void wait_Flash( void );
void get_Ihex( void );
uint8_t char_Byte( void );
void write_Flash ( uint16_t data, uint32_t address );
void verify_Load( void );
void send_Byte( uint8_t );
void send_Uint32( uint32_t );

//From linker file
extern const uint32_t __User_F_Top, __User_F_Bottom;

#define link_val(x) (uint32_t)&x

#endif
