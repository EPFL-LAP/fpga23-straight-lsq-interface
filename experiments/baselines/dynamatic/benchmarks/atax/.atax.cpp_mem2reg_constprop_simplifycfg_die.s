	.text
	.file	"atax.cpp"
	.globl	_Z4ataxPA20_iPiS1_S1_   # -- Begin function _Z4ataxPA20_iPiS1_S1_
	.p2align	4, 0x90
	.type	_Z4ataxPA20_iPiS1_S1_,@function
_Z4ataxPA20_iPiS1_S1_:                  # @_Z4ataxPA20_iPiS1_S1_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movq	%rdx, -8(%rbp)          # 8-byte Spill
	movq	%rsi, -16(%rbp)         # 8-byte Spill
	movq	%rdi, -24(%rbp)         # 8-byte Spill
	movq	%rcx, -32(%rbp)         # 8-byte Spill
	movl	%eax, -36(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
                                        #     Child Loop BB0_4 Depth 2
	movl	-36(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, %edx
	movl	%edx, %esi
	movq	-32(%rbp), %rdi         # 8-byte Reload
	movl	(%rdi,%rsi,4), %edx
	movl	%eax, -40(%rbp)         # 4-byte Spill
	movl	%edx, -44(%rbp)         # 4-byte Spill
	movl	%ecx, -48(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-48(%rbp), %eax         # 4-byte Reload
	movl	-44(%rbp), %ecx         # 4-byte Reload
	movl	-40(%rbp), %edx         # 4-byte Reload
	movl	%edx, %esi
	movl	%esi, %edi
	movl	%eax, %esi
	movl	%esi, %r8d
	imulq	$80, %rdi, %rdi
	movq	-24(%rbp), %r9          # 8-byte Reload
	addq	%rdi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	movl	(%r9), %esi
	movl	%eax, %r10d
	movl	%r10d, %edi
	movq	-16(%rbp), %r8          # 8-byte Reload
	imull	(%r8,%rdi,4), %esi
	addl	%esi, %ecx
	addl	$1, %eax
	cmpl	$20, %eax
	movl	%ecx, %esi
	movl	%ecx, -52(%rbp)         # 4-byte Spill
	movl	%esi, -44(%rbp)         # 4-byte Spill
	movl	%eax, -48(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.3:                                # %for.end
                                        #   in Loop: Header=BB0_1 Depth=1
	xorl	%eax, %eax
	movl	%eax, -56(%rbp)         # 4-byte Spill
	jmp	.LBB0_4
.LBB0_4:                                # %for.body12
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-56(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movq	-8(%rbp), %rsi          # 8-byte Reload
	movl	(%rsi,%rdx,4), %ecx
	movl	-40(%rbp), %edi         # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %edx
	movl	%eax, %r8d
	movl	%r8d, %r9d
	imulq	$80, %rdx, %rdx
	movq	-24(%rbp), %r10         # 8-byte Reload
	addq	%rdx, %r10
	shlq	$2, %r9
	addq	%r9, %r10
	movl	-52(%rbp), %r8d         # 4-byte Reload
	imull	(%r10), %r8d
	addl	%r8d, %ecx
	movl	%eax, %r8d
	movl	%r8d, %edx
	movl	%ecx, (%rsi,%rdx,4)
	addl	$1, %eax
	cmpl	$20, %eax
	movl	%eax, -56(%rbp)         # 4-byte Spill
	jb	.LBB0_4
# %bb.5:                                # %for.end25
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-40(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movq	-32(%rbp), %rsi         # 8-byte Reload
	movl	-52(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, (%rsi,%rdx,4)
	addl	$1, %eax
	cmpl	$20, %eax
	movl	%eax, %edi
	movl	%eax, -60(%rbp)         # 4-byte Spill
	movl	%edi, -36(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.6:                                # %for.end30
	movl	-60(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z4ataxPA20_iPiS1_S1_, .Lfunc_end0-_Z4ataxPA20_iPiS1_S1_
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
	subq	$1872, %rsp             # imm = 0x750
	movl	$13, %edi
	callq	srand
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
                                        #       Child Loop BB1_3 Depth 3
	xorl	%eax, %eax
	movl	%eax, -1844(%rbp)       # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_3 Depth 3
	movl	-1844(%rbp), %eax       # 4-byte Reload
	movl	%eax, -1848(%rbp)       # 4-byte Spill
	callq	rand
	xorl	%ecx, %ecx
	movl	$100, %edx
	movl	%edx, -1852(%rbp)       # 4-byte Spill
	cltd
	movl	-1852(%rbp), %esi       # 4-byte Reload
	idivl	%esi
	movl	-1848(%rbp), %edi       # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	%edx, -1680(%rbp,%r9,4)
	movl	%edi, %edx
	movl	%edx, %r9d
	movl	$0, -1760(%rbp,%r9,4)
	movl	%edi, %edx
	movl	%edx, %r9d
	movl	$0, -1840(%rbp,%r9,4)
	movl	%ecx, -1856(%rbp)       # 4-byte Spill
.LBB1_3:                                # %for.body17
                                        #   Parent Loop BB1_1 Depth=1
                                        #     Parent Loop BB1_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-1856(%rbp), %eax       # 4-byte Reload
	movl	%eax, -1860(%rbp)       # 4-byte Spill
	callq	rand
	leaq	-1600(%rbp), %rcx
	movl	$100, %edx
	movl	%edx, -1864(%rbp)       # 4-byte Spill
	cltd
	movl	-1864(%rbp), %esi       # 4-byte Reload
	idivl	%esi
	movl	-1848(%rbp), %edi       # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	-1860(%rbp), %r8d       # 4-byte Reload
	movl	%r8d, %r10d
	movl	%r10d, %r11d
	imulq	$80, %r9, %r9
	addq	%r9, %rcx
	shlq	$2, %r11
	addq	%r11, %rcx
	movl	%edx, (%rcx)
	addl	$1, %r8d
	cmpl	$20, %r8d
	movl	%r8d, -1856(%rbp)       # 4-byte Spill
	jb	.LBB1_3
# %bb.4:                                # %for.inc26
                                        #   in Loop: Header=BB1_2 Depth=2
	movl	-1848(%rbp), %eax       # 4-byte Reload
	addl	$1, %eax
	cmpl	$20, %eax
	movl	%eax, -1844(%rbp)       # 4-byte Spill
	jb	.LBB1_2
# %bb.5:                                # %for.inc29
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_6
.LBB1_6:                                # %for.end31
	leaq	-1840(%rbp), %rcx
	leaq	-1760(%rbp), %rdx
	leaq	-1680(%rbp), %rsi
	leaq	-1600(%rbp), %rdi
	callq	_Z4ataxPA20_iPiS1_S1_
	xorl	%r8d, %r8d
	movl	%eax, -1868(%rbp)       # 4-byte Spill
	movl	%r8d, %eax
	addq	$1872, %rsp             # imm = 0x750
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
