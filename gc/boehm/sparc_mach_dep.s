!	SPARCompiler 3.0 and later apparently no loner handles
!	asm outside functions.  So we need a separate .s file
!	This is only set up for SunOS 5, not SunOS 4.
!	Assumes this is called before the stack contents are
!	examined.

	.seg 	"text"
	.globl	GC_save_regs_in_stack
	.globl 	GC_push_regs
GC_save_regs_in_stack:
GC_push_regs:
	ta	0x3   ! ST_FLUSH_WINDOWS
	mov	%sp,%o0
	retl
	nop
	
	.globl	GC_clear_stack_inner
GC_clear_stack_inner:
	mov	%sp,%o2		! Save sp
	add	%sp,-8,%o3	! p = sp-8
	clr	%g1		! [g0,g1] = 0
	add	%o1,-0x60,%sp	! Move sp out of the way,
				! so that traps still work.
				! Includes some extra words
				! so we can be sloppy below.
loop:
	std	%g0,[%o3]	! *(long long *)p = 0
	cmp	%o3,%o1
	bgu	loop		! if (p > limit) goto loop
	  add	%o3,-8,%o3	! p -= 8 (delay slot)
	retl
	  mov	%o2,%sp		! Restore sp., delay slot
	
		
		
		
		
	