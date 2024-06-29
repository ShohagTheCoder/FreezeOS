ENTRY(main)
SECTIONS
{
    . = 0x100; /* Load the kernel at 4KB */
    .text :
    {
        *(.text)
    }
    .rodata :
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}
