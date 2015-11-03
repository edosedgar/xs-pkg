#ifndef Tree_H
#define Tree_H

/***************************************************************************/

enum EKindOfToken
{
    EMPTY,
    ENDSTR,
    OPERATOR,
    NUMBER,
    FUNCTION,
    BRACKET,
    VAR
};

struct Token
{
    char* Symbols;
    double Number;
    EKindOfToken KindOfToken;
};

typedef Token data_t;

struct TreeNode
{
    data_t Data;
    TreeNode* Parent;
    TreeNode* Left;
    TreeNode* Middle;
    TreeNode* Right;
};

enum NodeType
{
    NONE,
    LEFT,
    MIDDLE,
    RIGHT
};

const int TreeERNOStartCode = 42;

enum TreeERNO
{
    ETSTRUCTERR = TreeERNOStartCode,
    ETNOMEM,
    ETNOVAL
};

/**************************************************************
*
*
*
**************************************************************/
TreeNode* NewNode ( TreeNode* Parent, NodeType Mode );
/**************************************************************
*
*
*
**************************************************************/
int IsTreeValid ( TreeNode* ParentNode );
/**************************************************************
*
*
*
**************************************************************/
int TreeDump ( TreeNode* ParentNode );
/**************************************************************
*
*
*
**************************************************************/
int TreeOutInFile ( TreeNode* ParentNode );
/**************************************************************
*
*
*
**************************************************************/
int TreeDestroy ( TreeNode* ParentNode );
/***************************************************************************/
#endif // Tree
