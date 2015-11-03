#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <time.h>
#include "Compiler.h"

/******************* Math function *******************/
const int NumOfMathFunc = 10;

const char* MathFunctionTable [ NumOfMathFunc ] =
{
    "sin",
    "cos",
    "+",
    "-",
    "*",
    "/",
    "^",
    ">",
    "<",
    "dx"
};

/******************* Language  *******************/
const int NumOfOperator = 9;

const char* OperatorTable [ NumOfOperator ] =
{
    "=",
    "begin",
    "end",
    "read",
    "write",
    "if",
    "while",
    "out",
    "in"
};

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

Token* CodeToToken ( const char* FileName )
{
    FILE* File = fopen ( FileName, "rb" );
    ///*******************************************************************
    if ( File == NULL )
    {
        errno = 0;
        return NULL;
    }
    ///*******************************************************************
    int FileLength = filelength ( fileno ( File ) );
    char* FileBuffer;
    FileBuffer = ( char* ) calloc ( FileLength + 1, sizeof ( char ) );
    if ( FileBuffer == NULL )
    {
        errno = 0;
        return NULL;
    }
    ///*******************************************************************
    fread ( FileBuffer, sizeof ( *FileBuffer ), FileLength, File );
    fclose ( File );
    ///*******************************************************************
    Token* TokenEx = ( Token* ) calloc ( FileLength + 1, sizeof ( Token ) );
    int Position = 0;
    while ( *FileBuffer != '\0' )
    {
        char* OldPosition = FileBuffer;
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( IsAlpha ( *FileBuffer ) == OK || IsArOper ( *FileBuffer ) == OK )
        {
            char* Buffer = ( char* ) calloc ( MaxLengthIdent, sizeof ( char) );

            sscanf ( FileBuffer, "%[A-Za-z]", Buffer );

            if ( strlen ( Buffer ) == 0 )
            {
                *Buffer = *FileBuffer;
                *(Buffer+1)='\0';
                FileBuffer++;
            }
            else
                while ( IsAlpha ( *FileBuffer ) == 0 )
                    FileBuffer++;

            int FunLen = strlen ( Buffer );
            TokenEx[Position].Symbols = ( char* ) calloc ( FunLen, sizeof ( char ) );
            strcpy ( TokenEx[Position].Symbols, Buffer );
            ///******************************************************
            if ( IsBelongTable ( Buffer, MathFunctionTable, NumOfMathFunc ) == OK )
                TokenEx[Position].KindOfToken = FUNCTION;
            else
                if ( IsBelongTable ( Buffer, OperatorTable, NumOfOperator ) == OK )
                    TokenEx[Position].KindOfToken = OPERATOR;
                else
                    TokenEx[Position].KindOfToken = VAR;
            ///******************************************************
            //free ( Buffer );
            Position++;
        }
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( IsDigit ( *FileBuffer ) == OK )
        {
            double Buffer = 0;

            sscanf ( FileBuffer, "%lg", &Buffer );

            while ( IsDigit ( *FileBuffer ) == 0 || *FileBuffer == '.' )
                FileBuffer++;

            TokenEx[Position].Number = Buffer;

            TokenEx[Position].KindOfToken = NUMBER;

            Position++;
        }
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( IsSpace ( *FileBuffer ) == OK )
            FileBuffer++;
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( *FileBuffer == '(' || *FileBuffer == ')' )
        {
            TokenEx[Position].Symbols = ( char* ) calloc ( 1, sizeof ( char ) );
            *( TokenEx[Position].Symbols ) = *FileBuffer;
            TokenEx[Position].KindOfToken = BRACKET;
            FileBuffer++;
            Position++;
        }
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( IsEndString ( *FileBuffer ) == OK )
            FileBuffer++;
        ///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@*/
        if ( OldPosition == FileBuffer )
        {
            free ( FileBuffer );
            return NULL;
        }
    }
    TokenEx[Position].KindOfToken = EMPTY;
    //free ( FileBuffer );
    return TokenEx;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

int IsAlpha ( char Symbol )
{
    if ( ( 'A' <= Symbol && Symbol <= 'Z' ) || ( 'a' <= Symbol  && Symbol <= 'z' ) )
        return 0;
    else
        return -1;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
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
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

int IsDigit ( char Symbol )
{
    if ( '0' <= Symbol && Symbol <= '9' )
        return 0;
    else
        return -1;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

int IsEndString ( char Symbol )
{
    if ( Symbol == '\n' || Symbol == '\r' )
        return 0;
    else
        return -1;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

int IsArOper ( char Symbol )
{
    if ( Symbol == '*' || Symbol == '/' ||
         Symbol == '-' || Symbol == '+' ||
         Symbol == '^' || Symbol == '>' ||
         Symbol == '<' || Symbol == '=' )
        return 0;
    else
        return -1;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

int IsBelongTable ( char* String, const char** Table, int Size )
{
    for ( int i = 0; i < Size; i ++ )
    {
        if ( strcmp ( Table[i], String ) == 0 )
            return 0;
    }
    return -1;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
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
            fprintf ( Dump, "NUMBER  : %lg\n", Express->Number );
            break;
        case BRACKET:
            fprintf ( Dump, "BRACKET : %s\n", Express->Symbols );
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
        case ENDSTR:
            fprintf ( Dump, "ENDSTRNG: \n" );
            break;
        case OPERATOR:
            fprintf ( Dump, "OPERATOR: %s\n", Express->Symbols );
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
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

int TokenCopy ( Token* First, Token* Second )
{
    First->KindOfToken = Second->KindOfToken;
    switch ( First->KindOfToken )
    {
    case NUMBER:
        First->Number = Second->Number;
        break;
    default:
        First->Symbols = (char*) calloc ( strlen ( Second->Symbols ), sizeof ( char ) );
        strcpy ( First->Symbols, Second->Symbols );
    }
    return 0;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

Token* GlobalPosition = NULL;

TreeNode* CodeToTree ( Token* Code )
{
    if ( Code->KindOfToken == EMPTY ) return NULL;
    GlobalPosition = Code;
    return CallStart ();
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallStart ()
{
    TreeNode* Node = CallIn ( );

    if ( GlobalPosition->KindOfToken == OPERATOR &&
         strcmp ( GlobalPosition->Symbols, "begin" ) == 0 )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Middle = CallStart();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallIn ()
{
    TreeNode* Node = CallOut ( );

    if ( GlobalPosition->KindOfToken == OPERATOR &&
         strcmp ( GlobalPosition->Symbols, "in" ) == 0 )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = CallNum();
        CurrentNode->Right = CallIn();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallOut ()
{
    TreeNode* Node = CallIf ( );

    if ( GlobalPosition->KindOfToken == OPERATOR &&
         strcmp ( GlobalPosition->Symbols, "out" ) == 0 )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = CallNum();
        CurrentNode->Right = CallIn();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallIf ()
{
    TreeNode* Node = CallWhile ( );

    if ( GlobalPosition->KindOfToken == OPERATOR &&
         strcmp ( GlobalPosition->Symbols, "if" ) == 0 )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = CallBracket();
        CurrentNode->Middle = CallStart();
        CurrentNode->Right = CallIn();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallWhile ()
{
    TreeNode* Node = CallAssign ( );

    if ( GlobalPosition->KindOfToken == OPERATOR &&
         strcmp ( GlobalPosition->Symbols, "while" ) == 0 )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = CallBracket();
        CurrentNode->Middle = CallStart();
        CurrentNode->Right = CallIn();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallAssign ()
{
    TreeNode* Node = CallLess ( );

    if ( GlobalPosition->KindOfToken == OPERATOR &&
         strcmp ( GlobalPosition->Symbols, "=" ) == 0 )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = Node;
        CurrentNode->Middle = CallPlusMinus();
        CurrentNode->Right = CallIn();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallLess ()
{
    TreeNode* Node = CallMore ( );

    if ( GlobalPosition->KindOfToken == FUNCTION &&
         strcmp ( GlobalPosition->Symbols, "<" ) == 0 )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = Node;
        CurrentNode->Right = CallPlusMinus();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallMore ()
{
    TreeNode* Node = CallPlusMinus ( );

    if ( GlobalPosition->KindOfToken == FUNCTION &&
         strcmp ( GlobalPosition->Symbols, ">" ) == 0 )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = Node;
        CurrentNode->Right = CallPlusMinus();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallPlusMinus ( )
{
    TreeNode* Node = CallMulDiv ( );

    if ( GlobalPosition->KindOfToken == FUNCTION &&
         ( strcmp ( GlobalPosition->Symbols, "+" ) == 0 ||
           strcmp ( GlobalPosition->Symbols, "-" ) == 0 ) )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = Node;
        CurrentNode->Right = CallPlusMinus();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallMulDiv ( )
{
    TreeNode* Node = CallBracket ( );

    if ( GlobalPosition->KindOfToken == FUNCTION &&
         ( strcmp ( GlobalPosition->Symbols, "*" ) == 0 ||
           strcmp ( GlobalPosition->Symbols, "/" ) == 0 ) )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = Node;
        CurrentNode->Right = CallMulDiv();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallBracket ( )
{
    TreeNode* Result = NULL;
    {
        if ( GlobalPosition->KindOfToken == BRACKET &&
             strcmp ( GlobalPosition->Symbols, "(" ) == 0 )
        {
            GlobalPosition++;
            Result = CallLess ( );
        }

        if ( GlobalPosition->KindOfToken == BRACKET &&
             strcmp ( GlobalPosition->Symbols, ")" ) == 0 )
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

///#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_

TreeNode* CallNum ( )
{
    if ( GlobalPosition->KindOfToken == NUMBER )
    {
        TreeNode* NumNode = NewNode ( NULL, NONE );
        TokenCopy ( &( NumNode->Data ), GlobalPosition );
        GlobalPosition++;
        return NumNode;
    }
    if ( GlobalPosition->KindOfToken == VAR )
    {
        TreeNode* NumNode = NewNode ( NULL, NONE );
        TokenCopy ( &( NumNode->Data ), GlobalPosition );
        GlobalPosition++;
        return NumNode;
    }
    return NULL;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

int GenerateASMCode ( char* NameFile, TreeNode* SourceCode )
{
    int i = 0;
    while ( NameFile[i] != '\0' ) i++;

    if  ( strcmp ( NameFile + i - LengthFileExtension, SrcFormat ) != 0 )
    {
        printf( "   === Invalid inputting file extension! ===   " );
        return Error;
    }

    NameFile = strcpy ( NameFile + i - LengthFileExtension, ASMFormat );

    freopen ( NameFile + LengthFileExtension - i, "wb", stdout );
    ///****************************************************************************/

    RecurGenerate ( SourceCode );
    printf ( "  END" );

    ///****************************************************************************/
    fclose ( stdout );
    return OK;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

int RecurGenerate ( TreeNode* SourceCode )
{
    char* Label = NULL;
    char* Reg   = NULL;

    if ( SourceCode->Data.KindOfToken == OPERATOR &&
         strcmp ( SourceCode->Data.Symbols, "begin" ) == 0 )
        RecurGenerate ( SourceCode->Middle );

    if ( SourceCode->Data.KindOfToken == OPERATOR &&
         strcmp ( SourceCode->Data.Symbols, "in" ) == 0 )
    {
        printf ( "  %s ", SourceCode->Data.Symbols );
        Reg = ConvertVarToRegistr ( SourceCode->Left );
        printf ( "%s \n", Reg );
        if ( SourceCode->Right != NULL )
            RecurGenerate ( SourceCode->Right );
    }

    if ( SourceCode->Data.KindOfToken == OPERATOR &&
         strcmp ( SourceCode->Data.Symbols, "out" ) == 0 )
    {
        printf ( "  %s ", SourceCode->Data.Symbols );
        Reg = ConvertVarToRegistr ( SourceCode->Left );
        printf ( "%s \n", Reg );
        if ( SourceCode->Right != NULL )
            RecurGenerate ( SourceCode->Right );
    }

    if ( SourceCode->Data.KindOfToken == FUNCTION &&
         SourceCode->Left->Data.KindOfToken != FUNCTION &&
         SourceCode->Right->Data.KindOfToken != FUNCTION )
    {
        Reg = ConvertVarToRegistr ( SourceCode->Left );
        printf ( "  MOV R31, %s \n", Reg );
        Reg = ConvertVarToRegistr ( SourceCode->Right );
        if ( strcmp ( SourceCode->Data.Symbols, "+" ) == 0 )
            printf ( "  ADD R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "-" ) == 0 )
            printf ( "  SUB R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "*" ) == 0 )
            printf ( "  MUL R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "\\" ) == 0 )
            printf ( "  DIV R31, %s \n", Reg );
        return OK;
    }

    if ( SourceCode->Data.KindOfToken == FUNCTION &&
         SourceCode->Left->Data.KindOfToken == FUNCTION &&
         SourceCode->Right->Data.KindOfToken != FUNCTION )
    {
        RecurGenerate ( SourceCode->Left );
        Reg = ConvertVarToRegistr ( SourceCode->Right );
        if ( strcmp ( SourceCode->Data.Symbols, "+" ) == 0 )
            printf ( "  ADD R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "-" ) == 0 )
            printf ( "  SUB R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "*" ) == 0 )
            printf ( "  MUL R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "\\" ) == 0 )
            printf ( "  DIV R31, %s \n", Reg );
        return OK;
    }

    if ( SourceCode->Data.KindOfToken == FUNCTION &&
         SourceCode->Left->Data.KindOfToken != FUNCTION &&
         SourceCode->Right->Data.KindOfToken == FUNCTION )
    {
        RecurGenerate ( SourceCode->Right );
        Reg = ConvertVarToRegistr ( SourceCode->Left );
        if ( strcmp ( SourceCode->Data.Symbols, "+" ) == 0 )
            printf ( "  ADD R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "-" ) == 0 )
            printf ( "  SUB R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "*" ) == 0 )
            printf ( "  MUL R31, %s \n", Reg );
        if ( strcmp ( SourceCode->Data.Symbols, "\\" ) == 0 )
            printf ( "  DIV R31, %s \n", Reg );
        return OK;
    }


    if ( SourceCode->Data.KindOfToken == OPERATOR &&
         strcmp ( SourceCode->Data.Symbols, "=" ) == 0 )
    {
        if ( SourceCode->Middle != NULL )
            RecurGenerate ( SourceCode->Middle );
        Reg = ConvertVarToRegistr ( SourceCode->Left );
        printf ( "  MOV %s, R31 \n", Reg );
        if ( SourceCode->Right != NULL )
            RecurGenerate ( SourceCode->Right );
    }


    return OK;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

char* ConvertVarToRegistr ( TreeNode* Node )
{
    char* Mem = (char*) calloc ( MaxLengthRegVar  , sizeof ( char ));
    if ( Node->Data.KindOfToken == VAR )
    {
        int ConvertVar = ( unsigned char ) ( Node->Data.Symbols[0] );
        if ( ConvertVar >= 97 && ConvertVar <= 122 )
            ConvertVar -= 97;
        if ( ConvertVar >= 65 && ConvertVar <= 90 )
            ConvertVar -= 65;
        for ( int i = 0; i < MaxLengthRegVar; i++ ) Mem[i] = 0;
        Mem[0] = 'R';
        sprintf ( &(Mem[1]), "%d", ConvertVar );
        return Mem;
    }
    if ( Node->Data.KindOfToken == NUMBER )
    {
        int ConvertVar = Node->Data.Number;
        for ( int i = 0; i < MaxLengthRegVar; i++ ) Mem[i] = 0;
        sprintf ( Mem, "%d", ConvertVar );
        return Mem;
    }
    return NULL;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/

char* ConvertVarToLabel ( int Number, char* Mem )
{
    Mem = (char*) calloc ( MaxLengthLabel, sizeof ( char ));
    for ( int i = 0; i < MaxLengthLabel; i++ ) Mem[i] = 0;
    strcpy ( Mem, "Label" );
    int Pos = strlen ( Mem );
    sprintf ( &(Mem[Pos]), "%d:", Number );
    return Mem;
}

/****************************************************************************/
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/****************************************************************************/
