#ifndef BOS_STATUS_H
#define BOS_STATUS_H

#include "BlueOS_defines.h"

void status_LED_Task( void );
void status_Startup( void );
void status_Set_Led_wait( uint32_t wait_time );
void status_Set_Error( char* s );
void status_Show_Error( void );

#ifdef MAPLE_MINI
#define LED_PIN BIT1
#define LED_PORT GPIOB_BSRR
#endif

#ifdef BLUE_PILL
#define LED_PIN BIT13
#define LED_PORT GPIOC_BSRR
#endif

#endif
