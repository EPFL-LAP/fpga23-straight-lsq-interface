	.text
	.file	"kernel_2mm.cpp"
	.globl	_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_ # -- Begin function _Z10kernel_2mmiiPA10_iS0_S0_S0_S0_
	.p2align	4, 0x90
	.type	_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_,@function
_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_:     # @_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	16(%rbp), %rax
	xorl	%r10d, %r10d
	movq	%rax, -8(%rbp)          # 8-byte Spill
	movq	%r9, -16(%rbp)          # 8-byte Spill
	movq	%r8, -24(%rbp)          # 8-byte Spill
	movq	%rcx, -32(%rbp)         # 8-byte Spill
	movq	%rdx, -40(%rbp)         # 8-byte Spill
	movl	%esi, -44(%rbp)         # 4-byte Spill
	movl	%edi, -48(%rbp)         # 4-byte Spill
	movl	%r10d, -52(%rbp)        # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
                                        #       Child Loop BB0_3 Depth 3
	movl	-52(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -56(%rbp)         # 4-byte Spill
	movl	%ecx, -60(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB0_3 Depth 3
	movl	-60(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	-56(%rbp), %edx         # 4-byte Reload
	movl	%edx, %esi
	movl	%esi, %edi
	movl	%eax, %esi
	movl	%esi, %r8d
	imulq	$40, %rdi, %rdi
	movq	-40(%rbp), %r9          # 8-byte Reload
	addq	%rdi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	movl	(%r9), %esi
	movl	%eax, -64(%rbp)         # 4-byte Spill
	movl	%esi, -68(%rbp)         # 4-byte Spill
	movl	%ecx, -72(%rbp)         # 4-byte Spill
.LBB0_3:                                # %for.body8
                                        #   Parent Loop BB0_1 Depth=1
                                        #     Parent Loop BB0_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-72(%rbp), %eax         # 4-byte Reload
	movl	-68(%rbp), %ecx         # 4-byte Reload
	movl	-56(%rbp), %edx         # 4-byte Reload
	movl	%edx, %esi
	movl	%esi, %edi
	movl	%eax, %esi
	movl	%esi, %r8d
	imulq	$40, %rdi, %rdi
	movq	-32(%rbp), %r9          # 8-byte Reload
	addq	%rdi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	movl	-48(%rbp), %esi         # 4-byte Reload
	imull	(%r9), %esi
	movl	%eax, %r10d
	movl	%r10d, %edi
	movl	-64(%rbp), %r10d        # 4-byte Reload
	movl	%r10d, %r11d
	movl	%r11d, %r8d
	imulq	$40, %rdi, %rdi
	movq	-24(%rbp), %r9          # 8-byte Reload
	addq	%rdi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	imull	(%r9), %esi
	addl	%esi, %ecx
	addl	$1, %eax
	cmpl	$10, %eax
	movl	%ecx, %esi
	movl	%ecx, -76(%rbp)         # 4-byte Spill
	movl	%esi, -68(%rbp)         # 4-byte Spill
	movl	%eax, -72(%rbp)         # 4-byte Spill
	jb	.LBB0_3
# %bb.4:                                # %for.end
                                        #   in Loop: Header=BB0_2 Depth=2
	movl	-56(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	-64(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	imulq	$40, %rdx, %rdx
	movq	-40(%rbp), %r8          # 8-byte Reload
	addq	%rdx, %r8
	shlq	$2, %rdi
	addq	%rdi, %r8
	movl	-76(%rbp), %esi         # 4-byte Reload
	movl	%esi, (%r8)
	addl	$1, %ecx
	cmpl	$10, %ecx
	movl	%ecx, -60(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.5:                                # %for.inc25
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-56(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$10, %eax
	movl	%eax, -52(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.6:                                # %for.end27
	xorl	%eax, %eax
	movl	%eax, -80(%rbp)         # 4-byte Spill
	jmp	.LBB0_7
.LBB0_7:                                # %for.body30
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_8 Depth 2
                                        #       Child Loop BB0_9 Depth 3
	movl	-80(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -84(%rbp)         # 4-byte Spill
	movl	%ecx, -88(%rbp)         # 4-byte Spill
.LBB0_8:                                # %for.body33
                                        #   Parent Loop BB0_7 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB0_9 Depth 3
	movl	-88(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	-84(%rbp), %edx         # 4-byte Reload
	movl	%edx, %esi
	movl	%esi, %edi
	movl	%eax, %esi
	movl	%esi, %r8d
	imulq	$40, %rdi, %rdi
	movq	-8(%rbp), %r9           # 8-byte Reload
	addq	%rdi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	movl	-44(%rbp), %esi         # 4-byte Reload
	imull	(%r9), %esi
	movl	%eax, -92(%rbp)         # 4-byte Spill
	movl	%esi, -96(%rbp)         # 4-byte Spill
	movl	%ecx, -100(%rbp)        # 4-byte Spill
.LBB0_9:                                # %for.body42
                                        #   Parent Loop BB0_7 Depth=1
                                        #     Parent Loop BB0_8 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-100(%rbp), %eax        # 4-byte Reload
	movl	-96(%rbp), %ecx         # 4-byte Reload
	movl	-84(%rbp), %edx         # 4-byte Reload
	movl	%edx, %esi
	movl	%esi, %edi
	movl	%eax, %esi
	movl	%esi, %r8d
	imulq	$40, %rdi, %rdi
	movq	-40(%rbp), %r9          # 8-byte Reload
	addq	%rdi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	movl	(%r9), %esi
	movl	%eax, %r10d
	movl	%r10d, %edi
	movl	-92(%rbp), %r10d        # 4-byte Reload
	movl	%r10d, %r11d
	movl	%r11d, %r8d
	imulq	$40, %rdi, %rdi
	movq	-16(%rbp), %r9          # 8-byte Reload
	addq	%rdi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	imull	(%r9), %esi
	addl	%esi, %ecx
	addl	$1, %eax
	cmpl	$10, %eax
	movl	%ecx, %esi
	movl	%eax, %r11d
	movl	%ecx, -104(%rbp)        # 4-byte Spill
	movl	%eax, -108(%rbp)        # 4-byte Spill
	movl	%esi, -96(%rbp)         # 4-byte Spill
	movl	%r11d, -100(%rbp)       # 4-byte Spill
	jb	.LBB0_9
# %bb.10:                               # %for.end55
                                        #   in Loop: Header=BB0_8 Depth=2
	movl	-84(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	-92(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	imulq	$40, %rdx, %rdx
	movq	-8(%rbp), %r8           # 8-byte Reload
	addq	%rdx, %r8
	shlq	$2, %rdi
	addq	%rdi, %r8
	movl	-104(%rbp), %esi        # 4-byte Reload
	movl	%esi, (%r8)
	addl	$1, %ecx
	cmpl	$10, %ecx
	movl	%ecx, -88(%rbp)         # 4-byte Spill
	jb	.LBB0_8
# %bb.11:                               # %for.inc63
                                        #   in Loop: Header=BB0_7 Depth=1
	movl	-84(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$10, %eax
	movl	%eax, -80(%rbp)         # 4-byte Spill
	jb	.LBB0_7
# %bb.12:                               # %for.end65
	movl	-108(%rbp), %eax        # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_, .Lfunc_end0-_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_
	.cfi_endproc
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$2064, %rsp             # imm = 0x810
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
                                        #       Child Loop BB1_3 Depth 3
	callq	rand
	movl	%eax, -4(%rbp)
	callq	rand
	xorl	%ecx, %ecx
	movl	%eax, -8(%rbp)
	movl	%ecx, -2020(%rbp)       # 4-byte Spill
.LBB1_2:                                # %for.body6
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_3 Depth 3
	movl	-2020(%rbp), %eax       # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -2024(%rbp)       # 4-byte Spill
	movl	%ecx, -2028(%rbp)       # 4-byte Spill
.LBB1_3:                                # %for.body9
                                        #   Parent Loop BB1_1 Depth=1
                                        #     Parent Loop BB1_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-2028(%rbp), %eax       # 4-byte Reload
	movl	%eax, -2032(%rbp)       # 4-byte Spill
	callq	rand
	leaq	-816(%rbp), %rcx
	movl	$100, %edx
	movl	%edx, -2036(%rbp)       # 4-byte Spill
	cltd
	movl	-2036(%rbp), %esi       # 4-byte Reload
	idivl	%esi
	movl	-2024(%rbp), %edi       # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	-2032(%rbp), %r8d       # 4-byte Reload
	movl	%r8d, %r10d
	movl	%r10d, %r11d
	imulq	$40, %r9, %r9
	addq	%r9, %rcx
	shlq	$2, %r11
	addq	%r11, %rcx
	movl	%edx, (%rcx)
	callq	rand
	leaq	-1216(%rbp), %rcx
	movl	$100, %edx
	movl	%edx, -2040(%rbp)       # 4-byte Spill
	cltd
	movl	-2040(%rbp), %esi       # 4-byte Reload
	idivl	%esi
	movl	-2024(%rbp), %edi       # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	-2032(%rbp), %r8d       # 4-byte Reload
	movl	%r8d, %r10d
	movl	%r10d, %r11d
	imulq	$40, %r9, %r9
	addq	%r9, %rcx
	shlq	$2, %r11
	addq	%r11, %rcx
	movl	%edx, (%rcx)
	callq	rand
	leaq	-1616(%rbp), %rcx
	movl	$100, %edx
	movl	%edx, -2044(%rbp)       # 4-byte Spill
	cltd
	movl	-2044(%rbp), %esi       # 4-byte Reload
	idivl	%esi
	movl	-2024(%rbp), %edi       # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	-2032(%rbp), %r8d       # 4-byte Reload
	movl	%r8d, %r10d
	movl	%r10d, %r11d
	imulq	$40, %r9, %r9
	addq	%r9, %rcx
	shlq	$2, %r11
	addq	%r11, %rcx
	movl	%edx, (%rcx)
	callq	rand
	leaq	-2016(%rbp), %rcx
	movl	$100, %edx
	movl	%edx, -2048(%rbp)       # 4-byte Spill
	cltd
	movl	-2048(%rbp), %esi       # 4-byte Reload
	idivl	%esi
	movl	-2024(%rbp), %edi       # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	-2032(%rbp), %r8d       # 4-byte Reload
	movl	%r8d, %r10d
	movl	%r10d, %r11d
	imulq	$40, %r9, %r9
	addq	%r9, %rcx
	shlq	$2, %r11
	addq	%r11, %rcx
	movl	%edx, (%rcx)
	addl	$1, %r8d
	cmpl	$10, %r8d
	movl	%r8d, -2028(%rbp)       # 4-byte Spill
	jb	.LBB1_3
# %bb.4:                                # %for.inc41
                                        #   in Loop: Header=BB1_2 Depth=2
	movl	-2024(%rbp), %eax       # 4-byte Reload
	addl	$1, %eax
	cmpl	$10, %eax
	movl	%eax, -2020(%rbp)       # 4-byte Spill
	jb	.LBB1_2
# %bb.5:                                # %for.inc44
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_6
.LBB1_6:                                # %for.end46
	leaq	-2016(%rbp), %rax
	leaq	-1616(%rbp), %r9
	leaq	-1216(%rbp), %r8
	leaq	-816(%rbp), %rcx
	leaq	-416(%rbp), %rdx
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %esi
	movq	%rax, (%rsp)
	callq	_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_
	xorl	%esi, %esi
	movl	%eax, -2052(%rbp)       # 4-byte Spill
	movl	%esi, %eax
	addq	$2064, %rsp             # imm = 0x810
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
