#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include "VirtualCPU.h"


//==============================================================
/*!
 *     ASSERT macro
 *
 *     \brief  This macro show error if condition is false \n
 *             This macro works if DEBUG defined!
 *     \param  Condition
 *     \return Abort program or do nothing.
 */
//==============================================================

#define DEBUG

#ifdef DEBUG
    #define ASSERT( cond )                       \
        if ( ! ( cond ) )                        \
        {                                        \
            printf ( "FAIL %s in %s, %s ( %d )", \
            #cond, __PRETTY_FUNCTION__,          \
            __FILE__, __LINE__ );                \
            abort();                             \
        }
#else
    #define ASSERT( cond ) ;
#endif

uint32_t MaxRAM = 4096;
uint32_t MaxSizeStack = 100;

/****************************************************************/
void StartProcessor ( CPU_t* Proccesor );
/****************************************************************/

int main ( int argc, char* argv[] )
{
	CPU_t* EmCPU = NULL;
	//**************************************************************************
	int opt = 0;
	int flag = 0;
	opterr = 0;
	while ( ( opt = getopt(argc, argv, "hi:m:s:" ) ) != -1 ) 
	{
		switch ( opt ) 
		{
		case 'h':
		{
			printf ("\n\n**************** VirtualMachine v1 help: ******************\n");
			printf ("*                                                         *\n");                               
			printf ("* -i ---> Source .bin file path                           *\n");
			printf ("*                                                         *\n");
			printf ("* -m ---> RAM size (default 4096 dwords)                  *\n");
			printf ("*                                                         *\n");
			printf ("* -s ---> Stack size (default 100)                        *\n");
			printf ("*                                                         *\n");
			printf ("\033[94m***********************************************************\n");
			printf ("*                                                         *\n");
			printf ("* Required program for working: none                      *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\n");
			printf ("\033[91m*                                                         *\n");
			printf ("* Program is written on C by EdOS at xx.xx.2014           *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\033[0m\n\n");
			exit(EXIT_SUCCESS);
			break;
		}
		case 's':
		case 'm': 
		{	
			flag = 2;
			char* endptr = NULL;
			long int Number = strtol(optarg, &endptr, 10);
			if ( optarg + strlen ( optarg ) != endptr ) {
				fprintf ( stderr, "\nUncorrect input data (found symbols in stack or RAM size).\n\n" );
				exit(EXIT_FAILURE);
			}
			if ( errno == ENOENT && ( Number == LONG_MAX || Number == LONG_MIN ) ) {
				fprintf ( stderr, "\nStack or RAM size long for me.\n");
				exit(EXIT_FAILURE);				
			}
			if ( Number < 1 ) {
				fprintf ( stderr, "\nUncorrect stack or RAM size (min: 1).\n");
				exit(EXIT_FAILURE);
			}
			if ( opt == 's' ) MaxSizeStack = Number;
			if ( opt == 'm' ) MaxRAM = Number;
			break;
		}
		case 'i': 
		{
			flag = 1;
			EmCPU = CPU_Create ( MaxSizeStack );
			if ( ReadProgramToRAM ( EmCPU, optarg ) != 0 )
			{
				if ( CPU_Errno == ENOTDIR ) 
					fprintf (stderr, "\nInput file not exist.\n\n");
				if ( CPU_Errno == EBADF )
					fprintf (stderr, "\nIncorrect signature in input file.\n\n");
				if ( CPU_Errno == ENOENT )
					fprintf (stderr, "\nSize of RAM less than program size.\n\n");
				if ( CPU_Destroy ( EmCPU ) != 0 ) CPU_Dump ( EmCPU );
				exit(EXIT_FAILURE);
			}
			break;
		}
		case '?':
		{
			fprintf (stderr, "\nUncorrect parameters. See help: \"Compiler -h\".\n\n");
			exit(EXIT_FAILURE);
		}
		}
	}
	if (flag == 0 || flag == 2)
	{
		(flag == 0) && fprintf (stderr, "\nNo right parameters. See help.\n\n");
		(flag == 2) && fprintf (stderr, "\nNo input path parameters. See help.\n\n");
		exit(EXIT_FAILURE);
	}
	//**************************************************************************
    StartProcessor ( EmCPU );
	//**********************************************************
    if ( CPU_Destroy ( EmCPU ) != 0 ) CPU_Dump ( EmCPU );
    return 0;
}
/****************************************************************/
/****************************************************************/
/****************************************************************/
void StartProcessor ( CPU_t* Proccesor )
{
	Proccesor->RAM += StartMachineCode;
    int ResultOperation = 0;
    while ( ResultOperation != END )
    {
        ResultOperation = CPU_DoCommand ( Proccesor, Proccesor->RAM[Proccesor->ProgramCounter] );
        if ( ResultOperation == -1 )
        {
            CPU_Dump ( Proccesor );
            ResultOperation = END;
        }
        Proccesor->ProgramCounter++;
    }
    Proccesor->RAM -= StartMachineCode;
}
/****************************************************************/
/****************************************************************/
/****************************************************************/
