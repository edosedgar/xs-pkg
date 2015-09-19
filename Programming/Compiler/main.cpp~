#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Compiler.h"
#include "unistd.h"

//#define BUG 

int main ( int argc, char* argv[] )
{
    char** SourceCode = NULL;
    char* FileBuffer = NULL;
	char* FileName = NULL;
	//********************************************************************************
	int opt = 0;
	int flag = 0;
	opterr = 0;
	while ( ( opt = getopt(argc, argv, "i:h" ) ) != -1 ) 
	{
		switch ( opt ) 
		{
		case 'h':
		{
			printf ("\n\n****************** Translator v2 help: ********************\n");
			printf ("*                                                         *\n");                               
			printf ("* -i ---> Source .asm file path                           *\n");
			printf ("*                                                         *\n");
			printf ("\033[94m***********************************************************\n");
			printf ("*                                                         *\n");
			printf ("* Required program for working: none                      *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\n");
			printf ("\033[91m*                                                         *\n");
			printf ("* Program is written on C by EdOS at xx.xx.2014           *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\n\n\033[0m");
			exit(EXIT_SUCCESS);
			break;
		}
		case 'i': 
		{
			flag = 1;
			if (optarg == NULL )
			{
				fprintf(stderr, "\nPlease check path on source file\n\n");
				exit(EXIT_FAILURE);
			}
			SourceCode = CreateArrayOfStrings ( optarg, &FileBuffer );
			FileName = optarg;

        	if ( SourceCode == NULL && errno != 0 )
        	{
            	fprintf(stderr, "\nError opening source file.\n\n");
            	exit(EXIT_FAILURE);
        	};
			printf ( "\nCompiling...\n");
			break;
		}
		case '?':
		{
			fprintf (stderr, "\nUncorrect parameters. See help: \"Compiler -h\".\n\n");
			exit(EXIT_FAILURE);
		}
		}
	}
	if (flag == 0)
	{
		fprintf (stderr, "\nNo right parameters. See help.\n\n");
		exit(EXIT_FAILURE);
	}
	//********************************************************************************
    Command* CommandsTable = CreateCommandsTable ( );
    if ( CommandsTable == NULL && errno != 0 )
    {
        getchar ( );
        exit(EXIT_FAILURE);
    }
	//********************************************************************************
    uint32_t ProgramSize = 0;
    Label* LabelTable = CreateAndFillLabelTable ( SourceCode, CommandsTable, &ProgramSize );
    if ( LabelTable == NULL && errno != 0 )
    {
        getchar ( );
        exit(EXIT_FAILURE);
    }
	//********************************************************************************
	printf("Generating machine code...\n");
    MemoryType* MachineCode = CreateCodeMachine ( SourceCode, LabelTable, CommandsTable, ProgramSize );
    if ( MachineCode == NULL && errno != 0 )
    {
        getchar ( );
        exit(EXIT_FAILURE);
    }
	//********************************************************************************
#ifndef BUG
    if ( PrintBinFile ( MachineCode, FileName, ProgramSize ) != 0 )
#endif
    {
        getchar ( );
    }
	//*******************************************************************************
#ifdef BUG
	uint32_t i=10;
    while ( i!= ProgramSize)
    {
        printf( "%ld ", MachineCode[i] );
        i++;
    }
#endif
	//********************************************************************************
    printf( "Ready.\n\n" );

    free ( SourceCode );    SourceCode    = NULL;
    free ( FileBuffer );    FileBuffer    = NULL;
    free ( LabelTable );    LabelTable    = NULL;
    free ( CommandsTable ); CommandsTable = NULL;
    free ( MachineCode );   MachineCode   = NULL;
    exit(EXIT_SUCCESS);
}
