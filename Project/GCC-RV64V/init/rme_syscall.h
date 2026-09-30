/******************************************************************************
Filename    : rme_syscall.h
Author      : yhy
Date        : 30/09/2026
Licence     : The Unlicense; see LICENSE for details.
Description : User-mode system call interface for the RV64V port on QEMU
              virt. Mirrors the kernel's syscall ABI and constants
              (Include/rme.h) without depending on kernel headers, so any
              user program can include it standalone.
              Syscall ABI on RV64V:
                a0 = (Svc_Num << 16) | Cid
                a1 = Param0, a2 = Param1, a3 = Param2
                ecall                 (cause 8, delegated to S-mode by OpenSBI)
                return value comes back in a0
              rme_syscall() is the ONLY ecall in a user program - everything
              else is built on top of it.
******************************************************************************/

/* Define ********************************************************************/
#ifndef __RME_SYSCALL_H__
#define __RME_SYSCALL_H__

typedef unsigned long rme_ptr_t;

/* System call numbers (mirror of Include/rme.h) *****************************/
#define RME_SVC_INV_RET                 (0U)
#define RME_SVC_INV_ACT                 (1U)
#define RME_SVC_SIG_SND                 (2U)
#define RME_SVC_SIG_RCV                 (3U)
#define RME_SVC_KFN                     (4U)
#define RME_SVC_THD_SCHED_FREE          (5U)
#define RME_SVC_THD_EXEC_SET            (6U)
#define RME_SVC_THD_SCHED_PRIO          (7U)
#define RME_SVC_THD_TIME_XFER           (8U)
#define RME_SVC_THD_SWT                 (9U)
#define RME_SVC_CPT_CRT                 (10U)
#define RME_SVC_CPT_DEL                 (11U)
#define RME_SVC_CPT_FRZ                 (12U)
#define RME_SVC_CPT_ADD                 (13U)
#define RME_SVC_CPT_REM                 (14U)
#define RME_SVC_PGT_CRT                 (15U)
#define RME_SVC_PGT_DEL                 (16U)
#define RME_SVC_PGT_ADD                 (17U)
#define RME_SVC_PGT_REM                 (18U)
#define RME_SVC_PGT_CON                 (19U)
#define RME_SVC_PGT_DES                 (20U)
#define RME_SVC_PRC_CRT                 (21U)
#define RME_SVC_PRC_DEL                 (22U)
#define RME_SVC_PRC_CPT                 (23U)
#define RME_SVC_PRC_PGT                 (24U)
#define RME_SVC_THD_CRT                 (25U)
#define RME_SVC_THD_DEL                 (26U)
#define RME_SVC_THD_SCHED_BIND          (27U)
#define RME_SVC_THD_SCHED_RCV           (28U)
#define RME_SVC_SIG_CRT                 (29U)
#define RME_SVC_SIG_DEL                 (30U)
#define RME_SVC_INV_CRT                 (31U)
#define RME_SVC_INV_DEL                 (32U)
#define RME_SVC_INV_SET                 (33U)

/* Kernel function IDs (mirror of Include/rme.h) *****************************/
#define RME_KFN_DEBUG_PRINT             (0xF800U)

/* Boot capability slot for the init KFN (mirror of rme_platform_rv64v.h) ****/
#define RME_BOOT_INIT_KFN               (4U)
/* End Define ****************************************************************/

/* Function:rme_syscall *******************************************************
Description : The generic system call - the only ecall in the user program.
              Everything else is built on top of this primitive.
Input       : rme_ptr_t Svc_Num - The system call number.
              rme_ptr_t Cid - The capability ID (in the caller's capability table).
              rme_ptr_t Param0 - The first system call parameter.
              rme_ptr_t Param1 - The second system call parameter.
              rme_ptr_t Param2 - The third system call parameter.
Output      : None.
Return      : rme_ptr_t - The kernel return value (in a0).
******************************************************************************/
static inline rme_ptr_t rme_syscall(rme_ptr_t Svc_Num,
                                    rme_ptr_t Cid,
                                    rme_ptr_t Param0,
                                    rme_ptr_t Param1,
                                    rme_ptr_t Param2)
{
    register rme_ptr_t A0 asm("a0") = (Svc_Num << 16) | Cid;
    register rme_ptr_t A1 asm("a1") = Param0;
    register rme_ptr_t A2 asm("a2") = Param1;
    register rme_ptr_t A3 asm("a3") = Param2;

    asm volatile("ecall"
                 : "+r"(A0)
                 : "r"(A1), "r"(A2), "r"(A3)
                 : "memory");

    return A0;
}
/* End Function:rme_syscall **************************************************/

#endif /* __RME_SYSCALL_H__ */

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
