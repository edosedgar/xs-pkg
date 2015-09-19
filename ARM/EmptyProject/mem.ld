SECTIONS
{
  . = 0x08000000;
  .text : 
  { 
    KEEP(*(.isr))
    *(.text);
  }  
}
