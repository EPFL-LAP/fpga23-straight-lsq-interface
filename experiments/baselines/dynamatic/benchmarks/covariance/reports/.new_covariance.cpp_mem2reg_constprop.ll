; ModuleID = '.new_covariance.cpp_mem2reg.ll'
source_filename = "src/new_covariance.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z14new_covariancePA32_iS0_([32 x i32]* %data, [32 x i32]* %cov) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc17
  %j.09 = phi i32 [ 0, %entry ], [ %inc18, %for.inc17 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body, %for.inc
  %m.07 = phi i32 [ 0, %for.body ], [ %add, %for.inc ]
  %i.06 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %idxprom = sext i32 %i.06 to i64
  %arrayidx = getelementptr inbounds [32 x i32], [32 x i32]* %data, i64 %idxprom
  %idxprom4 = sext i32 %j.09 to i64
  %arrayidx5 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx, i64 0, i64 %idxprom4
  %0 = load i32, i32* %arrayidx5, align 4
  %add = add nsw i32 %m.07, %0
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %i.06, 1
  %cmp2 = icmp slt i32 %inc, 32
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.inc
  %m.0.lcssa = phi i32 [ %add, %for.inc ]
  %mul = mul nsw i32 %m.0.lcssa, 2
  br label %for.body9

for.body9:                                        ; preds = %for.end, %for.inc14
  %i6.08 = phi i32 [ 0, %for.end ], [ %inc15, %for.inc14 ]
  %idxprom10 = sext i32 %i6.08 to i64
  %arrayidx11 = getelementptr inbounds [32 x i32], [32 x i32]* %data, i64 %idxprom10
  %idxprom12 = sext i32 %j.09 to i64
  %arrayidx13 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  %1 = load i32, i32* %arrayidx13, align 4
  %sub = sub nsw i32 %1, %mul
  store i32 %sub, i32* %arrayidx13, align 4
  br label %for.inc14

for.inc14:                                        ; preds = %for.body9
  %inc15 = add nsw i32 %i6.08, 1
  %cmp8 = icmp slt i32 %inc15, 32
  br i1 %cmp8, label %for.body9, label %for.end16

for.end16:                                        ; preds = %for.inc14
  br label %for.inc17

for.inc17:                                        ; preds = %for.end16
  %inc18 = add nsw i32 %j.09, 1
  %cmp = icmp slt i32 %inc18, 32
  br i1 %cmp, label %for.body, label %for.end19

for.end19:                                        ; preds = %for.inc17
  br label %for.body23

for.body23:                                       ; preds = %for.end19, %for.inc52
  %i20.05 = phi i32 [ 0, %for.end19 ], [ %inc53, %for.inc52 ]
  %cmp263 = icmp slt i32 %i20.05, 32
  br i1 %cmp263, label %for.body27.lr.ph, label %for.end51

for.body27.lr.ph:                                 ; preds = %for.body23
  br label %for.body27

for.body27:                                       ; preds = %for.body27.lr.ph, %for.inc49
  %j24.04 = phi i32 [ %i20.05, %for.body27.lr.ph ], [ %inc50, %for.inc49 ]
  br label %for.body30

for.body30:                                       ; preds = %for.body27, %for.inc41
  %k.02 = phi i32 [ 0, %for.body27 ], [ %inc42, %for.inc41 ]
  %c.01 = phi i32 [ 0, %for.body27 ], [ %add40, %for.inc41 ]
  %idxprom31 = sext i32 %k.02 to i64
  %arrayidx32 = getelementptr inbounds [32 x i32], [32 x i32]* %data, i64 %idxprom31
  %idxprom33 = sext i32 %i20.05 to i64
  %arrayidx34 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx32, i64 0, i64 %idxprom33
  %2 = load i32, i32* %arrayidx34, align 4
  %idxprom35 = sext i32 %k.02 to i64
  %arrayidx36 = getelementptr inbounds [32 x i32], [32 x i32]* %data, i64 %idxprom35
  %idxprom37 = sext i32 %j24.04 to i64
  %arrayidx38 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx36, i64 0, i64 %idxprom37
  %3 = load i32, i32* %arrayidx38, align 4
  %mul39 = mul nsw i32 %2, %3
  %add40 = add nsw i32 %c.01, %mul39
  br label %for.inc41

for.inc41:                                        ; preds = %for.body30
  %inc42 = add nsw i32 %k.02, 1
  %cmp29 = icmp slt i32 %inc42, 32
  br i1 %cmp29, label %for.body30, label %for.end43

for.end43:                                        ; preds = %for.inc41
  %c.0.lcssa = phi i32 [ %add40, %for.inc41 ]
  %mul44 = mul nsw i32 %c.0.lcssa, 2
  %idxprom45 = sext i32 %i20.05 to i64
  %arrayidx46 = getelementptr inbounds [32 x i32], [32 x i32]* %cov, i64 %idxprom45
  %idxprom47 = sext i32 %j24.04 to i64
  %arrayidx48 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx46, i64 0, i64 %idxprom47
  store i32 %mul44, i32* %arrayidx48, align 4
  br label %for.inc49

for.inc49:                                        ; preds = %for.end43
  %inc50 = add nsw i32 %j24.04, 1
  %cmp26 = icmp slt i32 %inc50, 32
  br i1 %cmp26, label %for.body27, label %for.cond25.for.end51_crit_edge

for.cond25.for.end51_crit_edge:                   ; preds = %for.inc49
  br label %for.end51

for.end51:                                        ; preds = %for.cond25.for.end51_crit_edge, %for.body23
  br label %for.inc52

for.inc52:                                        ; preds = %for.end51
  %inc53 = add nsw i32 %i20.05, 1
  %cmp22 = icmp slt i32 %inc53, 32
  br i1 %cmp22, label %for.body23, label %for.end54

for.end54:                                        ; preds = %for.inc52
  %i20.0.lcssa = phi i32 [ %inc53, %for.inc52 ]
  ret i32 %i20.0.lcssa
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %data = alloca [32 x [32 x i32]], align 16
  %cov = alloca [32 x [32 x i32]], align 16
  %data_ = alloca [32 x [32 x i32]], align 16
  %cov_ = alloca [32 x [32 x i32]], align 16
  call void @srand(i32 9) #3
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc27
  %i.011 = phi i32 [ 0, %entry ], [ %inc28, %for.inc27 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body, %for.inc
  %j.010 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %call = call i32 @rand() #3
  %idxprom = sext i32 %i.011 to i64
  %arrayidx = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %idxprom
  %idxprom4 = sext i32 %j.010 to i64
  %arrayidx5 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %call, i32* %arrayidx5, align 4
  %idxprom6 = sext i32 %i.011 to i64
  %arrayidx7 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %idxprom6
  %idxprom8 = sext i32 %j.010 to i64
  %arrayidx9 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  %0 = load i32, i32* %arrayidx9, align 4
  %idxprom10 = sext i32 %i.011 to i64
  %arrayidx11 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data_, i64 0, i64 %idxprom10
  %idxprom12 = sext i32 %j.010 to i64
  %arrayidx13 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 %0, i32* %arrayidx13, align 4
  %call14 = call i32 @rand() #3
  %idxprom15 = sext i32 %i.011 to i64
  %arrayidx16 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov, i64 0, i64 %idxprom15
  %idxprom17 = sext i32 %j.010 to i64
  %arrayidx18 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx16, i64 0, i64 %idxprom17
  store i32 %call14, i32* %arrayidx18, align 4
  %idxprom19 = sext i32 %i.011 to i64
  %arrayidx20 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov_, i64 0, i64 %idxprom19
  %idxprom21 = sext i32 %j.010 to i64
  %arrayidx22 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx20, i64 0, i64 %idxprom21
  %1 = load i32, i32* %arrayidx22, align 4
  %idxprom23 = sext i32 %i.011 to i64
  %arrayidx24 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov_, i64 0, i64 %idxprom23
  %idxprom25 = sext i32 %j.010 to i64
  %arrayidx26 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx24, i64 0, i64 %idxprom25
  store i32 %1, i32* %arrayidx26, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.010, 1
  %cmp2 = icmp slt i32 %inc, 32
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.inc
  br label %for.inc27

for.inc27:                                        ; preds = %for.end
  %inc28 = add nsw i32 %i.011, 1
  %cmp = icmp slt i32 %inc28, 32
  br i1 %cmp, label %for.body, label %for.end29

for.end29:                                        ; preds = %for.inc27
  br label %for.body33

for.body33:                                       ; preds = %for.end29, %for.inc56
  %j30.09 = phi i32 [ 0, %for.end29 ], [ %inc57, %for.inc56 ]
  br label %for.body37

for.body37:                                       ; preds = %for.body33, %for.inc42
  %m.07 = phi i32 [ 0, %for.body33 ], [ %add, %for.inc42 ]
  %i34.06 = phi i32 [ 0, %for.body33 ], [ %inc43, %for.inc42 ]
  %idxprom38 = sext i32 %i34.06 to i64
  %arrayidx39 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %idxprom38
  %idxprom40 = sext i32 %j30.09 to i64
  %arrayidx41 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx39, i64 0, i64 %idxprom40
  %2 = load i32, i32* %arrayidx41, align 4
  %add = add nsw i32 %m.07, %2
  br label %for.inc42

for.inc42:                                        ; preds = %for.body37
  %inc43 = add nsw i32 %i34.06, 1
  %cmp36 = icmp slt i32 %inc43, 32
  br i1 %cmp36, label %for.body37, label %for.end44

for.end44:                                        ; preds = %for.inc42
  %m.0.lcssa = phi i32 [ %add, %for.inc42 ]
  %mul = mul nsw i32 %m.0.lcssa, 2
  br label %for.body48

for.body48:                                       ; preds = %for.end44, %for.inc53
  %i45.08 = phi i32 [ 0, %for.end44 ], [ %inc54, %for.inc53 ]
  %idxprom49 = sext i32 %i45.08 to i64
  %arrayidx50 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %idxprom49
  %idxprom51 = sext i32 %j30.09 to i64
  %arrayidx52 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx50, i64 0, i64 %idxprom51
  %3 = load i32, i32* %arrayidx52, align 4
  %sub = sub nsw i32 %3, %mul
  store i32 %sub, i32* %arrayidx52, align 4
  br label %for.inc53

for.inc53:                                        ; preds = %for.body48
  %inc54 = add nsw i32 %i45.08, 1
  %cmp47 = icmp slt i32 %inc54, 32
  br i1 %cmp47, label %for.body48, label %for.end55

for.end55:                                        ; preds = %for.inc53
  br label %for.inc56

for.inc56:                                        ; preds = %for.end55
  %inc57 = add nsw i32 %j30.09, 1
  %cmp32 = icmp slt i32 %inc57, 32
  br i1 %cmp32, label %for.body33, label %for.end58

for.end58:                                        ; preds = %for.inc56
  br label %for.body62

for.body62:                                       ; preds = %for.end58, %for.inc95
  %i59.05 = phi i32 [ 0, %for.end58 ], [ %inc96, %for.inc95 ]
  %cmp653 = icmp slt i32 %i59.05, 32
  br i1 %cmp653, label %for.body66.lr.ph, label %for.end94

for.body66.lr.ph:                                 ; preds = %for.body62
  br label %for.body66

for.body66:                                       ; preds = %for.body66.lr.ph, %for.inc92
  %j63.04 = phi i32 [ %i59.05, %for.body66.lr.ph ], [ %inc93, %for.inc92 ]
  br label %for.body69

for.body69:                                       ; preds = %for.body66, %for.inc80
  %k.02 = phi i32 [ 0, %for.body66 ], [ %inc81, %for.inc80 ]
  %c.01 = phi i32 [ 0, %for.body66 ], [ %add79, %for.inc80 ]
  %idxprom70 = sext i32 %k.02 to i64
  %arrayidx71 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %idxprom70
  %idxprom72 = sext i32 %i59.05 to i64
  %arrayidx73 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx71, i64 0, i64 %idxprom72
  %4 = load i32, i32* %arrayidx73, align 4
  %idxprom74 = sext i32 %k.02 to i64
  %arrayidx75 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %idxprom74
  %idxprom76 = sext i32 %j63.04 to i64
  %arrayidx77 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx75, i64 0, i64 %idxprom76
  %5 = load i32, i32* %arrayidx77, align 4
  %mul78 = mul nsw i32 %4, %5
  %add79 = add nsw i32 %c.01, %mul78
  br label %for.inc80

for.inc80:                                        ; preds = %for.body69
  %inc81 = add nsw i32 %k.02, 1
  %cmp68 = icmp slt i32 %inc81, 32
  br i1 %cmp68, label %for.body69, label %for.end82

for.end82:                                        ; preds = %for.inc80
  %c.0.lcssa = phi i32 [ %add79, %for.inc80 ]
  %mul83 = mul nsw i32 %c.0.lcssa, 2
  %idxprom84 = sext i32 %i59.05 to i64
  %arrayidx85 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov, i64 0, i64 %idxprom84
  %idxprom86 = sext i32 %j63.04 to i64
  %arrayidx87 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx85, i64 0, i64 %idxprom86
  store i32 %mul83, i32* %arrayidx87, align 4
  %idxprom88 = sext i32 %j63.04 to i64
  %arrayidx89 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov, i64 0, i64 %idxprom88
  %idxprom90 = sext i32 %i59.05 to i64
  %arrayidx91 = getelementptr inbounds [32 x i32], [32 x i32]* %arrayidx89, i64 0, i64 %idxprom90
  store i32 %mul83, i32* %arrayidx91, align 4
  br label %for.inc92

for.inc92:                                        ; preds = %for.end82
  %inc93 = add nsw i32 %j63.04, 1
  %cmp65 = icmp slt i32 %inc93, 32
  br i1 %cmp65, label %for.body66, label %for.cond64.for.end94_crit_edge

for.cond64.for.end94_crit_edge:                   ; preds = %for.inc92
  br label %for.end94

for.end94:                                        ; preds = %for.cond64.for.end94_crit_edge, %for.body62
  br label %for.inc95

for.inc95:                                        ; preds = %for.end94
  %inc96 = add nsw i32 %i59.05, 1
  %cmp61 = icmp slt i32 %inc96, 32
  br i1 %cmp61, label %for.body62, label %for.end97

for.end97:                                        ; preds = %for.inc95
  %arraydecay = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data_, i32 0, i32 0
  %arraydecay98 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov_, i32 0, i32 0
  %call99 = call i32 @_Z14new_covariancePA32_iS0_([32 x i32]* %arraydecay, [32 x i32]* %arraydecay98)
  ret i32 0
}

; Function Attrs: nounwind
declare void @srand(i32) #2

; Function Attrs: nounwind
declare i32 @rand() #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
