/******************************************************************************
Filename    : start.S
Author      : yhy
Date        : 28/09/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The assembly entry point of the user-mode init program.
              The kernel's __RME_User_Enter srets here with:
                sp = INIT_STACK, a0 = CPUID, sstatus.SPIE set (U-mode).
              Sets up the C runtime environment (gp, .bss), then calls
              main() - the standard RISC-V freestanding bootstrap.
******************************************************************************/

/* Entry *********************************************************************/
    .section .text._start
    .align              2
    .global             _start

_start:
    /* Set up the global pointer for small-data accesses */
    .option push
    .option norelax
    LA                  gp,__global_pointer$
    .option pop

    /* Zero the .bss section */
    LA                  a1,__bss_start
    LA                  a2,__bss_end
    BEQ                 a1,a2,2f
1:
    SD                  zero,0(a1)
    ADDI                a1,a1,8
    BNE                 a1,a2,1b
2:

    /* Copy .data from load to run address would go here, but the QEMU
     * loader places the image directly at its link address, so there is
     * no LMA/VMA split - the copy is unnecessary. */

    /* Call the C main function */
    CALL                main

    /* main returned - should never happen; idle forever */
1:
    J                   1b
/* End Entry *****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
