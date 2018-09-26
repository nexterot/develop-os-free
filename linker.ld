ENTRY (loader)
LMA = 0x00100000;
SECTIONS
{
    . = LMA;
    .multiboot ALIGN (0x1000) :   {  loader.o( .text ) }
    .text      ALIGN (0x1000) :   {  *(.text)          }
    .rodata    ALIGN (0x1000) :   {  *(.rodata*)       }
    .data      ALIGN (0x1000) :   {  *(.data)          }
    .bss :                        {  *(COMMON) *(.bss) }
    /DISCARD/ :                   {  *(.comment)       }
}
