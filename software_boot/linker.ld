OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
  /* Simulate SRAM mapped at 0x00020000 to match TITAN-X core RESET_PC */
  ram (wxa) : ORIGIN = 0x00020000, LENGTH = 32K
}

SECTIONS
{
  .text : {
    *(.init)
    *(.text*)
  } > ram

  .data : {
    *(.data*)
    *(.sdata*)
  } > ram

  .bss : {
    *(.bss*)
    *(.sbss*)
  } > ram

  . = ALIGN(8);
  _stack_top = ORIGIN(ram) + LENGTH(ram);
}
