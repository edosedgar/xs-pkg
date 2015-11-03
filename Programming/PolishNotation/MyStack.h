#ifndef MyStack_H
#define MyStack_H

typedef struct
{
	int Head;
	int Size;
	double* Data;
} MyStack;

/**************************************************************
*
*                   EXTERN ERRNO DEFINITION
*
**************************************************************/
#define DefaultERRNOMax 42
#define ESTCKOVRFLW 1+DefaultERRNOMax

/**************************************************************
*
*   The function StackCreate
*       \param  StackSize
*       \return 1) If stack created function return pointer
*                  on stack
*               2) If stack not created function return NULL
*                  pointer ( errno = ENOMEM )
*               3) If stack created, but not created array
*                  of Data, function return NULL
*                  pointer ( errno = ENOMEM );
*
***************************************************************/
MyStack* StackCreate ( int Size );
/**************************************************************
*
*   The function StackResize
*       \param  Stack pointer, Old Size, New Size
*       \return 1) If stack resized function return 0
*               2) If stack no valid function return -1
*                  ( see errno in f. IsStackValid )
*               3) If stack not resized, function return -1
*                  ( errno = ENOMEM );
*
***************************************************************/
int StackResize ( MyStack* Stack, int OldSize, int NewSize );
/**************************************************************
*
*   The function StackDestroy
*       \param  Stack pointer
*       \return 1) If stack destroyed function return 0
*               2) If stack no valid function return -1
*                  ( see errno in f. IsStackValid )
*
***************************************************************/
int StackDestroy ( MyStack* Stack );
/**************************************************************
*
*   The function IsEmptyStack
*       \param  Stack pointer
*       \return 1) If stack not empty function return 0
*               2) If stack empty function return 1
*               3) If stack no valid function return -1
*                  ( see errno in f. IsStackValid )
*
***************************************************************/
int IsEmptyStack ( MyStack* Stack );
/**************************************************************
*
*   The function IsFullStack
*       \param  Stack pointer
*       \return 1) If stack not full function return 0
*               2) If stack full function return 1
*               3) If stack no valid function return -1
*                  ( see errno in f. IsStackValid )
*
***************************************************************/
int IsFullStack ( MyStack* Stack );
/**************************************************************
*
*   The function PopStack
*       \param  Stack pointer
*       \return 1) Function return Data from stack
*               2) If stack no valid function return -1
*                  ( see errno in f. IsStackValid )
*               3) If stack empty function return -1
*                  ( see errno in f. IsEmptyStack )
*
***************************************************************/
double PopStack ( MyStack* Stack );
/**************************************************************
*
*   The function PushStack
*       \param  Stack pointer, Data
*       \return 1) Function move data to stack and return 0
*               2) If stack no valid function return -1
*                  ( see errno in f. IsStackValid )
*               3) If stack full function return -1
*                  ( see errno in f. IsEmptyStack )
*
***************************************************************/
int PushStack ( MyStack* Stack, double Data );
/**************************************************************
*
*   The function IsStackValid
*       \param  Stack pointer
*       \return 1) If stack valid function return 0
*               2) If stack has NULL pointer function return -1
*                  ( errno = EFAULT )
*               3) If data has NULL pointer function return -1
*                  ( errno = EAGAIN )
*               4) If stack head more then size function
*                  return -1 ( errno = EACCES )
*
***************************************************************/
int IsStackValid ( MyStack* Stack );
/**************************************************************
*
*   The function ADDst
*       \param  Stack pointer
*       \return 1) The function return 0 if function move
*                  result of adding two last elements from
*                  stack
*               2) If function return -1 see f. PushStack
*                  ( see errno in f. PushStack )
*
***************************************************************/
int ADDst ( MyStack* Stack );
/**************************************************************
*
*   The function SUBst
*       \param  Stack pointer
*       \return 1) The function return 0 if function move
*                  result of subbing two last elements from
*                  stack
*               2) If function return -1 see f. PushStack
*                  ( see errno in f. PushStack )
*
***************************************************************/
int SUBst ( MyStack* Stack );
/**************************************************************
*
*   The function MULst
*       \param  Stack pointer
*       \return 1) The function return 0 if function move
*                  result of mulling two last elements from
*                  stack
*               2) If function return -1 see f. PushStack
*                  ( see errno in f. PushStack )
*
***************************************************************/
int MULst ( MyStack* Stack );
/**************************************************************
*
*   The function DIVst
*       \param  Stack pointer
*       \return 1) The function return 0 if function move
*                  result of diving two last elements from
*                  stack
*               2) If function return -1 see f. PushStack
*                  or f. Div ( see errno in f. PushStack
*                  or f. Div )
*
***************************************************************/
int DIVst ( MyStack* Stack );
/**************************************************************/
/**************************************************************
*
*   The function StackDump
*       \param  Stack pointer
*       \return 0
*
***************************************************************/
int StackDump ( MyStack* Stack );
/**************************************************************/
#endif // MyStack
