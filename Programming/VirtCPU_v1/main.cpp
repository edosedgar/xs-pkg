#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include "VirtualCPU.h"

//==============================================================
/*!
 *     ASSERT macro
 *
 *     \brief  This macro show error if condition is false \n
 *             This macro works if DEBUG defined!
 *     \param  Condition
 *     \return Abort program or do nothing.
 */
//==============================================================

#define DEBUG

#ifdef DEBUG
    #define ASSERT( cond )                       \
        if ( ! ( cond ) )                        \
        {                                        \
            printf ( "FAIL %s in %s, %s ( %d )", \
            #cond, __PRETTY_FUNCTION__,          \
            __FILE__, __LINE__ );                \
            abort();                             \
        }
#else
    #define ASSERT( cond ) ;
#endif

//#define BUG

/****************************************************************/
void StartProcessor ( CPU_t* Proccesor );
/****************************************************************/

int main ( int argc, char* argv[] )
{
    CPU_t* EmCPU = CPU_Create ( MaxSizeStack );

#ifndef BUG
    if ( argc > 1 )
    {
        if ( ReadProgramToRAM ( EmCPU, argv[1] ) != 0 )
        {
            CPU_Dump ( EmCPU );
            return 0;
        }
    } else
    {
        if ( CPU_Destroy ( EmCPU ) != 0 ) CPU_Dump ( EmCPU );
        return 0;
    }
#else
    if ( ReadProgramToRAM ( EmCPU, "C:/Users/EdgarAdmin/Documents/Repositories/Informatic/Hometask4_2/bin/Debug/program.bin" ) != 0 )
    {
        CPU_Dump ( EmCPU );
        return 0;
    }
#endif
    StartProcessor ( EmCPU );

    if ( CPU_Destroy ( EmCPU ) != 0 ) CPU_Dump ( EmCPU );
    return 0;
}
/****************************************************************/
/****************************************************************/
/****************************************************************/
void StartProcessor ( CPU_t* Proccesor )
{
    int ResultOperation = 0;
    while ( ResultOperation != END )
    {
        ResultOperation = CPU_DoCommand ( Proccesor, Proccesor->RAM[Proccesor->ProgramCounter] );
        if ( ResultOperation == -1 )
        {
            CPU_Dump ( Proccesor );
            ResultOperation = END;
        }
        Proccesor->ProgramCounter++;
    }
}
/****************************************************************/
/****************************************************************/
/****************************************************************/
