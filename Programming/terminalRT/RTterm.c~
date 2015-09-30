#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <fcntl.h>

#define RX 1
#define TX 2

const char* pCommonFIFO = "/tmp/pipeCommon.pipe";
const int BufferSize = 4096;

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
	//Get my id, convert to string and print out own info
	char* Buffer = (char*) calloc ( BufferSize, sizeof(*Buffer) );
	int ReceiverPID = getpid();
	sprintf( Buffer, "%d", ReceiverPID );
	fprintf( stderr, "Trying to find a transmitter(MyPID: %s).\n", Buffer );
	//Trying to find FIFO channel (if fifo doesn't exist then wait)
	int CommonFIFO = open( pCommonFIFO, O_WRONLY );
	//Send my id to some transmitter
	write( CommonFIFO, Buffer, BufferSize );
	//Generate data pipe path
	char* pDataFIFO = (char*) calloc ( 255, sizeof(*pDataFIFO) );
	sprintf( pDataFIFO, "/tmp/pipe%d.pipe", ReceiverPID );
	int DataFIFO = -1;
	free(pDataFIFO);
	//Trying to find FIFO and to open it
	while (( DataFIFO = open( pDataFIFO,O_RDONLY ) )== -1 )
		usleep (100);
	//Try read from data fifo
	read( DataFIFO, Buffer, BufferSize );
	//Tell that connection established and read transmitter PID
	int TransmitterPID = 0;
	long int FileLength = 0;
	sscanf( Buffer, "%d %ld", &TransmitterPID, &FileLength );
	fprintf(stderr, "Connection established with transmitter(PID: %d).\n", \
				   	TransmitterPID);
	fprintf(stderr, "File size %ldB.\n", FileLength) ;
	//Start receive
	int csize = 0;
	long int SumSize = 0;
	while ( ( csize = read( DataFIFO, Buffer, BufferSize ) ) > 0 ) {
		write(STDOUT_FILENO, Buffer, csize );
		SumSize += csize;
	}
	//Check size
	if ( SumSize != FileLength )
		fprintf( stderr, "Transmitter terminated.\n" );
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
	FILE* FileD= fopen(FileName, "r");
	if (FileD == NULL ) {
		fprintf(stderr, "File doesn't exist.\n");
		return -1;	
	}
	//Create buffer for other workes
	char* Buffer = (char*) calloc ( BufferSize, sizeof(*Buffer) );
	//Read my PID and print out
	int TransmitterPID = getpid();	
	fprintf( stderr, "Trying to find a receiver(MyPID: %d).\n", TransmitterPID);
	//Create common fifo or use exist
	mkfifo(pCommonFIFO, 0666);
	int CommonFIFO = open ( pCommonFIFO, O_RDONLY );
	//Wait PID from some receiver
	read (CommonFIFO, Buffer, BufferSize );
	fprintf(stderr, "Connection established with receiver(PID: %s).\n", Buffer);
	int ReceiverPID = 0;
	sscanf ( Buffer, "%d", &ReceiverPID );
	//Generate path for FIFO, that redirection all data to receiver
	char* pDataFIFO = (char*) calloc ( 255, sizeof(*pDataFIFO) );
	sprintf( pDataFIFO, "/tmp/pipe%d.pipe", ReceiverPID );
	mkfifo( pDataFIFO, 0666 );
	int DataFIFO =  open ( pDataFIFO, O_WRONLY );
	free(pDataFIFO);
	//Compute file size
	fseek ( FileD, 0, SEEK_END );
	long int FileLength = ftell ( FileD );
	rewind ( FileD );
	//Send information packet to receiver
	sprintf( Buffer, "%d %ld", TransmitterPID, FileLength );
	write( DataFIFO, Buffer, BufferSize );
	//Send file
	int CurrentBuf = 0;
	if ( FileLength >= BufferSize ) {
		CurrentBuf = BufferSize;
		FileLength -= CurrentBuf;
	} else
		CurrentBuf = FileLength;

	while ( fread( Buffer, CurrentBuf, sizeof(char), FileD ) > 0 ) {
		write(DataFIFO, Buffer, CurrentBuf);
		//sleep(1);
		//fprintf(stderr, "ok.\n");
		if ( FileLength >= BufferSize ) {
			CurrentBuf = BufferSize;
			FileLength -= CurrentBuf;
		} else
			CurrentBuf = FileLength;	 
	}
	//Free decriptors and memory
	free( Buffer );
	close( CommonFIFO );
	close( DataFIFO );
	fclose( FileD );
	return 0;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
