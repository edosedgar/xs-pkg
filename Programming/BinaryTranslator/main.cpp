#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ARM.h"
#include "Translator.h"
#include "VirtComMnem.h"

//The Virtual Machine to ARM translator

int main ( int argc, char* argv[] )
{	
	uint32_t* BinFile = NULL;
	uint32_t BinSize = 0;
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	int opt = 0;
	int flag = 0;
	opterr = 0;
	while ( ( opt = getopt(argc, argv, "i:h" ) ) != -1 ) 
	{
		switch ( opt ) 
		{
		case 'h':
		{
			printf ("\n\n************ ARM Binary Translator v1 help: ***************\n");
			printf ("*                                                         *\n");                               
			printf ("* -i ---> Source .bin file path.                          *\n");
			printf ("*                                                         *\n");
			printf ("* Attention! Output file always arm.bin in source dir.    *\n");
			printf ("*                                                         *\n");
			printf ("\033[94m***********************************************************\n");
			printf ("*                                                         *\n");
			printf ("* Required program for working: none                      *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\n");
			printf ("\033[91m*                                                         *\n");
			printf ("* Program is written on C by EdOS at xx.05.2015           *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\033[0m\n\n");
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
        	if ( ( BinFile = ReadBin ( optarg, &BinSize ) ) == NULL )
        	{
            	fprintf(stderr, "\nError opening source file.\n\n");
            	exit(EXIT_FAILURE);
        	};
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
	printf ( "\nVirtual bin size: %u bytes.\n", BinSize*4 + SignatureSize*4 );
	printf ( "Starting translation...\n");	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	if ( GenerateInterruptTable( "arm.bin" ) != 0 )
	{
		printf ( "Error with generating interrupt table!\n" );
		exit(EXIT_FAILURE);
	}
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	
	if ( ConvertLabel16bit ( BinFile ) != 0 )
	{
		printf ( "Error with converting label!\n" );			
		exit(EXIT_FAILURE);
	}
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	printf ( "Decoding...\n");	
	if ( DecodingAndGenerateBin ( BinFile, "arm.bin", BinSize ) != 0 )
	{
		printf ( "Error with decoding!\n" );
		exit(EXIT_FAILURE);
	}	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	printf ( "\nFinishing...Ready.\n\n");
	free ( BinFile );
	exit(EXIT_SUCCESS);
}
