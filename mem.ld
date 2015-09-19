MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
}

SECTIONS
{
  .text : ALIGN(4)
  {
    ld_sisr = . ;

    KEEP(*(.isr))

    . = ALIGN(4);

    *(.text)

    . = ALIGN(4);
    ld_etext = . ;
  } > FLASH 
}
