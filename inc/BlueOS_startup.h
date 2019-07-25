#ifndef BOS_STARTUP_H
#define BOS_STARTUP_H

#include "BlueOS_defines.h"

int main( void );
void nm_hdlr( void );
void hold_hdlr( void ) __attribute__( ( naked ) );
void hf_hdlr( void ) __attribute__( ( naked ) );
void mm_hdlr( void ) __attribute__( ( naked ) );
void bf_hdlr( void ) __attribute__( ( naked ) );
void uf_hdlr( void ) __attribute__( ( naked ) );
void sc_hdlr( void );
void dm_hdlr( void );

#endif
