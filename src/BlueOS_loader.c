#include "BlueOS_loader.h"
#include "BlueOS_registers.h"
#include "BlueOS_debug_messages.h"

static void loader_Startup( void );

void loader( void ){
    send_Str( BOOTLOAD_BANNER );                                   //Display the loader banner and tell the user we are erasing flash

    //Configure processor for internal flash writing
    loader_Startup();

    //Erase Flash
    erase_Flash();                                          //If erase_Flash doesn't return 0, there is an error.

    debug_Hex_Load();                                               //Tell user we are ready to load a HEX file.

    get_Ihex();                                            //Receive HEX file and write to flash.                                                     //Return no errors
}

static void loader_Startup( void ){
    //Save clock state

    //Configure Clock to 8MHz HSI
    reg( RCC_CR )      |= (uint32_t)0x00000001;                 //Reset RCC clock to default HSI (don't change HSI trim)
    reg( RCC_CFGR )    &= (uint32_t)0xF0FF0000;                 //Set HSI as clock, not divided for AHB, APB1 and APB2.  /2 for ADC.  No clock output
    reg( RCC_CR )      &= (uint32_t)0xFEF6FFFF;                 //Turn off PLL, HSE and Clock security
    reg( RCC_CR )      &= (uint32_t)0xFFFBFFFF;                 //HSE not bypassed
    reg( RCC_CFGR )    &= (uint32_t)0xFF80FFFF;                 //Reset PLL and USB OTG values
    reg( RCC_CIR )      = (uint32_t)0x009F0000;                 //Clear interrupt flags for clock security and PLL, HSE, HSI, LSE and LSI ready

    //Configure Flash
    reg( FLASH_ACR )    = (uint32_t)0x00000030;                 //18? Reset state of flash. 0 wait states (HSI is 8Mhz), prefetch buffer enabled.
}

//Erase all of the user flash (non-bootloader flash) in the processor.
void erase_Flash( void ){
    uint32_t p, a;
    unlock_Flash();
    wait_Flash();
    reg( FLASH_CR ) &= (uint16_t)0xFFFE;                            //Clear programming bit.  Somehow it is set when we get here
    for( p = link_val( __User_F_Bottom ); p < link_val( __User_F_Top ); p += 0x00000400 ){  //Increment page
            reg( FLASH_CR ) |= (uint16_t)0x0002;                    //Set page erase bit
            reg( FLASH_ADR ) = p;                                   //Load page address to be erased
            reg( FLASH_CR ) |= (uint16_t)0x0040;                    //Set flash start bit
            wait_Flash();                                           //Wait for page to erase
            for( a = 0; a < 0x0400; a += 0x04 ){                    //Loop through each word of the page
                if( reg( p + a ) != (uint32_t)0xFFFFFFFF ){         //Check that the word was erased
                    debug_Erase_Error();                                        //Tell the user there was a flash error.
                    while(1);//return p + a;                                   //If a word isn't erased, return page address
                }
            }
            send_Char( '.' );                                       //Print Progress "."
    }
    reg( FLASH_CR ) &= (uint16_t)0xFFFD;                            //Clear the page erase bit
    lock_Flash();
}

//Unlock the internal flash
void unlock_Flash( void ){
    if(( reg( FLASH_CR ) & 0x0080) == (uint16_t)0x0080 ){           //Check that flash is locked
        reg( FLASH_KEY ) = (uint32_t)0x45670123;                    //Write unlock key 1
        reg( FLASH_KEY ) = (uint32_t)0xCDEF89AB;                    //Write unlock key 2
    }
    while(( reg( FLASH_CR ) & 0x0080) == (uint16_t)0x0080 ){};      //Wait for flash to be unlocked
}

//Lock the internal flash
void lock_Flash( void ){
    if(( reg( FLASH_CR ) & 0x0080) != (uint16_t)0x0080 ){           //Check that flash is unlocked
        reg( FLASH_CR ) |= 0x0080;
    }
}

