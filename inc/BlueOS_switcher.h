#ifndef BOS_SWITCHER_H
#define BOS_SWITCHER_H

#include "BlueOS_registers.h"

//Structure to hold the schedule
typedef struct{
    uint8_t     task_id;
    uint32_t    task_ticks;
} schedule_table_type;

typedef struct{
    void*       task_sp;
    //NVIC pushes r0-r3, r12, lr, pc and psr to the stack
} task_table_type;

void switcher_Startup( void );
void switcher_Handler( void );

#define MAX_SCHEDULE_LEN    50
#define MAX_NUM_TASKS        10

#endif
