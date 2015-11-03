#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int Coff_A = 0;
const int Coff_B = 1;
const int Coff_C = 2;

const int MaxCoefficients = 3;
const int MaxRoots        = 2;

const int X1 = 0;
const int X2 = 1;

const int Infinum = -1;

const double ABSNeighbourhoodZero = 0.000001;

#define IsEqualFloatZero( FloatNumber ) ( fabs ( FloatNumber ) < ABSNeighbourhoodZero )

//==============================================================
/*!
 *     <i> ASSERT macros </i>
 *
 *     \brief This macros show error if condition is false \n
                <i> <u>This macros works if DEBUG defined!</i> </u>
 *            <b> Input :</b> Condition \n
 *            <b> Output:</b> Abort program or do nothing.
 *     \return                None
 *
 */
//==============================================================

#define DEBUG

#ifdef DEBUG
    #define ASSERT( cond )  if ( ! ( cond ) )                        \
                            {                                        \
                                printf ( "FAIL %s in %s, %s ( %d )", \
                                         #cond, __PRETTY_FUNCTION__, \
                                         __FILE__, __LINE__ );       \
                                abort();                             \
                            }
#else
    #define ASSERT( cond ) ;
#endif

//=================== Prototype ===================

int GetRoots ( const double Coefficients[], const int Degree, double Roots[] );
int GetCoefficients ( double Coefficients[], const int Degree );

//=================================================

int main()
{
    double Coefficients [MaxCoefficients] = {};
    double Roots [MaxRoots] = {};
    int Degree = MaxRoots+1;

    //============= Enter coefficients ================
    //======== and test on correct input data==========

    if ( GetCoefficients ( Coefficients, Degree ) == 0 )
    {
        return 0;
    }

    //==============================================

    int NumberRoots = GetRoots ( Coefficients, Degree, Roots );

    printf ( "===================\n" );

    switch ( NumberRoots )
    {
    case 0:

        printf ( "There are not number of solutions" );
        break;

    case 1:

        printf ( "X = %g", Roots[X1] );
        break;

    case 2:

        printf ( "X1 = %g\nX2 = %g", Roots[X1], Roots[X2] );
        break;

    case Infinum:

        printf ( "There is infinite number of solutions" );
        break;

    default:

        printf ( "Unknown error" );
        break;

    }
    printf ( "\n===================\n" );
    return 0;
}

//======================FUNCTIONS===============================

//==============================================================
/*!
 *
 *     \brief The function returns the polynomial solutions
 *     \param Coefficients     Array with coefficients polynomial
 *     \param Degree           Degree polynomial
 *     \param Roots            Array of solutions
 *     \return                 Number of solutions
 *
 */
//==============================================================

int GetRoots ( const double Coefficients[], const int Degree, double Roots[] )
{
    //Test on correct beginning address of array
    ASSERT ( Coefficients != NULL );
    ASSERT ( Roots != NULL );
    //=========================================

    if ( Degree > 2 )
    {
        //==========
        //Your code
        ASSERT ( !"TODO: Solve equation with degree > 2" );
        //Double negative give positive, the string more than zero, so condition is satisfied
        //==========
    } else
    {
        //======= Linear equation =======
        if ( IsEqualFloatZero ( Coefficients[Coff_A] ) && !IsEqualFloatZero ( Coefficients[Coff_B] ) )
        {
            Roots[X1] = - Coefficients[Coff_C] / Coefficients[Coff_B];
            return 1;
        }
        //===============================
        if ( !IsEqualFloatZero ( Coefficients[Coff_A] )  && IsEqualFloatZero ( Coefficients[Coff_B] ) )
        {
            if ( IsEqualFloatZero ( Coefficients[Coff_C] ) )
            {
                Roots[X1] = 0;
                return 1;
            } else

            {
                if ( - Coefficients[Coff_C] / Coefficients[Coff_A] < 0 )
                {
                    return 0;
                } else

                {
                    Roots[X1] = + sqrt ( - Coefficients[Coff_C] / Coefficients[Coff_A] );
                    Roots[X2] = - Roots[X1];
                    return 2;
                }
            }
        }
        //===============================
        if ( IsEqualFloatZero ( Coefficients[Coff_A] ) && \
             IsEqualFloatZero ( Coefficients[Coff_B] ) && \
             IsEqualFloatZero ( Coefficients[Coff_C] ) )
        {
            return Infinum;
        }
        //===============================
        double Discriminant = Coefficients[Coff_B] * Coefficients[Coff_B] -   \
                              Coefficients[Coff_A] * Coefficients[Coff_C] * 4;

        double SqrtDiscriminant = sqrt ( Discriminant );

        if ( Discriminant > 0 )
        {
            Roots[X1] = ( - Coefficients[Coff_B] - SqrtDiscriminant ) / ( 2 * Coefficients[Coff_A] );
            Roots[X2] = ( - Coefficients[Coff_B] + SqrtDiscriminant ) / ( 2 * Coefficients[Coff_A] );
            return 2;
        }
        //===============================
        if ( IsEqualFloatZero ( Discriminant ) )
        {
            Roots[X1] = - Coefficients[Coff_B] / ( 2 * Coefficients[Coff_A] );
            return 1;
        }
        //===============================
        if ( Discriminant  < 0 )
        {
            return 0;
        }
    }

    return Infinum;
}

//==============================================================
/*!
 *
 *     \brief The function returns the coefficients of equation
 *     \param Coefficients     Array for coefficients polynomial
 *     \param Degree           Degree polynomial
 *     \return                 Function returns 1 if input data entered successfully
 *
 */
//==============================================================

int GetCoefficients ( double Coefficients[], const int Degree )
{
    printf ( "Enter coefficients:\n" );

    char CoffSymbol = 'a';

    for ( int i = 0 ; i <= Degree ; i++ )
    {
        ASSERT ( i <= Degree );
        printf ( "%c = ", CoffSymbol+i );
        fflush(stdin);
        if ( ( scanf ( "%lg", &Coefficients[i] ) == 0) || ( getchar () != '\n' ) )
        {
            printf ( "===================\n" );
            printf ( "Write correct data! Example: %c = 123.4\n", CoffSymbol );
            i -= 1;
        }
    }
    return 1;
}
