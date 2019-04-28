
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
__RAM_Top       = 0x200007FF;
__RAM_Length    = 0x0800;

/*OS Stack location in RAM*/
__StackTop      = 0x200007FF;

MEMORY{
    CONFIG_FLASH        ( rx )  : ORIGIN = 0x08000000, LENGTH = 1K
    OS_FLASH            ( rx )  : ORIGIN = 0x08000400, LENGTH = 5K
    OS_RAM              ( rwx ) : ORIGIN = 0x20000000, LENGTH = 2K
}

SECTIONS{
    . = ORIGIN( CONFIG_FLASH );

    .config_flash : {
        . = ALIGN( 4 );
        __config_start = .;                           /*Pointer to start of programmed config flash*/
        KEEP( *( .vector_table ))
        KEEP( *( .schedule_table ))
        KEEP( *( .task_table))
        __config_end = .;                             /*Pointer to end of programmed config flash*/
    } > CONFIG_FLASH

    . = ORIGIN( OS_FLASH );

    .text : {
        . = ALIGN( 4 );
        __os_start = .;
        *( .text* )
        *( .rodata* )
        . = ALIGN( 4 );
        __os_end = .;
    } > OS_FLASH

    .data : AT( ADDR( .text) + SIZEOF( .text )){    /*AT tells the linker where in flash to put .data (at the end of .text).  Otherwise initial variable values would be lost*/
        . = ALIGN( 4 );
        *( .data* )
        . = ALIGN( 4 );
    } > OS_RAM

    .bss : {
        *( .bss* )
        *( COMMON )
    } > OS_RAM
}
