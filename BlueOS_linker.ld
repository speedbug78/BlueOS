
ENTRY (os_entry)

/**
 * Processor Memory Addresses
**/
/*Total Flash address*/
__Flash_Bottom  = 0x08000000;
__Flash_Top     = 0x0801FFFF;
__Flash_Length  = 0x20000;

/*Total RAM address*/
__RAM_Bottom    = 0x20000000;
__RAM_Top       = 0x20004FFF;
__RAM_Length    = 0x5000;

/**
 * OS Memory Addresses
**/
/*OS Config Flash address*/
__Config_F_Bottom = 0x08000000;
__Config_F_Top    = 0x080003FF;
__Config_F_Length = 0x400;

/*OS Flash address*/
__OS_F_Bottom = 0x08000400;
__OS_F_Top    = 0x080017FF;
__OS_F_Length = 0x1400;

/*OS RAM address*/
__RAM_Bottom    = 0x20000000;
__RAM_Top       = 0x200013FF;
__RAM_Length    = 0x1400;

/*OS Stack location in RAM*/
__StackTop      = 0x200007FF;

SECTIONS{
    .text __Config_F_Bottom : {
        . = ALIGN( 4 );
        __config_start = .;
        KEEP( *( .vector_table ))
        FILL(0xCEADDEAD)
        . = 0x0150;
        __schedule_table_begin = .;
        KEEP( *( .schedule_table ))
        __schedule_table_end = .;
        FILL(0xCEADDEAD)
        . = 0x0360;
        __task_table_begin = .;
        KEEP( *( .task_table))
        __task_table_end = .;
        FILL(0xCEADDEAD)
        __config_end = .;
        . = ALIGN( 0x0400 );    /*Align on memory block boundry*/
        __os_start = .;
        *( .text* )
        *( .rodata* )
        . = ALIGN( 4 );
        __os_end = .;
    }

    .data 0x20000000 : AT( ADDR( .text) + SIZEOF( .text )){    /*AT tells the linker where in flash to put .data (at the end of .text).  Otherwise initial variable values would be lost*/
        . = ALIGN( 4 );
        *( .data* )
        . = ALIGN( 4 );
    }

    .bss 0x20000000 : {
        *( .bss* )
        *( COMMON )
    }
}
