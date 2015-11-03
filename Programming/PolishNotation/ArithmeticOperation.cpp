#include "stdio.h"
#include "ArithmeticOperation.h"
#include <errno.h>
//**************************************************************************
double Add ( double Var1, double Var2 )
{
    return Var1 + Var2;
}
//**************************************************************************
double Sub ( double Var1, double Var2 )
{
    return Var1 - Var2;
}
//**************************************************************************
double Mul ( double Var1, double Var2 )
{
    return Var1 * Var2;
}
//**************************************************************************
double Div ( double Var1, double Var2 )
{
    if ( Var2 == 0 )
    {
        errno = EDOM;
        return -1;
    }

    errno = 0;
    return Var1 / Var2;
}
//**************************************************************************
