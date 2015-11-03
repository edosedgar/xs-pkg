#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <windows.h>

#include "Tree.h"
#include "Compiler.h"

#define BUG

int main ( int argc, char* argv[] )
{
    Token* TokenCode = NULL;
    ///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    #ifndef BUG
    if ( argc > 1 )
    {
        TokenCode = CodeToToken ( argv[1] );

        if ( TokenCode == NULL && errno != 0 )
        {
            getchar ( );
            return 0;
        };
    } else return 0;
    #else
    TokenCode = CodeToToken ( "C:/Users/EdgarAdmin/Documents/Repositories/Informatic/Hometask8/bin/Debug/Startup.eds" );
    if ( TokenCode != NULL )
        ExpressionDump ( TokenCode );
    else
        return 0;
    TreeNode* TreeCode = CodeToTree ( TokenCode );
    TreeDump ( TreeCode );
    char* CopyN = (char*) calloc ( 1, sizeof ( char ));
    strcpy ( CopyN, "C:/Users/EdgarAdmin/Documents/Repositories/Informatic/Hometask8/bin/Debug/Startup.eds" );
    GenerateASMCode (  CopyN,TreeCode );
    #endif
    ///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    free ( TokenCode );    TokenCode = NULL;
    return 0;
}

