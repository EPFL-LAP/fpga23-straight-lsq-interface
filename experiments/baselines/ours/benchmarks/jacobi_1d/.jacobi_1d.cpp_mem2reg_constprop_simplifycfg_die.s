	.text
	.file	"jacobi_1d.cpp"
	.globl	_Z9jacobi_1dPiS_S_      # -- Begin function _Z9jacobi_1dPiS_S_
	.p2align	4, 0x90
	.type	_Z9jacobi_1dPiS_S_,@function
_Z9jacobi_1dPiS_S_:                     # @_Z9jacobi_1dPiS_S_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movq	%rsi, -8(%rbp)          # 8-byte Spill
	movq	%rdi, -16(%rbp)         # 8-byte Spill
	movq	%rdx, -24(%rbp)         # 8-byte Spill
	movl	%eax, -28(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
                                        #     Child Loop BB0_4 Depth 2
	movl	-28(%rbp), %eax         # 4-byte Reload
	movl	$1, %ecx
	movl	%eax, -32(%rbp)         # 4-byte Spill
	movl	%ecx, -36(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-36(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	addl	$-1, %ecx
	movslq	%ecx, %rdx
	movq	-16(%rbp), %rsi         # 8-byte Reload
	movl	(%rsi,%rdx,4), %ecx
	movl	%eax, %edi
	movl	%edi, %edx
	addl	(%rsi,%rdx,4), %ecx
	movl	%eax, %edi
	addl	$1, %edi
	movl	%edi, %edi
	movl	%edi, %edx
	addl	(%rsi,%rdx,4), %ecx
	imull	$3, %ecx, %ecx
	movl	%eax, %edi
	movl	%edi, %edx
	movq	-24(%rbp), %r8          # 8-byte Reload
	movl	%ecx, (%r8,%rdx,4)
	addl	$1, %eax
	cmpl	$99, %eax
	movl	%eax, -36(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.3:                                # %for.end
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	$1, %eax
	movl	%eax, -40(%rbp)         # 4-byte Spill
	jmp	.LBB0_4
.LBB0_4:                                # %for.body14
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-40(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movq	-24(%rbp), %rsi         # 8-byte Reload
	movl	(%rsi,%rdx,4), %ecx
	movl	%eax, %edi
	movl	%edi, %edx
	movq	-8(%rbp), %r8           # 8-byte Reload
	movl	%ecx, (%r8,%rdx,4)
	addl	$1, %eax
	cmpl	$99, %eax
	movl	%eax, -40(%rbp)         # 4-byte Spill
	jb	.LBB0_4
# %bb.5:                                # %for.inc22
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-32(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$3, %eax
	movl	%eax, %ecx
	movl	%eax, -44(%rbp)         # 4-byte Spill
	movl	%ecx, -28(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.6:                                # %for.end24
	movl	-44(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z9jacobi_1dPiS_S_, .Lfunc_end0-_Z9jacobi_1dPiS_S_
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
	subq	$1216, %rsp             # imm = 0x4C0
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
	xorl	%eax, %eax
	movl	%eax, -1204(%rbp)       # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-1204(%rbp), %eax       # 4-byte Reload
	movl	%eax, -1208(%rbp)       # 4-byte Spill
	callq	rand
	movl	$100, %ecx
	cltd
	idivl	%ecx
	movl	-1208(%rbp), %ecx       # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	movl	%edx, -400(%rbp,%rdi,4)
	callq	rand
	movl	$100, %ecx
	cltd
	idivl	%ecx
	movl	-1208(%rbp), %ecx       # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	movl	%edx, -800(%rbp,%rdi,4)
	callq	rand
	movl	$100, %ecx
	cltd
	idivl	%ecx
	movl	-1208(%rbp), %ecx       # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	movl	%edx, -1200(%rbp,%rdi,4)
	addl	$1, %ecx
	cmpl	$100, %ecx
	movl	%ecx, -1204(%rbp)       # 4-byte Spill
	jb	.LBB1_2
# %bb.3:                                # %for.inc18
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_4
.LBB1_4:                                # %for.end20
	leaq	-1200(%rbp), %rdx
	leaq	-800(%rbp), %rsi
	leaq	-400(%rbp), %rdi
	callq	_Z9jacobi_1dPiS_S_
	xorl	%ecx, %ecx
	movl	%eax, -1212(%rbp)       # 4-byte Spill
	movl	%ecx, %eax
	addq	$1216, %rsp             # imm = 0x4C0
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
