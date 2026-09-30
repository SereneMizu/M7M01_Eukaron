/******************************************************************************
Filename    : init.ld
Author      : yhy
Date        : 30/09/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The linker script for the user-mode init program.
              QEMU loads the init binary at 0x81030000 (see qemu.sh), and
              the kernel's RME_RV64V_INIT_ENTRY points there. The init
              thread runs it in U-mode; until the Sv39 MMU is enabled the
              address is a plain VA = PA mapping in RAM.
******************************************************************************/

OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS
{
  /* QEMU virt RAM; clear of the kernel (0x80200000) and kernel objects
   * (0x81000000..0x81020000, incl. the init thread stack at 0x81020000). */
  . = 0x81030000;

  /* The entry section must come first so that the image starts exactly at
   * 0x81030000, where the kernel's RME_RV64V_INIT_ENTRY srets to. */
  .text : {
    *(.text._start)
    *(.text .text.* .gnu.linkonce.t.*)
  }

  . = ALIGN(0x8);

  .rodata : {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  . = ALIGN(0x8);

  .data : {
    *(.data .data.* .gnu.linkonce.d.*)
  }

  . = ALIGN(0x8);

  .sdata : {
    *(.sdata .sdata.*)
  }

  /* Global pointer - set by the assembly bootstrap in start.S; placed after
   * the small-data sections so gp-relative accesses stay within range. */
  . = ALIGN(0x8);
  PROVIDE(__global_pointer$ = .);

  . = ALIGN(0x8);

  .bss : {
    PROVIDE(__bss_start = .);
    *(.sbss .sbss.*)
    *(.bss .bss.*)
    PROVIDE(__bss_end = .);
  }
}

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
