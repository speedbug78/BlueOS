#ifndef BOS_QUEUE_H
#define BOS_QUEUE_H

#include "BlueOS_defines.h"

typedef enum{
    QUEUE_UN_INIT,
    QUEUE_FREE,
    QUEUE_BUSY,
    QUEUE_FULL,
    QUEUE_EMPTY
} queue_status_t;

typedef enum{
    QUEUE_OP_PERMISSION_DENIED,
    QUEUE_OP_INVALID_REQUEST,
    QUEUE_OP_BUSY,
    QUEUE_OP_SUCCESS,
    QUEUE_OP_FULL,
    QUEUE_OP_EMPTY
} queue_op_status_t;

typedef union queue_data{
    uint32_t word;
    uint16_t h_word[2];
    uint8_t  byte[4];
} queue_data_t;

typedef volatile struct {
    queue_status_t status : 4;

    uint8_t writer_id;
    uint8_t reader_id;
    uint16_t write_idx;
    uint16_t read_idx;
    uint16_t current_usage;
    uint16_t max_usage;
    uint32_t queue_len;
    uint32_t num_reads;
    uint32_t num_writes;
    uint32_t num_overflows;
    uint32_t num_underflows;
    uint32_t num_collisions;
    uint32_t num_unauthorized;
    queue_data_t* queue_addr;

} queue_header_t;

extern queue_header_t* queue_list[];

void queues_Startup( void );
queue_op_status_t queue_Erase( uint32_t queue_id );
queue_op_status_t queue_Read( uint32_t queue_id, queue_data_t* data );
queue_op_status_t queue_Write( uint32_t queue_id, queue_data_t data );
queue_op_status_t queue_Get_Num_Writes ( uint32_t queue_id, uint32_t* num_writes );
queue_op_status_t queue_Get_Num_Reads ( uint32_t queue_id, uint32_t* num_reads );
queue_op_status_t queue_Get_Max_Usage ( uint32_t queue_id, uint32_t* max_usage );
queue_op_status_t queue_Get_Num_Collisions ( uint32_t queue_id, uint32_t* num_collisions );
queue_op_status_t queue_Get_Num_Underflows ( uint32_t queue_id, uint32_t* num_underflows );
queue_op_status_t queue_Get_Num_Overflows ( uint32_t queue_id, uint32_t* num_overflows );
queue_op_status_t queue_Get_Num_Unauthorized ( uint32_t queue_id, uint32_t* num_unauthorized );
queue_op_status_t queue_Get_Size ( uint32_t queue_id, uint16_t* len );
queue_op_status_t queue_Get_Percent_Used ( uint32_t queue_id, float_t* percent_used );
queue_op_status_t queue_Get_Used ( uint32_t queue_id, uint16_t* num_used );

#endif
