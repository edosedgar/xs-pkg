#include "Translator.h"
#include <stdlib.h>
#include <stdio.h>
#include "ARMOpcodes.h"
#include "VirtSize.h"
#include "VirtComMnem.h"
#include "ARM.h"
#include <string.h>


//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
//_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_

uint32_t* ReadBin ( const char* Name, uint32_t* BinSize )
{
	if ( Name == NULL )
		return NULL;
	//##########################################################################
	FILE* BinFile = fopen ( Name, "rb" );

	if ( BinFile == NULL )
		return NULL;
	//##########################################################################
	uint32_t* Bin = ( uint32_t* ) calloc ( SignatureSize, sizeof ( uint32_t ) );

	if ( Bin == NULL ) 
		return NULL;
	//##########################################################################
	fread ( Bin, sizeof ( uint32_t ), SignatureSize, BinFile );
	
	fseek ( BinFile, 0, SEEK_SET );
	//##########################################################################
	uint32_t ProgramSize = Bin[SignatureSize - 1];
	*BinSize = ProgramSize;
	if ( Bin[0] != 'E' || Bin[1] != 'A' ||
		 Bin[2] != 'S' || Bin[3] != 'M' ||
		 Bin[4] != 'D' || Bin[5] != 'V' ||
		 Bin[6] != '1' || Bin[7] != '.' ||
		 Bin[8] != '1' )
		return NULL;
	//##########################################################################
	free ( Bin );	
	Bin = ( uint32_t* ) calloc ( SignatureSize + ProgramSize, \
								 sizeof ( uint32_t ) );				 
	if ( Bin == NULL )
		return NULL;
	fread ( Bin, sizeof ( uint32_t ), ProgramSize + SignatureSize, BinFile );
	//##########################################################################
	uint32_t* Bin32 = ( uint32_t* ) calloc ( ProgramSize, sizeof ( uint32_t ) );
	if ( Bin32 == NULL )
		return NULL;
	for ( uint32_t Count = 0; Count < ProgramSize; Count++ )
		Bin32[Count] = Bin[Count + SignatureSize];
	//##########################################################################	
	free ( Bin );	
	fclose ( BinFile );		
	return Bin32;
}	

//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
//_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_

int ConvertLabel16bit ( uint32_t* BinFile )
{
	uint32_t Counter = 0; 
	while ( BinFile[Counter] != END )
	{
		if ( BinFile[Counter] == JA  || BinFile[Counter] == JB  ||
			 BinFile[Counter] == JE  || BinFile[Counter] == JAE ||
			 BinFile[Counter] == JBE || BinFile[Counter] == JP  || 
			 BinFile[Counter] == CALL )
		{
			uint32_t Offset = 0;
			uint32_t CurrentAddress = 0;
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			if ( BinFile[Counter+1] <= Counter )
			{
				CurrentAddress = BinFile[Counter+1];
				while ( CurrentAddress != Counter )
				{
					Offset += (strlen(OpcodesARM[BinFile[CurrentAddress]])/16);
					CurrentAddress += VirtComSize[BinFile[CurrentAddress]];
				}
				BinFile[Counter+1] = ConvertOffsetToOpcode ( Offset, Lower, \
															 BinFile[Counter] );
			} else
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			{
				CurrentAddress = Counter + VirtComSize[BinFile[Counter]];
				while ( CurrentAddress != BinFile[Counter+1] )
				{
					Offset += (strlen(OpcodesARM[BinFile[CurrentAddress]])/16);
					CurrentAddress += VirtComSize[BinFile[CurrentAddress]];
				}
				BinFile[Counter+1] = ConvertOffsetToOpcode ( Offset, Higher, \
															 BinFile[Counter] );												 
			}
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		}
		Counter += VirtComSize[BinFile[Counter]];
	}
	return 0;
}

//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
//_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
