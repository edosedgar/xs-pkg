#ifndef VirtualCPU_H
#define VirtualCPU_H

#include "stdint.h"

#include "MyStack.h"

#define ESTCK 1+StackERRNOMax

extern int CPU_Errno;

extern uint32_t MaxRAM ;
const uint8_t MaxRegisters = 32;
extern uint32_t MaxSizeStack;

const uint16_t StartMachineCode = 10;

const uint16_t LengthServiceInfo = 10;

const uint8_t MoreBit = ( 1 << 2 );
const uint8_t LessBit = ( 1 << 1 );
const uint8_t EqualBit = ( 1 << 0 );

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

typedef uint32_t MemoryType;

typedef struct
{
    MemoryType* RAM;
    MyStack* CPUStack;
    MemoryType* CPURegister;
    uint32_t ProgramCounter;
    uint8_t StatusReg;
    // |.||...||>||<||=|
} CPU_t;

/**************************************************************
*
*
*
**************************************************************/
CPU_t* CPU_Create ( uint32_t StackSize );
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
int CPU_DoCommand ( CPU_t* Processor, uint16_t Command );
/*************************************************************/
#endif //my
