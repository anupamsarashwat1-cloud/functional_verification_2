    .section .init, "ax"
    .global _start
_start:
    # Set up stack pointer
    la sp, _stack_top

    # Jump to C main
    call main

    # Infinite loop if main returns
1:
    j 1b
