
#include "BlueOS_queues.h"

/* Queue definitions */

/* Queue data arrays */
/* Queues are managed by the OS, so reside in OS RAM */
queue_data_t queue1_data[10];

/* Queue headers and initialization */
queue_header_t queue1_header = {
    QUEUE_UN_INIT,  /* Queue Status */
    1,              /* Writer Task ID */
    2,              /* Reader Task ID */
    0,              /* Writer Index in Queue Data */
    0,              /* Reader Index in Queue Data */
    0,              /* current usage */
    0,              /* max usage */
    10,             /* Queue Data Length (must match data array size) */
    0,              /* number of attempted reads */
    0,              /* number of attempted writes */
    0,              /* number of overflows */
    0,              /* number of underflows */
    0,              /* number of collisions */
    0,              /* number of unauthorized attempts */
    queue1_data     /* Pointer to Queue Data Array */ };

/* Queue List */
/* Pointers to each queue */
queue_header_t* queue_list[] = {
    &queue1_header,
    0
};

