
ENTRY (main)

/**
 * Task2 Memory Addresses
**/
/*Task2 Flash address*/
__Flash_Bottom  = 0x08004000;
__Flash_Top     = 0x0801FFFF;
__Flash_Length  = 0x1C000;

/*Task2 RAM address*/
__RAM_Bottom    = 0x20002800;
__RAM_Top       = 0x20004FFF;
__RAM_Length    = 0x2800;

/*Stack location in RAM*/
__StackTop      = 0x20004FFF;

MEMORY{
    TASK2_FLASH         ( rx )  : ORIGIN = 0x08004000, LENGTH = 112K
    TASK2_RAM           ( rwx ) : ORIGIN = 0x20002800, LENGTH = 10K
}

SECTIONS{
    . = ORIGIN( TASK2_FLASH );

    .text : {
        . = ALIGN( 4 );
        __task2_start = .;
        *( .text* )
        *( .rodata* )
        . = ALIGN( 4 );
        __task2_end = .;
    } > TASK2_FLASH

    .data : AT( ADDR( .text) + SIZEOF( .text )){    /*AT tells the linker where in flash to put .data (at the end of .text).  Otherwise initial variable values would be lost*/
        . = ALIGN( 4 );
        *( .data* )
        . = ALIGN( 4 );
    } > TASK2_RAM

    .bss : {
        *( .bss* )
        *( COMMON )
    } > TASK2_RAM
}
