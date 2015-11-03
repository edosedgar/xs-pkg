#ifndef Hash_H
#define Hash_H

#include "List.h"

const int SizeHashTable = 31973;

extern int HashErrno;

#define ELST ListERRNOMax+1

/**************************************************************
*
*
*
**************************************************************/
ListHead** CreateHashTable ( int Size );
/**************************************************************
*
*
*
**************************************************************/
int DestroyHashTable ( ListHead** HashTable, int Size );
/**************************************************************
*
*
*
**************************************************************/
int IsHashTableValid ( ListHead** HashTable, int Size );
/**************************************************************
*
*
*
**************************************************************/
int HashTableDump ( ListHead** HashTable, int Size );
/**************************************************************
*
*
*
**************************************************************/
int PrintExcelTable ( const char* FileName, ListHead** HashTable, int SizeHashTable );
/**************************************************************
*
*
*
**************************************************************/
int FillTheHashTable ( ListHead** HashTable, int Size, char** Words, int (* HashFunction ) ( char* ) );
/**************************************************************
*
*
*
**************************************************************/
int HashFunction1 ( char* Word );
/**************************************************************
*
*
*
**************************************************************/
int HashFunction2 ( char* Word );
/**************************************************************
*
*
*
**************************************************************/
int HashFunction3 ( char* Word );
/**************************************************************
*
*
*
**************************************************************/
int HashFunction4 ( char* Word );
/**************************************************************
*
*
*
**************************************************************/
int HashFunction5 ( char* Word );
/**************************************************************
*
*
*
**************************************************************/
char** CreateWordArray ( const char* NameFile, char** FileBuffer );
/**************************************************************
*
*
*
**************************************************************/
int CountWord ( char* FileBuffer, long int FileLength );
/**************************************************************
*
*
*
**************************************************************/
int FillArrayOfAddressOnWords ( char** Words, char* FileBuffer, int FileWords );
/**************************************************************
*
*
*
**************************************************************/
int IsAlpha ( char Symbol );
/**************************************************************
*
*
*
**************************************************************/
int HashFunction6 ( char* Word );
/**************************************************************************/
#endif // Hash
