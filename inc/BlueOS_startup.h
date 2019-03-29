#ifndef BMOS_STARTUP_H
#define BMOS_STARTUP_H

#include <stdint.h>

void reset_hdlr( void );
void hold_hdlr( void );
void hf_hdlr( void );
void mm_hdlr( void );
void bf_hdlr( void );
void uf_hdlr( void );
void sc_hdlr( void );

//From the jump assembly file
extern void User_Jump( void );

#endif
