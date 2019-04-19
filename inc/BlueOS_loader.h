#ifndef BOS_LOADER
#define BOS_LOADER

#include "BlueOS_defines.h"

void loader( void );

#define link_val(x) (uint32_t)&x

#define LOADER_BANNER "\nLOADER\n"

#endif
