section .data

message: db 'hello, world!', 13, 10, 0
size equ $ - message
Counter: db 0
TestNum: dq 0xF0F0F0F0F0F0F0F0
Result: db 0

section .text

	global _start
	
_start:

	MOV RAX, TestNum
	MOV RDI, 0
	MOVS Counter, RDI
	
LogicShift:
	
	MOV RAX, TestNum
	SAR RAX, Counter
	MOV RDI, 0x01
	AND RAX, RDI
	INC Counter
	CMP Counter, 64
	JE Exit
	CMP RAX, 0x01
	JNE LogicShift
	INC Result
	JP LogicShift
	
	MOV RAX, Result
	ADD RAX, 0x30
	MOV Result, RAX

	MOV RAX, 1
	MOV RDI, 1
	MOV RSI, Result
	MOV RDX, 1
	SYSCALL
	
	MOV RAX, 60
	XOR RDI, RDI
	SYSCALL
	
	
