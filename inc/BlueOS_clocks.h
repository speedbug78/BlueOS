#ifndef BOS_CLOCKS_H
#define BOS_CLOCKS_H

#include "BlueOS_registers.h"

void clocks_Safe_sysClock( void );
void clocks_Start_sysClock( void );
void clocks_Start_periphClocks( void );
void clocks_Start_sysTick( void );
uint32_t clocks_sysTime( void );
void sysTick_Handler( void );
void sysTick_Start_Task_Switching( void );
void sysTick_Stop_Task_Switching( void );

#endif
