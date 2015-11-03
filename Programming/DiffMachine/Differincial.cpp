#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>

#include "Differincial.h"
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

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
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
            while ( IsAlpha ( *Expression ) == 0 )
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
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
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
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

int IsAlpha ( char Symbol )
{
    if ( ( 'A' <= Symbol && Symbol <= 'Z' ) || ( 'a' <= Symbol  && Symbol <= 'z' ) )
        return 0;
    else
        return -1;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
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
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

int IsDigit ( char Symbol )
{
    if ( '0' <= Symbol && Symbol <= '9' )
        return 0;
    else
        return -1;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

int IsEnd ( char Symbol )
{
    if ( Symbol == '\n' || Symbol == '\0' || Symbol == '\r' )
        return 0;
    else
        return -1;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

int IsArOper ( char Symbol )
{
    if ( Symbol == '*' || Symbol == '/' || Symbol == '-' || Symbol == '+' || Symbol == '^' )
        return 0;
    else
        return -1;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
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
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
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
            fprintf ( Dump, "BRACKET: %s\n", Express->Symbols );
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
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

Token* GlobalPosition = NULL;

TreeNode* ExpressionToTree ( Token* Express )
{
    if ( Express->KindOfToken == EMPTY ) return NULL;
    GlobalPosition = Express;
    return CallPlusMinus ( );
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* CallPlusMinus ( )
{
    TreeNode* Node = CallMulDiv ( );

    if ( GlobalPosition->KindOfToken == FUNCTION &&  ( GlobalPosition->Symbols[0] == '+' ||
                                                       GlobalPosition->Symbols[0] == '-' ) )
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

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* CallMulDiv ( )
{
    TreeNode* Node = CallSinCos ( );

    if ( GlobalPosition->KindOfToken == FUNCTION &&  ( GlobalPosition->Symbols[0] == '*' ||
                                                       GlobalPosition->Symbols[0] == '/' ) )
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

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* CallSinCos ( )
{
    TreeNode* Node = CallDegr ( );

    if ( GlobalPosition->KindOfToken == FUNCTION &&
       ( strcmp ( GlobalPosition->Symbols, "sin" ) == 0 ||
         strcmp ( GlobalPosition->Symbols, "cos" ) == 0  ) )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = CallSinCos();
        CurrentNode->Right = Node;
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* CallDegr ( )
{
    TreeNode* Node = CallBrac ( );

    if ( GlobalPosition->KindOfToken == FUNCTION && GlobalPosition->Symbols[0] == '^' )
    {
        TreeNode* CurrentNode = NewNode ( NULL, NONE );
        TokenCopy ( &( CurrentNode->Data ), GlobalPosition );
        GlobalPosition++;
        CurrentNode->Left = Node;
        CurrentNode->Right = CallDegr();
        return CurrentNode;
    }
    else
        return Node;

    return NULL;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* CallBrac ( )
{
    TreeNode* Result = NULL;
    {
        if ( GlobalPosition->KindOfToken == BRACKET && GlobalPosition->Symbols[0] == '(' )
        {
            GlobalPosition++;
            Result = CallPlusMinus ( );
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
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

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
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
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
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

int CreateToken ( Token* NewToken, const char* Data, double Num, EKindOfToken KindOfToken )
{
    NewToken->KindOfToken = KindOfToken;
    NewToken->Number = Num;
    if ( KindOfToken != NUMBER )
    {
        (NewToken)->Symbols = ( char* ) calloc ( strlen ( Data ), sizeof ( char ) );
        strcpy ( (NewToken)->Symbols, Data );
    }
    return 0;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* DifferentiationTree ( TreeNode* Differentation )
{
    if ( Differentation->Data.KindOfToken == FUNCTION &&
        (Differentation->Data.Symbols[0] == '+' ||
         Differentation->Data.Symbols[0] == '-' ) )
    {
        TreeNode* DifNode = NewNode ( NULL, NONE );
        TokenCopy ( &( DifNode->Data ), &( Differentation->Data ) );
        (DifNode->Left) = DifferentiationTree ( Differentation->Left );
        (DifNode->Right) = DifferentiationTree ( Differentation->Right );
        return DifNode;
    }

    if ( Differentation->Data.KindOfToken == FUNCTION &&
        (Differentation->Data.Symbols[0] == '*' ) )
    {
        TreeNode* DifNode = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Data ), "+", 0, FUNCTION );

        DifNode->Left = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Data ), "*", 0, FUNCTION );

        DifNode->Right = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Right->Data ), "*", 0, FUNCTION );

        DifNode->Left->Left = DifferentiationTree ( Differentation->Left );
        DifNode->Left->Right = CopyTree ( Differentation->Right );

        DifNode->Right->Left = CopyTree ( Differentation->Left );
        DifNode->Right->Right = DifferentiationTree ( Differentation->Right );
        return DifNode;
    }

    if ( Differentation->Data.KindOfToken == FUNCTION &&
        (Differentation->Data.Symbols[0] == '/' ) )
    {
        TreeNode* DifNode = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Data ), "/", 0, FUNCTION );

        DifNode->Left = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Data ), "-", 0, FUNCTION );

        DifNode->Right = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Right->Data ), "^", 0, FUNCTION );

        DifNode->Left->Left = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Left->Data ), "*", 0, FUNCTION );

        DifNode->Left->Right = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Right->Data ), "*", 0, FUNCTION );

        DifNode->Left->Left->Left = DifferentiationTree ( Differentation->Left );
        DifNode->Left->Left->Right = CopyTree ( Differentation->Right );

        DifNode->Left->Right->Left = CopyTree ( Differentation->Left );
        DifNode->Left->Right->Right = DifferentiationTree ( Differentation->Right );

        DifNode->Right->Left = CopyTree ( Differentation->Right );
        DifNode->Right->Right = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Right->Right->Data ), "", 2, NUMBER );

        return DifNode;
    }

    if ( Differentation->Data.KindOfToken == NUMBER )
    {
        TreeNode* DifNode = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Data ), "", 0, NUMBER );
        return DifNode;
    }

    if ( Differentation->Data.KindOfToken == VAR )
    {
        TreeNode* DifNode = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Data ), "", 1, NUMBER );
        return DifNode;
    }

    if ( strcmp ( Differentation->Data.Symbols, "sin" ) == 0 )
    {
        TreeNode* DifNode = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Data ), "*", 0, FUNCTION );

        DifNode->Left = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Data ), "cos", 0, FUNCTION );

        DifNode->Left->Left = CopyTree ( Differentation->Left );

        DifNode->Right = NewNode ( NULL, NONE );
        DifNode->Right = DifferentiationTree ( Differentation->Left );

        return DifNode;
    }

    if ( strcmp ( Differentation->Data.Symbols, "cos" ) == 0 )
    {
        TreeNode* DifNode = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Data ), "-", 0, FUNCTION );

        DifNode->Left = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Data ), "", 0, NUMBER );

        DifNode->Right = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Right->Data ), "*", 0, FUNCTION );

        DifNode->Right->Left = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Right->Left->Data ), "sin", 0, FUNCTION );

        DifNode->Right->Left->Left = CopyTree ( Differentation->Left );

        DifNode->Right->Right = NewNode ( NULL, NONE );
        DifNode->Right->Right = DifferentiationTree ( Differentation->Left );

        return DifNode;
    }

    if ( Differentation->Data.KindOfToken == FUNCTION &&
        (Differentation->Data.Symbols[0] == '^' ) )
    {
        TreeNode* DifNode = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Data ), "*", 0, FUNCTION );

        DifNode->Left = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Data ), "*", 0, FUNCTION );

        DifNode->Right = DifferentiationTree ( Differentation->Left );

        DifNode->Left->Left = CopyTree ( Differentation->Right );

        DifNode->Left->Right = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Right->Data ), "^", 0, FUNCTION );

        DifNode->Left->Right->Left = CopyTree ( Differentation->Left );

        DifNode->Left->Right->Right = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Right->Right->Data ), "-", 0, FUNCTION );

        DifNode->Left->Right->Right->Right = NewNode ( NULL, NONE );
        CreateToken ( &( DifNode->Left->Right->Right->Right->Data ), "", 1, NUMBER );

        DifNode->Left->Right->Right->Left = CopyTree ( Differentation->Right );
        return DifNode;
    }

    return NULL;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* CopyTree ( TreeNode* Source )
{
    TreeNode* CopyNode = NewNode( NULL, NONE );
    TokenCopy ( &(CopyNode->Data), &(Source->Data) );
    if ( Source->Left != NULL ) CopyNode->Left = CopyTree ( Source->Left );
    if ( Source->Right != NULL ) CopyNode->Right = CopyTree ( Source->Right );
    return CopyNode;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* OptimizationTreeConst ( TreeNode* Expression, int* OptNum )
{
    if ( Expression == NULL )
        return NULL;

    if ( Expression->Data.KindOfToken == FUNCTION &&
       ( strcmp ( Expression->Data.Symbols, "sin" ) == 0 ||
         strcmp ( Expression->Data.Symbols, "cos" ) == 0 ) )
    {
        if ( Expression->Left == NULL )
            return NULL;
        if ( Expression->Left->Data.KindOfToken == FUNCTION )
            OptimizationTreeConst ( Expression->Left, OptNum );
        if ( Expression->Left->Data.KindOfToken == NUMBER )
            Expression = DoAriphmeticInNode ( Expression );
        return Expression;
    }

    if ( Expression->Left == NULL )
        return Expression;

    if ( Expression->Right == NULL )
        return Expression;

    if ( Expression->Left->Data.KindOfToken == NUMBER &&
         Expression->Right->Data.KindOfToken == NUMBER &&
         Expression->Left->Left   == NULL &&
         Expression->Left->Right  == NULL &&
         Expression->Right->Left  == NULL &&
         Expression->Right->Right == NULL )
    {
        (*OptNum)++;
        Expression = DoAriphmeticInNode ( Expression );
        return Expression;
    }

    if ( Expression->Left->Data.KindOfToken  == FUNCTION &&
       ( Expression->Right->Data.KindOfToken == NUMBER ||
         Expression->Right->Data.KindOfToken == VAR ) )
    {
        Expression->Left = OptimizationTreeConst ( Expression->Left, OptNum );
        return Expression;
    }

    if ( Expression->Right->Data.KindOfToken ==  FUNCTION &&
         ( Expression->Left->Data.KindOfToken == NUMBER ||
           Expression->Left->Data.KindOfToken == VAR ) )
    {
        Expression->Right = OptimizationTreeConst ( Expression->Right, OptNum );
        return Expression;
    }

    if ( Expression->Left->Data.KindOfToken == FUNCTION &&
         Expression->Right->Data.KindOfToken == FUNCTION )
    {
        Expression->Left = OptimizationTreeConst ( Expression->Left, OptNum );
        Expression->Right = OptimizationTreeConst ( Expression->Right, OptNum );
        return Expression;
    }
    return Expression;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* DoAriphmeticInNode ( TreeNode* Expression )
{
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "+" ) == 0 )
    {
        Expression->Data.KindOfToken = NUMBER;
        Expression->Data.Number = Expression->Left->Data.Number +
                                  Expression->Right->Data.Number;

        TreeDestroy( Expression->Left );
        TreeDestroy( Expression->Right );

        Expression->Left = NULL;
        Expression->Right = NULL;

        return Expression;
    }
    ///*********************************************************
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "-" ) == 0 )
    {
        Expression->Data.KindOfToken = NUMBER;
        Expression->Data.Number = Expression->Left->Data.Number -
                                  Expression->Right->Data.Number;

        TreeDestroy( Expression->Left );
        TreeDestroy( Expression->Right );

        Expression->Left = NULL;
        Expression->Right = NULL;

        return Expression;
    }
    ///*********************************************************
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "*" ) == 0 )
    {
        Expression->Data.KindOfToken = NUMBER;
        Expression->Data.Number = Expression->Left->Data.Number *
                                  Expression->Right->Data.Number;

        TreeDestroy( Expression->Left );
        TreeDestroy( Expression->Right );

        Expression->Left = NULL;
        Expression->Right = NULL;

        return Expression;
    }
    ///*********************************************************
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "/" ) == 0 )
    {
        Expression->Data.KindOfToken = NUMBER;
        Expression->Data.Number = Expression->Left->Data.Number /
                                  Expression->Right->Data.Number;

        TreeDestroy( Expression->Left );
        TreeDestroy( Expression->Right );

        Expression->Left = NULL;
        Expression->Right = NULL;

        return Expression;
    }
    ///*********************************************************
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "^" ) == 0 )
    {
        Expression->Data.KindOfToken = NUMBER;
        Expression->Data.Number = pow ( Expression->Left->Data.Number,
                                        Expression->Right->Data.Number );

        TreeDestroy( Expression->Left );
        TreeDestroy( Expression->Right );

        Expression->Left = NULL;
        Expression->Right = NULL;

        return Expression;
    }
    ///*********************************************************
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "sin" ) == 0 )
    {
        Expression->Data.KindOfToken = NUMBER;
        Expression->Data.Number = sin ( Expression->Left->Data.Number );

        TreeDestroy( Expression->Left );

        Expression->Left = NULL;

        return Expression;
    }
    ///*********************************************************
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "cos" ) == 0 )
    {
        Expression->Data.KindOfToken = NUMBER;
        Expression->Data.Number = cos ( Expression->Left->Data.Number );

        TreeDestroy( Expression->Left );

        Expression->Left = NULL;

        return Expression;
    }
    ///*********************************************************
    return 0;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

