/*
 * Alias BlueOS names to HAL function names.
*/

#ifdef USE_HAL_DRIVER

#define switcher_PendSV_Handler     PendSV_Handler
#define switcher_SysTick_Handler    SysTick_Handler

#else

#define BLUE_OS_STARTUP

#endif
