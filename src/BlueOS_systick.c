#include "BlueOS_systick.h"

static volatile uint32_t us_Up_Time = 0;

static volatile uint32_t us_Delay_Time = 0;

void SysTick_Handler(void) {

  us_Up_Time++;

  if(us_Delay_Time){
    us_Delay_Time--;
  }
}



void sysTick_Delay (uint32_t microseconds_delay) {

    us_Delay_Time = microseconds_delay;
    while (us_Delay_Time);  //Do nothing
}



uint32_t sysTick_Time (void){
    return us_Up_Time;
}
