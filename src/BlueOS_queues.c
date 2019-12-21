/***
 *
 * Queues for BlueOS
 *
 *
***/

#include "BlueOS_queues.h"
#include "BlueOS_config.h"
#include "BlueOS_switcher.h"

uint32_t num_queues;

void queues_Startup( void ){
    queue_header_t* queue_ptr;

    num_queues = 0;
    queue_ptr = queue_list[num_queues];
    while( queue_ptr != 0 ){
        num_queues++;
        queue_Erase( num_queues - 1 );
        queue_ptr = queue_list[num_queues];
    }
}

queue_op_status_t queue_Erase( uint32_t queue_id ){
    uint8_t task_id;
    uint16_t i;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Don't erase a busy queue */
    if( queue.status == QUEUE_BUSY ){
        queue.num_collisions++;
        return QUEUE_OP_BUSY;
    }

    /* Check that task ID is the queue writer or OS */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    /* Set queue status to busy */
    queue.status = QUEUE_BUSY;

    /* Reset the read and write indexes */
    queue.write_idx = 0;
    queue.read_idx = 0;

    /* Zero out all data */
    for( i = 0; i < queue.queue_len; i++ ){
        queue.queue_addr[i].word = (uint32_t)0;
    }

    /* Free the Queue */
    queue.status = QUEUE_FREE;

    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Write( uint32_t queue_id, queue_data_t data ){
    uint8_t task_id;
    uint16_t read_idx, write_idx, queue_len;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    queue.num_writes++;

    /* Check that the queue isn't busy */
    if( queue.status == QUEUE_BUSY ){
        queue.num_collisions++;
        return QUEUE_OP_BUSY;
    }

    /* Check that task ID is the queue writer or OS */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    /* Set queue status to busy */
    queue.status = QUEUE_BUSY;

    read_idx = queue.read_idx;
    write_idx = queue.write_idx;
    queue_len = queue.queue_len;

    if( write_idx + 1 < queue_len ){
        if( write_idx + 1 < read_idx ){
            write_idx++;
        }
        /* Queue is full */
        else{
            /* Free the Queue */
            queue.num_overflows++;
            queue.status = QUEUE_FREE;
            return QUEUE_OP_FULL;
        }
    }
    else{
        if( read_idx != 0 ){
            write_idx = 0;
        }
        /* Queue is full */
        else{
            /* Free the Queue */
            queue.num_overflows++;
            queue.status = QUEUE_FREE;
            return QUEUE_OP_FULL;
        }
    }

    /* Write data to queue */
    queue.current_usage++;
    queue.queue_addr[write_idx] = data;

    if( queue.current_usage > queue.max_usage ){
        queue.max_usage = queue.current_usage;
    }

    /* Free the Queue */
    queue.status = QUEUE_FREE;

    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Read( uint32_t queue_id, queue_data_t* data ){
    uint8_t task_id;
    uint16_t read_idx, write_idx, queue_len;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    queue.num_reads++;

    /* Check that the queue isn't busy */
    if( queue.status == QUEUE_BUSY ){
        queue.num_collisions++;
        return QUEUE_OP_BUSY;
    }

    /* Check that task ID is the queue writer or OS */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    /* Set queue status to busy */
    queue.status = QUEUE_BUSY;

    read_idx = queue.read_idx;
    write_idx = queue.write_idx;
    queue_len = queue.queue_len;

    /* Queue is empty */
    if( read_idx == write_idx ){
        queue.num_underflows++;
        queue.status = QUEUE_FREE;
        return QUEUE_OP_EMPTY;
    }

    /* Retrieve data */
    queue.current_usage--;
    *data = queue.queue_addr[read_idx];

    /* Increment read index */
    if( read_idx + 1 >= queue_len ){
        read_idx = 0;
    }
    else{
        read_idx++;
    }

    /* Free the Queue */
    queue.status = QUEUE_FREE;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Size ( uint32_t queue_id, uint16_t* len ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *len = queue.queue_len;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Num_Unauthorized ( uint32_t queue_id, uint32_t* num_unauthorized ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *num_unauthorized = queue.num_unauthorized;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Num_Overflows ( uint32_t queue_id, uint32_t* num_overflows ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *num_overflows = queue.num_overflows;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Num_Underflows ( uint32_t queue_id, uint32_t* num_underflows ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *num_underflows = queue.num_underflows;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Num_Collisions ( uint32_t queue_id, uint32_t* num_collisions ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *num_collisions = queue.num_collisions;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Max_Usage ( uint32_t queue_id, uint32_t* max_usage ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *max_usage = queue.max_usage;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Num_Reads ( uint32_t queue_id, uint32_t* num_reads ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *num_reads = queue.num_reads;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Num_Writes ( uint32_t queue_id, uint32_t* num_writes ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *num_writes = queue.num_writes;
    return QUEUE_OP_SUCCESS;
}

queue_op_status_t queue_Get_Used ( uint32_t queue_id, uint16_t* num_used ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    *num_used = queue.current_usage;

    /* Return state of the queue */
    if( queue.status == QUEUE_BUSY ){
        queue.num_collisions++;
        return QUEUE_OP_BUSY;
    }
    else{
        return QUEUE_OP_SUCCESS;
    }
}

queue_op_status_t queue_Get_Percent_Used ( uint32_t queue_id, float_t* percent_used ){
    uint8_t task_id;
    queue_header_t queue;

    /* Check that requested queue exists */
    if( queue_id >= num_queues ){
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check for null pointer */
    if( queue_list[queue_id] != 0 ){
        queue = *queue_list[queue_id];
    }
    else{
        return QUEUE_OP_INVALID_REQUEST;
    }

    /* Check task permissions */
    task_id = switcher_Get_Curent_Task();
    if( task_id != queue.reader_id &&
        task_id != queue.writer_id &&
        task_id != 0 ){

            queue.num_unauthorized++;
            return QUEUE_OP_PERMISSION_DENIED;
    }

    /* Return percent used number */
    *percent_used = ( (float_t)queue.current_usage / (float_t)queue.queue_len );

    /* Return state of the queue */
    if( queue.status == QUEUE_BUSY ){
        queue.num_collisions++;
        return QUEUE_OP_BUSY;
    }
    else{
        return QUEUE_OP_SUCCESS;
    }
}
