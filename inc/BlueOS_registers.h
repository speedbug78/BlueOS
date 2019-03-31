#ifndef BOS_SETUP
#define BOS_SETUP

#include "BlueOS_types.h"

//Macro to simplify register access
#define reg(x)          (*(volatile uint32_t*)(x))

//#include "your processor's register definitions here"
#include "STM32F103_Regs.h"

#endif
