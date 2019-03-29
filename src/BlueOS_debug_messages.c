#include "BlueOS_debug_messages.h"
#include "BlueOS_registers.h"
#include "BlueOS_serial.h"

void debug_Boot_Finished( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nBootloader Finished." );
    #endif
}

void debug_Hard_Fault( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nHard Fault: \r\nHFSR: " );
    send_Uint32( reg( SCB_HFSR ));
    send_Str( "\r\nCFSR: " );
    send_Uint32( reg( SCB_CFSR ));
    #endif
}

void debug_Erase_Error( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nError erasing flash!" );
    #endif
}

void debug_Hex_Error( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nError loading .hex file!" );
    #endif
}

void debug_User_Jump( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nJumping to user code." );
    #endif
}

void debug_Hex_Load( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nWaiting for .hex file." );
    #endif
}

void debug_Write_Error( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nFlash write failed!" );
    #endif
}

void debug_Invalid_Record( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nInvalide record!" );
    #endif
}

void debug_Invalid_Checksum( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nInvalid checksum!" );
    #endif
}

void debug_Write_Verify_Error( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nWrite verify failed!" );
    #endif
}

void debug_Invalid_Address( void ){
    #ifdef BOS_DEBUG
    send_Str( "\r\nAddress out of range!" );
    #endif
}
