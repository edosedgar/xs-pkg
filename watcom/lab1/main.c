#include <stdio.h>

typedef unsigned int uint32;
typedef unsigned short int uint16;

#define CR0_PE 0x1
#define CR0_PG (1<<31) //0x80000000

void main()
{
    uint32 _cr0 = 0;
    uint16 _cs = 0;
    __asm {
        mov eax, cr0
        mov _cr0, eax
        mov ax, cs
        mov _cs, ax

        //xor ax, ax
        //mov cs, ax
    }
    printf("0x%08X - %s, %s \n",
        _cr0,
        _cr0 & CR0_PE ? "Protected mode" : "Real mode",
        _cr0 & CR0_PG ? "Paging on" : "Paging off"
    );
    printf("0x%08X - %s \n",
        (int)_cs,
        ((_cs & 0x03)==0) ? "Ring 0" : "Ring BAD-BAD"
    );
}
