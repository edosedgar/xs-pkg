#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "ARMInterrupt.h"
#include "Translator.h"
#include "ARM.h"
#include "VirtComMnem.h"
#include "string.h"


//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
//_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_

int GenerateInterruptTable ( const char* File )
{
	if ( File == NULL )	
		return -1;

	FILE* BinFile = fopen ( File, "wb" );

	if ( BinFile == NULL )
		return -1;

	fwrite ( InterruptTableARM, sizeof ( uint16_t ), InterTableSize, BinFile );

	fclose ( BinFile );
	return 0;
}
	
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
//_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_

int DecodingAndGenerateBin ( uint32_t* BinFile, const char* File, \
							 uint32_t Size )
{
	uint32_t CurrentCommand = 0;
	uint16_t* ARMBin = ( uint16_t* ) calloc ( Size*MaxLengthCommand_word, \
											 sizeof ( uint16_t ) );
	uint32_t ARMBinSize = 0;
	
	while ( BinFile[CurrentCommand] != END )
	{
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		ARMBinSize += WriteOpcodeInARMBin ( (ARMBin+ARMBinSize), \
											(BinFile+CurrentCommand) );
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		CurrentCommand += VirtComSize[BinFile[CurrentCommand]];
	} 
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	// Out ARM Bin 
	FILE* ARM_F_B = fopen ( File, "ab" );
	fwrite ( ARMBin, sizeof ( uint16_t ), ARMBinSize, ARM_F_B );
	//----- Out ARM bin information -----------
	printf ( "ARM bin size: %u bytes. ", (ARMBinSize + InterTableSize)*2 );
	double Percent = (double)((ARMBinSize + InterTableSize)*2 )/ARMFlashSize_B;
	printf ( "%g%% memory usage.", Percent*100 );
	//-----------------------------------------
	fclose ( ARM_F_B );
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	free ( ARMBin );
	return 0;
}

//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
//_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_

uint32_t ConvertOffsetToOpcode ( uint32_t Offset, uint8_t Direction,  \
								 uint8_t Command )
{
	uint32_t RetVal = 0;
	switch ( Command )
	{
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	case CALL:
	{
		/*int16_t ARMOffset=0;
		// Calculate relative address
		if ( Direction == Lower )
			ARMOffset = ( 0xFFC0 - ( ( ( uint16_t ) Offset ) << 5) );
		else
			ARMOffset = ( 0xFFE0 + ( ( ( uint16_t ) Offset ) << 5) );
		// Convert int16_t to int11_t
		RetVal = ( ( uint16_t ) ARMOffset ) >> 5 ;
		break;*/
		int32_t ARMOffset = 0;
		// Calculate relative address
		if ( Direction == Lower )
			ARMOffset = ( 0xFFFFFD00 - ( Offset << 8 ));
		else
			ARMOffset = ( 0x00000200 + ( Offset << 8 ));
		// Convert int32_t to int24_t
		RetVal = ( uint32_t ) ARMOffset >> 8;
		// I1, I2 - bits 22, 21, before convert
		// S - 23 bits
		// J1 = (~I1) xor S ( after convert )
		// J2 = (~I2) xor S ( after convert )
		RetVal = (RetVal&0xFFBFFFFF) | \
				 ( ((~RetVal)&0x00400000) xor ((RetVal&0x00800000)>>1) );
		RetVal = (RetVal&0xFFDFFFFF) | \
				 ( ((~RetVal)&0x00200000) xor ((RetVal&0x00800000)>>2) );
		break;
	}
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	case JP:
	{	
		int32_t ARMOffset = 0;
		// Calculate relative address
		if ( Direction == Lower )
			ARMOffset = ( 0xFFFFFE00 - ( Offset << 8 ));
		else
			ARMOffset = ( 0x00000000 + ( Offset << 8 ));
		// Convert int32_t to int24_t
		RetVal = ( uint32_t ) ARMOffset >> 8;
		// I1, I2 - bits 22, 21, before convert
		// S - 23 bits
		// J1 = (~I1) xor S ( after convert )
		// J2 = (~I2) xor S ( after convert )
		RetVal = (RetVal&0xFFBFFFFF) | \
				 ( ((~RetVal)&0x00400000) xor ((RetVal&0x00800000)>>1) );
		RetVal = (RetVal&0xFFDFFFFF) | \
				 ( ((~RetVal)&0x00200000) xor ((RetVal&0x00800000)>>2) );
		break;
	}
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	case JA...JNZ:
	{
		int8_t ARMOffset = 0;
		if ( Direction == Lower )
			ARMOffset = ( 0xFE - ( ( uint8_t ) Offset ));
		else
			ARMOffset = ( 0xFF + ( ( uint8_t ) Offset ));
			
		RetVal = ( uint8_t ) ARMOffset;
		break;
	}
	}
	return RetVal;	
}

