#include "Sort.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <time.h>

/*************************************************************************/
int FillRandomArray ( int* Array, int Size )
{
    srand ( time ( NULL ) );
    for ( int i = 0; i < Size; i++ )
        Array[i] = rand ();
    return 0;
}
/*************************************************************************/
int BubbleSort ( int* Array, int Size, long* Compare, long* Assignment )
{
    for ( int i = 0; i <= Size; i++ )
    {
        for ( int j = 0; j < Size - i - 1; j++ )
        {
            if ( Array[j] > Array[j+1] )
            {
                Swap ( &Array[j], &Array[j+1] );
                ( *Assignment ) += 3;
            }
            ( *Assignment ) += 2;
            ( *Compare ) += 2;
        }
        ( *Assignment ) += 1;
        ( *Compare ) += 1;
    }
    return 0;
}
/*************************************************************************/
int SelectSort ( int* Array, int Size, long* Compare, long* Assignment )
{
    for ( int i = 0; i < Size; i++ )
    {
        int Min = i;
        for ( int j = i; j < Size; j++ )
        {
            if ( Array[j] < Array[Min] )
            {
                Min = j;
                ( *Assignment )++;
            }
            ( *Compare ) += 2;
            ( *Assignment ) ++;
        }
        Swap ( &Array[i], &Array[Min] );
        ( *Assignment ) += 5;
        ( *Compare ) += 1;
    }
    return 0;
}
/*************************************************************************/
int InsertSort ( int* Array, int Size, long* Compare, long* Assignment )
{
    for ( int i = 1; i < Size; i++ )
    {
        int Temp = Array[i];
        for ( int j = i - 1; j >= 0; j-- )
        {
            if ( Array[j] < Temp )
                break;
            ( *Compare ) += 2;
            Array[j + 1] = Array[j];
            Array[j] = Temp;
            ( *Assignment ) += 3;
        }
        ( *Assignment ) += 4;
        ( *Compare ) += 1;
    }
    return 0;
}
/*************************************************************************/

/*************************************************************************/

/*************************************************************************/
int Swap ( int* First, int* Two )
{
    int Buffer = *First;
    *First = *Two;
    *Two = Buffer;
    return 0;
}
/*************************************************************************/
int OpenTable ( const char* FileName )
{
    freopen ( FileName, "w", stdout );
    printf ( "Compare;Size;Assignment\n" );
    return 0;
}
/*************************************************************************/
int AddDataToTable ( int Size, int Compare, int Assignment )
{
    printf ( "%d;%d;%d\n", Compare, Size, Assignment );
    return 0;
}
/*************************************************************************/
int CloseTable ( )
{
    fclose ( stdout );
    return 0;
}
/*************************************************************************/
