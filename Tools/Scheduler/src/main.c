/* ===============================================================
 *
 *                          BlueOS Scheduler
 *
 * ===============================================================*/
 
#include <stdio.h> 

#define MAX_LINE_LEN		1024
#define MAX_ARG_LEN			255
#define MAX_LINE_ARGS		30

typedef unsigned short int uint16_t;
typedef unsigned long  int uint32_t;

uint16_t make_line_args( void );
void begin_files( void );
void parse_args( void );


static FILE* file_pointer;
static char line[MAX_LINE_LEN];
static char line_args[MAX_LINE_ARGS][MAX_ARG_LEN];
static uint32_t schedule_length;
static uint16_t num_tasks;
static uint16_t num_task_args;

int main( int argc, char *argv[] ){
	if(( file_pointer = fopen( "Input_File_Example.txt", "r" )) == NULL ){
		printf( "Input file doesn't exist\n" );
		return -1;
	}

	while( fgets( line, MAX_LINE_LEN, file_pointer )){
		uint16_t num_args, arg_index;
		if(( num_args = make_line_args() ) == 0 ){
			printf( "Error parsing input file\n" );
			return -1;
		}

		for( arg_index = 0; arg_index <= num_args; arg_index++ ){
			printf( "%s ", line_args[arg_index] );
			for( uint16_t c = 0; c < MAX_ARG_LEN; c++ ){
				line_args[arg_index][c] = 0;
			}
		}
		printf( "\n" );
	}
	
	fclose( file_pointer );
	return 0;
}

//
// TODO: Need to handle comments
//
uint16_t make_line_args( void ){
	uint16_t c, arg_num, arg_char_num;
	
	arg_num = 0;
	for( c = 0; c < MAX_LINE_LEN; c++ ){
		switch( line[c] ){
			
			//End of line
			case 0:
				return arg_num;
				
			//Argumnet seperators
			case ':':
			case ',':
				if( arg_num < MAX_LINE_ARGS ){
					arg_num++;
					arg_char_num = 0;
				}
				else{
					printf( "Error: Too many arguments in line!\n" );
					return 0;
				}
				break;
				
			//Skip these characters
			case ' ':
			case '\n':
			case '*':
			case '/':
				break;
				
			//Add a character to the argument
			default:
				line_args[arg_num][arg_char_num] = line[c];
				if( arg_char_num < MAX_ARG_LEN ){
					arg_char_num++;
				}
				else{
					printf( "Error: argument %u too long!\n", arg_num );
					return 0;
				}
		}
		line[c] = 0;
	}
	return arg_num;
}

void begin_files( void ){
}

void parse_args( void ){
}
