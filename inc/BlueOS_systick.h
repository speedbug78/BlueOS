#ifndef BOS_SYSTICK_H
#define BOS_SYSTICK_H

#include "BlueOS_registers.h"

void sysTick_Startup( void );
uint32_t sysTick_Time( void );
void sysTick_Delay( uint32_t );

#endif
