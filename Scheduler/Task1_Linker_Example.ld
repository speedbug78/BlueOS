
ENTRY (main)

/**
 * Task1 Memory Addresses
**/
/*Task1 Flash address*/
__Flash_Bottom  = 0x08001400;
__Flash_Top     = 0x08003FFF;
__Flash_Length  = 0x02800;

/*Task1 RAM address*/
__RAM_Bottom    = 0x20000800;
__RAM_Top       = 0x200027FF;
__RAM_Length    = 0x2000;

/*Stack location in RAM*/
__StackTop      = 0x200027FF;

MEMORY{
    TASK1_FLASH         ( rx )  : ORIGIN = 0x08001400, LENGTH = 10K
    TASK1_RAM           ( rwx ) : ORIGIN = 0x20000800, LENGTH = 8K
}

SECTIONS{
    . = ORIGIN( TASK1_FLASH );

    .text : {
        . = ALIGN( 4 );
        __task1_start = .;
        *( .text* )
        *( .rodata* )
        . = ALIGN( 4 );
        __task1_end = .;
    } > TASK1_FLASH

    .data : AT( ADDR( .text) + SIZEOF( .text )){    /*AT tells the linker where in flash to put .data (at the end of .text).  Otherwise initial variable values would be lost*/
        . = ALIGN( 4 );
        *( .data* )
        . = ALIGN( 4 );
    } > TASK1_RAM

    .bss : {
        *( .bss* )
        *( COMMON )
    } > TASK1_RAM
}
