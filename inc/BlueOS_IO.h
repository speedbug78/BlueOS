#ifndef BOS_IO_H
#define BOS_IO_H

#include "BlueOS_defines.h"

#define IO_RESET(x) ( x << 16 )

void io_Startup( void );

#endif
