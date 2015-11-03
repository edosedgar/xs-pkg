#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include "TeX.h"

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

int OutTeXFinalDifMachineWork ( const char* FileName, TreeNode* Source )
{
    Source = OptimizeTree ( Source );

    freopen ( FileName, "w", stdout );

    printf("\\documentclass{article} \n\
\\usepackage{amsfonts,longtable,amssymb,amsmath} \n\
\\begin{document} \n\
\\begin{center} \n\
\\huge{\\textbf{The final work on informatics}} \
\\end{center} \n\
\\begin{flushright} \n\
{\\textit{Work performed: Kaziahmedov E.A.}} \n\n\n\
{\\textit{MIPT student}} \n\
\\end{flushright} \n" );

    printf("\\Large{ \n\
{{ 1) Source function: } } \n");

    printf ("$$ f(x) = {");
    ///*****************************************************************************%%
    ReccurOut ( Source );
    ///*****************************************************************************%%
    printf ( "}$$\n" );

    printf ( "{{2) After differentiation: }}\n" );
    printf ("$$ f'(x) = {");
    ///*****************************************************************************%%
    TreeNode* DiffTree = DifferentiationTree ( Source );
    ReccurOut ( DiffTree );
    ///*****************************************************************************%%
    printf ( "}$$\n" );

    printf ( "{{3) First optimization (delete constant): }}\n" );
    printf ("$$ f'(x) = {");
    ///*****************************************************************************%%
    int OptimNum = 0;
    DiffTree = OptimizationTreeConst ( DiffTree, &OptimNum );
    ReccurOut ( DiffTree );
    ///*****************************************************************************%%
    printf ( "}$$\n" );

    printf ( "{{4) Second optimization (delete not meaning number): }}\n" );
    printf ("$$ f'(x) = {");
    ///*****************************************************************************%%
    DiffTree = OptizWithDelNoMeanNode ( DiffTree, &OptimNum );
    ReccurOut ( DiffTree );
    ///*****************************************************************************%%
    printf ( "}$$\n" );

    printf ( "{{5) Finish result (after combined two optimization): }}\n" );
    printf ("$$ \\boxed { f'(x) = {");
    ///*****************************************************************************%%
    DiffTree = OptimizeTree ( DiffTree );
    ReccurOut ( DiffTree );
    ///*****************************************************************************%%
    printf ( "}}$$\n" );

    printf ( "\\Large{ \n\
{ \\textbf {References:} } \n\n\
{1) \\textit {Kernighan B., Ritchie D.} The C Programming Language (second edition) } \n\n\
{2) \\textit {Knuth D.E.} The Art of Computer Programming} \n\n\
{3) \\textit {Lvovsky S.M.} Set and layout of the system \n\
LATEX}" );
    printf ( "\\end{document}" );

    fclose( stdout );
    return 0;
}

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/

int ReccurOut ( TreeNode* Expression )
{
    if ( Expression->Data.KindOfToken == NUMBER  )
    {
        printf( "%lg", Expression->Data.Number );
        return 0;
    }

    if ( Expression->Data.KindOfToken == VAR  )
    {
        printf( "%s", Expression->Data.Symbols );
        return 0;
    }

    if ( Expression->Data.KindOfToken == FUNCTION )
    {
        if ( strcmp( Expression->Data.Symbols, "/" ) == 0 )
        {
            printf ( "\\frac{" );
            ReccurOut ( Expression->Left );
            printf ( "}" );
            printf ( "{" );
            ReccurOut ( Expression->Right );
            printf ( "} " );
        }

        if ( strcmp( Expression->Data.Symbols, "*" ) == 0 )
        {
            printf ( " {" );
            //*********************************************************
            if ( Expression->Left->Data.KindOfToken == FUNCTION &&
                (strcmp( Expression->Left->Data.Symbols, "+" ) == 0 ||
                 strcmp( Expression->Left->Data.Symbols, "-" ) == 0 ) )
            {
                printf ( "(" );
                ReccurOut ( Expression->Left );
                printf ( ")" );
            } else
                ReccurOut ( Expression->Left );
            //*********************************************************
            printf ( " } " );
            printf ( "*" );
            printf ( " { " );
            //*********************************************************
            if ( Expression->Right->Data.KindOfToken == FUNCTION &&
                (strcmp( Expression->Right->Data.Symbols, "+" ) == 0 ||
                 strcmp( Expression->Right->Data.Symbols, "-" ) == 0 ) )
            {
                printf ( "(" );
                ReccurOut ( Expression->Right );
                printf ( ")" );
            } else
                ReccurOut ( Expression->Right );
            //*********************************************************
            printf ( " } " );
        }

        if ( strcmp( Expression->Data.Symbols, "+" ) == 0 )
        {
            ReccurOut ( Expression->Left );
            printf ( "+" );
            ReccurOut ( Expression->Right );
        }

        if ( strcmp( Expression->Data.Symbols, "-" ) == 0 )
        {
            ReccurOut ( Expression->Left );
            printf ( "-" );
            if ( Expression->Right->Data.KindOfToken == FUNCTION &&
                strcmp( Expression->Right->Data.Symbols, "*" ) != 0 )
            {
                printf ( "(" );
                ReccurOut ( Expression->Right );
                printf ( ")" );
            } else
                ReccurOut ( Expression->Right );
        }

        if ( strcmp( Expression->Data.Symbols, "^" ) == 0 )
        {
            printf ( " { " );
            //*********************************************************
            if ( Expression->Left->Data.KindOfToken == FUNCTION &&
                (strcmp( Expression->Left->Data.Symbols, "+" ) == 0 ||
                 strcmp( Expression->Left->Data.Symbols, "*" ) == 0 ||
                 strcmp( Expression->Left->Data.Symbols, "/" ) == 0 ||
                 strcmp( Expression->Left->Data.Symbols, "-" ) == 0 ) )
            {
                printf ( "(" );
                ReccurOut ( Expression->Left );
                printf ( ")" );
            } else
                ReccurOut ( Expression->Left );
            //*********************************************************
            printf ( " } " );
            printf ( "^" );
            printf ( " { " );
            //*********************************************************
            if ( Expression->Right->Data.KindOfToken == FUNCTION &&
                (strcmp( Expression->Right->Data.Symbols, "+" ) == 0 ||
                 strcmp( Expression->Right->Data.Symbols, "*" ) == 0 ||
                 strcmp( Expression->Right->Data.Symbols, "/" ) == 0 ||
                 strcmp( Expression->Right->Data.Symbols, "-" ) == 0 ) )
            {
                printf ( "(" );
                ReccurOut ( Expression->Right );
                printf ( ")" );
            } else
                ReccurOut ( Expression->Right );
            //*********************************************************
            printf ( " } " );
        }

        if ( strcmp( Expression->Data.Symbols, "sin" ) == 0 )
        {
            printf ( "\\sin " );
            //printf ( " {" );
            if ( Expression->Left != NULL &&
                 Expression->Left->Data.KindOfToken != VAR )
            {
                printf ( "{(" );
                ReccurOut ( Expression->Left );
                printf ( ")}" );
            } else
                ReccurOut ( Expression->Left );
        }

        if ( strcmp( Expression->Data.Symbols, "cos" ) == 0 )
        {
            printf ( "\\cos " );
            if ( Expression->Left != NULL &&
                 Expression->Left->Data.KindOfToken != VAR )
            {
                printf ( "{(" );
                ReccurOut ( Expression->Left );
                printf ( ")}" );
            } else
            ReccurOut ( Expression->Left );
        }
    }
    return 0;
};

/****************************************************************************/
///_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@_@///
/****************************************************************************/
