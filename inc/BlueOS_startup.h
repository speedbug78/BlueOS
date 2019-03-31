#ifndef BOS_STARTUP_H
#define BOS_STARTUP_H

#include "BlueOS_types.h"

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
