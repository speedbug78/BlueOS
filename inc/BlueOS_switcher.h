#ifndef BOS_SWITCHER_H
#define BOS_SWITCHER_H

#include "BlueOS_registers.h"

void switcher_Startup( void );
void switcher_Handler( void );

extern const uint32_t __schedule_table_begin;
extern const uint32_t __schedule_table_end;
extern const uint32_t __task_table_begin;
extern const uint32_t __task_table_end;

#define TASK_TABLE_BEGIN        &__task_table_begin
#define TASK_TABLE_END          &__task_table_end
#define SCHEDULE_TABLE_BEGIN    &__schedule_table_begin
#define SCHEDULE_TABLE_END      &__schedule_table_end

#endif
