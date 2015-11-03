#include "MyStack.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
//**************************************************************
MyStack* StackCreate ( int Size )
{
	MyStack* NewStack = ( MyStack* ) calloc ( 1, sizeof ( MyStack ) );
	if ( NewStack == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }
    NewStack->Size = Size;
    NewStack->Data = ( double* ) calloc ( Size, sizeof ( *NewStack->Data ) );
    if ( NewStack->Data == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }
    NewStack->Head = -1;

    errno = 0;
    return NewStack;
}
//**************************************************************
int StackResize ( MyStack* Stack, int OldSize, int NewSize )
{
    if ( IsStackValid ( Stack ) != 0 )
        return -1;

    double* Data = ( double* ) realloc ( Stack->Data, NewSize * sizeof ( *Data ) );

	if ( Data == NULL )
    {
        errno = ENOMEM;
        return -1;
    }

    if ( NewSize > OldSize )
    {
        for ( int i = 0; i < OldSize; i++ )
        {
            Data[i] = Stack->Data[i];
        }

    } else Stack->Head = NewSize - 1;
    Stack->Size = NewSize;
    free ( Stack->Data );
    Stack->Data = Data;

    errno = 0;
    return 0;
}
//**************************************************************
int StackDestroy ( MyStack* Stack )
{
    if ( IsStackValid ( Stack ) != 0 )
        return -1;

    free ( Stack->Data );
    Stack->Data = NULL;
    free ( Stack );

    errno = 0;
    return 0;
}
//**************************************************************
int IsEmptyStack ( MyStack* Stack )
{
    if ( IsStackValid ( Stack ) != 0 )
        return -1;

    if ( Stack->Head == -1 )
    {
        return -1;
    }

    errno = 0;
    return 0;
}
//**************************************************************
int IsFullStack ( MyStack* Stack )
{
    if ( IsStackValid ( Stack ) != 0 )
        return -1;

    if ( (Stack->Head) == (Stack->Size - 1) )
    {
        return -1;
    }

    errno = 0;
    return 0;
}
//**************************************************************
double PopStack ( MyStack* Stack )
{
    if ( IsEmptyStack ( Stack ) == -1 )
        return -1;

    ( Stack->Head )--;
    if ( IsStackValid ( Stack ) == -1 )
        return -1;

    errno = 0;
    return Stack->Data[( Stack->Head ) + 1];
}
//**************************************************************
int PushStack ( MyStack* Stack, double Data )
{
    if ( IsFullStack ( Stack ) == -1 )
        return -1;

    ( Stack->Head )++;
    if ( IsStackValid ( Stack ) == -1 )
        return -1;

    Stack->Data[Stack->Head] = Data;
    errno = 0;
    return 0;
}
//**************************************************************
int IsStackValid ( MyStack* Stack )
{
    if ( Stack == NULL )
    {
        errno = EFAULT;
        return -1;
    }

    if ( Stack->Data == NULL )
    {
        errno = EAGAIN;
        return -1;
    }

    if ( Stack->Head > Stack->Size )
    {
        errno = ESTCKOVRFLW;
        return -1;
    }
    if ( Stack->Head < -1 )
    {
        errno = EINVAL;
    }
    errno = 0;
    return 0;
}
//***************************************************************
int StackDump ( MyStack* Stack )
{
    freopen ( "dump.log", "a", stdout );
    time_t RawTime = time ( NULL );
    printf ( "\n\n$$$$$ Date: %s \n", ctime ( &RawTime ) );
    int IsOutStack = 0;
    switch ( errno )
    {
    case EINVAL:
        printf ( "-----> Stack head no valid!  \n\r" );
        break;
    case ESTCKOVRFLW:
        printf ( "-----> Stack overflow  \n\r" );
        break;
    case EAGAIN:
        printf ( "-----> Stack data address no valid  \n\r" );
        break;
    case EFAULT:
        printf ( "-----> Stack address no valid  \n\r" );
        break;
    case EDOM:
        printf ( "-----> Division by zero  \n\r" );
        IsOutStack = 1;
        break;
    case ENOMEM:
        printf ( "-----> No memory for expansion stack or no memory for create \n\r" );
        IsOutStack = 1;
        break;
    default:
        printf ( "-----> Unknown error  \n\r" );
        IsOutStack = 1;
    }
    if ( IsOutStack )
    {
        printf ( "The contents of the stack at address 0x%p:\n", Stack->Data );
        if ( Stack->Head == -1 )
        {
            printf ( " NO DATA " );
        }
        while ( Stack->Head != -1 )
        {
            printf ( "%lg ", PopStack ( Stack ) );
        }
    }
    printf ( "\n************************************************************************ " );
    fclose ( stdout );
    errno = 0;
    return 0;
}
//**************************************************************
