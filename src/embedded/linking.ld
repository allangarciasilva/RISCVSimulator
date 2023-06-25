MEMORY 
{
    RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 256K
}

SECTIONS
{
    PROVIDE( _stack_ptr = ORIGIN(RAM) + LENGTH(RAM));

    .text :
    {
        _text = .;
        *(.text.prologue)
        *(.text*)
        _etext = .;
    }
    
    .data : 
    {
        _data = .;
        *(.rodata*)
        *(.got*)
        *(.data*)
        _edata = .;
    }
    
    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)      
        _ebss = .;
    }
}