//Wait for flash operation to complete
void wait_Flash( void ){
    __asm__("nop");                                                 //This prevents errata 2.12 in STM32F103
    while(( reg( FLASH_SR ) & 0x0001 ) == (uint16_t)0x0001 ){};     //Loop until flash busy bit is cleared
}


/*
Receives a byte from the USART and adds it to a running sum,
to use for the checksum at the end of the .hex line.
*/
/*uint8_t data_To_Check( void ){
    uint8_t temp_data;
    temp_data = char_Byte();
    //Line_Datasum += (int8_t)temp_data;
    return temp_data;
}*/


/*
Receive an Intel hex file, and program it to user flash space
*/
void get_Ihex( void ){
    uint8_t record, line_datasum, the_end = 0;
    uint32_t flash_address = 0x00000000, branch_address;

//In the SPL this is only done once for multiple writes
    unlock_Flash();
    reg( FLASH_CR ) |= 0x0001;                                          //Set programming bit to 1

    while( !the_end ){                                                       //Read and process a line of the HEX file
        uint8_t data[259], checksum;
        while( receive_Char() != 0x3A ){};                          //Wait for beginning of line
        flash_address &= 0xFFFF0000;                                //Clear low halfword of flash_address (for this line)
        data[0] = char_Byte();                                       //Receive data size in this line, and reset line-by-line checksum
        line_datasum = 0;
        for( uint16_t i = 1; i <= data[0] + 3; i++ ){
            data[i] = char_Byte();
            line_datasum += data[i];
        }

        send_Char( '*' );

        checksum = char_Byte();
        if( !( checksum + line_datasum )){                //Verify line checksum
            debug_Invalid_Checksum();
            send_Byte( line_datasum );
            send_Char( ' ' );
            send_Byte( checksum );
            the_end = 4;                                    //Checksum Error, return address of error
        }

        flash_address |= data[1] << 8;                       //Receive high byte of data address for this line

        flash_address |= data[2];                            //Receive low byte of data address for this line.

        record = data[3];                                    //Receive the record byte for this line.

        switch( record ){
            case 0x00:                                               //Regular data to write into flash
                for( uint8_t i = 0; i < data[0]; i += 2){
                    uint16_t halfword = 0x0000;

                    halfword += data[i+4];

                    halfword |= ( data[i+5] << 8 ) & 0xFF00;     //2nd Received byte is the high byte

                    write_Flash( halfword, flash_address );

                    flash_address += 2;                              //Increment flash write address by 2 bytes (1 halfword).
                }
                break;
            case 0x01:                                               //End of file
                the_end = 2;//end of file
                break;
            case 0x04:                                               //Extended linear address (high address for data write location)
                flash_address &= 0x0000FFFF;                         //Clear top halfword of flash_address

                flash_address |= data[4] << 24;              //Set high byte of flash_address

                flash_address |= data[5] << 16;              //Set 3rd byte of flash_address
                break;
            case 0x05:                                               //Start linear address (user code branch location?)
                branch_address  = data[4] << 24;             //Set high byte of branch_address

                branch_address |= data[5] << 16;             //Set 3rd byte of branch_address

                branch_address |= data[6] << 8;              //Set 2nd byte of branch_address

                branch_address |= data[7];                   //Set low byte of branch_address
                break;

            default:                                                 //Error, we have an invalid record type
                debug_Invalid_Record();
                the_end = 3;                             //Return the invalid record
                break;
        }
    }
    send_Char( '^' );
    reg( FLASH_CR ) &= 0x1FFE;                                          //Reset the programming bit
    lock_Flash();
    if( the_end != 2 ){
        while(1);
    }
}

//Write data to internal flash
inline void write_Flash ( uint16_t data, uint32_t address ){
    uint32_t verify = 0x00000000;
    if(( address < link_val( __User_F_Bottom )) || ( address > link_val( __User_F_Top ))){    //Check if write address is out of range.
        debug_Invalid_Address();
        while(1);
    }
    wait_Flash();
    *(volatile uint16_t*)address = data;

    verify = *(volatile uint16_t*)address;                              //Verify that data was written properly
    if( verify != data){
        debug_Write_Verify_Error();
        while(1);
    }
}
