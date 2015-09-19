#ifndef VIRCOMMNEM_H
#define VIRCOMMNEM_H

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// Mnemomics uses on Virtual Processors
typedef enum 
{
	ADD,	ADD2,	SUB,
	SUB2,	DIV,	DIV2,
	MUL,	MUL2,	MOV,
	MOV2,	CMP,	CMP2,
	POP,	PUSH,	CALL,
	JP,		JA,		JB,
	JAE,	JBE,	JE,
	JZ,		JNZ,	RET,
	OUT,	IN,		CLR,
	DEC,	INC,	NOP,
	END
} TVirtCom;

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

#endif
