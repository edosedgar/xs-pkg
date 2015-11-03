#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include "Calculator.h"
#include <time.h>
#include "MyStack.h"
#include "Math.h"

/******************* Math function *******************/
const char* MathFunctionTable [ NumOfMathFunc ] =
{
    "sin",
    "cos",
    "+",
    "-",
    "*",
    "/",
    "^",
    "dx"
};
/*****************************************************/

/****************************************************************************/
Token* StringToToken ( char* Expression )
{
    int Length = strlen ( Expression );
    Token* TokenEx = ( Token* ) calloc ( Length, sizeof ( Token ) );
    int Position = 0;
    while ( IsEnd ( *Expression ) != 0 )
    {
        char* OldPosition = Expression;
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( IsAlpha ( *Expression ) == 0 || IsArOper ( *Expression ) == 0 )
        {
            char* Buffer = ( char* ) calloc ( MaxLengthFunc, sizeof ( char) );

            sscanf ( Expression, "%[A-Za-z]", Buffer );

            if ( strlen ( Buffer ) == 0 )
            {
                *Buffer = *Expression;
                Expression++;
            }
            else
                while ( IsAlpha ( *Expression ) == 0 || IsArOper ( *Expression ) == 0 )
                    Expression++;

            int FunLen = strlen ( Buffer );
            TokenEx[Position].Symbols = ( char* ) calloc ( FunLen, sizeof ( char ) );
            strcpy ( TokenEx[Position].Symbols, Buffer );

            if ( IsMathFunction ( Buffer ) == 0 )
                TokenEx[Position].KindOfToken = FUNCTION;
            else
                TokenEx[Position].KindOfToken = VAR;

            free ( Buffer );

            Position++;
        }
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( IsDigit ( *Expression ) == 0 )
        {
            double Buffer = 0;

            sscanf ( Expression, "%lg", &Buffer );

            while ( IsDigit ( *Expression ) == 0 || *Expression == '.' )
                Expression++;

            TokenEx[Position].Number = Buffer;

            TokenEx[Position].KindOfToken = NUMBER;

            Position++;
        }
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( IsSpace ( *Expression ) == 0 )
            Expression++;
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( *Expression == '(' || *Expression == ')' )
        {
            TokenEx[Position].Symbols = ( char* ) calloc ( 1, sizeof ( char ) );
            *( TokenEx[Position].Symbols ) = *Expression;
            TokenEx[Position].KindOfToken = BRACKET;
            Expression++;
            Position++;
        }
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if  ( OldPosition == Expression )
            return NULL;
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
    }
    TokenEx[Position].KindOfToken = EMPTY;
    return TokenEx;
}
/****************************************************************************/
int SyntaxCheck ( Token* Express )
{
    ///**********| Check brackets |**********///
    Token* BeginAdress = Express;
    MyStack* BracketBacket = StackCreate ( 50 );
    while ( Express->KindOfToken != EMPTY )
    {
        if ( Express->KindOfToken == BRACKET && Express->Symbols[0] == '(' )
            PushStack ( BracketBacket, '(' );
        if ( Express->KindOfToken == BRACKET && Express->Symbols[0] == ')' )
            if ( PopStack ( BracketBacket ) == -1 )
            {
                StackDestroy ( BracketBacket );
                return -1;
            }
        Express++;
    }

    if ( PopStack ( BracketBacket ) == -1 && errno == 0 )
        StackDestroy ( BracketBacket );
    else
    {
        StackDestroy ( BracketBacket );
        return -1;
    }

    Express = BeginAdress;
    ///**************************************///
    return 0;
}
/****************************************************************************/
int IsAlpha ( char Symbol )
{
    if ( ( 'A' <= Symbol && Symbol <= 'Z' ) || ( 'a' <= Symbol  && Symbol <= 'z' ) )
        return 0;
    else
        return -1;
}
/****************************************************************************/
int IsSpace ( char Symbol )
{
    char ASCIICodeSpace = 32;
    char ASCIICodeTab   =  9;
    if ( Symbol == ASCIICodeSpace || Symbol == ASCIICodeTab )
        return 0;
    else
        return -1;
}
/****************************************************************************/
int IsDigit ( char Symbol )
{
    if ( '0' <= Symbol && Symbol <= '9' )
        return 0;
    else
        return -1;
}
/****************************************************************************/
int IsEnd ( char Symbol )
{
    if ( Symbol == '\n' || Symbol == '\0' || Symbol == '\r' )
        return 0;
    else
        return -1;
}
/****************************************************************************/
int IsArOper ( char Symbol )
{
    if ( Symbol == '*' || Symbol == '/' || Symbol == '-' || Symbol == '+' || Symbol == '^' )
        return 0;
    else
        return -1;
}
/****************************************************************************/
int IsMathFunction ( char* String )
{
    for ( int i = 0; i < NumOfMathFunc; i ++ )
    {
        if ( strcmp ( MathFunctionTable[i], String ) == 0 )
            return 0;
    }
    return -1;
}
/****************************************************************************/
int ExpressionDump ( Token* Express )
{
    FILE* Dump = fopen ( "dump.log", "a" );
    time_t RawTime = time ( NULL );
    fprintf ( Dump, "\n/****************************************************************************/\n");
    fprintf ( Dump, "\n$$$$$ Date: %s \n", ctime ( &RawTime ) );

    /*****************************************/
    while ( Express->KindOfToken != EMPTY )
    {
        switch ( Express->KindOfToken )
        {
        case NUMBER:
            fprintf ( Dump, "NUMBER: %lg\n", Express->Number );
            break;
        case BRACKET:
            fprintf ( Dump, "BRACKET: %c\n", Express->Symbols[0] );
            break;
        case FUNCTION:
            fprintf ( Dump, "FUNCTION: %s\n", Express->Symbols );
            break;
        case VAR:
            fprintf ( Dump, "VARIABLE: %s\n", Express->Symbols );
            break;
        case EMPTY:
            fprintf ( Dump, "END OF EXPRESSION! \n" );
            break;
        }
        Express++;
    }
    /*****************************************/

    fprintf ( Dump, "\n/****************************************************************************/\n");
    errno = 0;
    fclose ( Dump );
    return 0;
}
/****************************************************************************/

