#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>

//****************************************************
int main (int argc, char* argv[] ) {
	//******** Handler input data ********
	if ( argc == 1 ) {
		fprintf ( stderr, "No arguments.\n" );
		exit(EXIT_FAILURE);
	}
	execvp(argv[1], &argv[1]);
	if ( errno == ENOENT ) 
		fprintf ( stderr, "File not exist.\n" );
	exit(EXIT_FAILURE);
}
