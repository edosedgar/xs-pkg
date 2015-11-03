#ifndef VirtualCPU_H
#define VirtualCPU_H

#include "MyStack.h"

#define ESTCK 1+StackERRNOMax

extern int CPU_Errno;

const int MaxRAM = 4096;
const int MaxRegisters = 32;
const int MaxSizeStack = 100;

const unsigned int StartMachineCode = 10;

const unsigned int LengthServiceInfo = 10;

const unsigned int MoreBit = ( 1 << 2 );
const unsigned int LessBit = ( 1 << 1 );
const unsigned int EqualBit = ( 1 << 0 );

enum Mnemonics
{
    ADD,
    ADD2,
    SUB,
    SUB2,
    DIV,
    DIV2,
    MUL,
    MUL2,
    MOV,
    MOV2,
    CMP,
    CMP2,
    POP,
    PUSH,
    CALL,
    JP,
    JA,
    JB,
    JAE,
    JBE,
    JE,
    JZ,
    JNZ,
    RET,
    OUT,
    IN,
    CLR,
    DEC,
    INC,
    NOP,
    END
};

typedef struct
{
    double* RAM;
    MyStack* CPUStack;
    double* CPURegister;
    unsigned long int ProgramCounter;
    unsigned int StatusReg;
    // |.||...||>||<||=|
} CPU_t;

/**************************************************************
*
*
*
**************************************************************/
CPU_t* CPU_Create ( int StackSize );
/**************************************************************
*
*
*
**************************************************************/
int CPU_Destroy ( CPU_t* Processor );
/**************************************************************
*
*
*
**************************************************************/
int IsCPU_Valid ( CPU_t* Processor );
/**************************************************************
*
*
*
**************************************************************/
int CPU_Dump ( CPU_t* Processor );
/**************************************************************
*
*
*
**************************************************************/
int ReadProgramToRAM ( CPU_t* Processor, const char* File );
/**************************************************************
*
*
*
**************************************************************/
int CPU_DoCommand ( CPU_t* Processor, int Command );
/*************************************************************/
#endif //my
