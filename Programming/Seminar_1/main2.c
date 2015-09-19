#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

//****************************************************
int main ( int argc, char* argv[] ) {
	long int Number = 0;
	//******** Handler input data ********
	switch (argc) {
		case 1:	{
			fprintf ( stderr, "No argumnets\n" );
			exit(EXIT_FAILURE);
			break;
		}
		case 2: {
			char* endptr = NULL;	
			Number = strtol ( argv[1], &endptr, 10 );
			if ( argv[1] + strlen ( argv[1] ) != endptr ) {
				fprintf ( stderr, "Uncorrect input data (found symbols).\n" );
				exit( EXIT_FAILURE );
			}
			if ( errno == 34 && ( Number == LONG_MAX || Number == LONG_MIN ) ) {
				fprintf ( stderr, "Too long for me.\n");
				exit( EXIT_FAILURE);				
			}
			if ( Number < 1 ) {
				fprintf ( stderr, "Uncorrect interval (only from 1).\n");
				exit(EXIT_FAILURE);
			}
			break;
		}
		default: {
			fprintf ( stderr, "Many arguments for me.\n" );
			exit(EXIT_FAILURE);
		}
	}	
	//************************************
	int TempVar = 1;
	FILE* OutFile = fopen ("text.txt","w");
	int status = 0;
	while ( TempVar != Number+1 ) {
		if ( fork() == 0 ) {
			fprintf(OutFile, "%d %d %d\n", TempVar, getpid(), getppid());
			exit(EXIT_SUCCESS);
		}
		//wait(&status);
		TempVar++;
	}
	fclose(OutFile);
	//*************************************/
	exit(EXIT_SUCCESS);
}
