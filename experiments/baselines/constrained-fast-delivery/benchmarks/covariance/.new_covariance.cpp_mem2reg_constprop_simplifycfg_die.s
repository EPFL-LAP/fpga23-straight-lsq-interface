	.text
	.file	"new_covariance.cpp"
	.globl	_Z14new_covariancePA32_iS0_ # -- Begin function _Z14new_covariancePA32_iS0_
	.p2align	4, 0x90
	.type	_Z14new_covariancePA32_iS0_,@function
_Z14new_covariancePA32_iS0_:            # @_Z14new_covariancePA32_iS0_
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
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
                                        #     Child Loop BB0_4 Depth 2
	movl	-20(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movl	%eax, -24(%rbp)         # 4-byte Spill
	movl	%edx, -28(%rbp)         # 4-byte Spill
	movl	%ecx, -32(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-32(%rbp), %eax         # 4-byte Reload
	movl	-28(%rbp), %ecx         # 4-byte Reload
	movl	%eax, %edx
	movl	%edx, %esi
	movl	-24(%rbp), %edx         # 4-byte Reload
	movl	%edx, %edi
	movl	%edi, %r8d
	shlq	$7, %rsi
	movq	-8(%rbp), %r9           # 8-byte Reload
	addq	%rsi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	addl	(%r9), %ecx
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%ecx, %edi
	movl	%ecx, -36(%rbp)         # 4-byte Spill
	movl	%edi, -28(%rbp)         # 4-byte Spill
	movl	%eax, -32(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.3:                                # %for.end
                                        #   in Loop: Header=BB0_1 Depth=1
	xorl	%eax, %eax
	movl	-36(%rbp), %ecx         # 4-byte Reload
	shll	$1, %ecx
	movl	%ecx, -40(%rbp)         # 4-byte Spill
	movl	%eax, -44(%rbp)         # 4-byte Spill
.LBB0_4:                                # %for.body9
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-44(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	-24(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	shlq	$7, %rdx
	movq	-8(%rbp), %r8           # 8-byte Reload
	addq	%rdx, %r8
	shlq	$2, %rdi
	addq	%rdi, %r8
	movl	(%r8), %esi
	movl	-40(%rbp), %r9d         # 4-byte Reload
	subl	%r9d, %esi
	movl	%esi, (%r8)
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, -44(%rbp)         # 4-byte Spill
	jb	.LBB0_4
# %bb.5:                                # %for.inc17
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-24(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, -20(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.6:                                # %for.end19
	xorl	%eax, %eax
	movl	%eax, -48(%rbp)         # 4-byte Spill
	jmp	.LBB0_7
.LBB0_7:                                # %for.body23
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_9 Depth 2
                                        #       Child Loop BB0_10 Depth 3
	movl	-48(%rbp), %eax         # 4-byte Reload
	cmpl	$32, %eax
	movl	%eax, -52(%rbp)         # 4-byte Spill
	jae	.LBB0_12
# %bb.8:                                # %for.body27.lr.ph
                                        #   in Loop: Header=BB0_7 Depth=1
	movl	-52(%rbp), %eax         # 4-byte Reload
	movl	%eax, -56(%rbp)         # 4-byte Spill
	jmp	.LBB0_9
.LBB0_9:                                # %for.body27
                                        #   Parent Loop BB0_7 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB0_10 Depth 3
	movl	-56(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movl	%eax, -60(%rbp)         # 4-byte Spill
	movl	%edx, -64(%rbp)         # 4-byte Spill
	movl	%ecx, -68(%rbp)         # 4-byte Spill
.LBB0_10:                               # %for.body30
                                        #   Parent Loop BB0_7 Depth=1
                                        #     Parent Loop BB0_9 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-68(%rbp), %eax         # 4-byte Reload
	movl	-64(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, %edx
	movl	%edx, %esi
	movl	-52(%rbp), %edx         # 4-byte Reload
	movl	%edx, %edi
	movl	%edi, %r8d
	shlq	$7, %rsi
	movq	-8(%rbp), %r9           # 8-byte Reload
	addq	%rsi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	movl	(%r9), %edi
	movl	%ecx, %r10d
	movl	%r10d, %esi
	movl	-60(%rbp), %r10d        # 4-byte Reload
	movl	%r10d, %r11d
	movl	%r11d, %r8d
	shlq	$7, %rsi
	movq	-8(%rbp), %r9           # 8-byte Reload
	addq	%rsi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	imull	(%r9), %edi
	addl	%edi, %eax
	addl	$1, %ecx
	cmpl	$32, %ecx
	movl	%eax, %edi
	movl	%eax, -72(%rbp)         # 4-byte Spill
	movl	%ecx, -64(%rbp)         # 4-byte Spill
	movl	%edi, -68(%rbp)         # 4-byte Spill
	jb	.LBB0_10
# %bb.11:                               # %for.end43
                                        #   in Loop: Header=BB0_9 Depth=2
	movl	-72(%rbp), %eax         # 4-byte Reload
	shll	$1, %eax
	movl	-52(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, %edx
	movl	%edx, %esi
	movl	-60(%rbp), %edx         # 4-byte Reload
	movl	%edx, %edi
	movl	%edi, %r8d
	shlq	$7, %rsi
	movq	-16(%rbp), %r9          # 8-byte Reload
	addq	%rsi, %r9
	shlq	$2, %r8
	addq	%r8, %r9
	movl	%eax, (%r9)
	addl	$1, %edx
	cmpl	$32, %edx
	movl	%edx, -56(%rbp)         # 4-byte Spill
	jb	.LBB0_9
.LBB0_12:                               # %for.inc52
                                        #   in Loop: Header=BB0_7 Depth=1
	movl	-52(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, %ecx
	movl	%eax, -76(%rbp)         # 4-byte Spill
	movl	%ecx, -48(%rbp)         # 4-byte Spill
	jb	.LBB0_7
# %bb.13:                               # %for.end54
	movl	-76(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z14new_covariancePA32_iS0_, .Lfunc_end0-_Z14new_covariancePA32_iS0_
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
	pushq	%rbx
	subq	$12376, %rsp            # imm = 0x3058
	.cfi_offset %rbx, -24
	movl	$9, %edi
	callq	srand
	xorl	%edi, %edi
	movl	%edi, -12308(%rbp)      # 4-byte Spill
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
	movl	-12308(%rbp), %eax      # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -12312(%rbp)      # 4-byte Spill
	movl	%ecx, -12316(%rbp)      # 4-byte Spill
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-12316(%rbp), %eax      # 4-byte Reload
	movl	%eax, -12320(%rbp)      # 4-byte Spill
	callq	rand
	leaq	-8208(%rbp), %rcx
	leaq	-4112(%rbp), %rdx
	movl	-12312(%rbp), %esi      # 4-byte Reload
	movl	%esi, %edi
	movl	%edi, %r8d
	movl	-12320(%rbp), %edi      # 4-byte Reload
	movl	%edi, %r9d
	movl	%r9d, %r10d
	shlq	$7, %r8
	movq	%rdx, %r11
	addq	%r8, %r11
	shlq	$2, %r10
	addq	%r10, %r11
	movl	%eax, (%r11)
	movl	%esi, %eax
	movl	%eax, %r8d
	movl	%edi, %eax
	movl	%eax, %r10d
	shlq	$7, %r8
	addq	%r8, %rdx
	shlq	$2, %r10
	addq	%r10, %rdx
	movl	(%rdx), %eax
	movl	%esi, %r9d
	movl	%r9d, %edx
	movl	%edi, %r9d
	movl	%r9d, %r8d
	shlq	$7, %rdx
	addq	%rdx, %rcx
	shlq	$2, %r8
	addq	%r8, %rcx
	movl	%eax, (%rcx)
	callq	rand
	leaq	-12304(%rbp), %rcx
	movl	-12312(%rbp), %esi      # 4-byte Reload
	movl	%esi, %edi
	movl	%edi, %edx
	movl	-12320(%rbp), %edi      # 4-byte Reload
	movl	%edi, %r9d
	movl	%r9d, %r8d
	shlq	$7, %rdx
	movq	%rcx, %r10
	addq	%rdx, %r10
	shlq	$2, %r8
	addq	%r8, %r10
	movl	(%r10), %r9d
	movl	%esi, %ebx
	movl	%ebx, %edx
	movl	%edi, %ebx
	movl	%ebx, %r8d
	shlq	$7, %rdx
	addq	%rdx, %rcx
	shlq	$2, %r8
	addq	%r8, %rcx
	movl	%r9d, (%rcx)
	addl	$1, %edi
	cmpl	$32, %edi
	movl	%eax, -12324(%rbp)      # 4-byte Spill
	movl	%edi, -12316(%rbp)      # 4-byte Spill
	jb	.LBB1_2
# %bb.3:                                # %for.inc27
                                        #   in Loop: Header=BB1_1 Depth=1
	movl	-12312(%rbp), %eax      # 4-byte Reload
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, -12308(%rbp)      # 4-byte Spill
	jb	.LBB1_1
# %bb.4:                                # %for.end29
	xorl	%eax, %eax
	movl	%eax, -12328(%rbp)      # 4-byte Spill
	jmp	.LBB1_5
.LBB1_5:                                # %for.body33
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_6 Depth 2
                                        #     Child Loop BB1_8 Depth 2
	movl	-12328(%rbp), %eax      # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movl	%eax, -12332(%rbp)      # 4-byte Spill
	movl	%edx, -12336(%rbp)      # 4-byte Spill
	movl	%ecx, -12340(%rbp)      # 4-byte Spill
.LBB1_6:                                # %for.body37
                                        #   Parent Loop BB1_5 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-12340(%rbp), %eax      # 4-byte Reload
	movl	-12336(%rbp), %ecx      # 4-byte Reload
	leaq	-4112(%rbp), %rdx
	movl	%eax, %esi
	movl	%esi, %edi
	movl	-12332(%rbp), %esi      # 4-byte Reload
	movl	%esi, %r8d
	movl	%r8d, %r9d
	shlq	$7, %rdi
	addq	%rdi, %rdx
	shlq	$2, %r9
	addq	%r9, %rdx
	addl	(%rdx), %ecx
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%ecx, %r8d
	movl	%ecx, -12344(%rbp)      # 4-byte Spill
	movl	%r8d, -12336(%rbp)      # 4-byte Spill
	movl	%eax, -12340(%rbp)      # 4-byte Spill
	jb	.LBB1_6
# %bb.7:                                # %for.end44
                                        #   in Loop: Header=BB1_5 Depth=1
	xorl	%eax, %eax
	movl	-12344(%rbp), %ecx      # 4-byte Reload
	shll	$1, %ecx
	movl	%ecx, -12348(%rbp)      # 4-byte Spill
	movl	%eax, -12352(%rbp)      # 4-byte Spill
.LBB1_8:                                # %for.body48
                                        #   Parent Loop BB1_5 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-12352(%rbp), %eax      # 4-byte Reload
	leaq	-4112(%rbp), %rcx
	movl	%eax, %edx
	movl	%edx, %esi
	movl	-12332(%rbp), %edx      # 4-byte Reload
	movl	%edx, %edi
	movl	%edi, %r8d
	shlq	$7, %rsi
	addq	%rsi, %rcx
	shlq	$2, %r8
	addq	%r8, %rcx
	movl	(%rcx), %edi
	movl	-12348(%rbp), %r9d      # 4-byte Reload
	subl	%r9d, %edi
	movl	%edi, (%rcx)
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, -12352(%rbp)      # 4-byte Spill
	jb	.LBB1_8
# %bb.9:                                # %for.inc56
                                        #   in Loop: Header=BB1_5 Depth=1
	movl	-12332(%rbp), %eax      # 4-byte Reload
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, -12328(%rbp)      # 4-byte Spill
	jb	.LBB1_5
# %bb.10:                               # %for.end58
	xorl	%eax, %eax
	movl	%eax, -12356(%rbp)      # 4-byte Spill
	jmp	.LBB1_11
.LBB1_11:                               # %for.body62
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_13 Depth 2
                                        #       Child Loop BB1_14 Depth 3
	movl	-12356(%rbp), %eax      # 4-byte Reload
	cmpl	$32, %eax
	movl	%eax, -12360(%rbp)      # 4-byte Spill
	jae	.LBB1_16
# %bb.12:                               # %for.body66.lr.ph
                                        #   in Loop: Header=BB1_11 Depth=1
	movl	-12360(%rbp), %eax      # 4-byte Reload
	movl	%eax, -12364(%rbp)      # 4-byte Spill
	jmp	.LBB1_13
.LBB1_13:                               # %for.body66
                                        #   Parent Loop BB1_11 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_14 Depth 3
	movl	-12364(%rbp), %eax      # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -12368(%rbp)      # 4-byte Spill
	movl	%ecx, -12372(%rbp)      # 4-byte Spill
.LBB1_14:                               # %for.body69
                                        #   Parent Loop BB1_11 Depth=1
                                        #     Parent Loop BB1_13 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-12372(%rbp), %eax      # 4-byte Reload
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, -12372(%rbp)      # 4-byte Spill
	jb	.LBB1_14
# %bb.15:                               # %for.end82
                                        #   in Loop: Header=BB1_13 Depth=2
	movl	-12368(%rbp), %eax      # 4-byte Reload
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, -12364(%rbp)      # 4-byte Spill
	jb	.LBB1_13
.LBB1_16:                               # %for.inc95
                                        #   in Loop: Header=BB1_11 Depth=1
	movl	-12360(%rbp), %eax      # 4-byte Reload
	addl	$1, %eax
	cmpl	$32, %eax
	movl	%eax, -12356(%rbp)      # 4-byte Spill
	jb	.LBB1_11
# %bb.17:                               # %for.end97
	leaq	-12304(%rbp), %rsi
	leaq	-8208(%rbp), %rdi
	callq	_Z14new_covariancePA32_iS0_
	xorl	%ecx, %ecx
	movl	%eax, -12376(%rbp)      # 4-byte Spill
	movl	%ecx, %eax
	addq	$12376, %rsp            # imm = 0x3058
	popq	%rbx
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
