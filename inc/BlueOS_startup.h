#ifndef BOS_STARTUP_H
#define BOS_STARTUP_H

#include "BlueOS_defines.h"

void os_entry( void );
void hold_hdlr( void );
void nm_hdlr( void );
void hf_hdlr( void );
void mm_hdlr( void );
void bf_hdlr( void );
void uf_hdlr( void );
void sc_hdlr( void );
void dm_hdlr( void );

#endif
