; ModuleID = '.triangular.cpp_mem2reg.ll'
source_filename = "src/triangular.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z10triangularPiPA100_ii(i32* %x, [100 x i32]* %A, i32 %n) #0 {
entry:
  %sub = sub nsw i32 %n, 1
  %cmp4 = icmp sge i32 %sub, 0
  br i1 %cmp4, label %for.body.lr.ph, label %for.end16

for.body.lr.ph:                                   ; preds = %entry
  br label %for.body

for.body:                                         ; preds = %for.body.lr.ph, %for.inc14
  %i.05 = phi i32 [ %sub, %for.body.lr.ph ], [ %dec15, %for.inc14 ]
  %sub1 = sub nsw i32 %i.05, 1
  %cmp31 = icmp sge i32 %sub1, 0
  br i1 %cmp31, label %for.body4.lr.ph, label %for.end

for.body4.lr.ph:                                  ; preds = %for.body
  br label %for.body4

for.body4:                                        ; preds = %for.body4.lr.ph, %for.inc
  %k.02 = phi i32 [ %sub1, %for.body4.lr.ph ], [ %dec, %for.inc ]
  %idxprom = sext i32 %k.02 to i64
  %arrayidx = getelementptr inbounds [100 x i32], [100 x i32]* %A, i64 %idxprom
  %idxprom5 = sext i32 %i.05 to i64
  %arrayidx6 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx, i64 0, i64 %idxprom5
  %0 = load i32, i32* %arrayidx6, align 4
  %idxprom7 = sext i32 %i.05 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %x, i64 %idxprom7
  %1 = load i32, i32* %arrayidx8, align 4
  %mul = mul nsw i32 %0, %1
  %idxprom9 = sext i32 %k.02 to i64
  %arrayidx10 = getelementptr inbounds [100 x i32], [100 x i32]* %A, i64 %idxprom9
  %idxprom11 = sext i32 %n to i64
  %arrayidx12 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx10, i64 0, i64 %idxprom11
  %2 = load i32, i32* %arrayidx12, align 4
  %sub13 = sub nsw i32 %2, %mul
  store i32 %sub13, i32* %arrayidx12, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %dec = add nsw i32 %k.02, -1
  %cmp3 = icmp sge i32 %dec, 0
  br i1 %cmp3, label %for.body4, label %for.cond2.for.end_crit_edge

for.cond2.for.end_crit_edge:                      ; preds = %for.inc
  br label %for.end

for.end:                                          ; preds = %for.cond2.for.end_crit_edge, %for.body
  br label %for.inc14

for.inc14:                                        ; preds = %for.end
  %dec15 = add nsw i32 %i.05, -1
  %cmp = icmp sge i32 %dec15, 0
  br i1 %cmp, label %for.body, label %for.cond.for.end16_crit_edge

for.cond.for.end16_crit_edge:                     ; preds = %for.inc14
  %split = phi i32 [ %dec15, %for.inc14 ]
  br label %for.end16

for.end16:                                        ; preds = %for.cond.for.end16_crit_edge, %entry
  %i.0.lcssa = phi i32 [ %split, %for.cond.for.end16_crit_edge ], [ %sub, %entry ]
  %add = add nsw i32 %i.0.lcssa, %n
  ret i32 %add
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %xArray = alloca [1 x [100 x i32]], align 16
  %A = alloca [1 x [100 x [100 x i32]]], align 16
  %n = alloca [1 x i32], align 4
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc22
  %i.03 = phi i32 [ 0, %entry ], [ %inc23, %for.inc22 ]
  %idxprom = sext i32 %i.03 to i64
  %arrayidx = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 %idxprom
  store i32 100, i32* %arrayidx, align 4
  br label %for.body3

for.body3:                                        ; preds = %for.body, %for.inc19
  %x.02 = phi i32 [ 0, %for.body ], [ %inc20, %for.inc19 ]
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 1000
  %idxprom4 = sext i32 %i.03 to i64
  %arrayidx5 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %xArray, i64 0, i64 %idxprom4
  %idxprom6 = sext i32 %x.02 to i64
  %arrayidx7 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx5, i64 0, i64 %idxprom6
  store i32 %rem, i32* %arrayidx7, align 4
  br label %for.body10

for.body10:                                       ; preds = %for.body3, %for.inc
  %y.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %call11 = call i32 @rand() #3
  %rem12 = srem i32 %call11, 1000
  %idxprom13 = sext i32 %i.03 to i64
  %arrayidx14 = getelementptr inbounds [1 x [100 x [100 x i32]]], [1 x [100 x [100 x i32]]]* %A, i64 0, i64 %idxprom13
  %idxprom15 = sext i32 %y.01 to i64
  %arrayidx16 = getelementptr inbounds [100 x [100 x i32]], [100 x [100 x i32]]* %arrayidx14, i64 0, i64 %idxprom15
  %idxprom17 = sext i32 %x.02 to i64
  %arrayidx18 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx16, i64 0, i64 %idxprom17
  store i32 %rem12, i32* %arrayidx18, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body10
  %inc = add nsw i32 %y.01, 1
  %cmp9 = icmp slt i32 %inc, 10
  br i1 %cmp9, label %for.body10, label %for.end

for.end:                                          ; preds = %for.inc
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %inc20 = add nsw i32 %x.02, 1
  %cmp2 = icmp slt i32 %inc20, 100
  br i1 %cmp2, label %for.body3, label %for.end21

for.end21:                                        ; preds = %for.inc19
  br label %for.inc22

for.inc22:                                        ; preds = %for.end21
  %inc23 = add nsw i32 %i.03, 1
  %cmp = icmp slt i32 %inc23, 1
  br i1 %cmp, label %for.body, label %for.end24

for.end24:                                        ; preds = %for.inc22
  %arrayidx27 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %xArray, i64 0, i64 0
  %arraydecay = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx27, i32 0, i32 0
  %arrayidx29 = getelementptr inbounds [1 x [100 x [100 x i32]]], [1 x [100 x [100 x i32]]]* %A, i64 0, i64 0
  %arraydecay30 = getelementptr inbounds [100 x [100 x i32]], [100 x [100 x i32]]* %arrayidx29, i32 0, i32 0
  %arrayidx32 = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 0
  %0 = load i32, i32* %arrayidx32, align 4
  %call33 = call i32 @_Z10triangularPiPA100_ii(i32* %arraydecay, [100 x i32]* %arraydecay30, i32 %0)
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
