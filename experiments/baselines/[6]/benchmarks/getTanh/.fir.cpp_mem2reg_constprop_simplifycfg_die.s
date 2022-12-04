	.text
	.file	"fir.cpp"
	.globl	_Z3firPiS_              # -- Begin function _Z3firPiS_
	.p2align	4, 0x90
	.type	_Z3firPiS_,@function
_Z3firPiS_:                             # @_Z3firPiS_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movq	%rdi, -8(%rbp)          # 8-byte Spill
	movq	%rsi, -16(%rbp)         # 8-byte Spill
	movl	%eax, -20(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	movl	-20(%rbp), %eax         # 4-byte Reload
	movl	$1, %ecx
	movl	%eax, %edx
	movl	%edx, %esi
	movq	-16(%rbp), %rdi         # 8-byte Reload
	movl	(%rdi,%rsi,4), %edx
	movslq	%edx, %rsi
	movq	-8(%rbp), %r8           # 8-byte Reload
	movl	(%r8,%rsi,4), %r9d
	cmpl	$0, %r9d
	movl	%eax, -24(%rbp)         # 4-byte Spill
	movl	%r9d, -28(%rbp)         # 4-byte Spill
	movl	%edx, -32(%rbp)         # 4-byte Spill
	movl	%ecx, -36(%rbp)         # 4-byte Spill
	jg	.LBB0_3
# %bb.2:                                # %if.else
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-28(%rbp), %eax         # 4-byte Reload
	shll	$2, %eax
	addl	$5, %eax
	movl	-28(%rbp), %ecx         # 4-byte Reload
	imull	%eax, %ecx
	movl	%ecx, -36(%rbp)         # 4-byte Spill
.LBB0_3:                                # %if.end
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-36(%rbp), %eax         # 4-byte Reload
	movl	-32(%rbp), %ecx         # 4-byte Reload
	movslq	%ecx, %rdx
	movq	-8(%rbp), %rsi          # 8-byte Reload
	movl	%eax, (%rsi,%rdx,4)
	movl	-24(%rbp), %edi         # 4-byte Reload
	addl	$1, %edi
	cmpl	$1000, %edi             # imm = 0x3E8
	movl	%eax, -40(%rbp)         # 4-byte Spill
	movl	%edi, -20(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.4:                                # %for.end
	movl	-40(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z3firPiS_, .Lfunc_end0-_Z3firPiS_
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
	subq	$8048, %rsp             # imm = 0x1F70
	xorl	%eax, %eax
	movl	%eax, -8004(%rbp)       # 4-byte Spill
.LBB1_1:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	movl	-8004(%rbp), %eax       # 4-byte Reload
	movl	%eax, -8008(%rbp)       # 4-byte Spill
	callq	rand
	movl	%eax, -8012(%rbp)       # 4-byte Spill
	callq	rand
	movl	%eax, -8016(%rbp)       # 4-byte Spill
	callq	rand
	movl	%eax, -8020(%rbp)       # 4-byte Spill
	callq	rand
	movl	-8008(%rbp), %ecx       # 4-byte Reload
	addl	$1, %ecx
	cmpl	$10, %ecx
	movl	%eax, -8024(%rbp)       # 4-byte Spill
	movl	%ecx, -8004(%rbp)       # 4-byte Spill
	jb	.LBB1_1
# %bb.2:                                # %for.end
	xorl	%eax, %eax
	movl	%eax, -8028(%rbp)       # 4-byte Spill
	jmp	.LBB1_3
.LBB1_3:                                # %for.body16
                                        # =>This Inner Loop Header: Depth=1
	movl	-8028(%rbp), %eax       # 4-byte Reload
	movl	%eax, -8032(%rbp)       # 4-byte Spill
	callq	rand
	movl	-8032(%rbp), %ecx       # 4-byte Reload
	addl	$1, %ecx
	cmpl	$1000, %ecx             # imm = 0x3E8
	movl	%eax, -8036(%rbp)       # 4-byte Spill
	movl	%ecx, -8028(%rbp)       # 4-byte Spill
	jb	.LBB1_3
# %bb.4:                                # %for.end23
	xorl	%eax, %eax
	movl	%eax, -8040(%rbp)       # 4-byte Spill
	jmp	.LBB1_5
.LBB1_5:                                # %for.body27
                                        # =>This Inner Loop Header: Depth=1
	movl	-8040(%rbp), %eax       # 4-byte Reload
	movl	$100, %ecx
	movl	%eax, -8044(%rbp)       # 4-byte Spill
	xorl	%edx, %edx
	divl	%ecx
	movl	-8044(%rbp), %esi       # 4-byte Reload
	movl	%esi, %edi
	movl	%edi, %r8d
	movl	%edx, -4000(%rbp,%r8,4)
	shll	$1, %esi
	movl	%esi, %eax
	xorl	%edx, %edx
	divl	%ecx
	movl	-8044(%rbp), %ecx       # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %r8d
	movl	%edx, -8000(%rbp,%r8,4)
	addl	$1, %ecx
	cmpl	$1000, %ecx             # imm = 0x3E8
	movl	%ecx, -8040(%rbp)       # 4-byte Spill
	jb	.LBB1_5
# %bb.6:                                # %for.end36
	leaq	-8000(%rbp), %rsi
	leaq	-4000(%rbp), %rdi
	callq	_Z3firPiS_
	xorl	%ecx, %ecx
	movl	%eax, -8048(%rbp)       # 4-byte Spill
	movl	%ecx, %eax
	addq	$8048, %rsp             # imm = 0x1F70
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
