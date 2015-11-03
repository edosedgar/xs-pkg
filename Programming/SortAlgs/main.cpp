#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "Sort.h"

//#define DEBUG

#define OutArray { for ( int i = 0; i < Size; i++ ) \
                        printf ( "%d ", Array[i] );};
const int MaxSize = 1000;

int main()
{
    int* Array = ( int* ) calloc ( MaxSize, sizeof ( int ) );
    long Compare = 0;
    long Assignment = 0;
#ifdef DEBUG
    int Size = 10;
    FillRandomArray ( Array, Size );
    OutArray
    putchar('\n');
    InsertSort ( Array, Size, &Compare, &Assignment );
    OutArray
#else
    OpenTable ( "table.csv" );
    for ( int Size = 1; Size <= MaxSize; Size++ )
    {
        Compare = 0;
        Assignment = 0;
        FillRandomArray ( Array, Size );
        //BubbleSort ( Array, Size, &Compare, &Assignment );
        //SelectSort ( Array, Size, &Compare, &Assignment );
        InsertSort ( Array, Size, &Compare, &Assignment );
        AddDataToTable ( Size, Compare, Assignment );
    };
    CloseTable ( );
#endif
    free ( Array );
    return 0;
}
