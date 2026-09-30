/******************************************************************************
Filename    : rme_platform_qemu_virt.ld
Author      : yhy
Date        : 29/09/2026
Licence     : LGPL v3+; see COPYING for details.
Description : The linker script for the RV64V platform on QEMU virt.
              QEMU virt RAM starts at 0x80000000; OpenSBI loads the kernel
              at 0x80200000. Until the MMU is enabled, VA = PA holds, so the
              kernel is linked directly at the load address. The symbols used
              by rme_platform_rv64v_gcc.s are provided at the bottom.
******************************************************************************/

OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS
{
  /* QEMU virt: kernel loaded by OpenSBI at 0x80200000, VA = PA */
  . = 0x80200000;

  PROVIDE(begin = .);

  /* The entry section must come first so that the ELF entry point lands
   * exactly at 0x80200000, where OpenSBI jumps to. */
  .text : {
    *(.text._start)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  PROVIDE(etext = .);

  . = ALIGN(0x1000);

  .rodata : {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  . = ALIGN(0x1000);

  PROVIDE(data = .);

  .data : {
    *(.data .data.*)
  }

  . = ALIGN(0x1000);

  PROVIDE(edata = .);

  .sdata : {
    *(.sdata .sdata.*)
  }

  . = ALIGN(0x1000);

  .bss : {
    *(.bss .bss.* COMMON)
  }

  . = ALIGN(0x1000);
  PROVIDE(end = .);

  /* Reserve the kernel stack - grows downwards from __RME_Stack */
  . = ALIGN(16);
  __RME_Stack_Base = .;
  . += 0x10000;
  __RME_Stack = .;

  /DISCARD/ : {
    *(.eh_frame .rela.eh_frame .note.*)
  }
}

/* Symbols referenced by rme_platform_rv64v_gcc.s */
__RME_Global     = ADDR(.data);
__RME_Data_Load  = LOADADDR(.data);
__RME_Data_Start = ADDR(.data);
__RME_Data_End   = ADDR(.data) + SIZEOF(.data);
__RME_Zero_Start = ADDR(.bss);
__RME_Zero_End   = ADDR(.bss) + SIZEOF(.bss);

/* End Of File ***************************************************************/
