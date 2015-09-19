#ifndef Compiler_H
#define Compiler_H

#include <stdint.h> 
#include <limits.h>

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
    #define ASSERT( cond )   do  {               \
        if ( ! ( cond ) )                        \
        {                                        \
            printf ( "FAIL %s in %s, %s ( %d )", \
            #cond, __PRETTY_FUNCTION__,          \
            __FILE__, __LINE__ );                \
            abort();                             \
        }                                        \
        } while ( 0 );
#else
    #define ASSERT( cond ) ;	
#endif


#define DefaultOutputFile ".bin"

const int OK = 0;
const int Error = UINT_MAX;

const uint8_t LengthFileExtension = 4;
const uint8_t SignatureSize = 10;

const uint8_t MaxCommands = 31;
const uint8_t MaxLengthCommands = 5;

const uint8_t MaxNumReg = 31;

const uint8_t RegArg = 0;
const uint8_t NumArg = 1;
const uint8_t LblArg = 2;

typedef uint32_t MemoryType;

enum Mnemonics
{
    ADD,
    ADD2,
    SUB,
    SUB2,
    DIV,
    DIV2,
    MUL,
    MUL2,
    MOV,
    MOV2,
    CMP,
    CMP2,
    POP,
    PUSH,
    CALL,
    JP,
    JA,
    JB,
    JAE,
    JBE,
    JE,
    JZ,
    JNZ,
    RET,
    OUT,
    IN,
    CLR,
    DEC,
    INC,
    NOP,
    END
};

typedef struct
{
    char* NameLabel;
    uint32_t ProgramAddress;
} Label;

typedef struct
{
    const char* Mnemonic;
    uint8_t Size;
} Command;

/**************************************************************************************
*
*     CreateArrayOfStrings function
*
*     \brief  This function create strings from input file
*     \param  FileName, FileBuffer
*     \return Strings pointer.
*
**************************************************************************************/
char** CreateArrayOfStrings ( const char* FileName, char** FileBuffer );
/**************************************************************************************
*
*     CountLines function
*
*     \brief  This function find number of string
*     \param  Array, Array length
*     \return Number of strings.
*
**************************************************************************************/
uint32_t CountLines ( char* FileBuffer, uint32_t Length );
/**************************************************************************************
*
*     FillArrayOfAddressOnString function
*
*     \brief  This function write in array of point address of string
*     \param  Array of point, array with strings, number of lines
*     \return None.
*
**************************************************************************************/
uint32_t FillArrayOfAddressOnString ( char** Strings, char* FileBuffer, uint32_t Length );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint32_t GetNumberLabel ( char** SourceCode );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
Label* CreateAndFillLabelTable ( char** SourceCode, Command* CommandsTable, uint32_t* ProgramSize );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
Command* CreateCommandsTable ( void );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint32_t GetCommandCodeAndSize ( Command* CommandsTable, char* Command, uint8_t* Size );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
MemoryType* CreateCodeMachine ( char** SourceCode, Label* LabelTable, Command* CommandsTable, uint32_t ProgramSize );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint32_t GetAddressLabel ( Label* LabelTable, char* StringLabel );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint32_t PrintBinFile ( MemoryType* MachineCode, char* argv, uint32_t SizeFile );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint32_t IsAlpha ( char Symbol );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint32_t IsSpace ( char Symbol );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint32_t IsDigit ( char Symbol );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint64_t GetFirstArg ( char* String, uint32_t* Position, uint8_t TypeArg );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
uint32_t GetSecondArg ( char* String, uint32_t* Position, uint8_t TypeArg );
/**************************************************************************************
*
*
*
*
*
**************************************************************************************/
char* GetLabelName ( char* String );

#endif
