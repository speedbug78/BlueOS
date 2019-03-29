
ENTRY (reset_hdlr)

/*Total Flash address pointers*/
__Flash_Bottom  = 0x08000000;
__Flash_Top     = 0x08020000;
__Flash_Length  = 0x20000;

/*Bootloader Flash address pointers*/
__Boot_F_Bottom = 0x08000000;
__Boot_F_Top    = 0x080007FF;
__Boot_F_Length = 0x800;

/*User Program Flash address pointers*/
__User_F_Bottom = 0x08000800;
__User_F_Top    = 0x08020000;
__User_F_Length = 0x1F800;

/*Total RAM address pointers*/
__RAM_Bottom    = 0x20000000;
__RAM_Top       = 0x20005000;
__RAM_Length    = 0x5000;

/*Stack location in RAM*/
__StackTop      = 0x20005000;

MEMORY{
    BOOTLOADER_FLASH    ( rx )  : ORIGIN = __Boot_F_Bottom, LENGTH = 2K
    USER_FLASH          ( rx )  : ORIGIN = __User_F_Bottom, LENGTH = 126K
    INTERNAL_RAM        ( rwx ) : ORIGIN = __RAM_Bottom   , LENGTH = 20K
}

SECTIONS{
    . = ORIGIN( BOOTLOADER_FLASH );

    .text : {
        . = ALIGN( 4 );
        __boot_start = .;                           /*Pointer to start of programmed bootloader flash*/
        KEEP( *( .vector_table ))
        *( .text* )
        *( .rodata* )
        . = ALIGN( 4 );
        __boot_end = .;                             /*Pointer to end of programmed bootloader flash*/
    } > BOOTLOADER_FLASH

    .user : {
        . = ALIGN( 4 );
        __user_start = .;                           /*Pointer to start of programmed user flash*/
        *( .user* )
        __user_end = .;                             /*Pointer to end of programmed user flash*/
    } > USER_FLASH

    .data : AT( ADDR( .text) + SIZEOF( .text )){    /*AT tells the linker where in flash to put .data.  Otherwise initial variable values would be lost*/
        . = ALIGN( 4 );
        *( .data* )
        . = ALIGN( 4 );
    } > INTERNAL_RAM

    .bss : {
        *( .bss* )
        *( COMMON )
    } > INTERNAL_RAM
}
