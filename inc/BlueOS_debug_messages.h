#ifndef BOS_DEBUG_MSG_H
#define BOS_DEBUG_MSG_H

#define BOS_DEBUG

void debug_Boot_Finished( void );
void debug_Hard_Fault( void );
void debug_Erase_Error( void );
void debug_Hex_Error( void );
void debug_User_Jump( void );
void debug_Hex_Load( void );
void debug_Write_Error( void );
void debug_Invalid_Record( void );
void debug_Invalid_Checksum( void );
void debug_Write_Verify_Error( void );
void debug_Invalid_Address( void );

#ifdef MAPLE_MINI
#define BOOTLOAD_BANNER "\r\nBlue Metal Maple Mini Bootloader."
#endif

#ifdef BLUE_PILL
#define BOOTLOAD_BANNER "\r\nBlue Metal Blue Pill Bootloader."
#endif

#endif
