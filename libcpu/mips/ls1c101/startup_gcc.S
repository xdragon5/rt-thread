/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName: 	 start_gcc.S
* Desc:
*
*
* Author: 	 zhuangwei
* Date: 	 2018-11-08
* Notes:    1 这个start_gcc.S已经禁止了总中断
            2 禁止了TIMER中断使能，但是代码从头开始执行时，TMIER中断的标志位还是会存在的
              因此在中断处理中 忽略TMER中断标志位就行。
*
* -----------------------------------------------------------------
* Histroy: v1.0   2018-11-08, zhuangwei create this file
*
******************************************************************************/
#include "../common/mips.inc"
#include "../common/stackframe.h"

.extern main
.extern interrupt_dispatch
.extern __bss_start
.extern __bss_end

.extern __data_start
.extern __data_end
.extern _sidata

	.section ".start", "ax"
	.set noreorder
	/* the program entry */
	.globl _start
_start:
    la		gp, _gp
	li		sp, SYSTEM_STACK
	li		t0, 0x8000000         #disable cp0_count, no 8M timer_interrupt
	mtc0	t0, CP0_CAUSE
	li      t0, 0x10400000        #关闭FPU, 中断入口地址在0xbfc00380
    mtc0    t0, CP0_STATUS
    .set mips32r2
    ehb
    .set mips0
#	li		t0, 0x00000000         #set fp reg, it's necessory
#	ctc1	t0, $31
#	nop

	/* disable NMI */
#	li		t0, 0xbfea8010
#	li		t1, 0x5a
#	sb		t1, 0(t0)
#	li		t1, 0xa5
#	sb		t1, 0(t0)
#	li		t1, 0x0
#	sb		t1, 0(t0)

	/* clean 8M时钟中断 */
	li		t0, 1
	mtc0	t0, CP0_COMPARE

	/* clear bss */
	la	t0, __data_start
	la	t1, __data_end
	la	t2, _sidata
_copy_data_loop:
	lw	t3, 0(t2)
	nop
	sw	t3, 0(t0)
	addu	t0, 4
	addu	t2, 4
	bne	t0, t1, _copy_data_loop
	nop
	
	/* clear bss */
	la	t0, __bss_start
	la	t1, __bss_end
_clr_bss_loop:
	sw	zero, 0(t0)
	bne	t0, t1, _clr_bss_loop
	addiu	t0, t0, 4

	/* disable interrupt */
	mfc0	t0, CP0_STATUS
	and 	t0, 0xfffffffe	# By default it will be disabled.
	mtc0	t0, CP0_STATUS	# Set CPU to disable interrupt.
	nop
	
	/* jump to RT-Thread RTOS */
	jal	rtthread_startup
	nop

	/* restart, never die */
	j	_start
	nop

	.set	reorder
	.globl	cp0_get_hi
cp0_get_hi:
	mfhi	v0
	jr	ra
	nop

	.globl	cp0_get_lo
cp0_get_lo:
	mflo	v0
	jr	ra
	nop
	
/********************************************************
***标号Exception以上的代码反汇编不能超过0Xbfc00380******
//	中断异常入口地址，确保是0xbfc00380。
//	芯片跳到0xbfc00380后，硬件上自动就禁止了总中断
//	然后中断返回后的eret指令，硬件上自动就把总中断打开了
********************************************************/
/**************************异常处理**********************************/
	.org  0x380
	.global irq_exception
	.type	irq_exception,@function
_irq_handler:
	.set	noreorder
	la	k0, mips_irq_handle
	jr	k0
	nop
	.set	reorder
/******************************************************************/
