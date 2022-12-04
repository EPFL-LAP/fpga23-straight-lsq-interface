; ModuleID = '.covariance.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/covariance.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z10covariancePA32_iS0_([32 x i32]* %data, [32 x i32]* %cov) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.inc17, %entry
  %j.09 = phi i32 [ 0, %entry ], [ %inc18, %for.inc17 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %m.07 = phi i32 [ 0, %for.body ], [ %add, %for.body3 ]
  %i.06 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %0 = zext i32 %i.06 to i64
  %1 = zext i32 %j.09 to i64
  %arrayidx5 = getelementptr inbounds [32 x i32], [32 x i32]* %data, i64 %0, i64 %1
  %2 = load i32, i32* %arrayidx5, align 4
  %add = add nsw i32 %m.07, %2
  %inc = add nuw nsw i32 %i.06, 1
  %cmp2 = icmp ult i32 %inc, 32
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.body3
  %mul = shl nsw i32 %add, 1
  br label %for.body9

for.body9:                                        ; preds = %for.body9, %for.end
  %i6.08 = phi i32 [ 0, %for.end ], [ %inc15, %for.body9 ]
  %3 = zext i32 %i6.08 to i64
  %4 = zext i32 %j.09 to i64
  %arrayidx13 = getelementptr inbounds [32 x i32], [32 x i32]* %data, i64 %3, i64 %4
  %5 = load i32, i32* %arrayidx13, align 4
  %sub = sub nsw i32 %5, %mul
  store i32 %sub, i32* %arrayidx13, align 4
  %inc15 = add nuw nsw i32 %i6.08, 1
  %cmp8 = icmp ult i32 %inc15, 32
  br i1 %cmp8, label %for.body9, label %for.inc17

for.inc17:                                        ; preds = %for.body9
  %inc18 = add nuw nsw i32 %j.09, 1
  %cmp = icmp ult i32 %inc18, 32
  br i1 %cmp, label %for.body, label %for.end19

for.end19:                                        ; preds = %for.inc17
  br label %for.body23

for.body23:                                       ; preds = %for.inc52, %for.end19
  %i20.05 = phi i32 [ 0, %for.end19 ], [ %inc53, %for.inc52 ]
  %cmp263 = icmp ult i32 %i20.05, 32
  br i1 %cmp263, label %for.body27.lr.ph, label %for.inc52

for.body27.lr.ph:                                 ; preds = %for.body23
  br label %for.body27

for.body27:                                       ; preds = %for.end43, %for.body27.lr.ph
  %j24.04 = phi i32 [ %i20.05, %for.body27.lr.ph ], [ %inc50, %for.end43 ]
  br label %for.body30

for.body30:                                       ; preds = %for.body30, %for.body27
  %k.02 = phi i32 [ 0, %for.body27 ], [ %inc42, %for.body30 ]
  %c.01 = phi i32 [ 0, %for.body27 ], [ %add40, %for.body30 ]
  %6 = zext i32 %k.02 to i64
  %7 = zext i32 %i20.05 to i64
  %arrayidx34 = getelementptr inbounds [32 x i32], [32 x i32]* %data, i64 %6, i64 %7
  %8 = load i32, i32* %arrayidx34, align 4
  %9 = zext i32 %k.02 to i64
  %10 = zext i32 %j24.04 to i64
  %arrayidx38 = getelementptr inbounds [32 x i32], [32 x i32]* %data, i64 %9, i64 %10
  %11 = load i32, i32* %arrayidx38, align 4
  %mul39 = mul nsw i32 %8, %11
  %add40 = add nsw i32 %c.01, %mul39
  %inc42 = add nuw nsw i32 %k.02, 1
  %cmp29 = icmp ult i32 %inc42, 32
  br i1 %cmp29, label %for.body30, label %for.end43

for.end43:                                        ; preds = %for.body30
  %mul44 = shl nsw i32 %add40, 1
  %12 = zext i32 %i20.05 to i64
  %13 = zext i32 %j24.04 to i64
  %arrayidx48 = getelementptr inbounds [32 x i32], [32 x i32]* %cov, i64 %12, i64 %13
  store i32 %mul44, i32* %arrayidx48, align 4
  %inc50 = add nuw nsw i32 %j24.04, 1
  %cmp26 = icmp ult i32 %inc50, 32
  br i1 %cmp26, label %for.body27, label %for.inc52

for.inc52:                                        ; preds = %for.end43, %for.body23
  %inc53 = add nuw nsw i32 %i20.05, 1
  %cmp22 = icmp ult i32 %inc53, 32
  br i1 %cmp22, label %for.body23, label %for.end54

for.end54:                                        ; preds = %for.inc52
  ret i32 %inc53
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %data = alloca [32 x [32 x i32]], align 16
  %data_ = alloca [32 x [32 x i32]], align 16
  %cov_ = alloca [32 x [32 x i32]], align 16
  call void @srand(i32 9) #3
  br label %for.body

for.body:                                         ; preds = %for.inc27, %entry
  %i.011 = phi i32 [ 0, %entry ], [ %inc28, %for.inc27 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %j.010 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %call = call i32 @rand() #3
  %0 = zext i32 %i.011 to i64
  %1 = zext i32 %j.010 to i64
  %arrayidx5 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %0, i64 %1
  store i32 %call, i32* %arrayidx5, align 4
  %2 = zext i32 %i.011 to i64
  %3 = zext i32 %j.010 to i64
  %arrayidx9 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %2, i64 %3
  %4 = load i32, i32* %arrayidx9, align 4
  %5 = zext i32 %i.011 to i64
  %6 = zext i32 %j.010 to i64
  %arrayidx13 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data_, i64 0, i64 %5, i64 %6
  store i32 %4, i32* %arrayidx13, align 4
  %call14 = call i32 @rand() #3
  %7 = zext i32 %i.011 to i64
  %8 = zext i32 %j.010 to i64
  %arrayidx22 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov_, i64 0, i64 %7, i64 %8
  %9 = load i32, i32* %arrayidx22, align 4
  %10 = zext i32 %i.011 to i64
  %11 = zext i32 %j.010 to i64
  %arrayidx26 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov_, i64 0, i64 %10, i64 %11
  store i32 %9, i32* %arrayidx26, align 4
  %inc = add nuw nsw i32 %j.010, 1
  %cmp2 = icmp ult i32 %inc, 32
  br i1 %cmp2, label %for.body3, label %for.inc27

for.inc27:                                        ; preds = %for.body3
  %inc28 = add nuw nsw i32 %i.011, 1
  %cmp = icmp ult i32 %inc28, 32
  br i1 %cmp, label %for.body, label %for.end29

for.end29:                                        ; preds = %for.inc27
  br label %for.body33

for.body33:                                       ; preds = %for.inc56, %for.end29
  %j30.09 = phi i32 [ 0, %for.end29 ], [ %inc57, %for.inc56 ]
  br label %for.body37

for.body37:                                       ; preds = %for.body37, %for.body33
  %m.07 = phi i32 [ 0, %for.body33 ], [ %add, %for.body37 ]
  %i34.06 = phi i32 [ 0, %for.body33 ], [ %inc43, %for.body37 ]
  %12 = zext i32 %i34.06 to i64
  %13 = zext i32 %j30.09 to i64
  %arrayidx41 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %12, i64 %13
  %14 = load i32, i32* %arrayidx41, align 4
  %add = add nsw i32 %m.07, %14
  %inc43 = add nuw nsw i32 %i34.06, 1
  %cmp36 = icmp ult i32 %inc43, 32
  br i1 %cmp36, label %for.body37, label %for.end44

for.end44:                                        ; preds = %for.body37
  %mul = shl nsw i32 %add, 1
  br label %for.body48

for.body48:                                       ; preds = %for.body48, %for.end44
  %i45.08 = phi i32 [ 0, %for.end44 ], [ %inc54, %for.body48 ]
  %15 = zext i32 %i45.08 to i64
  %16 = zext i32 %j30.09 to i64
  %arrayidx52 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data, i64 0, i64 %15, i64 %16
  %17 = load i32, i32* %arrayidx52, align 4
  %sub = sub nsw i32 %17, %mul
  store i32 %sub, i32* %arrayidx52, align 4
  %inc54 = add nuw nsw i32 %i45.08, 1
  %cmp47 = icmp ult i32 %inc54, 32
  br i1 %cmp47, label %for.body48, label %for.inc56

for.inc56:                                        ; preds = %for.body48
  %inc57 = add nuw nsw i32 %j30.09, 1
  %cmp32 = icmp ult i32 %inc57, 32
  br i1 %cmp32, label %for.body33, label %for.end58

for.end58:                                        ; preds = %for.inc56
  br label %for.body62

for.body62:                                       ; preds = %for.inc95, %for.end58
  %i59.05 = phi i32 [ 0, %for.end58 ], [ %inc96, %for.inc95 ]
  %cmp653 = icmp ult i32 %i59.05, 32
  br i1 %cmp653, label %for.body66.lr.ph, label %for.inc95

for.body66.lr.ph:                                 ; preds = %for.body62
  br label %for.body66

for.body66:                                       ; preds = %for.end82, %for.body66.lr.ph
  %j63.04 = phi i32 [ %i59.05, %for.body66.lr.ph ], [ %inc93, %for.end82 ]
  br label %for.body69

for.body69:                                       ; preds = %for.body69, %for.body66
  %k.02 = phi i32 [ 0, %for.body66 ], [ %inc81, %for.body69 ]
  %inc81 = add nuw nsw i32 %k.02, 1
  %cmp68 = icmp ult i32 %inc81, 32
  br i1 %cmp68, label %for.body69, label %for.end82

for.end82:                                        ; preds = %for.body69
  %inc93 = add nuw nsw i32 %j63.04, 1
  %cmp65 = icmp ult i32 %inc93, 32
  br i1 %cmp65, label %for.body66, label %for.inc95

for.inc95:                                        ; preds = %for.end82, %for.body62
  %inc96 = add nuw nsw i32 %i59.05, 1
  %cmp61 = icmp ult i32 %inc96, 32
  br i1 %cmp61, label %for.body62, label %for.end97

for.end97:                                        ; preds = %for.inc95
  %arraydecay = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %data_, i64 0, i64 0
  %arraydecay98 = getelementptr inbounds [32 x [32 x i32]], [32 x [32 x i32]]* %cov_, i64 0, i64 0
  %call99 = call i32 @_Z10covariancePA32_iS0_([32 x i32]* nonnull %arraydecay, [32 x i32]* nonnull %arraydecay98)
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