//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
//_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_

uint32_t WriteOpcodeInARMBin ( uint16_t* BinFile, uint32_t* VirtMem )
{
	//Input1: ARM_Bin_Memory
	//Input1: Pointer on current command in VirtBin
	//Output: Writing block size
	uint16_t LengthCurARMOpcode = strlen ( OpcodesARM[VirtMem[0]] );
	char* CurrentOpcode = ( char* ) calloc ( LengthCurARMOpcode, sizeof(char));
	CurrentOpcode = strcpy ( CurrentOpcode, OpcodesARM[VirtMem[0]] );
	//##########################################################################
	uint16_t CurrentOpcodeBit = 0;
	uint16_t WordBlock = 0;
	for ( CurrentOpcodeBit = 0; CurrentOpcodeBit < LengthCurARMOpcode; \
		  CurrentOpcodeBit++ )
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if ( CurrentOpcode[CurrentOpcodeBit] == '1' )
		{
			WordBlock |= 1 << ( 15 - (CurrentOpcodeBit%16) );
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
		// Reading first arg 
		if ( CurrentOpcode[CurrentOpcodeBit] >= 'A' &&
			 CurrentOpcode[CurrentOpcodeBit] <= 'Z' )
		{
			uint8_t ArgNumBit = CurrentOpcode[CurrentOpcodeBit] - 'A';
			if ( ( ( VirtMem[1] >> ( ArgNumBit ) ) & 1 ) == 1 ) 
				WordBlock |= 1 << ( 15 - (CurrentOpcodeBit%16) );
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
		// Reading second arg ( first 26 bit )
		if ( CurrentOpcode[CurrentOpcodeBit] >= 'a' &&
			 CurrentOpcode[CurrentOpcodeBit] <= 'z' )
		{
			uint8_t ArgNumBit = CurrentOpcode[CurrentOpcodeBit] - 'a';
			if ( ( ( VirtMem[2] >> ( ArgNumBit ) ) & 1 ) == 1 )
				WordBlock |= 1 << ( 15 - (CurrentOpcodeBit%16) );
		}
		// Reading second arg ( last 6 bit )
		if ( CurrentOpcode[CurrentOpcodeBit] >= '2' &&
			 CurrentOpcode[CurrentOpcodeBit] <= '7' )
		{
			uint8_t ArgNumBit = CurrentOpcode[CurrentOpcodeBit] - '2' + 26;
			if ( ( ( VirtMem[2] >> ( ArgNumBit ) ) & 1 ) == 1 )
				WordBlock |= 1 << ( 15 - (CurrentOpcodeBit%16) );
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
		if ( CurrentOpcodeBit % 16 == 15 )
		{
			*BinFile = WordBlock;
			WordBlock = 0;
			BinFile++;
		}
	}
	//##########################################################################	
	free ( CurrentOpcode );
	return LengthCurARMOpcode/16;
}

//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
//_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@
//@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_
