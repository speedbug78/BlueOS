#ifndef BOS_UTILITIES_H
#define BOS_UTILITIES_H

#include "BlueOS_registers.h"

#define disable_irq()     asm( "cpsid i" )
#define enable_irq()      asm( "cpsie i" )

#endif
