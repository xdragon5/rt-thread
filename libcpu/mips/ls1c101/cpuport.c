/*
 * File      : cpuport.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2011, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date                Author         Notes
 * 2010-07-09     Bernard        first version
 * 2010-09-11     Bernard        add CPU reset implementation
 * 2015-07-06     chinesebear  	 modified for loongson 1c
 */

#include <rtthread.h>
#include "../common/mipsregs.h"

register rt_uint32_t $GP __asm__ ("$28");

extern rt_uint32_t cp0_get_hi(void);
extern rt_uint32_t cp0_get_lo(void);

/**
 * this function will reset CPU
 *
 */
void rt_hw_cpu_reset(void)
{
	rt_kprintf("reboot system...\n");
	while (1);
}

/**
 * this function will shutdown CPU
 *
 */
void rt_hw_cpu_shutdown(void)
{
	rt_kprintf("shutdown...\n");

	while (1);
}

/**
 * This function will initialize thread stack
 *
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * @param texit the function will be called when thread exit
 *
 * @return stack address
 */
rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit)
{
	rt_uint32_t *stk;
    static rt_uint32_t g_sr = 0;
	static rt_uint32_t g_gp = 0;

    if (g_sr == 0)
    {
    	g_sr = read_c0_status();
    	g_sr &= 0xfffffffe;
    	g_sr |= 0x8401;

		g_gp = $GP;
    }

    /** Start at stack top */
    stk = (rt_uint32_t *)stack_addr;
	*(stk)   = (rt_uint32_t) tentry;        	/* pc: Entry Point */
	*(--stk) = (rt_uint32_t) read_c0_cause(); 	/* c0_cause */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* c0_badvaddr */
	*(--stk) = (rt_uint32_t) cp0_get_lo();		/* lo */
	*(--stk) = (rt_uint32_t) cp0_get_hi();		/* hi */
	*(--stk) = (rt_uint32_t) g_sr; 				/* C0_SR: HW2 = En, IE = En */
	*(--stk) = (rt_uint32_t) texit;	        	/* ra */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s8 */
	*(--stk) = (rt_uint32_t) stack_addr;		/* sp */
	*(--stk) = (rt_uint32_t) g_gp;	        	/* gp */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* k1 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* k0 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t9 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t8 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s7 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s6 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s5 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s4 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s3 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s2 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s1 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* s0 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t7 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t6 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t5 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t4 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t3 */
	*(--stk) = (rt_uint32_t) 0x00000000; 		/* t2 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t1 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* t0 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* a3 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* a2 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* a1 */
	*(--stk) = (rt_uint32_t) parameter;	    	/* a0 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* v1 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* v0 */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* at */
	*(--stk) = (rt_uint32_t) 0x00000000;		/* zero */

	/* return task's current stack address */
	return (rt_uint8_t *)stk;
}


/*@}*/

