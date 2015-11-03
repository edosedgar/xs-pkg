#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include "List.h"
#include "Hash.h"
#include <locale.h>

int main()
{
    setlocale ( LC_CTYPE, "russian" );

    char* FileBuffer = NULL;
    char** Words = CreateWordArray ( "file.txt", &FileBuffer );

    ListHead** MyHashTable = CreateHashTable ( SizeHashTable );

    FillTheHashTable ( MyHashTable, SizeHashTable, Words, HashFunction4 );

    PrintExcelTable ( "Table.csv", MyHashTable, SizeHashTable );

    DestroyHashTable ( MyHashTable, SizeHashTable );

    free ( Words ); Words = 0;
    free ( FileBuffer ); FileBuffer = 0;
    return 0;
}
