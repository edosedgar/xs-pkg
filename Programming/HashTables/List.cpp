#include "List.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define DEBUG

/****************************************************************************/
ListHead* ListCreate ( )
{
    ListHead* NewListHead = ( ListHead* ) calloc ( 1, sizeof ( ListHead ) );
    if ( NewListHead == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    ( NewListHead->CurElement ) = ( ListElement* ) calloc ( 1, sizeof ( ListElement ) );
    if ( NewListHead->CurElement == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    NewListHead->NumElement = 1;
    NewListHead->CurElement->Data = NULL;
    NewListHead->CurElement->Prev = NULL;
    NewListHead->CurElement->Next = NULL;

    NewListHead->Start = NewListHead->CurElement;
    NewListHead->End   = NewListHead->CurElement;
    return NewListHead;
}
/****************************************************************************/
int ListDestroy ( ListHead* List )
{
    if ( IsListValid ( List ) != 0 )
        return -1;

    List->CurElement = List->Start;

    while ( List->CurElement != NULL )
    {
        ListElement* Buffer = NULL;
        Buffer = List->CurElement->Next;

        List->CurElement->Data = NULL;
        List->CurElement->Next = NULL;
        List->CurElement->Prev = NULL;

        free ( List->CurElement );
        List->CurElement = Buffer;
    }

    List->Start = NULL;
    List->End = NULL;
    List->NumElement = 0;

    free ( List );
    return 0;
}
/****************************************************************************/
int AddElement ( ListHead* List, data_t Data )
{
    if ( IsListValid ( List ) != 0 )
        return -1;

    ListElement* NewElem = ( ListElement* ) calloc ( 1, sizeof ( ListElement ) ) ;
    if ( List->CurElement->Next != NULL )
    {
        ListElement* Buffer = List->CurElement->Next;
        List->CurElement->Next = NewElem;
        NewElem->Prev = List->CurElement;
        NewElem->Next = Buffer;
        NewElem->Next->Prev = NewElem;
    }
    else
    {
        List->CurElement->Next = NewElem;
        NewElem->Prev = List->CurElement;
        NewElem->Next = NULL;
        List->End = NewElem;
    }

    List->CurElement = List->CurElement->Next;
    List->CurElement->Data = Data;

    ( List->NumElement )++;
    return 0;
}
/****************************************************************************/
int DelElement ( ListHead* List )
{
    if ( IsListValid ( List ) != 0 )
        return -1;

    if  ( List->NumElement == 1 )
    {
        errno = EACCES;
        return -1;
    }

    if ( List->CurElement == List->Start )
    {
        errno = EACCES;
        return -1;
    }

    if ( List->CurElement->Next != NULL )
    {
        ListElement* Buffer = List->CurElement;
        List->CurElement->Next->Prev = List->CurElement->Prev;
        List->CurElement->Prev->Next = List->CurElement->Next;

        List->CurElement = List->CurElement->Prev;
        Buffer->Data = NULL;
        Buffer->Next = NULL;
        Buffer->Prev = NULL;

        free ( Buffer );
    }
    else
    {
        List->CurElement = List->CurElement->Prev;
        List->CurElement->Next->Data = NULL;
        List->CurElement->Next->Next = NULL;
        List->CurElement->Next->Prev = NULL;

        free ( List->CurElement->Next );
        List->CurElement->Next = NULL;
        List->End = List->CurElement;
    }

    ( List->NumElement )--;
    return 0;
}
/****************************************************************************/
int ListDump ( ListHead* List )
{
#ifdef DEBUG
    freopen ( "dump.log", "a", stdout );
    time_t RawTime = time ( NULL );
    printf ( "\n/****************************************************************************/\n");
    printf ( "\n$$$$$ Date: %s \n", ctime ( &RawTime ) );
    int IsOutList = 0;
    switch ( errno )
    {
    case ELSTERRSTRCT:
        printf ( "-----> List has defect in structure! \n" );
        break;
    case EFAULT:
        printf ( "-----> List address no valid! \n" );
        break;
    case ENOMEM:
        printf ( "-----> No memory for create list! \n" );
        break;
    case EACCES:
        printf ( "-----> Read zero element in list! \n" );
        IsOutList = 1;
        break;
    default:
        printf ( "-----> Unknown error  \n" );
        IsOutList = 1;
    }
    if ( IsOutList )
    {
        ListElement* Buffer = List->CurElement;
        printf ( "The list at address:            0x%p \n", List );
        printf ( "The address on first element:   0x%p \n", List->Start );
        printf ( "The address on last element:    0x%p \n", List->End );
        printf ( "The address on current element: 0x%p \n", List->CurElement );
        printf ( "The number element of list: %d \n", List->NumElement );
        List->CurElement = List->Start;
        while ( List->CurElement != NULL )
        {
            printf ( "|+++++++++++++++++++++++++++++++++++++| \n" );
            printf ( "Address on element: 0x%p \n", List->CurElement );
            printf ( "Data: " );
            if ( List->CurElement->Data == NULL )
                printf ( "No data \n" );
            else
                printf ( "%s \n", List->CurElement->Data );
            printf ( "Prev address: 0x%p \n", List->CurElement->Prev );
            printf ( "Next address: 0x%p \n", List->CurElement->Next );
            List->CurElement = List->CurElement->Next;
            printf ( "|+++++++++++++++++++++++++++++++++++++| \n" );
        }
        List->CurElement = Buffer;
    }
    printf ( "\n/****************************************************************************/\n");
    errno = 0;
    fclose ( stdout );
#endif // DEBUG
    return 0;
}
/****************************************************************************/
int IsListValid ( ListHead* List )
{
    if ( List == NULL )
    {
        errno = EFAULT;
        return -1;
    }
    if ( List->NumElement == 0 )
    {
        errno = ELSTERRSTRCT;
        return -1;
    }
    if ( List->CurElement == NULL )
    {
        errno = ELSTERRSTRCT;
        return -1;
    }
    if ( List->End == NULL || List->Start == NULL )
    {
        errno = ELSTERRSTRCT;
        return -1;
    }
    ListElement* Buffer = List->CurElement;
    List->CurElement = List->Start;

    int Count = ( List->NumElement ) - 1;

    while ( List->CurElement != NULL )
    {
        if ( List->CurElement != List->End &&
             List->CurElement != List->CurElement->Next->Prev )
        {
            errno = ELSTERRSTRCT;
            return -1;
        }
        if ( List->CurElement != List->Start &&
             List->CurElement != List->CurElement->Prev->Next )
        {
            errno = ELSTERRSTRCT;
            return -1;
        }

        if ( List->CurElement != List->End &&
             List->CurElement->Data != List->CurElement->Next->Prev->Data )
        {
            errno = ELSTERRSTRCT;
            return -1;
        }
        if ( List->CurElement != List->Start &&
             List->CurElement->Data != List->CurElement->Prev->Next->Data )
        {
            errno = ELSTERRSTRCT;
            return -1;
        }
        if ( List->CurElement->Data == NULL && List->CurElement != List->Start )
        {
            errno = EFAULT;
            return -1;
        }
        List->CurElement = ( List->CurElement->Next );
        Count--;
    }
    List->CurElement = Buffer;
    return 0;
}
/****************************************************************************/
int ShiftCurElement ( ListHead* List, int Shift )
{
    ListElement* Buffer = List->CurElement;
    if ( Shift > 0 )
    {
        while ( Shift != 0 )
        {
            if ( List->CurElement->Next == NULL )
            {
                List->CurElement = Buffer;
                return 0;
            }
            Shift--;
            List->CurElement = List->CurElement->Next;
        }
    }
    if ( Shift < 0 )
    {
        while ( Shift != 0 )
        {
            if ( List->CurElement->Prev == NULL )
            {
                List->CurElement = Buffer;
                return 0;
            }
            Shift++;
            List->CurElement = List->CurElement->Prev;
        }
    }
    return 0;
}
/****************************************************************************/
int IsElementInList ( ListHead* List, char* Element )
{
    ListElement* Buffer = List->CurElement;
    List->CurElement = List->Start->Next;

    while ( List->CurElement != NULL )
    {
        if ( strcmp ( Element, List->CurElement->Data ) == 0 )
        {
            List->CurElement = Buffer;
            return 1;
        }
        List->CurElement = List->CurElement->Next;
    }
    List->CurElement = Buffer;
    return 0;
}
/****************************************************************************/

