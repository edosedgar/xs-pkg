#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <locale.h>

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

//================== Prototypes ===================
char** CreateArrayOfStrings ( const char* FileName, char** FileBuffer );
int CountLines ( char* FileBuffer, int Length );
int FillArrayOfAddressOnString ( char** Strings, char* FileBuffer, int Length );
int PrepareArrayToPrintInFile ( char** Strings, int FileLines );
int CompareStringsAZ ( const void* String1, const void* String2 );
int CompareStringsZA ( const void* String1, const void* String2 );
void PrintPoem ( char** Strings, const char* FileName, const char ModeSorting );
//=================================================

int main()
{
    setlocale ( LC_CTYPE, "russian" );

    char* FileBuffer = NULL;
    char** Strings = CreateArrayOfStrings ( "file.txt", &FileBuffer );

    PrintPoem ( Strings, "out.txt", 'Z-A' );

    free ( Strings ); Strings = 0;
    free ( FileBuffer ); FileBuffer = 0;
    return 0;
}

//==============================================================
/*!
 *     CreateArrayOfStrings function
 *
 *     \brief  This function create strings from input file
 *     \param  FileName, FileBuffer
 *     \return Strings pointer.
 */
//==============================================================

char** CreateArrayOfStrings ( const char* FileName, char** FileBuffer )
{
    FILE* File = fopen ( FileName, "r" );
    ASSERT ( File );

    int FileLength = filelength ( fileno ( File ) );
    *FileBuffer = ( char* ) calloc ( FileLength + 1, sizeof ( **FileBuffer ) );
    ASSERT ( *FileBuffer );

    fread ( *FileBuffer, sizeof ( **FileBuffer ), FileLength, File );
    fclose ( File );
    //==== =============================================================== ==========

    int FileLines = CountLines ( *FileBuffer, FileLength ) + 1;
    char** Strings = ( char** ) calloc ( FileLines , sizeof ( Strings ) );
    ASSERT ( Strings );

    FillArrayOfAddressOnString ( Strings, *FileBuffer, FileLines );
    return Strings;
}

//==============================================================
/*!
 *     PrintPoem function
 *
 *     \brief  This function print strings from array Strings in file
 *     \param  Strings, Length of Strings
 *     \return None.
 */
//==============================================================

void PrintPoem ( char** Strings, const char* FileName, const char ModeSorting )
{
    int FileLines = 0;
    while ( Strings[FileLines] != NULL ) FileLines++;

    if ( ModeSorting == 'A-Z' )
        qsort ( Strings, FileLines, sizeof  ( *Strings ), CompareStringsAZ );
    else
        qsort ( Strings, FileLines, sizeof  ( *Strings ), CompareStringsZA );

    freopen ( FileName, "w", stdout );

    for ( int i = 0; i < FileLines; i++ )
    {
        ASSERT ( i >= 0 && i < FileLines );
        printf ( "%s\n", Strings[i] );
    }
    fclose ( stdout );
}

//==============================================================
/*!
 *     CountLines function
 *
 *     \brief  This function find number of string
 *     \param  Array, Array length
 *     \return Number of strings.
 */
//==============================================================

int CountLines ( char* FileBuffer, int Length )
{
    int n = 0;
    ASSERT ( FileBuffer != 0 );
    ASSERT ( Length != 0 );
    for ( int i = 0; FileBuffer[i] != '\0'; i++ )
    {
        ASSERT ( i >= 0 && i < Length );
        if ( FileBuffer[i] == '\n' ) n++;
    }
    return n;
}

//==============================================================
/*!
 *     FillArrayOfAddressOnString function
 *
 *     \brief  This function write in array of point address of string
 *     \param  Array of point, array with strings, number of lines
 *     \return None.
 */
//==============================================================

int FillArrayOfAddressOnString ( char** Strings, char* FileBuffer, int Lines )
{
    ASSERT ( Strings != 0 );
    ASSERT ( FileBuffer != 0 );
    ASSERT ( Lines != 0 );
    int NumberString = 1;
    char* OldPositionInFile = 0;
    char* NewPositionInFile = FileBuffer;
    Strings[0] = FileBuffer;

    while ( *NewPositionInFile != '\0' )
    {
        OldPositionInFile = NewPositionInFile;

        NewPositionInFile = ( strchr ( OldPositionInFile, '\n' ) + 1);
        *( NewPositionInFile - 1 ) = '\0';
        ASSERT ( NewPositionInFile );

        ASSERT ( NumberString >= 0 && NumberString < Lines );
        Strings[NumberString] = NewPositionInFile;

        NumberString++;
    }
    return 0;
}

//==============================================================
/*!
 *     CompareStrings function
 *
 *     \brief  This function compare two string ( A - Z )
 *     \param  Point to element of array with points on strings.
 *     \return 1 if String1 > String2; 0 if equal; -1 if String1 < String2
 */
//==============================================================

int CompareStringsAZ ( const void* String1, const void* String2 )
{
    char Buffer = 0;
    int i = 0;
    while ( Buffer == 0 && ( *(char** ) String1 )[i] != '\0' && ( *( char** ) String2 )[i] != '\0' )
    {
        Buffer = ( *( char** ) String1 )[i] - ( *( char** ) String2)[i];
        i++;
    }
    return Buffer;
}

//==============================================================
/*!
 *     CompareStrings function
 *
 *     \brief  This function compare two string ( Z - A )
 *     \param  Point to element of array with points on strings.
 *     \return 1 if String1 > String2; 0 if equal; -1 if String1 < String2
 */
//==============================================================

int CompareStringsZA ( const void* String1, const void* String2 )
{
    char Buffer = 0;
    int i = 0;
    while ( Buffer == 0 && ( *(char** ) String1 )[i] != '\0' && ( *( char** ) String2 )[i] != '\0' )
    {
        Buffer = ( *( char** ) String2 )[i] - ( *( char** ) String1)[i];
        i++;
    }
    return Buffer;
}
