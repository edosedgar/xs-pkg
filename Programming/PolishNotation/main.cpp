#include <stdio.h>
#include <stdlib.h>
#include "MyStack.h"
#include "ArithmeticOperation.h"
#include "errno.h"

const int StackSize = 100;

const int IsOperation = 1;
const int IsNumber = 2;
const int IsResult = 3;
const int IsError = -1;

//======================== Prototype ==========================
int GetElementFromInput ( double* Var, char* Action );
//=============================================================

int main()
{
    MyStack* UserStack = StackCreate ( StackSize );
    if ( UserStack == NULL ) StackDump ( UserStack );

    char Action = ' ';
    double Var1 = 0;

    printf ( "Enter reverse polish notation: \n" );
    //========================================================
    int Select = 0;
    while ( Select != IsResult && Select != IsError )
    {
        Select =  GetElementFromInput ( &Var1, &Action );
        switch ( Select )
        {
        case IsOperation:

            switch ( Action )
            {
            case '+':

                if ( ADDst ( UserStack ) != 0 ) StackDump ( UserStack );
                break;

            case '-':

                if ( SUBst ( UserStack ) != 0 ) StackDump ( UserStack );
                break;

            case '*':

                if ( MULst ( UserStack ) != 0 ) StackDump ( UserStack );
                break;

            case '/':

                if ( DIVst ( UserStack ) != 0 )
                {
                    printf ( "Dividing by zero!" );
                    StackDump ( UserStack );
                    Select = IsError;
                }
                break;
            }
            break;

        case IsNumber:

            if ( PushStack ( UserStack, Var1 ) != 0 ) StackDump ( UserStack );
            break;

        case IsResult:

            Var1 = PopStack ( UserStack );
            if ( Var1 != 0 && errno != 0 ) StackDump ( UserStack );
            else printf ( "%lg", Var1 );
            break;

        case IsError:

            printf ( "Error inputting data" );
            break;
        };
    }
    StackDestroy ( UserStack );
    return 0;
}

//**************************************************************************
int GetElementFromInput ( double* Var, char* Action )
{
    char Symbol;
    Symbol = fgetc ( stdin );
    switch ( Symbol )
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        char* Number = ( char* ) calloc ( 100, sizeof ( char ) );
        int i = 0;
        while ( Symbol != ' ' && Symbol != '\0' )
        {
            Number[i] = Symbol;
            Symbol = fgetc ( stdin );
            i++;
        }
        *Var = atoi ( Number );
        free ( Number );
        return IsNumber;
    }
    case '-':
    case '+':
    case '*':
    case '/':
        *Action = Symbol;
        fgetc ( stdin );
        return IsOperation;
    case '=':
        return IsResult;
    default:
        return IsError;
    }
    return 0;
}
//**************************************************************************
