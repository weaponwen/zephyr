/*
 * Copyright (c) 2016 Jean-Paul Etienne <fractalclone@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <kernel_structs.h>
#include <inttypes.h>
#include <logging/log_ctrl.h>

FUNC_NORETURN void z_riscv32_fatal_error(unsigned int reason,
					 const z_arch_esf_t *esf)
{
	if (esf != NULL) {
		z_fatal_print("Faulting instruction address = 0x%08x",
			      esf->mepc);
		z_fatal_print("  ra: 0x%08x  gp: 0x%08x  tp: 0x%08x  t0: 0x%08x",
			      esf->ra, esf->gp, esf->tp, esf->t0);
		z_fatal_print("  t1: 0x%08x  t2: 0x%08x  t3: 0x%08x  t4: 0x%08x",
			      esf->t1, esf->t2, esf->t3, esf->t4);
		z_fatal_print("  t5: 0x%08x  t6: 0x%08x  a0: 0x%08x  a1: 0x%08x",
			      esf->t5, esf->t6, esf->a0, esf->a1);
		z_fatal_print("  a2: 0x%08x  a3: 0x%08x  a4: 0x%08x  a5: 0x%08x",
			      esf->a2, esf->a3, esf->a4, esf->a5);
		z_fatal_print("  a6: 0x%08x  a7: 0x%08x\n",
			      esf->a6, esf->a7);
	}

	z_fatal_error(reason, esf);
	CODE_UNREACHABLE;
}

static char *cause_str(u32_t cause)
{
	switch (cause) {
	case 0:
		return "Instruction address misaligned";
	case 1:
		return "Instruction Access fault";
	case 2:
		return "Illegal instruction";
	case 3:
		return "Breakpoint";
	case 4:
		return "Load address misaligned";
	case 5:
		return "Load access fault";
	default:
		return "unknown";
	}
}

FUNC_NORETURN void _Fault(const z_arch_esf_t *esf)
{
	u32_t mcause;

	__asm__ volatile("csrr %0, mcause" : "=r" (mcause));

	mcause &= SOC_MCAUSE_EXP_MASK;
	z_fatal_print("Exception cause %s (%d)", cause_str(mcause),
		      (int)mcause);

	z_riscv32_fatal_error(K_ERR_CPU_EXCEPTION, esf);
}
