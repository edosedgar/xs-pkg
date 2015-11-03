#ifndef Calculator_H
#define Calculator_H

#include "Tree.h"

/*******************************************************/

const int NumOfMathFunc = 8;

const int MaxLengthFunc = 10;

struct Var
{
    char* Name;
    double Data;
};

extern const char* MathFunctionTable [ NumOfMathFunc ];

/**************************************************************
*
*
*
**************************************************************/
Token* StringToToken ( char* Expression );
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
int IsSpace ( char Symbol );
/**************************************************************
*
*
*
**************************************************************/
int IsDigit ( char Symbol );
/**************************************************************
*
*
*
**************************************************************/
int IsEnd ( char Symbol );
/**************************************************************
*
*
*
**************************************************************/
int IsArOper ( char Symbol );
/**************************************************************
*
*
*
**************************************************************/
int IsMathFunction ( char* String );
/**************************************************************
*
*
*
**************************************************************/
int ExpressionDump ( Token* Express );
/**************************************************************
*
*
*
**************************************************************/
int SyntaxCheck ( Token* Express );
/**************************************************************
*
*
*
**************************************************************/
TreeNode* ExpressionToTree ( Token* Express );
    ///******* Set of rules ************
    TreeNode* CallPlusMinus ( );/// +-
    TreeNode* CallMulDiv    ( );/// */
    TreeNode* CallSinCos    ( );/// Sin Cos
    TreeNode* CallDegr      ( );/// ^
    TreeNode* CallBrac      ( );/// ()
    TreeNode* CallNum       ( );/// num, var
    ///*********************************
/**************************************************************
*
*
*
**************************************************************/
int TokenCopy ( Token* First, Token* Second );
/**************************************************************
*
*
*
**************************************************************/
TreeNode* DifferentiationTree ( TreeNode* Differentation );
/**************************************************************
*
*
*
**************************************************************/
int CreateToken ( Token* NewToken, const char* Data, double Num, EKindOfToken KindOfToken );
/**************************************************************
*
*
*
**************************************************************/
TreeNode* CopyTree ( TreeNode* Source );
/**************************************************************
*
*
*
**************************************************************/
TreeNode* OptimizationTreeConst ( TreeNode* Expression, int* OptNum );
/**************************************************************
*
*
*
**************************************************************/
TreeNode* DoAriphmeticInNode ( TreeNode* Expression );
/**************************************************************
*
*
*
**************************************************************/
TreeNode* OptizWithDelNoMeanNode ( TreeNode* Expression, int* OptNum );
/**************************************************************
*
*
*
**************************************************************/
TreeNode* OptimizeTree ( TreeNode* Expression );
/*************************************************************/


#endif // Calculator_H
