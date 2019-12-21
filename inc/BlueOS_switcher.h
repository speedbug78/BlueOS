#ifndef BOS_SWITCHER_H
#define BOS_SWITCHER_H

#include "BlueOS_registers.h"

void switcher_Startup( void );
uint32_t switcher_sysTime( void );
uint8_t switcher_Get_Curent_Task( void );
void switcher_Start_Task_Switching( void );
void switcher_Stop_Task_Switching( void );
void switcher_PendSV_Handler( void );
void switcher_SysTick_Handler( void );


uint32_t sysTick_sysTime( void );

extern uint32_t switch_debug;

extern const uint32_t __schedule_table_begin;
extern const uint32_t __schedule_table_end;
extern const uint32_t __task_table_begin;
extern const uint32_t __task_table_end;

#define TASK_TABLE_BEGIN        &__task_table_begin
#define TASK_TABLE_END          &__task_table_end
#define SCHEDULE_TABLE_BEGIN    &__schedule_table_begin
#define SCHEDULE_TABLE_END      &__schedule_table_end

#endif
