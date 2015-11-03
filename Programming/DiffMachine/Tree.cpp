#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <locale.h>
#include "Tree.h"
#include <time.h>

#define DEBUG

/****************************************************************************/
TreeNode* NewNode ( TreeNode* Parent, NodeType Mode )
{
    TreeNode* Node = ( TreeNode* ) calloc ( 1, sizeof ( TreeNode ) );
    if ( Node == NULL )
    {
        errno = ETNOMEM;
        return NULL;
    }
    switch ( Mode )
    {
    case LEFT:
        Parent->Left = Node;
        break;

    case RIGHT:
        Parent->Right = Node;
        break;

    default:
        break;
    }
    Node->Parent = Parent;
    if ( IsTreeValid ( Node ) == 0 )
        return Node;
    else
        return NULL;
}
/****************************************************************************/
int IsTreeValid ( TreeNode* ParentNode )
{
#ifdef DEBUG
    //****************************
    if ( ParentNode->Left != NULL )
    {
        if ( ParentNode != ParentNode->Left->Parent )
        {
            errno = ETSTRUCTERR;
            return -1;
        }
    }
    //****************************
    if ( ParentNode->Right != NULL )
    {
        if ( ParentNode != ParentNode->Right->Parent )
        {
            errno = ETSTRUCTERR;
            return -1;
        }
    }
    //****************************
    if ( ParentNode->Parent != NULL )
    {
        if ( ParentNode->Parent->Left != ParentNode )
        {
            errno = ETSTRUCTERR;
            return -1;
        }

        if ( ParentNode->Parent->Right != ParentNode )
        {
            errno = ETSTRUCTERR;
            return -1;
        }
    }
    //****************************
    if ( ParentNode->Left != NULL && IsTreeValid ( ParentNode->Left ) == -1 )
        return -1;
    //****************************
    if ( ParentNode->Right != NULL && IsTreeValid ( ParentNode->Right ) == -1 )
        return -1;
    //****************************
    return 0;
#endif
}
/****************************************************************************/
int TreeDump ( TreeNode* ParentNode )
{
#ifdef DEBUG
    freopen ( "dump.log", "a", stdout );
    time_t RawTime = time ( NULL );
    printf ( "\n/****************************************************************************/\n");
    printf ( "\n$$$$$ Date: %s \n", ctime ( &RawTime ) );
    int IsTreeOut = 0;
    switch ( errno )
    {
    case ETSTRUCTERR:
        printf ( "-----> Tree has defect in structure! \n" );
        break;
    case ETNOVAL:
        printf ( "-----> Tree address no valid! \n" );
        break;
    case ETNOMEM:
        printf ( "-----> No memory for create tree! \n" );
        break;
    default:
        printf ( "-----> Unknown error  \n" );
        IsTreeOut = 1;
    }

    if ( IsTreeOut )
        TreeOutInFile ( ParentNode );

    printf ( "\n/****************************************************************************/\n");
    errno = 0;
    fclose ( stdout );
#endif // DEBUG
    return 0;
}
/****************************************************************************/
int TreeOutInFile ( TreeNode* ParentNode )
{
    switch ( ParentNode->Data.KindOfToken )
    {
    case NUMBER:
        printf ( "%lg", ParentNode->Data.Number );
        break;
    default:
        printf ( "'%s'", ParentNode->Data.Symbols );
    }

    printf ( " { " );

    if ( ParentNode->Left != NULL )
        TreeOutInFile ( ParentNode->Left );
    else
        printf ( " NULL " );

    printf ( " } " );
    printf ( " { " );

    if ( ParentNode->Right != NULL )
        TreeOutInFile ( ParentNode->Right );
    else
        printf ( " NULL " );

    printf ( " } " );
    return 0;
}
/****************************************************************************/
int TreeDestroy ( TreeNode* ParentNode )
{
    if ( IsTreeValid ( ParentNode ) != 0 )
        return -1;

    if ( ParentNode->Left != NULL )
    {
        TreeDestroy ( ParentNode->Left );
        free( ParentNode->Left );
    }

    if ( ParentNode->Right != NULL )
    {
        TreeDestroy ( ParentNode->Right );
        free( ParentNode->Right );
    }

    free ( ParentNode );

    return 0;
}
/****************************************************************************/
/****************************************************************************/
