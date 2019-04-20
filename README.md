# BlueOS
Pre-Allocated Real Time Operating System for STM32F103 microcontrollers.

***Warning: this code is pre-alpha and may not work at all***

This code is being developed and tested on [Blue Pill](https://wiki.stm32duino.com/index.php?title=Blue_Pill) boards.



### Code Structure

    / BlueOS (root folder)
    |----src  (Source files for BlueOS)
    |----inc  (Include files for BlueOS)
    |----obj  (Location for temporary build files)
    |----bin  (Build output for BlueOS)
    |----config (Source and build files for task configuration)
    |----Task1  (Build files for Task1)
        |-----src  (Source files for Task1)
        |-----inc  (Include files for Task1)
        |-----obj  (Temporary build files for Task1)
        |-----bin  (Build ouptut for Task1)
    |----Task2  (Build files for Task2)
        |-----src  (Source files for Task2)
        |-----inc  (Include files for Task2)
        |-----obj  (Temporary build files for Task2)
        |-----bin  (Build ouptut for Task2)
    |----Tools   (Build files for BlueOS tools)
        |-----src  (Source files for tools)
        |-----inc  (Include files for tools)
        |-----obj  (Temporary build files for tools)
        |-----bin  (Build ouptut for tools)

The OS code is in the src and inc directories

Config code is used by the OS and tasks, it is in the config directory
The task schedule and interrupt vectors are in this code.

Task code is in the individual task directories

Tools contains user aids to use and maintain the build system

### Dependencies
This software requires an ARM Cortex M3 toolchain to be installed and
"findable", which generally means in your "PATH" environment variable.
tools used begin with "arm-none-eabi-", e.g. arm-none-eabi-gcc.  After
that you should be able to "cd" to the root director of the repository
and run "make".

### Memory Layout
BlueOS is statically allocated which makes finding code and debugging
easier at the expense of full processor utilization.  The current layout
is:

#### Flash:
**Page 1 (1K starting at 0x08000000):**

 -  Allocated in BlueOS_linker.ld
 
**Item** | **Size**|**Address**
--------------|-------------|-------------
Vector Table | 336 Bytes|0x08000000
Task Schedule | 528 Bytes|0x08000150
Task Table | 160 Bytes|0x08000360
       
**Remaining Flash (starting at 0x08000400):**

-  Allocated in the Task Table in BlueOS_config.c

**Item** | **Size** | **Address**
--------------|-------------|--------------
OS      | 5k | 0x08000400
Task 1 | 10k | 0x08001400
Task 2 | 112k | 0x08004000

#### RAM:
-  Allocated in the Task Table in BlueOS_config.c

|**Item**|**Size**|**Address**
|-----------|-----------|---------------
|__OS__|2k|0x20000000
|- _General RAM_|\\/|
|- _Stack_|/\\ |0x200007FF
|__Task 1__|8k|0x20000800
|- _General RAM_|\\/|
|- _Stack_|/\\ |0x200027FF
|__Task 2__|10k|0x20002800
|- _General RAM_|\\/|
|- _Stack_|/\\ |0x20004FFF
