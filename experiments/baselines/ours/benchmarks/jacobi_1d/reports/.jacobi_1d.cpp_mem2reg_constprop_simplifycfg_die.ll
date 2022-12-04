; ModuleID = '.jacobi_1d.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/jacobi_1d.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z9jacobi_1dPiS_S_(i32* %A_1, i32* %A_2, i32* %B) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.inc22, %entry
  %t.03 = phi i32 [ 0, %entry ], [ %inc23, %for.inc22 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %i.01 = phi i32 [ 1, %for.body ], [ %inc, %for.body3 ]
  %sub = add nsw i32 %i.01, -1
  %idxprom = sext i32 %sub to i64
  %arrayidx = getelementptr inbounds i32, i32* %A_1, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %1 = zext i32 %i.01 to i64
  %arrayidx5 = getelementptr inbounds i32, i32* %A_1, i64 %1
  %2 = load i32, i32* %arrayidx5, align 4
  %add = add nsw i32 %0, %2
  %add6 = add nuw nsw i32 %i.01, 1
  %3 = zext i32 %add6 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %A_1, i64 %3
  %4 = load i32, i32* %arrayidx8, align 4
  %add9 = add nsw i32 %add, %4
  %mul = mul nsw i32 %add9, 3
  %5 = zext i32 %i.01 to i64
  %arrayidx11 = getelementptr inbounds i32, i32* %B, i64 %5
  store i32 %mul, i32* %arrayidx11, align 4
  %inc = add nuw nsw i32 %i.01, 1
  %cmp2 = icmp ult i32 %inc, 99
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.body3
  br label %for.body14

for.body14:                                       ; preds = %for.body14, %for.end
  %j.02 = phi i32 [ 1, %for.end ], [ %inc20, %for.body14 ]
  %6 = zext i32 %j.02 to i64
  %arrayidx16 = getelementptr inbounds i32, i32* %B, i64 %6
  %7 = load i32, i32* %arrayidx16, align 4
  %8 = zext i32 %j.02 to i64
  %arrayidx18 = getelementptr inbounds i32, i32* %A_2, i64 %8
  store i32 %7, i32* %arrayidx18, align 4
  %inc20 = add nuw nsw i32 %j.02, 1
  %cmp13 = icmp ult i32 %inc20, 99
  br i1 %cmp13, label %for.body14, label %for.inc22

for.inc22:                                        ; preds = %for.body14
  %inc23 = add nuw nsw i32 %t.03, 1
  %cmp = icmp ult i32 %inc23, 3
  br i1 %cmp, label %for.body, label %for.end24

for.end24:                                        ; preds = %for.inc22
  ret i32 %inc23
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %A = alloca [1 x [100 x i32]], align 16
  %B = alloca [1 x [100 x i32]], align 16
  %C = alloca [1 x [100 x i32]], align 16
  br label %for.body

for.body:                                         ; preds = %for.inc18, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %0 = zext i32 %j.01 to i64
  %1 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %A, i64 0, i64 0, i64 %0
  store i32 %rem, i32* %1, align 4
  %call6 = call i32 @rand() #3
  %rem7 = srem i32 %call6, 100
  %2 = zext i32 %j.01 to i64
  %3 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %B, i64 0, i64 0, i64 %2
  store i32 %rem7, i32* %3, align 4
  %call12 = call i32 @rand() #3
  %rem13 = srem i32 %call12, 100
  %4 = zext i32 %j.01 to i64
  %5 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %C, i64 0, i64 0, i64 %4
  store i32 %rem13, i32* %5, align 4
  %inc = add nuw nsw i32 %j.01, 1
  %cmp2 = icmp ult i32 %inc, 100
  br i1 %cmp2, label %for.body3, label %for.inc18

for.inc18:                                        ; preds = %for.body3
  br i1 false, label %for.body, label %for.end20

for.end20:                                        ; preds = %for.inc18
  %arraydecay = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %A, i64 0, i64 0, i64 0
  %arraydecay26 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %B, i64 0, i64 0, i64 0
  %arraydecay29 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %C, i64 0, i64 0, i64 0
  %call30 = call i32 @_Z9jacobi_1dPiS_S_(i32* nonnull %arraydecay, i32* nonnull %arraydecay26, i32* nonnull %arraydecay29)
  ret i32 0
}

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
