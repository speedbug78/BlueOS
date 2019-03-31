#include "BlueOS_switcher.h"

void switcher_Startup( void ){
    //Check schedule
}


void switcher_Handler( void ){
    //Clear switch (PendSV) interrupt
    reg( SCB_ICSR ) |= 0x08000000;
    //Set next contect switch

    //Perform this context switch
    //Stash current working registers
    //Pull new working registers
}
