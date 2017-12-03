#include "stdio.h"

typedef unsigned int uint32;
typedef unsigned short int uint16;

#define CR0_PE 0x1
#define CR0_PG (1<<31) //0x80000000

#pragma pack (push, 1)
typedef struct _DTR {
    uint16 limit;
    uint32 base;
    uint16 _padding;
} DTR, *PDTR;

typedef union _SELECTOR {
    uint16 raw;
    struct {
        uint16 pl:2;
        uint16 table:1;
        uint16 index:13;
    };
} SELECTOR, *PSELECTOR;

typedef struct _SYSINFO {
    SELECTOR cs;
    uint32 cr0;
    DTR gdt;
    DTR idt;
    SELECTOR ldt;
    SELECTOR tr;
} SYSINFO, *PSYSINFO;

void get_sysinfo(PSYSINFO psysinfo)
{
    uint16 _cs;
    uint32 _cr0;
    PDTR _gdt = &psysinfo->gdt;
    PDTR _idt = &psysinfo->idt;
    uint16 _ldt;
    uint16 _tr;

    __asm {
        mov eax, cr0
        mov _cr0, eax
        mov ax, cs
        mov _cs, ax

        mov eax, _gdt
        sgdt [eax]
        mov eax, _idt
        sidt [eax]
        sldt _ldt
        str _tr

        //xor ax, ax
        //mov cs, ax
    }

    psysinfo->cr0 = _cr0;
    psysinfo->cs.raw = _cs;
    psysinfo->ldt.raw = _ldt;
    psysinfo->tr.raw = _tr;
}

SYSINFO sysinfo;

void main()
{
    printf("Hello, world! \n");
    get_sysinfo(&sysinfo);

    printf("0x%08X - %s, %s \n",
        sysinfo.cr0,
        sysinfo.cr0 & CR0_PE ? "Protected mode" : "Real mode",
        sysinfo.cr0 & CR0_PG ? "Paging on" : "Paging off"
    );
    printf("0x%08X - %s \n",
        (int)sysinfo.cs.raw,
        (sysinfo.cs.pl ==0) ? "Ring 0" : "Ring BAD-BAD"
    );

    printf("================ \n");
    printf("GDT: base=0x%08X limit=0x%04X \n", sysinfo.gdt.base, sysinfo.gdt.limit);
    printf("IDT: base=0x%08X limit=0x%04X \n", sysinfo.idt.base, sysinfo.idt.limit);
    printf("LDT: selector=0x%04X \n", sysinfo.ldt.raw);
    printf("TR: selector=0x%04X \n", sysinfo.tr.raw);

}
