ENTRY(kernel_main)

SECTIONS {
    . = 0x100000; /* 1MB */
    .text : {
        *(.multiboot)
        *(.text*)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
}