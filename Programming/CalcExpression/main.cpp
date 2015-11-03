#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <windows.h>

#include "Tree.h"
#include "Calculator.h"

int main ( )
{
    char* Expression = ( char* ) calloc ( 50, sizeof ( char ) );
    fgets ( Expression, 50, stdin );

    Token* NewTokens = StringToToken ( Expression );

    if ( NewTokens == NULL )
    {
        MessageBox ( NULL, "Check inputting expression!", "Error!", MB_OK | MB_SETFOREGROUND );
        return 0;
    }
    else
        ExpressionDump ( NewTokens );

    if ( SyntaxCheck ( NewTokens ) != 0 )
    {
        MessageBox ( NULL, "Syntax error in expression!", "Error!", MB_OK | MB_SETFOREGROUND );
        return 0;
    }

    double Result = CalcExpress ( NewTokens );

    printf ( "%lg", Result );

    return 0;
}
