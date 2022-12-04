	.text
	.file	"covariance.cpp"
	.globl	_Z10covariancePA2_iS0_Pi # -- Begin function _Z10covariancePA2_iS0_Pi
	.p2align	4, 0x90
	.type	_Z10covariancePA2_iS0_Pi,@function
_Z10covariancePA2_iS0_Pi:               # @_Z10covariancePA2_iS0_Pi
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
	movl	-28(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movl	%eax, -32(%rbp)         # 4-byte Spill
	movl	%edx, -36(%rbp)         # 4-byte Spill
	movl	%ecx, -40(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-40(%rbp), %eax         # 4-byte Reload
	movl	-36(%rbp), %ecx         # 4-byte Reload
	movl	%eax, %edx
	movl	%edx, %esi
	movl	-32(%rbp), %edx         # 4-byte Reload
	movl	%edx, %edi
	movl	%edi, %r8d
	shlq	$3, %rsi
	movq	-16(%rbp), %r9          # 8-byte Reload
	addq	%rsi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	addl	(%r9), %ecx
	addl	$1, %eax
	cmpl	$2, %eax
	movl	%ecx, %edi
	movl	%ecx, -44(%rbp)         # 4-byte Spill
	movl	%edi, -36(%rbp)         # 4-byte Spill
	movl	%eax, -40(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.3:                                # %for.end
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	$2, %eax
	movl	-44(%rbp), %ecx         # 4-byte Reload
	movl	%eax, -48(%rbp)         # 4-byte Spill
	movl	%ecx, %eax
	cltd
	movl	-48(%rbp), %esi         # 4-byte Reload
	idivl	%esi
	movl	-32(%rbp), %edi         # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movq	-24(%rbp), %r10         # 8-byte Reload
	movl	%eax, (%r10,%r9,4)
	addl	$1, %edi
	cmpl	$2, %edi
	movl	%edi, -28(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.4:                                # %for.end10
	xorl	%eax, %eax
	movl	%eax, -52(%rbp)         # 4-byte Spill
	jmp	.LBB0_5
.LBB0_5:                                # %for.body13
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_6 Depth 2
	movl	-52(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -56(%rbp)         # 4-byte Spill
	movl	%ecx, -60(%rbp)         # 4-byte Spill
.LBB0_6:                                # %for.body16
                                        #   Parent Loop BB0_5 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-60(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movq	-24(%rbp), %rsi         # 8-byte Reload
	movl	(%rsi,%rdx,4), %ecx
	movl	-56(%rbp), %edi         # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %edx
	movl	%eax, %r8d
	movl	%r8d, %r9d
	shlq	$3, %rdx
	movq	-16(%rbp), %r10         # 8-byte Reload
	addq	%rdx, %r10
	shlq	$2, %r9
	addq	%r9, %r10
	movl	(%r10), %r8d
	subl	%ecx, %r8d
	movl	%r8d, (%r10)
	addl	$1, %eax
	cmpl	$2, %eax
	movl	%eax, -60(%rbp)         # 4-byte Spill
	jb	.LBB0_6
# %bb.7:                                # %for.inc26
                                        #   in Loop: Header=BB0_5 Depth=1
	movl	-56(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$2, %eax
	movl	%eax, -52(%rbp)         # 4-byte Spill
	jb	.LBB0_5
# %bb.8:                                # %for.end28
	xorl	%eax, %eax
	movl	%eax, -64(%rbp)         # 4-byte Spill
	jmp	.LBB0_9
.LBB0_9:                                # %for.body31
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_11 Depth 2
                                        #       Child Loop BB0_12 Depth 3
	movl	-64(%rbp), %eax         # 4-byte Reload
	cmpl	$2, %eax
	movl	%eax, -68(%rbp)         # 4-byte Spill
	jae	.LBB0_14
# %bb.10:                               # %for.body34.lr.ph
                                        #   in Loop: Header=BB0_9 Depth=1
	movl	-68(%rbp), %eax         # 4-byte Reload
	movl	%eax, -72(%rbp)         # 4-byte Spill
	jmp	.LBB0_11
.LBB0_11:                               # %for.body34
                                        #   Parent Loop BB0_9 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB0_12 Depth 3
	movl	-72(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movl	%eax, -76(%rbp)         # 4-byte Spill
	movl	%edx, -80(%rbp)         # 4-byte Spill
	movl	%ecx, -84(%rbp)         # 4-byte Spill
.LBB0_12:                               # %for.body38
                                        #   Parent Loop BB0_9 Depth=1
                                        #     Parent Loop BB0_11 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-84(%rbp), %eax         # 4-byte Reload
	movl	-80(%rbp), %ecx         # 4-byte Reload
	movl	%eax, %edx
	movl	%edx, %esi
	movl	-68(%rbp), %edx         # 4-byte Reload
	movl	%edx, %edi
	movl	%edi, %r8d
	shlq	$3, %rsi
	movq	-16(%rbp), %r9          # 8-byte Reload
	addq	%rsi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	movl	(%r9), %edi
	movl	%eax, %r10d
	movl	%r10d, %esi
	movl	-76(%rbp), %r10d        # 4-byte Reload
	movl	%r10d, %r11d
	movl	%r11d, %r8d
	shlq	$3, %rsi
	movq	-16(%rbp), %r9          # 8-byte Reload
	addq	%rsi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	imull	(%r9), %edi
	addl	%edi, %ecx
	addl	$1, %eax
	cmpl	$2, %eax
	movl	%ecx, %edi
	movl	%ecx, -88(%rbp)         # 4-byte Spill
	movl	%edi, -80(%rbp)         # 4-byte Spill
	movl	%eax, -84(%rbp)         # 4-byte Spill
	jb	.LBB0_12
# %bb.13:                               # %for.end50
                                        #   in Loop: Header=BB0_11 Depth=2
	movl	-68(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	-76(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	shlq	$3, %rdx
	movq	-8(%rbp), %r8           # 8-byte Reload
	addq	%rdx, %r8
	shlq	$2, %rdi
	addq	%rdi, %r8
	movl	-88(%rbp), %esi         # 4-byte Reload
	movl	%esi, (%r8)
	movl	%ecx, %r9d
	movl	%r9d, %edx
	movl	%eax, %r9d
	movl	%r9d, %edi
	shlq	$3, %rdx
	movq	-8(%rbp), %r8           # 8-byte Reload
	addq	%rdx, %r8
	shlq	$2, %rdi
	addq	%rdi, %r8
	movl	%esi, (%r8)
	addl	$1, %ecx
	cmpl	$2, %ecx
	movl	%ecx, -72(%rbp)         # 4-byte Spill
	jb	.LBB0_11
.LBB0_14:                               # %for.inc62
                                        #   in Loop: Header=BB0_9 Depth=1
	movl	-68(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$2, %eax
	movl	%eax, %ecx
	movl	%eax, -92(%rbp)         # 4-byte Spill
	movl	%ecx, -64(%rbp)         # 4-byte Spill
	jb	.LBB0_9
# %bb.15:                               # %for.end64
	movl	-92(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z10covariancePA2_iS0_Pi, .Lfunc_end0-_Z10covariancePA2_iS0_Pi
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
	subq	$64, %rsp
	movl	$13, %edi
	callq	srand
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
                                        #       Child Loop BB1_3 Depth 3
	xorl	%eax, %eax
	movl	%eax, -44(%rbp)         # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_3 Depth 3
	movl	-44(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -48(%rbp)         # 4-byte Spill
	movl	%ecx, -52(%rbp)         # 4-byte Spill
.LBB1_3:                                # %for.body6
                                        #   Parent Loop BB1_1 Depth=1
                                        #     Parent Loop BB1_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-52(%rbp), %eax         # 4-byte Reload
	movl	%eax, -56(%rbp)         # 4-byte Spill
	callq	rand
	leaq	-16(%rbp), %rcx
	movl	$100, %edx
	movl	%edx, -60(%rbp)         # 4-byte Spill
	cltd
	movl	-60(%rbp), %esi         # 4-byte Reload
	idivl	%esi
	movl	-48(%rbp), %edi         # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	-56(%rbp), %r8d         # 4-byte Reload
	movl	%r8d, %r10d
	movl	%r10d, %r11d
	shlq	$3, %r9
	addq	%r9, %rcx
	shlq	$2, %r11
	addq	%r11, %rcx
	movl	%edx, (%rcx)
	addl	$1, %r8d
	cmpl	$2, %r8d
	movl	%r8d, -52(%rbp)         # 4-byte Spill
	jb	.LBB1_3
# %bb.4:                                # %for.inc11
                                        #   in Loop: Header=BB1_2 Depth=2
	movl	-48(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$2, %eax
	movl	%eax, -44(%rbp)         # 4-byte Spill
	jb	.LBB1_2
# %bb.5:                                # %for.inc14
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_6
.LBB1_6:                                # %for.end16
	leaq	-40(%rbp), %rdx
	leaq	-32(%rbp), %rsi
	leaq	-16(%rbp), %rdi
	callq	_Z10covariancePA2_iS0_Pi
	xorl	%ecx, %ecx
	movl	%eax, -64(%rbp)         # 4-byte Spill
	movl	%ecx, %eax
	addq	$64, %rsp
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