TreeNode* OptizWithDelNoMeanNode ( TreeNode* Expression, int* OptNum )
{
    if ( Expression == NULL )
        return NULL;

    if ( Expression->Data.KindOfToken == FUNCTION &&
       ( strcmp ( Expression->Data.Symbols, "sin" ) == 0 ||
         strcmp ( Expression->Data.Symbols, "cos" ) == 0 ) )
    {
        if ( Expression->Left == NULL )
            return NULL;
        if ( Expression->Left->Data.KindOfToken == FUNCTION )
            OptizWithDelNoMeanNode ( Expression->Left, OptNum );
        return Expression;
    }

    if ( Expression->Left == NULL )
        return Expression;

    if ( Expression->Right == NULL )
        return Expression;

    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "^" ) == 0 )
    {
        if ( Expression->Left->Data.KindOfToken == NUMBER &&
             Expression->Left->Data.Number == 1 )
        {
            TreeNode* NodeWithMean = Expression->Right;
            Expression->Right = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            NodeWithMean = OptizWithDelNoMeanNode ( NodeWithMean, OptNum );
            return NodeWithMean;
        }
        if ( Expression->Right->Data.KindOfToken == NUMBER &&
             Expression->Right->Data.Number == 1 )
        {
            TreeNode* NodeWithMean = Expression->Left;
            Expression->Left = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            NodeWithMean = OptizWithDelNoMeanNode ( NodeWithMean, OptNum );
            return NodeWithMean;
        }
        if ( Expression->Left->Data.KindOfToken == NUMBER &&
             Expression->Left->Data.Number == 0 )
        {
            TreeNode* NodeWithMean = Expression->Left;
            Expression->Left = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            return NodeWithMean;
        }
        if ( Expression->Right->Data.KindOfToken == NUMBER &&
             Expression->Right->Data.Number == 0 )
        {
            TreeNode* NodeWithMean = Expression->Right;
            Expression->Right = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            NodeWithMean->Data.Number = 1;
            return NodeWithMean;
        }
    }
    ///%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "*" ) == 0 )
    {
        if ( Expression->Left->Data.KindOfToken == NUMBER &&
             Expression->Left->Data.Number == 1 )
        {
            TreeNode* NodeWithMean = Expression->Right;
            Expression->Right = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            NodeWithMean = OptizWithDelNoMeanNode ( NodeWithMean, OptNum );
            return NodeWithMean;
        }
        if ( Expression->Right->Data.KindOfToken == NUMBER &&
             Expression->Right->Data.Number == 1 )
        {
            TreeNode* NodeWithMean = Expression->Left;
            Expression->Left = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            NodeWithMean = OptizWithDelNoMeanNode ( NodeWithMean, OptNum );
            return NodeWithMean;
        }
        if ( Expression->Left->Data.KindOfToken == NUMBER &&
             Expression->Left->Data.Number == 0 )
        {
            TreeNode* NodeWithMean = Expression->Left;
            Expression->Left = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            return NodeWithMean;
        }
        if ( Expression->Right->Data.KindOfToken == NUMBER &&
             Expression->Right->Data.Number == 0 )
        {
            TreeNode* NodeWithMean = Expression->Right;
            Expression->Right = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            return NodeWithMean;
        }
    }
    ///%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "+" ) == 0 )
    {
        if ( Expression->Left->Data.KindOfToken == NUMBER &&
             Expression->Left->Data.Number == 0 )
        {
            TreeNode* NodeWithMean = Expression->Right;
            Expression->Right = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            NodeWithMean = OptizWithDelNoMeanNode ( NodeWithMean, OptNum );
            return NodeWithMean;
        }
        if ( Expression->Right->Data.KindOfToken == NUMBER &&
             Expression->Right->Data.Number == 0 )
        {
            TreeNode* NodeWithMean = Expression->Left;
            Expression->Left = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            NodeWithMean = OptizWithDelNoMeanNode ( NodeWithMean, OptNum );
            return NodeWithMean;
        }
    }
    ///%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=
    if ( Expression->Data.KindOfToken == FUNCTION &&
         strcmp ( Expression->Data.Symbols, "-" ) == 0 )
    {
        if ( Expression->Left->Data.KindOfToken == NUMBER &&
             Expression->Left->Data.Number == 0 )
        {
            Expression->Left->Data.Number = -1;
            strcpy ( Expression->Data.Symbols, "*" );
            (*OptNum)++;
            Expression->Right = OptizWithDelNoMeanNode ( Expression->Right, OptNum );
            return Expression;
        }
        if ( Expression->Right->Data.KindOfToken == NUMBER &&
             Expression->Right->Data.Number == 0 )
        {
            TreeNode* NodeWithMean = Expression->Left;
            Expression->Left = NULL;
            TreeDestroy( Expression );
            (*OptNum)++;
            NodeWithMean = OptizWithDelNoMeanNode ( NodeWithMean, OptNum );
            return NodeWithMean;
        }
    }
    ///%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=%=
    if ( Expression->Data.KindOfToken == FUNCTION )
        if ( Expression->Left->Data.KindOfToken == FUNCTION )
            Expression->Left = OptizWithDelNoMeanNode ( Expression->Left, OptNum );

    if ( Expression->Data.KindOfToken == FUNCTION )
        if ( Expression->Right->Data.KindOfToken == FUNCTION )
            Expression->Right = OptizWithDelNoMeanNode ( Expression->Right, OptNum );

    return Expression;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/
TreeNode* OptimizeTree ( TreeNode* Expression )
{
    int OptimNum = 1;
    while ( OptimNum != 0 )
    {
        OptimNum = 0;
        Expression = OptimizationTreeConst ( Expression, &OptimNum );
        Expression = OptizWithDelNoMeanNode ( Expression, &OptimNum );
    }
    return Expression;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/
