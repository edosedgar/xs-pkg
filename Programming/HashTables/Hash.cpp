#include "Hash.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <io.h>

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

int HashErrno = 0;
#define DEBUG

/****************************************************************************/
ListHead** CreateHashTable ( int Size )
{
    ListHead** HashTable = ( ListHead** ) calloc ( Size, sizeof ( ListHead ) );

    if ( HashTable == NULL )
    {
        HashErrno = ENOMEM;
        return NULL;
    }

    for ( int i = 0; i < Size; i++ )
    {
        HashTable[i] = ListCreate ( );
        if ( HashTable[i] == NULL )
        {
            HashErrno = ENOMEM;
            return NULL;
        }
    }
    return HashTable;
}
/****************************************************************************/
int DestroyHashTable ( ListHead** HashTable, int Size )
{
    if ( IsHashTableValid ( HashTable, Size ) != 0 )
        return -1;

    for ( int i = 0; i < Size; i++ )
    {
        if ( ListDestroy ( HashTable[i] ) != 0 )
        {
            HashErrno = ELST;
            return -1;
        }
    }
    free ( HashTable );
    return 0;
}
/****************************************************************************/
int IsHashTableValid ( ListHead** HashTable, int Size )
{
#ifdef DEBUG
    for ( int i = 0; i < Size; i++ )
    {
        if ( IsListValid ( HashTable[i] ) != 0 )
        {
            HashErrno = ELST;
            return -1;
        }
    }
#endif
    return 0;
}
/****************************************************************************/
int HashTableDump ( ListHead** HashTable, int Size )
{
    freopen ( "dump.log", "a", stdout );
    time_t RawTime = time ( NULL );
    printf ( "\n/****************************************************************************/\n");
    printf ( "\n$$$$$ Date: %s \n", ctime ( &RawTime ) );
    int IsOutHash = 0;

    switch ( HashErrno )
    {
    case ENOMEM:
        printf ( "-----> No memory for create hash table! \n" );
        break;
    case ELST:
    {
        for ( int i = 0; i < Size; i++ )
        {
            if ( IsListValid ( HashTable[i] ) != 0 )
            {
                ListDump ( HashTable[i] );
            }
        }
        break;
    }
    default:
        printf ( "-----> Unknown error  \n" );
        IsOutHash = 1;
    }
    if ( IsOutHash )
    {
        printf ( "The hash table at address: 0x%p \n", HashTable );
        for ( int i = 0; i < Size; i++ )
        {
            printf ( "%d    ", HashTable[i]->NumElement );
            HashTable[i]->CurElement = HashTable[i]->Start->Next;
            for ( int j = 0; j < (HashTable[i]->NumElement) - 1; j++ )
            {
                printf ( "%s, ", HashTable[i]->CurElement->Data );
                HashTable[i]->CurElement = HashTable[i]->CurElement->Next;
            }
            HashTable[i]->CurElement = HashTable[i]->Start->Next;
            printf ( "\n" );
        }
    }
    printf ( "\n/****************************************************************************/\n");
    errno = 0;
    fclose ( stdout );
    return 0;
}
/****************************************************************************/
int FillTheHashTable ( ListHead** HashTable, int Size, char** Words, int (* HashFunction ) ( char* ) )
{
    long int i = 0;
    while ( Words[i] != NULL )
    {
        unsigned UnCode = ( unsigned ) HashFunction ( Words[i] );
        if ( UnCode >= Size )
            UnCode = UnCode % Size;
        if ( IsElementInList ( HashTable[UnCode], Words[i] ) == 0 )
            AddElement ( HashTable[UnCode], Words[i] );
        i++;
    }
    return 0;
}
/****************************************************************************/
int HashFunction1 ( char* Word )
{
    return 1;
}
/****************************************************************************/
int HashFunction2 ( char* Word )
{
    return ( int )( ( unsigned char )( *Word ) );
}
/****************************************************************************/
int HashFunction3 ( char* Word )
{
    return strlen(Word);
}
/****************************************************************************/
int HashFunction4 ( char* Word )
{
    int i = 0;
    int ASCIISum = 0;
    while ( *( Word + i ) != '\0' )
    {
        ASCIISum += ( int )( ( unsigned char )( *( Word + i ) ) );
        i++;
    }
    return ASCIISum;
}
/****************************************************************************/
int HashFunction5 ( char* Word )
{
    if ( *Word == '\0' )
        return 0;
    if ( *( Word + 1 ) == '\0' )
        return ( int )( ( unsigned char )( *( Word ) ) );
    return ( ( _lrotr ( HashFunction5 ( Word + 1 ), 1 ) ) ^ ( int )( ( unsigned char )( *Word ) ) );
}
/****************************************************************************/
int HashFunction6 ( char* Word )
{
    unsigned int HashC = 0;

    for ( int i = 0; *( Word + i ) != '\0'; i++ )
    {
        HashC += ( unsigned char ) ( *( Word + i ) );
        HashC += ( HashC << 10 );
        HashC ^= ( HashC >> 6 );
    }
    HashC += ( HashC << 3 );
    HashC ^= ( HashC >> 11 );
    HashC += ( HashC << 15 );

    return HashC;
}
/****************************************************************************/
int PrintExcelTable ( const char* FileName, ListHead** HashTable, int SizeHashTable )
{
    freopen ( FileName, "w", stdout );
    for ( int i = 0; i < SizeHashTable; i++ )
    {
        printf ( "%d;", i );
        printf ( "%d\n", ( HashTable[i]->NumElement ) - 1 );
    }
    return 0;
}
/****************************************************************************/
char** CreateWordArray ( const char* NameFile, char** FileBuffer )
{
    FILE* File = fopen ( NameFile, "r" );
    ASSERT ( File );

    int FileLength = filelength ( fileno ( File ) );
    *FileBuffer = ( char* ) calloc ( FileLength + 1, sizeof ( char ) );
    ASSERT ( *FileBuffer );

    fread ( *FileBuffer, sizeof ( char ), FileLength, File );
    fclose ( File );
    //==== =============================================================== ==========

    long int FileWords = CountWord ( *FileBuffer, FileLength );
    char** Words = ( char** ) calloc ( FileWords + 1, sizeof ( char* ) );
    ASSERT ( Words );

    FillArrayOfAddressOnWords ( Words, *FileBuffer, FileWords );
    return Words;
}
/****************************************************************************/
int CountWord ( char* FileBuffer, long int FileLength )
{
    long int i = 0;
    long int Count = 0;
    while ( 1 )
    {
        while ( IsAlpha ( FileBuffer[i] ) == -1 )
        {
            i++;
            if ( i >= FileLength ) return Count;
        }

        while ( IsAlpha ( FileBuffer[i] ) == 0 )
        {
            i++;
            if ( i >= FileLength ) return ++Count;
        }
        Count++;
        FileBuffer[i] = '\0';
    }
    return Count;
}
/****************************************************************************/
int IsAlpha ( char Symbol )
{
    const char ASCIIRUSA = ( char )( 192 );
    const char ASCIIRUSYA = ( char )( 255 );
    if ( ( 'A' <= Symbol && Symbol <= 'Z' ) || ( 'a' <= Symbol && Symbol <= 'z' ) ||
         ( ASCIIRUSA <= Symbol && Symbol <= ASCIIRUSYA ) )
        return 0;
    else
        return -1;
}
/****************************************************************************/
int FillArrayOfAddressOnWords ( char** Words, char* FileBuffer, int FileWords )
{
    long int j = 0;
    for ( long int i = 0; i < FileWords; i++ )
    {
        while ( IsAlpha ( FileBuffer[j] ) == -1 ) j++;

        *( Words + i ) = FileBuffer + j;

        while ( IsAlpha ( FileBuffer[j] ) == 0 )  j++;
    }
    return 0;
}
/****************************************************************************/