Token* GlobalPosition = NULL;

double CalcExpress ( Token* Express )
{
    GlobalPosition = Express;
    return CallPlus ( );
}
/****************************************************************************/
double CallPlus ( )
{
    double Result = 0;
    for (;;)
    {
        Result += CallMinus ( );
        if ( GlobalPosition->KindOfToken == FUNCTION && GlobalPosition->Symbols[0] == '+' )
        {
            GlobalPosition++;
        }
        else
            return Result;
    }
    return Result;
}
/****************************************************************************/
double CallMinus ( )
{
    double Result = CallMul ( );
    for (;;)
    {
        if ( GlobalPosition->KindOfToken == FUNCTION && GlobalPosition->Symbols[0] == '-' )
        {
            GlobalPosition++;
            Result -= CallMul ( );
        }
        else
            return Result;
    }
    return Result;
}
/****************************************************************************/
double CallMul ( )
{
    double Result = CallDiv ( );
    for (;;)
    {
        if ( GlobalPosition->KindOfToken == FUNCTION && GlobalPosition->Symbols[0] == '*' )
        {
            GlobalPosition++;
            Result *= CallDiv ( );
        }
        else
            return Result;
    }
    return Result;
}
/****************************************************************************/
double CallDiv ( )
{
    double Result = CallSin ( );
    for (;;)
    {
        if ( GlobalPosition->KindOfToken == FUNCTION && GlobalPosition->Symbols[0] == '/' )
        {
            GlobalPosition++;
            Result /= CallSin ( );
        }
        else
            return Result;
    }
    return Result;
}
/****************************************************************************/
double CallSin ( )
{
    double Result = CallCos ( );
    for (;;)
    {
        if ( GlobalPosition->KindOfToken == FUNCTION &&
             strcmp ( GlobalPosition->Symbols, "sin" ) == 0 )
        {
            GlobalPosition++;
            Result = sin ( CallCos ( ) );
        }
        else
            return Result;
    }
    return Result;
}
/****************************************************************************/
double CallCos ( )
{
    double Result = CallDegr ( );
    for (;;)
    {
        if ( GlobalPosition->KindOfToken == FUNCTION &&
             strcmp ( GlobalPosition->Symbols, "cos" ) == 0 )
        {
            GlobalPosition++;
            Result = cos ( CallDegr ( ) );
        }
        else
            return Result;
    }
    return Result;
}
/****************************************************************************/
double CallDegr ( )
{
    double Result = CallBrac ( );
    for (;;)
    {
        if ( GlobalPosition->KindOfToken == FUNCTION && GlobalPosition->Symbols[0] == '^' )
        {
            GlobalPosition++;
            Result = pow ( Result, CallBrac ( ) );
        }
        else
            return Result;
    }
    return Result;
}
/****************************************************************************/
double CallBrac ( )
{
    double Result = 0;
    {
        if ( GlobalPosition->KindOfToken == BRACKET && GlobalPosition->Symbols[0] == '(' )
        {
            GlobalPosition++;
            Result = CallPlus ( );
        }

        if ( GlobalPosition->KindOfToken == BRACKET && GlobalPosition->Symbols[0] == ')' )
        {
            GlobalPosition++;
            return Result;
        }

        if ( GlobalPosition->KindOfToken != BRACKET )
        {
            Result = CallNum ( );
        }
    }
    return Result;
}
/****************************************************************************/
double CallNum ( )
{
    if ( GlobalPosition->KindOfToken == NUMBER )
    {
        double Number = GlobalPosition->Number;
        GlobalPosition++;
        return Number;
    }
    return 0;
}
/****************************************************************************/
