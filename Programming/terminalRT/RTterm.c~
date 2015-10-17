#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <fcntl.h>
#include <limits.h>

#define RX 1
#define TX 2

const char* pCommonFIFO = "/tmp/pipeCommon.pipe";
const int BufferSize = PIPE_BUF;

int OptHandler (int argc, char* argv[], char** FileName);
int ReceiverModule(void);
int TransmitterModule(char* FileName);

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int main (int argc, char* argv[]) {

	char* FileName = NULL;
	int WorkMode = 0;

	if ( (WorkMode = OptHandler(argc, argv, &FileName)) == -1 ) 
		exit(EXIT_FAILURE);

	if ( WorkMode == RX )
		if (ReceiverModule() == -1 )
			exit(EXIT_FAILURE);

	if ( WorkMode == TX ) 
		if (TransmitterModule(FileName) == -1 ) 
			exit(EXIT_FAILURE);
	
	exit(EXIT_SUCCESS);
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int OptHandler (int argc, char* argv[], char** FileName) {
	int opt = 0;
	opterr = 0;
	int WorkMode = 0;
	while ( (opt = getopt(argc, argv, "i::rth")) != -1 ) 
	{
		switch ( opt ) 
		{
		case 'h': {
			printf ("\n\n********** ReceiveTransmitterTerminal v1 help: ************\n");
			printf ("*                                                         *\n");                               
			printf ("* rtterm [-r|-t] [-i /filepath]                           *\n");
			printf ("*                                                         *\n");                               
			printf ("* -r ---> Receive mode (to stdout)                        *\n");
			printf ("*                                                         *\n");
			printf ("* -t ---> Transmitter mode                                *\n");
			printf ("*                                                         *\n");
			printf ("* -i ---> Transmitted content (file PATH)                 *\n");
			printf ("*                                                         *\n");
			printf ("\033[94m***********************************************************\n");
			printf ("*                                                         *\n");
			printf ("* Required program for working: none                      *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\n");
			printf ("\033[91m*                                                         *\n");
			printf ("* Program is written on C by EdOS at 27.09.2015           *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\033[0m\n\n");
			exit(EXIT_SUCCESS);
			break;
		}
		case 'r': { 
			if ( WorkMode == 0 ) 
				WorkMode = RX;
			else {
				fprintf(stderr, "Please, select one working mode.\n");
			   	return -1;	
			}
			break;
		}
		case 't': { 
			if ( WorkMode == 0 ) 
				WorkMode = TX;
			else {
				fprintf(stderr, "Please, select one working mode.\n");
			   	return -1;	
			}
			break;
		}
		case 'i': {
			if ( argv[optind] == NULL || strlen(argv[optind]) == 0 || \
				 argv[optind][0] == '-' ) {
				fprintf(stderr, "Missing filepath after '-i'.\n" );	
				return -1;
			} else 
				*FileName = argv[optind];			
			break;
		}
		default: {
			fprintf(stderr, "Unrecognized command line option.\n");
			return -1;
		}			
		}
	}
	if ( WorkMode == 0 ) {
		fprintf(stderr, "Don't set a work mode. Use help for info.\n");
		return -1;
	}
	if ( WorkMode == TX && *FileName == NULL ) {
		fprintf(stderr, "Mode transmitter, but no input file.\n");
		return -1;
	}
	if ( WorkMode == RX && *FileName != NULL ) {
		fprintf(stderr, "In receiver mode path on file miss.\n");
	}
	return WorkMode;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int ReceiverModule(void){
	//Create common fifo or use exist
	mkfifo( pCommonFIFO, 0666 );
	//Generate DataPipe path, create buffers and other
	char* Buffer = (char*) calloc ( BufferSize, sizeof(*Buffer) );
	char* pDataFIFO = (char*) calloc ( 255, sizeof(*pDataFIFO) );
	int ReceiverPID = getpid();
	sprintf( Buffer, "%d", ReceiverPID );
	sprintf( pDataFIFO, "/tmp/pipe%d.pipe", ReceiverPID );
	fprintf( stderr, "Trying to find a transmitter(MyPID: %s).\n", Buffer );
	//Trying to find FIFO channel (if fifo doesn't exist then wait)
	int CommonFIFO = open( pCommonFIFO, O_WRONLY );
	//Send my id to some transmitter
	write( CommonFIFO, Buffer, BufferSize );
	//Create, trying to find FIFO and to open it
	mkfifo ( pDataFIFO, 0666 );
	int DataFIFO = open( pDataFIFO,O_RDONLY );
	free( pDataFIFO );
	//Try read from data fifo
	read( DataFIFO, Buffer, BufferSize );
	//Tell that connection established and read transmitter PID
	int TransmitterPID = 0;
	sscanf( Buffer, "%d", &TransmitterPID );
	fprintf(stderr, "Connection established with transmitter(PID: %d).\n", \
				   	TransmitterPID);
	//Start receive
	int csize = 0;
	while ( ( csize = read( DataFIFO, Buffer, BufferSize ) ) > 0 ) {
        //fprintf(stderr,"Block %dB received.\n", csize);
        //sleep(1)
		write(STDOUT_FILENO, Buffer, csize );
	}
	//Free memory and close descriptors
	free(Buffer);
	close(CommonFIFO);
	close(DataFIFO);
	return 0;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int TransmitterModule(char* FileName){
	//Open input file
	int FileD= open(FileName, O_RDONLY);
	if (FileD == -1 ) {
		fprintf(stderr, "File doesn't exist.\n");
		return -1;	
	}
	//Create buffers and read myPID and print out and create fifo
	char* Buffer = (char*) calloc ( BufferSize, sizeof(*Buffer) );
	char* pDataFIFO = (char*) calloc ( 255, sizeof(*pDataFIFO) );
	mkfifo(pCommonFIFO, 0666);
	int TransmitterPID = getpid();	
	fprintf( stderr, "Trying to find a receiver(MyPID: %d).\n", TransmitterPID);
    //Try to open common FIFO
	int CommonFIFO = open ( pCommonFIFO, O_RDONLY );
	//Read PID from some receiver
	if ( read (CommonFIFO, Buffer, BufferSize ) == 0 ) {
		//fprintf ( stderr, "Copy of program already started.\n" );
		return -1;
	}
    //close( CommonFIFO ); //!!!
	fprintf(stderr, "Connection established with receiver(PID: %s).\n", Buffer);
	int ReceiverPID = 0;
	sscanf ( Buffer, "%d", &ReceiverPID );
	//Generate path for FIFO, that redirection all data to receiver
	sprintf( pDataFIFO, "/tmp/pipe%d.pipe", ReceiverPID );
	mkfifo( pDataFIFO, 0666 );
	int DataFIFO =  open ( pDataFIFO, O_WRONLY );
	free(pDataFIFO);
	//Send information packet to receiver
	sprintf( Buffer, "%d", TransmitterPID );
	write( DataFIFO, Buffer, BufferSize );
	//Send file
    int csize = 0;
	while ( ( csize = read( FileD, Buffer, BufferSize ) ) > 0 ) {
        //fprintf(stderr, "Block %dB transmitted.\n", csize);
        //sleep(1);
		write(DataFIFO, Buffer, csize);
	}
	//Free decriptors and memory
	free( Buffer );
	close( CommonFIFO );
	close( DataFIFO );
	close( FileD );
	return 0;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
