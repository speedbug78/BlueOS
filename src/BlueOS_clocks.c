/***
 * This file should be auto genereated to reduce complexity
***/

#include "BlueOS_clocks.h"
#include "BlueOS_status.h"
#include "BlueOS_console.h"

//Number of system "ticks" since powerup.
static volatile uint32_t sys_Ticks = 0;
static volatile uint32_t next_Switch_Time = 0;
static uint8_t switch_On = 0;

/***
 * Configure SYSCLOCK for 8MHz HSI
***/
void clocks_Safe_sysClock( void ){
    reg( RCC_CR )      |= (uint32_t)0x00000001;                 //Reset RCC clock to default HSI (don't change HSI trim)
    reg( RCC_CFGR )    &= (uint32_t)0xF0FF0000;                 //Set HSI as clock, not divided for AHB, APB1 and APB2.  /2 for ADC.  No clock output
    reg( RCC_CR )      &= (uint32_t)0xFEF6FFFF;                 //Turn off PLL, HSE and Clock security
    reg( RCC_CR )      &= (uint32_t)0xFFFBFFFF;                 //HSE not bypassed
    reg( RCC_CFGR )    &= (uint32_t)0xFF80FFFF;                 //Reset PLL and USB OTG values
    reg( RCC_CIR )      = (uint32_t)0x009F0000;                 //Clear interrupt flags for clock security and PLL, HSE, HSI, LSE and LSI ready

    //Configure Flash wait states
    reg( FLASH_ACR )    = (uint32_t)0x00000030;                 //18? Reset state of flash. 0 wait states (HSI is 8Mhz), prefetch buffer enabled.

}

/***
 * Configure SYSCLOCK for 72Mhz from 8Mhz HSE (HSE is 8Mhz for Blue Pill and Maple Mini)
***/
void clocks_Start_sysClock( void ){
    uint16_t i;
    //Set clock control <RM0008 7.3.1>
    reg( RCC_CR ) |= BIT16;                 //Turn on HSE
    i = 0;
    while(( reg( RCC_CR ) & BIT17 ) == 0 ){      //Wait for HSE to start up (add timeout?)
        if( i == 0xFFFF ){
        clocks_Safe_sysClock ();
        status_Set_Error( "\nHSE Start Fail!" );
        return;
        }
        i++;
    }

    //Configure Flash <RM0008  Section 3.3.3>
    reg( FLASH_ACR ) = ( BIT1 | BIT4 );                 //Reset state of flash. 2 wait states (SYSCLK > 48Mhz), prefetch buffer enabled.

    //Set clock configuration <RM0008  Section 7.3.2>
    //Reset appropriate clock bits.
    reg( RCC_CFGR ) &= ~(
    BIT0 |
    BIT1 |
    BIT4 |
    BIT5 |
    BIT6 |
    BIT7 |
    BIT8 |
    BIT9 |
    BIT10 |
    BIT11 |
    BIT12 |
    BIT13 |
    BIT14 |
    BIT15 |
    BIT16 |
    BIT17 |
    BIT18 |
    BIT19 |
    BIT20 |
    BIT21 |
    BIT24 |
    BIT25 |
    BIT26
    );

    reg( RCC_CFGR ) |= ( BIT10 | BIT16 | BIT18 | BIT19 | BIT20 );                 //AHB, APB2 not divided (72Mhz), APB1 and ADC div 2 (36Mhz).

    //Set clock control <RM0008 7.3.1>
    reg( RCC_CR ) |= BIT24;                 //Turn on PLL
    i = 0;
    while(( reg( RCC_CR ) & BIT25 ) == 0 ){      //Wait for PLL to start up
        if( i == 0xFFFF ){
        clocks_Safe_sysClock ();
        status_Set_Error( "\nPLL Start Fail!" );
        return;
        }
        i++;
    }

    //Set clock configuration <RM0008  Section 7.3.2>
    reg( RCC_CFGR ) |= BIT1;                 //Set PLL as system clock source
    i = 0;
    while(( reg( RCC_CFGR ) & ( BIT2 | BIT3 )) != BIT3 ){ //Wait for PLL to become clock source
        if( i == 0xFFFF ){
        clocks_Safe_sysClock ();
        status_Set_Error( "\nSwitch to PLL Fail!" );
        return;
        }
        i++;
    }

    //Set clock interrupt register <RM0008  Section 7.3.3>
    //Clear interrupt flags for clock security and PLL, HSE, HSI, LSE and LSI ready
    reg( RCC_CIR ) = (
    BIT0 |
    BIT1 |
    BIT2 |
    BIT3 |
    BIT4 |
    BIT7
    );
}

/***
 * Configure Peripheral Clocks
***/
void clocks_Start_periphClocks( void ){
    //Enable Peripheral Clocks <RM0008  Section 7.3.7>
    reg( RCC_APB2ENR ) |= ( //Turn on GPIOA, GPIOB, GPIOC, AFIO and USART1
    BIT0 |
    BIT2 |
    BIT3 |
    BIT4 |
    BIT14
    );
}

void clocks_Start_sysTick( void ){
    //Setup tick interval time and start timer
    reg( SYSTICK_CTRL ) |= BIT2;                                //Set Systick to use AHB (72Mhz) clock.
    reg( SYSTICK_LOAD )  = (uint32_t)(7200 - 1);                //Systick re-load interval (.1mS).
    reg( SYSTICK_VAL )   = (uint32_t)0;                         //Reset Systick timer value.
    reg( SCB_SHPR3 )    |= ( BIT28 | BIT29 | BIT30 | BIT31 );   //Set Systick interrupt priority to highest.
    reg( SYSTICK_CTRL ) |= ( BIT0 | BIT1 );                     //Enable the Systick timer and interrupt.
}

void sysTick_Handler( void ){
    //Increment Up Time
    sys_Ticks++;

    if( next_Switch_Time == sys_Ticks && switch_On ){
        reg( SCB_ICSR ) |= BIT28; //Set switch (PendSV) interrupt
    }
}

//Getter to return system time.
uint32_t clocks_sysTime( void ){
    return sys_Ticks;
}

//Setter to set next switch time.
void sysTick_Set_Switch_Time( uint32_t time ){
    next_Switch_Time = time;
}

void sysTick_Start_Task_Switching( void ){
    switch_On = 1;
}

void sysTick_Stop_Task_Switching( void ){
    switch_On = 0;
}
