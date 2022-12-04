	.text
	.file	"triangular.cpp"
	.globl	_Z10triangularPiPA100_ii # -- Begin function _Z10triangularPiPA100_ii
	.p2align	4, 0x90
	.type	_Z10triangularPiPA100_ii,@function
_Z10triangularPiPA100_ii:               # @_Z10triangularPiPA100_ii
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	.cfi_offset %rbx, -24
	movl	%edx, %eax
	addl	$-1, %eax
	cmpl	$0, %edx
	movl	%eax, %ecx
	movq	%rsi, -16(%rbp)         # 8-byte Spill
	movq	%rdi, -24(%rbp)         # 8-byte Spill
	movl	%edx, -28(%rbp)         # 4-byte Spill
	movl	%eax, -32(%rbp)         # 4-byte Spill
	movl	%ecx, -36(%rbp)         # 4-byte Spill
	jle	.LBB0_6
# %bb.1:                                # %for.body.lr.ph
	movl	-32(%rbp), %eax         # 4-byte Reload
	movl	%eax, -40(%rbp)         # 4-byte Spill
	jmp	.LBB0_2
.LBB0_2:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_4 Depth 2
	movl	-40(%rbp), %eax         # 4-byte Reload
	cmpl	$0, %eax
	movl	%eax, -44(%rbp)         # 4-byte Spill
	jle	.LBB0_5
# %bb.3:                                # %for.body4.lr.ph
                                        #   in Loop: Header=BB0_2 Depth=1
	movl	-44(%rbp), %eax         # 4-byte Reload
	movl	%eax, -48(%rbp)         # 4-byte Spill
	jmp	.LBB0_4
.LBB0_4:                                # %for.body4
                                        #   Parent Loop BB0_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-48(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	addl	$-1, %ecx
	movslq	%ecx, %rdx
	movl	-44(%rbp), %esi         # 4-byte Reload
	movslq	%esi, %rdi
	imulq	$400, %rdx, %rdx        # imm = 0x190
	movq	-16(%rbp), %r8          # 8-byte Reload
	addq	%rdx, %r8
	shlq	$2, %rdi
	addq	%rdi, %r8
	movl	(%r8), %r9d
	movslq	%esi, %rdx
	movq	-24(%rbp), %rdi         # 8-byte Reload
	imull	(%rdi,%rdx,4), %r9d
	movslq	%ecx, %rdx
	movl	-28(%rbp), %r10d        # 4-byte Reload
	movslq	%r10d, %r8
	imulq	$400, %rdx, %rdx        # imm = 0x190
	movq	-16(%rbp), %r11         # 8-byte Reload
	addq	%rdx, %r11
	shlq	$2, %r8
	addq	%r8, %r11
	movl	(%r11), %ebx
	subl	%r9d, %ebx
	movl	%ebx, (%r11)
	cmpl	$1, %eax
	movl	%ecx, -48(%rbp)         # 4-byte Spill
	jg	.LBB0_4
.LBB0_5:                                # %for.inc14
                                        #   in Loop: Header=BB0_2 Depth=1
	movl	-44(%rbp), %eax         # 4-byte Reload
	addl	$-1, %eax
	movl	-44(%rbp), %ecx         # 4-byte Reload
	cmpl	$0, %ecx
	movl	%eax, %edx
	movl	%edx, -40(%rbp)         # 4-byte Spill
	movl	%eax, -36(%rbp)         # 4-byte Spill
	jg	.LBB0_2
.LBB0_6:                                # %for.end16
	movl	-36(%rbp), %eax         # 4-byte Reload
	movl	-28(%rbp), %ecx         # 4-byte Reload
	addl	%ecx, %eax
	popq	%rbx
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z10triangularPiPA100_ii, .Lfunc_end0-_Z10triangularPiPA100_ii
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
	subq	$40432, %rsp            # imm = 0x9DF0
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
                                        #       Child Loop BB1_3 Depth 3
	xorl	%eax, %eax
	movl	$100, -40404(%rbp)
	movl	%eax, -40408(%rbp)      # 4-byte Spill
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_3 Depth 3
	movl	-40408(%rbp), %eax      # 4-byte Reload
	movl	%eax, -40412(%rbp)      # 4-byte Spill
	callq	rand
	xorl	%ecx, %ecx
	movl	$1000, %edx             # imm = 0x3E8
	movl	%edx, -40416(%rbp)      # 4-byte Spill
	cltd
	movl	-40416(%rbp), %esi      # 4-byte Reload
	idivl	%esi
	movl	-40412(%rbp), %edi      # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	%edx, -400(%rbp,%r9,4)
	movl	%ecx, -40420(%rbp)      # 4-byte Spill
.LBB1_3:                                # %for.body10
                                        #   Parent Loop BB1_1 Depth=1
                                        #     Parent Loop BB1_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-40420(%rbp), %eax      # 4-byte Reload
	movl	%eax, -40424(%rbp)      # 4-byte Spill
	callq	rand
	leaq	-40400(%rbp), %rcx
	movl	$1000, %edx             # imm = 0x3E8
	movl	%edx, -40428(%rbp)      # 4-byte Spill
	cltd
	movl	-40428(%rbp), %esi      # 4-byte Reload
	idivl	%esi
	movl	-40424(%rbp), %edi      # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	-40412(%rbp), %r8d      # 4-byte Reload
	movl	%r8d, %r10d
	movl	%r10d, %r11d
	imulq	$400, %r9, %r9          # imm = 0x190
	addq	%r9, %rcx
	shlq	$2, %r11
	addq	%r11, %rcx
	movl	%edx, (%rcx)
	addl	$1, %edi
	cmpl	$10, %edi
	movl	%edi, -40420(%rbp)      # 4-byte Spill
	jb	.LBB1_3
# %bb.4:                                # %for.inc19
                                        #   in Loop: Header=BB1_2 Depth=2
	movl	-40412(%rbp), %eax      # 4-byte Reload
	addl	$1, %eax
	cmpl	$100, %eax
	movl	%eax, -40408(%rbp)      # 4-byte Spill
	jb	.LBB1_2
# %bb.5:                                # %for.inc22
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_6
.LBB1_6:                                # %for.end24
	leaq	-40400(%rbp), %rsi
	leaq	-400(%rbp), %rdi
	movl	-40404(%rbp), %edx
	callq	_Z10triangularPiPA100_ii
	xorl	%edx, %edx
	movl	%eax, -40432(%rbp)      # 4-byte Spill
	movl	%edx, %eax
	addq	$40432, %rsp            # imm = 0x9DF0
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
