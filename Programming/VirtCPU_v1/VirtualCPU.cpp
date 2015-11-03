#include "VirtualCPU.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>

int CPU_Errno = 0;

#define DEBUG

/************************************************************/
CPU_t* CPU_Create ( int StackSize )
{
    CPU_t* NewCPU = ( CPU_t* ) calloc ( 1, sizeof ( CPU_t ) );
	if ( NewCPU == NULL )
    {
        CPU_Errno = ENOMEM;
        return NULL;
    }

    NewCPU->ProgramCounter = 0;

    NewCPU->CPUStack = StackCreate( StackSize );
    if ( NewCPU->CPUStack  == NULL )
    {
        CPU_Errno = ENOMEM;
        return NULL;
    }

    NewCPU->RAM = ( double* ) calloc ( MaxRAM, sizeof ( double ) );
    if ( NewCPU->RAM == NULL )
    {
        CPU_Errno = ENOMEM;
        return NULL;
    }

    NewCPU->CPURegister = ( double* ) calloc ( MaxRegisters, sizeof ( double ) );
    if ( NewCPU->CPURegister == NULL )
    {
        CPU_Errno = ENOMEM;
        return NULL;
    }

    CPU_Errno = 0;
    return NewCPU;
}
/************************************************************/
int CPU_Destroy ( CPU_t* Processor )
{
    if ( IsCPU_Valid ( Processor ) != 0 )
        return -1;

    free ( Processor->CPURegister ); Processor->CPURegister = NULL;
    free ( Processor->RAM ); Processor->RAM = NULL;
    Processor->ProgramCounter = 0;
    Processor->StatusReg = 0;
    StackDestroy ( Processor->CPUStack );
    free ( Processor ); Processor = NULL;
    return 0;
}
/************************************************************/
int IsCPU_Valid ( CPU_t* Processor )
{
    if ( Processor == NULL )
    {
        CPU_Errno = EFAULT;
        return -1;
    }
    if ( Processor->CPURegister == NULL )
    {
        CPU_Errno = EAGAIN;
        return -1;
    }
    if ( Processor->RAM == NULL )
    {
        CPU_Errno = EAGAIN;
        return -1;
    }
    if ( IsStackValid ( Processor->CPUStack ) != 0 )
    {
        CPU_Errno = ESTCK;
        return -1;
    }
    return 0;
}
/************************************************************/
int CPU_Dump ( CPU_t* Processor )
{
#ifdef DEBUG
    freopen ( "dump.log", "a", stdout );
    time_t RawTime = time ( NULL );
    printf ( "\n\n$$$$$ Date: %s \n", ctime ( &RawTime ) );
    int IsOutProcessor = 0;
    switch ( CPU_Errno )
    {
    case ENOMEM:
        printf ( "-----> No memory for create structures \n\r" );
        break;
    case EFAULT:
        printf ( "-----> Processors address no valid \n\r" );
        break;
    case EAGAIN:
        printf ( "-----> Other address of inside field of processors no valid \n\r" );
        break;
    case ESTCK:
        printf ( "|====> Dump stack: \n\r" );
        StackDump ( Processor->CPUStack );
        break;
    case ENOTDIR:
        printf ( "-----> No bin file! \n\r" );
        break;
    case EBADF:
        printf ( "-----> Incorrect format bin file or size = 0! \n\r" );
        break;
    case EINVAL:
        printf ( "-----> Incorrect arithmetic operation! \n\r" );
        IsOutProcessor = 1;
        break;
    default:
        printf ( "-----> Unknown error! \n\r" );
        IsOutProcessor = 1;
    }
    if ( IsOutProcessor == 1 )
    {
        printf ( "Processor at address 0x%p:\n", Processor );
        printf ( "The contents of the registers at address 0x%p:\n", Processor->CPURegister );
        for ( int i = 0; i < MaxRegisters; i++ ) printf ( "%lg ", Processor->CPURegister[i] );
        printf ( "\nThe contents of the RAM at address 0x%p:\n", Processor->RAM );
        for ( int i = 0; i < Processor->RAM[9]; i++ ) printf ( "%lg ", Processor->RAM[i] );
        printf ( "\nProgramCounter = %ld \n", Processor->ProgramCounter );
    }
    printf ( "\n************************************************************************ " );
    fclose ( stdout );
    CPU_Errno  = 0;
#endif // DEBUG
    return 0;
}
/************************************************************/
int ReadProgramToRAM ( CPU_t* Processor, const char* File )
{
    if ( IsCPU_Valid ( Processor ) != 0 )
        return -1;
    FILE* FileProgram = fopen ( File, "rb" );
    if ( FileProgram == NULL )
    {
        CPU_Errno = ENOTDIR;
        return -1;
    }
    fread ( Processor->RAM, sizeof ( double ), LengthServiceInfo - 1, FileProgram );
    /**** Check format **********************/
    if ( Processor->RAM[0] != 'E' || Processor->RAM[1] != 'A' ||
         Processor->RAM[2] != 'S' || Processor->RAM[3] != 'M' ||
         Processor->RAM[4] != 'D' || Processor->RAM[5] != 'V' ||
         Processor->RAM[6] != '1' || Processor->RAM[7] != '.' ||
         Processor->RAM[8] != '1' )
    {
        CPU_Errno = EBADF;
        return -1;
    }
    /*****************************************/
    fseek ( FileProgram, 0, SEEK_SET );
    fread ( Processor->RAM, sizeof ( double ), LengthServiceInfo, FileProgram );
    if ( Processor->RAM[9] == 0 )
    {
        //Check size program
        CPU_Errno = EBADF;
        return -1;
    }

    fseek ( FileProgram, 0, SEEK_SET );

    long int Size = ( int ) Processor->RAM[9];
    fread ( Processor->RAM, sizeof ( double ), Size, FileProgram );
    fclose ( FileProgram );

    Processor->RAM += StartMachineCode;
    Processor->ProgramCounter = 0;
    return 0;
}
/************************************************************/
int CPU_DoCommand ( CPU_t* Processor, int Command )
{
    switch ( (int)Processor->RAM[Processor->ProgramCounter] )
    {
    case ADD:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        int NumberR2 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] = Processor->CPURegister[NumberR1] +
                                           Processor->CPURegister[NumberR2];
        break;
    }
    case ADD2:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        double Number = Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] += Number;

        break;
    }
    case SUB:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        int NumberR2 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] = Processor->CPURegister[NumberR1] -
                                           Processor->CPURegister[NumberR2];
        break;
    }
    case SUB2:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        double Number = Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] -= Number;

        break;
    }
    case DIV:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        int NumberR2 = (int) Processor->RAM[Processor->ProgramCounter];

        if ( Processor->CPURegister[NumberR2] == 0 )
        {
            CPU_Errno = EINVAL;
            return -1;
        }
        Processor->CPURegister[NumberR1] = Processor->CPURegister[NumberR1] /
                                           Processor->CPURegister[NumberR2];
        break;
    }
    case DIV2:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        double Number = Processor->RAM[Processor->ProgramCounter];

        if ( Number == 0 )
        {
            CPU_Errno = EINVAL;
            return -1;
        }
        Processor->CPURegister[NumberR1] /= Number;

        break;
    }
    case MUL:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        int NumberR2 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] = Processor->CPURegister[NumberR1] *
                                           Processor->CPURegister[NumberR2];
        break;
    }
    case MUL2:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        double Number = Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] *= Number;

        break;
    }
    case MOV:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        int NumberR2 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] = Processor->CPURegister[NumberR2];

        break;
    }
    case MOV2:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        int Number = Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] = Number;

        break;
    }
    case CMP:
    {
        (Processor->ProgramCounter)++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        (Processor->ProgramCounter)++;

        int NumberR2 = (int) Processor->RAM[Processor->ProgramCounter];

        double Different = Processor->CPURegister[NumberR1] - Processor->CPURegister[NumberR2];
        Processor->StatusReg = 0;

        if ( Different > 0 )
            (Processor->StatusReg) |= MoreBit;

        if ( Different < 0 )
            (Processor->StatusReg) |= LessBit;

        if ( Different == 0 )
            (Processor->StatusReg) |= EqualBit;

        break;
    }
    case CMP2:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->ProgramCounter++;

        double Number = Processor->RAM[Processor->ProgramCounter];

        double Different = Processor->CPURegister[NumberR1] - Number;
        Processor->StatusReg = 0;

        if ( Different > 0 )
            Processor->StatusReg |= MoreBit;

        if ( Different < 0 )
            Processor->StatusReg |= LessBit;

        if ( Different == 0 )
            Processor->StatusReg |= EqualBit;

        break;
    }
    case POP:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        double Number = PopStack( Processor->CPUStack );

        if ( Number == -1 && errno != 0 )
        {
            return -1;
        }
        Processor->CPURegister[NumberR1] = Number;

        break;
    }
    case PUSH:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        if ( PushStack( Processor->CPUStack, Processor->CPURegister[NumberR1] ) == -1 )
        {
            return -1;
        };

        break;
    }
    case CALL:
    {
        Processor->ProgramCounter++;

        unsigned int CallAdress = (int) Processor->RAM[Processor->ProgramCounter];

        if ( PushStack( Processor->CPUStack, (Processor->ProgramCounter+1) ) == -1 )
        {
            return -1;
        };

        Processor->ProgramCounter = CallAdress - 1;
        break;
    }
    case JP:
    {
        Processor->ProgramCounter++;
        Processor->ProgramCounter = (Processor->RAM[Processor->ProgramCounter] - 1);

        break;
    }
    case JA:
    {
        Processor->ProgramCounter++;
        if ( ( Processor->StatusReg & ( MoreBit ) ) == MoreBit )
            Processor->ProgramCounter = (Processor->RAM[Processor->ProgramCounter] - 1);
        break;
    }
    case JB:
    {
        Processor->ProgramCounter++;
        if ( ( Processor->StatusReg & ( LessBit ) ) == LessBit )
            Processor->ProgramCounter = (Processor->RAM[Processor->ProgramCounter] - 1);
        break;
    }
    case JAE:
    {
        Processor->ProgramCounter++;
        if ( ( Processor->StatusReg & ( MoreBit ) ) == MoreBit ||
             ( Processor->StatusReg & ( EqualBit ) ) == EqualBit )
            Processor->ProgramCounter = (Processor->RAM[Processor->ProgramCounter] - 1);
        break;
    }
    case JBE:
    {
        Processor->ProgramCounter++;
        if ( ( Processor->StatusReg & ( LessBit ) ) == LessBit ||
             ( Processor->StatusReg & ( EqualBit ) ) == EqualBit )
            Processor->ProgramCounter = (Processor->RAM[Processor->ProgramCounter] - 1);
        break;
    }
    case JE:
    {
        Processor->ProgramCounter++;
        if ( ( Processor->StatusReg & ( EqualBit ) ) == EqualBit )
            Processor->ProgramCounter = (Processor->RAM[Processor->ProgramCounter] - 1);
        break;
    }
    case JZ:
    {
        /********** Not use ***********/
        break;
    }
    case JNZ:
    {
        /********** Not use ***********/
        break;
    }
    case RET:
    {
        int CallAdress = PopStack ( Processor->CPUStack );

        if ( CallAdress == -1 && errno != 0 )
        {
            return -1;
        };

        Processor->ProgramCounter = ( CallAdress - 1 );

        break;
    }
    case OUT:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        printf ( "%lg\n", Processor->CPURegister[NumberR1] );

        break;
    }
    case IN:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        scanf ( "%lg", &Processor->CPURegister[NumberR1] );

        break;
    }
    case CLR:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1] = 0;

        break;
    }
    case DEC:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1]--;

        break;
    }
    case INC:
    {
        Processor->ProgramCounter++;

        int NumberR1 = (int) Processor->RAM[Processor->ProgramCounter];

        Processor->CPURegister[NumberR1]++;

        break;
    }
    case NOP:
    {
        //
        break;
    }
    case END:
    {
        return END;
    }
    }
    return 0;
}
/************************************************************/
