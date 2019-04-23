
ENTRY (main)

/**
 * Processor Memory Addresses
**/
/*Task2 Flash address*/
__Flash_Bottom  = 0x08004000;
__Flash_Top     = 0x08020000;
__Flash_Length  = 0x01C000;

/*Total RAM address*/
__RAM_Bottom    = 0x20002800;
__RAM_Top       = 0x20005000;
__RAM_Length    = 0x2800;

/*Stack location in RAM*/
__StackTop      = 0x20005000;

SECTIONS{
    .text __Flash_Bottom : {
        . = ALIGN( 4 );
        __task2_start = .;
        *( .task2* )
        *( .text* )
        *( .rodata* )
        . = ALIGN( 4 );
        __task2_end = .;
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
