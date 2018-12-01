/*
 * File      : interrupt.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2011, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date                  Author       Notes
 * 2010-10-15     Bernard      first version
 * 2010-10-15     lgnq           modified for LS1B
 * 2013-03-29     aozima       Modify the interrupt interface implementations.
 * 2015-07-06     chinesebear modified for loongson 1c
 */

#include <rtthread.h>
#include <rthw.h>
#include "ls1c101_irq.h"

extern rt_uint32_t rt_interrupt_nest;
rt_uint32_t rt_interrupt_from_thread;
rt_uint32_t rt_interrupt_to_thread;
rt_uint32_t rt_thread_switch_interrupt_flag;

void rt_interrupt_dispatch(void *ptreg);

/**
 * @addtogroup Loongson LS1B
 */

/*@{*/

static void rt_hw_interrupt_handler(int vector, void *param)
{
    rt_kprintf("Unhandled interrupt %d occured!!!\n", vector);
}

/**
 * This function will initialize hardware interrupt
 */
void rt_hw_interrupt_init(void)
{
    ls1c101_irq_init();

    /* init interrupt nest, and context in thread sp */
    rt_interrupt_nest = 0;
    rt_interrupt_from_thread = 0;
    rt_interrupt_to_thread = 0;
    rt_thread_switch_interrupt_flag = 0;
}

/**
 * This function will mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_mask(int vector)
{
    ls1c101_irq_mask(vector);
}

/**
 * This function will un-mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_umask(int vector)
{
    ls1c101_irq_unmask(vector);
}

/**
 * This function will install a interrupt service routine to a interrupt.
 * @param vector the interrupt number
 * @param new_handler the interrupt service routine to be installed
 * @param old_handler the old interrupt service routine
 */
rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
                                         void *param, char *name)
{
    ls1c101_irq_install(vector, (isr_handler_t)handler);

    return RT_NULL;
}


void rt_interrupt_dispatch(void *ptreg)
{
    ls1c101_irq_dispatch();
}

/*@}*/


