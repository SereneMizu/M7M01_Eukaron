/******************************************************************************
Filename    : init.c
Author      : yhy
Date        : 30/09/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The first user-mode program (init) for the RV64V port on
              QEMU virt. Runs in U-mode as the init thread, entered by the
              kernel via __RME_User_Enter. Prints the uppercase alphabet
              (A-Z) on a single line through the DEBUG_PRINT kernel-function
              capability syscall, then idles in a busy loop (WFI is an
              illegal instruction on QEMU).
******************************************************************************/

/* Include *******************************************************************/
#include "rme_syscall.h"
/* End Include ***************************************************************/

/* Function:Print_Char ********************************************************
Description : Print one character by invoking the DEBUG_PRINT kernel function.
              Param0 packs D0 = Func_ID (DEBUG_PRINT) and D1 = Sub_ID (the character).
Input       : char Ch - The character to print.
Output      : None.
Return      : None.
******************************************************************************/
static void Print_Char(char Ch)
{
    rme_ptr_t Param0 = (((rme_ptr_t)(unsigned char)Ch) << 32) |
                       (rme_ptr_t)RME_KFN_DEBUG_PRINT;

    (void)rme_syscall(RME_SVC_KFN, RME_BOOT_INIT_KFN, Param0, 0U, 0U);
}
/* End Function:Print_Char ***************************************************/

/* Function:Print_Str *********************************************************
Description : Print a NUL-terminated string, one syscall per character.
Input       : const char* Str - The string to print.
Output      : None.
Return      : None.
******************************************************************************/
static void Print_Str(const char* Str)
{
    while(*Str != 0)
    {
        Print_Char(*Str);
        Str++;
    }
}
/* End Function:Print_Str ****************************************************/

/* Function:main **************************************************************
Description : The entry point of the init program. The assembly bootstrap in
              start.S sets up the C runtime environment, then calls this
              function. Prints the uppercase alphabet, then idles forever.
Input       : None.
Output      : None.
Return      : None. This function never returns.
******************************************************************************/
void main(void)
{
    char Ch;

    /* Print the uppercase alphabet A-Z on a single line */
    for(Ch='A'; Ch<='Z'; Ch++)
    {
        Print_Char(Ch);
    }
    Print_Str("\r\n");

    /* Idle forever - busy loop, no WFI (illegal instruction on QEMU) */
    for(;;)
    {
    }
}
/* End Function:main *********************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
