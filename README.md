# BlueOS
Pre-Allocated Real Time Operating System for STM32F103 microcontrollers.

This code is being developed and tested on Blue Pill boards.


*Code Structure*
The OS code is in the src and inc directories

Config code is used by the OS and tasks, it is in the config directory
The task schedule and interrupt vectors are in this code.

Task code is in the individual task directories

Tools contains user aids to use and maintain the build system

*Dependencies*
This software requires an ARM Cortex M3 toolchain to be installed and
"findable", which generally means in your "PATH" environment variable.
tools used begin with "arm-none-eabi-", e.g. arm-none-eabi-gcc.  After
that you should be able to "cd" to the root director of the repository
and run "make".

*Memory Layout*
BlueOS is statically allocated which makes finding code and debugging
easier at the expense of full processor utilization.  The current layout
is:

Flash:
    Block 1 (1K):
        Vector Table
        Task Schedule
        Task Table

    Block 2 onward:
        OS     - 5k
        Task 1 - 10k
        Task 2 - 112k

RAM:
OS - 2k
    General RAM
    Stack
Task 1 - 8k
    General RAM
    Stack
Task 2 - 10k
    General RAM
    Stack

Warning: this code is pre-alpha and may not work at all
