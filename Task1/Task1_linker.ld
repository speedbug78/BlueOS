
ENTRY (main)

/**
 * Processor Memory Addresses
**/
/*Task1 Flash address*/
__Flash_Bottom  = 0x08001400;
__Flash_Top     = 0x08003FFF;
__Flash_Length  = 0x2C00;

/*Total RAM address*/
__RAM_Bottom    = 0x20000800;
__RAM_Top       = 0x200027FF;
__RAM_Length    = 0x2000;

/*Stack location in RAM*/
__StackTop      = 0x20002800;

SECTIONS{
    .text __Flash_Bottom : {
        . = ALIGN( 4 );
        __task1_start = .;
        *( .task1* )
        *( .text* )
        *( .rodata* )
        . = ALIGN( 4 );
        __task1_end = .;
    }

    .data __RAM_Bottom : AT( ADDR( .text) + SIZEOF( .text )){    /*AT tells the linker where in flash to put .data (at the end of .text).  Otherwise initial variable values would be lost*/
        . = ALIGN( 4 );
        *( .data* )
        . = ALIGN( 4 );
    }

    .bss __RAM_Bottom : {
        *( .bss* )
        *( COMMON )
    }
}
